/** 
 * @file	ITokenizer.h
 * @brief	header file for interfece of tokenizer
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

#include <iostream>
#include "DictionaryFacade.h"
#include "Exceptions.h"

using std::make_shared;


namespace DictionaryOps
{

DictionaryFacade::DictionaryFacade(Tools::Language i_language)
{
    m_dicInterface = 0;
    if (i_language == Tools::RU)
    {
        m_dicInterface = DictionaryInterface::CreateDictionary();
        m_dicInterface->setMainWordFormFromLinkedLemmas(true);
        m_morphologicalDictionary = make_shared<MorphologicalDictionary>(BIN_DIC_FILE_PATH_RUSSIAN);
        m_morphologicalDictionary->setPrediction(false);
        m_morphologicalDictionary->enableE(true);
    }
    else if (i_language == Tools::EN)
    {
        m_dicInterface = DictionaryInterface::CreateDictionary(BIN_DIC_FILE_PATH_ENGLISH1);
        m_morphologicalDictionary = make_shared<MorphologicalDictionary>(BIN_DIC_FILE_PATH_ENGLISH);
        m_morphologicalDictionary->enableRules(true);
    }
    else
    {
        throw Tools::LanguageNotFoundException();
    }
    m_language = i_language;
}

vector<Morphology> DictionaryFacade::getMorphologicalInformation(const wstring& i_wordForm) const
{
    shared_ptr<vector<shared_ptr<Morphology>>> result = make_shared<vector<shared_ptr<Morphology>>>();
    m_morphologicalDictionary->getMorphology(i_wordForm, result);
    vector<Morphology> info;
    for(auto& mptr : *result)
    {
        info.push_back(*mptr);
    }
    return info;
}

vector<wstring> DictionaryFacade::synthesizeWordform(const wstring& i_lemma, vector<wstring>& i_features) const
{
    auto& featureNameMap = m_morphologicalDictionary->getShortToLongMap();
    if(featureNameMap.find(i_features[0]) != featureNameMap.end())
    {
        for(size_t i = 0; i < i_features.size(); ++i)
        {
            i_features[i] = featureNameMap.at(i_features[i]);
        }
    }
    auto wordForms = m_dicInterface->getWordFormByFeatures(i_lemma, i_features);
    vector<wstring> tokens;
    for(auto& wordForm : wordForms)
    {
        tokens.push_back(wordForm.word);
    }
    return tokens;
}

vector<vector<Morphology>> DictionaryFacade::getParadigmForLemma(const wstring& i_lemma) const
{
    vector<vector<GrammInfo>> paradigms;
    m_dicInterface->getParadigmsForLemma(i_lemma, paradigms);

    vector<vector<Morphology>> result;
    for(size_t i = 0; i < paradigms.size(); i++) {
        vector<GrammInfo>& source_paradigm = paradigms[i];
        vector<Morphology> target_paradigm;
        for(size_t wordform = 0; wordform < source_paradigm.size(); wordform++) {
            target_paradigm.push_back(convertToMorphology(source_paradigm[wordform]));
        }
        result.push_back(target_paradigm);
    }
    return result;
}

Tools::Language DictionaryFacade::getLanguage() const
{
    return m_language;
}

//////////////////////////////////////////////////////////////////
// simple data type converters
//////////////////////////////////////////////////////////////////

/**
 * GramInfo to Morphology conversion
 */

Morphology DictionaryFacade::convertToMorphology(GrammInfo& gram_info) const
{
    Morphology morphology;

    morphology.lemma_id = gram_info.id;
    morphology.lemma = shared_ptr<wstring>( new wstring(gram_info.initial_form));
    morphology.word = shared_ptr<wstring>( new wstring(gram_info.word));
    for(size_t i = 0; i < gram_info.fid.size(); i++) {
        morphology.features.push_back(m_morphologicalDictionary->open_corpora_feature(gram_info.fid[i]));
    }

    return morphology;
}

}
