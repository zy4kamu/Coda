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

#ifndef DICTIONARY_CREATOR
#define DICTIONARY_CREATOR

#include <map>
#include <memory>
#include "DictionaryFacade.h"
#include "Tools.h"

using std::map;
using std::shared_ptr;

namespace DictionaryOps
{

class DictionaryCreator
{
public:
    static DictionaryCreator& getDictionaryCreator();
    shared_ptr<DictionaryFacade> getDictionary(Tools::Language i_language);
    void removeDictionary(Tools::Language i_language);
    void Register(std::shared_ptr<DictionaryOps::DictionaryFacade> i_toRegister);

private:
    DictionaryCreator();
    DictionaryCreator(const DictionaryCreator& i_creator);
    void operator = (const DictionaryCreator& i_creator);
    bool isSupported_(Tools::Language i_language);

    map<Tools::Language, shared_ptr<DictionaryOps::DictionaryFacade> > m_dictionaries;
};

}

#endif // DICTIONARY_CREATOR

