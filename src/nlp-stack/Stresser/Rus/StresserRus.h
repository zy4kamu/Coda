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

#ifndef STRESS_DISAMBIGUATOR_RUS
#define STRESS_DISAMBIGUATOR_RUS

#include "DisambiguatedData.h"
#include "IStresser.h"
#include "Tools.h"

namespace Stresser
{

using Disambiguation::DisambiguatedData;

class StresserRus : public IStresser
{
public:
    StresserRus(const string& configFolder);

    virtual vector<wstring> SetStress(
        const vector<DisambiguatedData>& disambiguatedData);

    virtual Tools::Language GetLanguage() const;
private:
	unordered_map<wstring, wstring> tokens_NonAmbig;
	unordered_map<wstring, unordered_map<wstring, wstring> > tokens_AmbigByMorpology;

	unordered_map<wstring, set<wstring> > exceptionToken_Stress;
	unordered_map<wstring, set<wstring> > exceptionStress_Tags;

	void prepareAmbigByMorphologyTokenData(
        const string& file);

	void prepareNonAmbigByMorphologyTokenData(
        const string& file);

	wstring setStressForExceptions(
		const wstring& token
        , const wstring& tag
        , const wstring& prevToken
        , const wstring& prevTag
        , const wstring& nextToken
        , const wstring& nextTag);

    wstring setStress(
        const wstring& token
        , const wstring& tag);

	wstring setStress(
		const wstring& token
        , const wstring& tag
        , const wstring& prevToken
        , const wstring& prevTag
        , const wstring& nextToken
        , const wstring& nextTag);
};

}

#endif // STRESS_DISAMBIGUATOR_RUS
