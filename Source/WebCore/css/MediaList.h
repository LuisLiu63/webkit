/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2006, 2008, 2009, 2010, 2012 Apple Inc. All rights reserved.
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

#pragma once

#include <memory>
#include <wtf/Forward.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace WebCore {

class CSSParser;
class CSSRule;
class CSSStyleSheet;
class Document;
class MediaQuery;

using ExceptionCode = int;

class MediaQuerySet : public RefCounted<MediaQuerySet> {
public:
    static Ref<MediaQuerySet> create()
    {
        return adoptRef(*new MediaQuerySet);
    }
    static Ref<MediaQuerySet> create(const String& mediaString)
    {
        return adoptRef(*new MediaQuerySet(mediaString, false));
    }
    static Ref<MediaQuerySet> createAllowingDescriptionSyntax(const String& mediaString)
    {
        return adoptRef(*new MediaQuerySet(mediaString, true));
    }
    ~MediaQuerySet();

    bool parse(const String&);
    bool add(const String&);
    bool remove(const String&);

    void addMediaQuery(MediaQuery&&);

    const Vector<MediaQuery>& queryVector() const { return m_queries; }

    int lastLine() const { return m_lastLine; }
    void setLastLine(int lastLine) { m_lastLine = lastLine; }

    String mediaText() const;

    Ref<MediaQuerySet> copy() const { return adoptRef(*new MediaQuerySet(*this)); }

    void shrinkToFit();

private:
    MediaQuerySet();
    MediaQuerySet(const String& mediaQuery, bool fallbackToDescription);
    MediaQuerySet(const MediaQuerySet&);

    Optional<MediaQuery> internalParse(CSSParser&, const String&);
    Optional<MediaQuery> internalParse(const String&);

    unsigned m_fallbackToDescriptor : 1; // true if failed media query parsing should fallback to media description parsing.
    signed m_lastLine : 31;
    Vector<MediaQuery> m_queries;
};

class MediaList : public RefCounted<MediaList> {
public:
    static Ref<MediaList> create(MediaQuerySet* mediaQueries, CSSStyleSheet* parentSheet)
    {
        return adoptRef(*new MediaList(mediaQueries, parentSheet));
    }
    static Ref<MediaList> create(MediaQuerySet* mediaQueries, CSSRule* parentRule)
    {
        return adoptRef(*new MediaList(mediaQueries, parentRule));
    }

    ~MediaList();

    unsigned length() const { return m_mediaQueries->queryVector().size(); }
    String item(unsigned index) const;
    void deleteMedium(const String& oldMedium, ExceptionCode&);
    void appendMedium(const String& newMedium, ExceptionCode&);

    String mediaText() const { return m_mediaQueries->mediaText(); }
    void setMediaText(const String&, ExceptionCode&);

    CSSRule* parentRule() const { return m_parentRule; }
    CSSStyleSheet* parentStyleSheet() const { return m_parentStyleSheet; }
    void clearParentStyleSheet() { ASSERT(m_parentStyleSheet); m_parentStyleSheet = nullptr; }
    void clearParentRule() { ASSERT(m_parentRule); m_parentRule = nullptr; }
    const MediaQuerySet* queries() const { return m_mediaQueries.get(); }

    void reattach(MediaQuerySet*);

private:
    MediaList();
    MediaList(MediaQuerySet*, CSSStyleSheet* parentSheet);
    MediaList(MediaQuerySet*, CSSRule* parentRule);

    RefPtr<MediaQuerySet> m_mediaQueries;
    CSSStyleSheet* m_parentStyleSheet { nullptr };
    CSSRule* m_parentRule { nullptr };
};

// Adds message to inspector console whenever dpi or dpcm values are used for "screen" media.
// FIXME: Seems strange to have this here in this file, and unclear exactly who should call this and when.
void reportMediaQueryWarningIfNeeded(Document*, const MediaQuerySet*);

#if !ENABLE(RESOLUTION_MEDIA_QUERY)

inline void reportMediaQueryWarningIfNeeded(Document*, const MediaQuerySet*)
{
}

#endif

} // namespace
