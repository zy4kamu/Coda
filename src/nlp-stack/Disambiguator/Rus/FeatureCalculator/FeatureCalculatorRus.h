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

#ifndef _FEATURE_CALCULATOR
#define _FEATURE_CALCULATOR

#include "IFeatureCalculator.h"
#include "PreDisambiguatedData.h"
#include "MorphologicalDictionary.h"
#include "OpenCorporaToSynagrusConverter.h"
#include "Token.h"
#include "SimpleFeatureCalculatorRus.h"

using std::vector;
using std::string;
using std::wstring;
using std::unordered_map;

using Tokenization::Token;
using TagsetConverting::OpenCorporaToSynagrusConverter;

namespace Disambiguation
{

class FeatureCalculatorRus
    : public SimpleFeatureCalculatorRus
{
public:
    FeatureCalculatorRus(
        const string& tokenLabelFrequencyDistributionFile
        , const string& translationsForUntranslatedTokens
        , const string& abbreviatureFile
        , shared_ptr<MorphologicalDictionary> dictionary
        , shared_ptr<OpenCorporaToSynagrusConverter> converter);

    vector<PredisambiguatedData> CalculateFeatures(
        const vector<Tokenization::Token>& sentence) const;
private:    
    vector<set<wstring> > propertiesByGroups; /**< Used to calculate feature related to possible properties*/

     /**
     * @brief	Fills propertiesByGroups
     */
    void preparePropertiesByGroups();

    /**
     * @brief	Calculates specific features for one token
     * @param[out]	predisambiguatedData - data where to add features
     * @param[in]	token - token to evaluate
     * @param[in]	tokenIndex - index of token position in the sentence
     */
   void evaluateToken(
       PredisambiguatedData* predisambiguatedData
        , const Token& token
        , int tokenIndex) const;

    /**
     * @brief	Technical function: calculates splittedArray and heap from features that are used in other functions calculating special features
     * @param[in]	features - feautures of the token
	 * @param[out]	splittedArray - array of small features (splitted usual Syntagrus features)
	 * @param[out]	splittedArray - non-duplicating set of small features
     */
    void getPossibleProperties(
	    const vector<wstring>& features
        , vector<vector<wstring> >* splittedArray
        , set<wstring>* heap) const;

    /**
     * @brief	Calculates special features for the token: intersection of all its porperties, and adds the to the collection
     * @param[out]	features - feautures to correct
	 * @param[in]	splittedArray - splitted array of features
	 * @param[in]	splittedArray - possible properties without duplications
     */
	void addSpecialFeature_PropertiesIntersection(
		vector<wstring>* features
        , const vector<vector<wstring> >& splittedArray
        , const set<wstring>& heap) const;

    /**
     * @brief	Calculates special feature for the token: union of some specific small properties
     * @param[out]	features - feautures to correct
	 * @param[in]	splittedArray - splitted array of features
	 * @param[in]	splittedArray - possible properties without duplications
     */
	void addSpecialFeature_PossiblePoperties(
		vector<wstring>* features
        , const vector<vector<wstring> >& splittedArray
        , const set<wstring>& heap) const;

    /**
     * @brief	Calculates special feature for the token: possible small properties (like 'S' - noun, 'V' - verb) and adds them to the collection
     * @param[out]	features - feautures to correct
	 * @param[in]	splittedArray - splitted array of features
	 * @param[in]	splittedArray - possible properties without duplications
     */
	void addSpecialFeature_SimpleProperties(
		vector<wstring>* features
        , const vector<vector<wstring> >& splittedArray
        , const set<wstring>& heap) const;

    /**
     * @brief	Adds features related to transitivity of verbs
     * @param[in]	token - token to evaluate
     */
    void addPEREHfeature(vector<PredisambiguatedData>* predisambiguatedData) const;

    /**
     * @brief	Adds the corresponding punctuation features to the vector with features
     * @param[out]	features - vector of features for the token
     * @param[in]	token - token containing punctuation 
     */
	void addPunctuationFeature(vector<wstring>* features, const Token& token) const;

    /**
     * @brief	Adds name entity feature to predisambiguatedData
     * @param[out]	predisambiguatedData - data that contains features
     * @param[in]	token - token containing punctuation
     */
    void addNERFeature(PredisambiguatedData* predisambiguatedData
        , const Token& token) const;

    /**
     * @brief	Adds the features related to the transitivity of the verbs to the sequence of the features in the sentence
     * @param[out]	features - feautures for the whole sentence. features[i] contains features of the i-th token in the sentence
     */
	void addPEREHfeature(vector<vector<wstring> >* features) const;

    /**
     * @brief	Verifies if the token with features may be verb (Used in order to calculate features related to transitivity of the verbs)
     * @param[in]	features - features of the token
     */
	bool mayBeVerb(const vector<wstring>& features) const;

	/**
     * @brief	Verifies if the token with features may be transitive verb (Used in order to calculate features related to transitivity of the verbs)
     * @param[in]	features - features of the token
     */
	bool mayBePEREH(const vector<wstring>& features) const;

	/**
     * @brief	Verifies if the token with features may be intransitive verb (Used in order to calculate features related to transitivity of the verbs)
     * @param[in]	features - features of the token
     */
	bool mayBeNEPEREH(const vector<wstring>& features) const;
};

}

#endif // _FEATURE_CALCULATOR
