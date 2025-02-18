/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 2004-2005 Allan Sandfeld Jensen (kde@carewolf.com)
 * Copyright (C) 2006, 2007 Nicholas Shanks (webkit@nickshanks.com)
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Alexey Proskuryakov <ap@webkit.org>
 * Copyright (C) 2007, 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 * Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 * Copyright (C) Research In Motion Limited 2011. All rights reserved.
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "DocumentRuleSets.h"

#include "CSSStyleSheet.h"
#include "ExtensionStyleSheets.h"
#include "MediaQueryEvaluator.h"
#include "StyleResolver.h"
#include "StyleSheetContents.h"

namespace WebCore {

DocumentRuleSets::DocumentRuleSets()
{
    m_authorStyle = std::make_unique<RuleSet>();
    m_authorStyle->disableAutoShrinkToFit();
}

DocumentRuleSets::~DocumentRuleSets()
{
}

void DocumentRuleSets::initUserStyle(ExtensionStyleSheets& extensionStyleSheets, const MediaQueryEvaluator& medium, StyleResolver& resolver)
{
    auto tempUserStyle = std::make_unique<RuleSet>();
    if (CSSStyleSheet* pageUserSheet = extensionStyleSheets.pageUserSheet())
        tempUserStyle->addRulesFromSheet(pageUserSheet->contents(), medium, &resolver);
    collectRulesFromUserStyleSheets(extensionStyleSheets.injectedUserStyleSheets(), *tempUserStyle, medium, resolver);
    collectRulesFromUserStyleSheets(extensionStyleSheets.documentUserStyleSheets(), *tempUserStyle, medium, resolver);
    if (tempUserStyle->ruleCount() > 0 || tempUserStyle->pageRules().size() > 0)
        m_userStyle = WTFMove(tempUserStyle);
}

void DocumentRuleSets::collectRulesFromUserStyleSheets(const Vector<RefPtr<CSSStyleSheet>>& userSheets, RuleSet& userStyle, const MediaQueryEvaluator& medium, StyleResolver& resolver)
{
    for (unsigned i = 0; i < userSheets.size(); ++i) {
        ASSERT(userSheets[i]->contents().isUserStyleSheet());
        userStyle.addRulesFromSheet(userSheets[i]->contents(), medium, &resolver);
    }
}

static std::unique_ptr<RuleSet> makeRuleSet(const Vector<RuleFeature>& rules)
{
    size_t size = rules.size();
    if (!size)
        return nullptr;
    auto ruleSet = std::make_unique<RuleSet>();
    for (size_t i = 0; i < size; ++i)
        ruleSet->addRule(rules[i].rule, rules[i].selectorIndex, rules[i].hasDocumentSecurityOrigin ? RuleHasDocumentSecurityOrigin : RuleHasNoSpecialState);
    ruleSet->shrinkToFit();
    return ruleSet;
}

void DocumentRuleSets::resetAuthorStyle()
{
    m_isAuthorStyleDefined = true;
    m_authorStyle = std::make_unique<RuleSet>();
    m_authorStyle->disableAutoShrinkToFit();
}

void DocumentRuleSets::appendAuthorStyleSheets(const Vector<RefPtr<CSSStyleSheet>>& styleSheets, MediaQueryEvaluator* medium, InspectorCSSOMWrappers& inspectorCSSOMWrappers, StyleResolver* resolver)
{
    // This handles sheets added to the end of the stylesheet list only. In other cases the style resolver
    // needs to be reconstructed. To handle insertions too the rule order numbers would need to be updated.
    for (auto& cssSheet : styleSheets) {
        ASSERT(!cssSheet->disabled());
        if (cssSheet->mediaQueries() && !medium->evaluate(*cssSheet->mediaQueries(), resolver))
            continue;
        m_authorStyle->addRulesFromSheet(cssSheet->contents(), *medium, resolver);
        inspectorCSSOMWrappers.collectFromStyleSheetIfNeeded(cssSheet.get());
    }
    m_authorStyle->shrinkToFit();
    collectFeatures();
}

void DocumentRuleSets::collectFeatures() const
{
    m_features.clear();
    // Collect all ids and rules using sibling selectors (:first-child and similar)
    // in the current set of stylesheets. Style sharing code uses this information to reject
    // sharing candidates.
    if (CSSDefaultStyleSheets::defaultStyle)
        m_features.add(CSSDefaultStyleSheets::defaultStyle->features());
    m_defaultStyleVersionOnFeatureCollection = CSSDefaultStyleSheets::defaultStyleVersion;

    if (m_authorStyle)
        m_features.add(m_authorStyle->features());
    if (m_userStyle)
        m_features.add(m_userStyle->features());

    m_siblingRuleSet = makeRuleSet(m_features.siblingRules);
    m_uncommonAttributeRuleSet = makeRuleSet(m_features.uncommonAttributeRules);

    m_ancestorClassRuleSets.clear();
    m_ancestorAttributeRuleSetsForHTML.clear();
}

RuleSet* DocumentRuleSets::ancestorClassRules(AtomicStringImpl* className) const
{
    auto addResult = m_ancestorClassRuleSets.add(className, nullptr);
    if (addResult.isNewEntry) {
        if (auto* rules = m_features.ancestorClassRules.get(className))
            addResult.iterator->value = makeRuleSet(*rules);
    }
    return addResult.iterator->value.get();
}

const DocumentRuleSets::AttributeRules* DocumentRuleSets::ancestorAttributeRulesForHTML(AtomicStringImpl* attributeName) const
{
    auto addResult = m_ancestorAttributeRuleSetsForHTML.add(attributeName, nullptr);
    auto& value = addResult.iterator->value;
    if (addResult.isNewEntry) {
        if (auto* rules = m_features.ancestorAttributeRulesForHTML.get(attributeName)) {
            value = std::make_unique<AttributeRules>();
            value->attributeSelectors.reserveCapacity(rules->selectors.size());
            for (auto* selector : rules->selectors.values())
                value->attributeSelectors.uncheckedAppend(selector);
            value->ruleSet = makeRuleSet(rules->features);
        }
    }
    return value.get();
}

} // namespace WebCore
