/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
 * Copyright (C) 2016 Yusuke Suzuki <utatane.tea@gmail.com>
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

#pragma once

#include "Identifier.h"
#include <wtf/RefPtr.h>

namespace JSC {

class ModuleScopeData : public RefCounted<ModuleScopeData> {
WTF_MAKE_NONCOPYABLE(ModuleScopeData);
WTF_MAKE_FAST_ALLOCATED;
public:
    typedef HashMap<RefPtr<UniquedStringImpl>, IdentifierSet, IdentifierRepHash, HashTraits<RefPtr<UniquedStringImpl>>> IdentifierAliasMap;

    static Ref<ModuleScopeData> create() { return adoptRef(*new ModuleScopeData); }

    const IdentifierAliasMap& exportedBindings() const { return m_exportedBindings; }

    bool exportName(const Identifier& exportedName)
    {
        return m_exportedNames.add(exportedName.impl()).isNewEntry;
    }

    void exportBinding(const Identifier& localName, const Identifier& exportedName)
    {
        m_exportedBindings.add(localName.impl(), IdentifierSet()).iterator->value.add(exportedName.impl());
    }

    void exportBinding(const Identifier& localName)
    {
        exportBinding(localName, localName);
    }

private:
    ModuleScopeData() = default;

    IdentifierSet m_exportedNames { };
    IdentifierAliasMap m_exportedBindings { };
};

} // namespace
