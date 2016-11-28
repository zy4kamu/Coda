/**
 *
 * .. invisible:
 *     _   _ _____ _     _____ _____
 *
 *    | | | |  ___| |   |  ___/  ___|
 *
 *    | | | | |__ | |   | |__ \ `--.
 *
 *    | | | |  __|| |   |  __| `--. \
 *
 *    \ \_/ / |___| |___| |___/\__/ /
 *
 *     \___/\____/\_____|____/\____/
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License
 *
 */

#ifndef SYNTAX_CREATOR
#define SYNTAX_CREATOR

#include <map>
#include <memory>

#include "ISyntaxParser.h"
#include "SyntaxSettings.h"
#include "Tools.h"

namespace SyntaxParser
{

using std::map;
using std::shared_ptr;
using Tools::Language;

class SyntaxCreator
{
public:
    static SyntaxCreator& GetSyntaxCreator();

    shared_ptr<ISyntaxParser> GetSyntaxParser(Language language);
    void RemoveSyntaxParser(Language language);

    template<class SyntaxSettings>
    void Register();

    bool ContainsLanguage(Language language);
private:
    SyntaxCreator();
    SyntaxCreator(const SyntaxCreator& creator);
    void operator = (const SyntaxCreator& creator);

    map<Language, shared_ptr<ISyntaxParser> > settings;
};

template<class LanguageSettings>
void SyntaxCreator::Register()
{
    typedef SyntaxSettings<LanguageSettings> Settings;

    Tools::Language language = Settings::GetLanguage();
    const string& modelPath = Settings::GetModelPath();

    settings[language]
        = std::shared_ptr<typename Settings::Parser>(new typename Settings::Parser(
            modelPath, language));
}

}

#endif // SYNTAX_CREATOR
