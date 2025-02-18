/*
 * Copyright (C) 2009 Alex Milowski (alex@milowski.com). All rights reserved.
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2010 François Sausset (sausset@gmail.com). All rights reserved.
 * Copyright (C) 2016 Igalia S.L.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(MATHML)

#include "MathMLNames.h"
#include "StyledElement.h"

namespace WebCore {

class MathMLElement : public StyledElement {
public:
    static Ref<MathMLElement> create(const QualifiedName& tagName, Document&);

    unsigned colSpan() const;
    unsigned rowSpan() const;

    bool isMathMLToken() const
    {
        return hasTagName(MathMLNames::miTag) || hasTagName(MathMLNames::mnTag) || hasTagName(MathMLNames::moTag) || hasTagName(MathMLNames::msTag) || hasTagName(MathMLNames::mtextTag);
    }

    bool isSemanticAnnotation() const
    {
        return hasTagName(MathMLNames::annotationTag) || hasTagName(MathMLNames::annotation_xmlTag);
    }

    virtual bool isPresentationMathML() const;

    bool hasTagName(const MathMLQualifiedName& name) const { return hasLocalName(name.localName()); }

    // MathML lengths (https://www.w3.org/TR/MathML3/chapter2.html#fund.units)
    // TeX's Math Unit is used internally for named spaces (1 mu = 1/18 em).
    // Unitless values are interpreted as a multiple of a reference value.
    enum class LengthType { Cm, Em, Ex, In, MathUnit, Mm, ParsingFailed, Pc, Percentage, Pt, Px, UnitLess, Infinity };
    struct Length {
        LengthType type { LengthType::ParsingFailed };
        float value { 0 };
    };
    static Length parseMathMLLength(const String&);

    enum class BooleanValue { True, False, Default };

    // These are the mathvariant values from the MathML recommendation.
    // The special value none means that no explicit mathvariant value has been specified.
    // Note that the numeral values are important for the computation performed in the mathVariant function of RenderMathMLToken, do not change them!
    enum class MathVariant {
        None = 0,
        Normal = 1,
        Bold = 2,
        Italic = 3,
        BoldItalic = 4,
        Script = 5,
        BoldScript = 6,
        Fraktur = 7,
        DoubleStruck = 8,
        BoldFraktur = 9,
        SansSerif = 10,
        BoldSansSerif = 11,
        SansSerifItalic = 12,
        SansSerifBoldItalic = 13,
        Monospace = 14,
        Initial = 15,
        Tailed = 16,
        Looped = 17,
        Stretched = 18
    };

    virtual Optional<bool> specifiedDisplayStyle();
    Optional<MathVariant> specifiedMathVariant();

protected:
    MathMLElement(const QualifiedName& tagName, Document&);

    void parseAttribute(const QualifiedName&, const AtomicString&) override;
    bool childShouldCreateRenderer(const Node&) const override;
    void attributeChanged(const QualifiedName&, const AtomicString& oldValue, const AtomicString& newValue, AttributeModificationReason) override;

    bool isPresentationAttribute(const QualifiedName&) const override;
    void collectStyleForPresentationAttribute(const QualifiedName&, const AtomicString&, MutableStyleProperties&) override;

    bool isPhrasingContent(const Node&) const;
    bool isFlowContent(const Node&) const;

    bool willRespondToMouseClickEvents() override;
    void defaultEventHandler(Event*) override;

    const Length& cachedMathMLLength(const QualifiedName&, Optional<Length>&);
    const BooleanValue& cachedBooleanAttribute(const QualifiedName&, Optional<BooleanValue>&);

    virtual bool acceptsDisplayStyleAttribute() { return false; }
    virtual bool acceptsMathVariantAttribute() { return false; }

    static Optional<bool> toOptionalBool(const BooleanValue& value) { return value == BooleanValue::Default ? Nullopt : Optional<bool>(value == BooleanValue::True); }
    Optional<BooleanValue> m_displayStyle;
    Optional<MathVariant> m_mathVariant;

private:
    virtual void updateSelectedChild() { }
    static Length parseNumberAndUnit(const StringView&);
    static Length parseNamedSpace(const StringView&);
    static MathVariant parseMathVariantAttribute(const AtomicString& attributeValue);

    bool canStartSelection() const final;
    bool isFocusable() const final;
    bool isKeyboardFocusable(KeyboardEvent*) const final;
    bool isMouseFocusable() const final;
    bool isURLAttribute(const Attribute&) const final;
    bool supportsFocus() const final;
    int tabIndex() const final;
};

inline bool Node::hasTagName(const MathMLQualifiedName& name) const
{
    return isMathMLElement() && downcast<MathMLElement>(*this).hasTagName(name);
}

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::MathMLElement)
    static bool isType(const WebCore::Node& node) { return node.isMathMLElement(); }
SPECIALIZE_TYPE_TRAITS_END()

#include "MathMLElementTypeHelpers.h"

#endif // ENABLE(MATHML)
