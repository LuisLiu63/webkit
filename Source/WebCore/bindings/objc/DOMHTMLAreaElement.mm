/*
 * Copyright (C) 2004-2016 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "config.h"
#import "DOMHTMLAreaElementInternal.h"

#import "DOMDOMTokenListInternal.h"
#import "DOMNodeInternal.h"
#import "DOMPrivate.h"
#import "DOMTokenList.h"
#import "ExceptionHandlers.h"
#import "HTMLAreaElement.h"
#import "HTMLNames.h"
#import "JSMainThreadExecState.h"
#import "ThreadCheck.h"
#import "URL.h"
#import "WebScriptObjectPrivate.h"
#import <wtf/GetPtr.h>

#define IMPL static_cast<WebCore::HTMLAreaElement*>(reinterpret_cast<WebCore::Node*>(_internal))

@implementation DOMHTMLAreaElement

- (NSString *)alt
{
    WebCore::JSMainThreadNullState state;
    return IMPL->getAttribute(WebCore::HTMLNames::altAttr);
}

- (void)setAlt:(NSString *)newAlt
{
    WebCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(WebCore::HTMLNames::altAttr, newAlt);
}

- (NSString *)coords
{
    WebCore::JSMainThreadNullState state;
    return IMPL->getAttribute(WebCore::HTMLNames::coordsAttr);
}

- (void)setCoords:(NSString *)newCoords
{
    WebCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(WebCore::HTMLNames::coordsAttr, newCoords);
}

- (BOOL)noHref
{
    WebCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(WebCore::HTMLNames::nohrefAttr);
}

- (void)setNoHref:(BOOL)newNoHref
{
    WebCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(WebCore::HTMLNames::nohrefAttr, newNoHref);
}

- (NSString *)ping
{
    WebCore::JSMainThreadNullState state;
    return IMPL->getAttribute(WebCore::HTMLNames::pingAttr);
}

- (void)setPing:(NSString *)newPing
{
    WebCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(WebCore::HTMLNames::pingAttr, newPing);
}

- (NSString *)rel
{
    WebCore::JSMainThreadNullState state;
    return IMPL->getAttribute(WebCore::HTMLNames::relAttr);
}

- (void)setRel:(NSString *)newRel
{
    WebCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(WebCore::HTMLNames::relAttr, newRel);
}

- (NSString *)shape
{
    WebCore::JSMainThreadNullState state;
    return IMPL->getAttribute(WebCore::HTMLNames::shapeAttr);
}

- (void)setShape:(NSString *)newShape
{
    WebCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(WebCore::HTMLNames::shapeAttr, newShape);
}

- (NSString *)target
{
    WebCore::JSMainThreadNullState state;
    return IMPL->getAttribute(WebCore::HTMLNames::targetAttr);
}

- (void)setTarget:(NSString *)newTarget
{
    WebCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(WebCore::HTMLNames::targetAttr, newTarget);
}

- (NSString *)accessKey
{
    WebCore::JSMainThreadNullState state;
    return IMPL->getAttribute(WebCore::HTMLNames::accesskeyAttr);
}

- (void)setAccessKey:(NSString *)newAccessKey
{
    WebCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(WebCore::HTMLNames::accesskeyAttr, newAccessKey);
}

- (NSURL *)absoluteLinkURL
{
    WebCore::JSMainThreadNullState state;
    return [self _getURLAttribute:@"href"];
}

- (DOMDOMTokenList *)relList
{
    WebCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->relList()));
}

- (NSString *)href
{
    WebCore::JSMainThreadNullState state;
    return IMPL->getURLAttribute(WebCore::HTMLNames::hrefAttr);
}

- (void)setHref:(NSString *)newHref
{
    WebCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(WebCore::HTMLNames::hrefAttr, newHref);
}

- (NSString *)origin
{
    WebCore::JSMainThreadNullState state;
    return IMPL->origin();
}

- (NSString *)protocol
{
    WebCore::JSMainThreadNullState state;
    return IMPL->protocol();
}

- (NSString *)host
{
    WebCore::JSMainThreadNullState state;
    return IMPL->host();
}

- (NSString *)hostname
{
    WebCore::JSMainThreadNullState state;
    return IMPL->hostname();
}

- (NSString *)port
{
    WebCore::JSMainThreadNullState state;
    return IMPL->port();
}

- (NSString *)pathname
{
    WebCore::JSMainThreadNullState state;
    return IMPL->pathname();
}

- (NSString *)search
{
    WebCore::JSMainThreadNullState state;
    return IMPL->search();
}

- (NSString *)hashName
{
    WebCore::JSMainThreadNullState state;
    return IMPL->hash();
}

@end

WebCore::HTMLAreaElement* core(DOMHTMLAreaElement *wrapper)
{
    return wrapper ? reinterpret_cast<WebCore::HTMLAreaElement*>(wrapper->_internal) : 0;
}
