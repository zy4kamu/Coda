#include "FeatureCalculatorRus.h"
#include "Tools.h"

#define MAX_VALUE 100000

namespace Disambiguation
{

FeatureCalculatorRus::FeatureCalculatorRus(
    const string& tokenLabelFrequencyDistributionFile
    , const string& translationsForUntranslatedTokens
    , const string& abbreviatureFile
    , shared_ptr<MorphologicalDictionary> dictionary
    , shared_ptr<OpenCorporaToSynagrusConverter> converter)
    : SimpleFeatureCalculatorRus(
    tokenLabelFrequencyDistributionFile
    , translationsForUntranslatedTokens
    , abbreviatureFile
    , dictionary, converter)
{
    preparePropertiesByGroups();
}

void FeatureCalculatorRus::preparePropertiesByGroups()
{
    set<wstring> v1;
	v1.insert(L"S");
	v1.insert(L"A");
	v1.insert(L"V");
	v1.insert(L"ADV");
	v1.insert(L"NUM");
	v1.insert(L"PR");
	v1.insert(L"COM");
	v1.insert(L"CONJ");
	v1.insert(L"PART");
	v1.insert(L"INTJ");
	v1.insert(L"NID");
	propertiesByGroups.push_back(v1);

	set<wstring> v2;
	v2.insert(L"\u0415\u0414");//here was cyrrilic symbols: ЕД
	v2.insert(L"\u041c\u041d");//here was cyrrilic symbols: МН
	propertiesByGroups.push_back(v2);

	set<wstring> v3;
	v3.insert(L"\u041c\u0423\u0416");//here was cyrrilic symbols: МУЖ
	v3.insert(L"\u0416\u0415\u041d");//here was cyrrilic symbols: ЖЕН
	v3.insert(L"\u0421\u0420\u0415\u0414");//here was cyrrilic symbols: СРЕД
	propertiesByGroups.push_back(v3);

	set<wstring> v4;
	v4.insert(L"\u0418\u041c");//here was cyrrilic symbols: ИМ
	v4.insert(L"\u0420\u041e\u0414");//here was cyrrilic symbols: РОД
	v4.insert(L"\u0414\u0410\u0422");//here was cyrrilic symbols: ДАТ
	v4.insert(L"\u0412\u0418\u041d");//here was cyrrilic symbols: ВИН
	v4.insert(L"\u0422\u0412\u041e\u0420");//here was cyrrilic symbols: ТВОР
	v4.insert(L"\u041f\u0420");//here was cyrrilic symbols: ПР
	v4.insert(L"\u041f\u0410\u0420\u0422");//here was cyrrilic symbols: ПАРТ
	v4.insert(L"\u041c\u0415\u0421\u0422\u041d");//here was cyrrilic symbols: МЕСТН
	v4.insert(L"\u0417\u0412");//here was cyrrilic symbols: ЗВ
	propertiesByGroups.push_back(v4);

	set<wstring> v5;
	v5.insert(L"\u041e\u0414");//here was cyrrilic symbols: ОД
	v5.insert(L"\u041d\u0415\u041e\u0414");//here was cyrrilic symbols: НЕОД
	propertiesByGroups.push_back(v5);

	set<wstring> v7;
	v7.insert(L"1-\u041b");//here was cyrrilic symbols: Л
	v7.insert(L"2-\u041b");//here was cyrrilic symbols: Л
	v7.insert(L"9-\u041b");//here was cyrrilic symbols: Л
	propertiesByGroups.push_back(v7);

	set<wstring> v8;
	v8.insert(L"\u0421\u041e\u0412");//here was cyrrilic symbols: СОВ
	v8.insert(L"\u041d\u0415\u0421\u041e\u0412");//here was cyrrilic symbols: НЕСОВ
	propertiesByGroups.push_back(v8);

	set<wstring> v9;
	v9.insert(L"\u041f\u0420\u041e\u0428");//here was cyrrilic symbols: ПРОШ
	v9.insert(L"\u041d\u0415\u041f\u0420\u041e\u0428");//here was cyrrilic symbols: НЕПРОШ
	v9.insert(L"\u041d\u0410\u0421\u0422");//here was cyrrilic symbols: НАСТ
	propertiesByGroups.push_back(v9);

	set<wstring> v;
	v.insert(L"\u0418\u041d\u0424");//here was cyrrilic symbols: ИНФ
	v.insert(L"\u041f\u0420\u0418\u0427");//here was cyrrilic symbols: ПРИЧ
	v.insert(L"\u0414\u0415\u0415\u041f\u0420");//here was cyrrilic symbols: ДЕЕПР
	propertiesByGroups.push_back(v);

	set<wstring> v10;
	v10.insert(L"\u0421\u0420\u0410\u0412");//here was cyrrilic symbols: СРАВ
	v10.insert(L"\u041f\u0420\u0415\u0412");//here was cyrrilic symbols: ПРЕВ
	propertiesByGroups.push_back(v10);

	set<wstring> v11;
	v11.insert(L"\u0418\u0417\u042a\u042f\u0412");//here was cyrrilic symbols: ИЗЪЯВ
	v11.insert(L"\u041f\u041e\u0412");//here was cyrrilic symbols: ПОВ
	propertiesByGroups.push_back(v11);

	set<wstring> v12;
	v12.insert(L"\u0421\u041b");//here was cyrrilic symbols: СЛ
	v12.insert(L"\u0421\u041c\u042f\u0413");//here was cyrrilic symbols: СМЯГ
	propertiesByGroups.push_back(v12);

	set<wstring> v13;
	v13.insert(L"PEREH");
	v13.insert(L"NEPEREH");
	propertiesByGroups.push_back(v13);
}

vector<PredisambiguatedData> FeatureCalculatorRus::CalculateFeatures(
    const vector<Token>& tokens) const
{
    vector<PredisambiguatedData> predisambiguatedData
        = SimpleFeatureCalculatorRus::CalculateFeatures(tokens);
    for (size_t tokenIndex = 0; tokenIndex < tokens.size(); ++tokenIndex)
	{
        evaluateToken(&predisambiguatedData[tokenIndex]
            , tokens[tokenIndex], tokenIndex);
	}
	addPEREHfeature(&predisambiguatedData);
    return predisambiguatedData;
}

void FeatureCalculatorRus::evaluateToken(
    PredisambiguatedData* predisambiguatedData
    , const Token& token, int tokenIndex) const
{
    // Get all possible properties (S,A,ED, etc).
	vector<vector<wstring> > splittedArray;
	set<wstring> heap;
	getPossibleProperties(predisambiguatedData->features
        , &splittedArray, &heap);
    // Add special features
    if (heap.size() != 1)
    {
	    addSpecialFeature_PropertiesIntersection(
            &(predisambiguatedData->features), splittedArray, heap);
        addSpecialFeature_PossiblePoperties(
            &(predisambiguatedData->features), splittedArray, heap);
        addSpecialFeature_SimpleProperties(
            &(predisambiguatedData->features), splittedArray, heap);
    }
    addPunctuationFeature(
        &(predisambiguatedData->features), token);
    // Add name entity feature
    addNERFeature(predisambiguatedData, token);
    // First upper letter
    if (tokenIndex > 0
        && token.content.size() > 0 && Tools::IsUpper(token.content[0])
        && (token.content.size() < 2 || !Tools::IsUpper(token.content[1])))
    {
        predisambiguatedData->features.push_back(L"IS_UPPER");
    }
}

void FeatureCalculatorRus::getPossibleProperties(
	const vector<wstring>& features
    , vector<vector<wstring> >* splittedArray
    , set<wstring>* heap) const
{
	size_t size = features.size();
	splittedArray->clear();
	splittedArray->resize(size);
	heap->clear();
	for (size_t i = 0; i < size; ++i)
    {
        (*splittedArray)[i] = Tools::Split(features[i], L"@");
        for (size_t j = 0; j < (*splittedArray)[i].size(); ++j)
        {
            (*splittedArray)[i][j] = Tools::Trim((*splittedArray)[i][j]);
			heap->insert((*splittedArray)[i][j]);
		}
	}
}

void FeatureCalculatorRus::addSpecialFeature_PropertiesIntersection(
	vector<wstring>* features
    , const vector<vector<wstring> >& splittedArray
    , const set<wstring>& heap) const
{
	size_t size = features->size();
	if (size < 2) 
    {
        return;
    }
	wstring featureIntersection = L"";
	for (set<wstring>::iterator it = heap.begin(); it != heap.end(); ++it)
	{
		bool shouldAdd = true;
		for (size_t j = 0; j < features->size(); ++j)
        {
            if (!Tools::Contains(splittedArray[j], *it))
            {
				shouldAdd = false;
            }
        }
        if (shouldAdd)
        {
			if (featureIntersection.size() > 0) 
            {
                featureIntersection += L"-";
                featureIntersection += *it;
            }
        }
    }
	if (featureIntersection.size() > 0) 
    {
        features->push_back(featureIntersection);
    }
}

void FeatureCalculatorRus::addSpecialFeature_PossiblePoperties(
	vector<wstring>* features
    , const vector<vector<wstring> >& splittedArray
    , const set<wstring>& heap) const
{
    for (size_t i = 0; i < propertiesByGroups.size(); ++i)
    {
		vector<wstring> toConcatenate;
		for (set<wstring>::iterator it = heap.begin(); it != heap.end(); ++it)
        {
            if (Tools::Contains(propertiesByGroups[i], *it))
            {
				toConcatenate.push_back(*it);
            }
        }
		if (toConcatenate.size() > 1)
		{
			wstring feature = toConcatenate[0];
			for (size_t j = 1; j < toConcatenate.size(); ++j)
				feature += L"U" + toConcatenate[j];
			features->push_back(feature);
		}
	}
}

void FeatureCalculatorRus::addSpecialFeature_SimpleProperties(
	vector<wstring>* features, 
	const vector<vector<wstring>>& splittedArray,
	const set<wstring>& heap) const
{
	for (set<wstring>::iterator it = heap.begin(); it != heap.end(); ++it)
    {
        if (!Tools::Contains(*features, *it))
        {
            features->push_back(*it);
        }
    }
}

void FeatureCalculatorRus::addPunctuationFeature(
    vector<wstring>* features, const Token& token) const
{
    for (size_t punctuationIndex = 0; 
        punctuationIndex < token.punctuation.size();
        ++punctuationIndex)
    {
        features->push_back(Tools::PunctuationToString(
            token.punctuation[punctuationIndex]));
    }
}

bool FeatureCalculatorRus::mayBeVerb(const vector<wstring>& features) const
{
	return features.size() > 0 && features[0][0] == 'V';
}

bool FeatureCalculatorRus::mayBePEREH(const vector<wstring>& features) const
{
    return mayBeVerb(features) && Tools::Contains(features, L"PEREH");
}

bool FeatureCalculatorRus::mayBeNEPEREH(const vector<wstring>& features) const
{
    return mayBeVerb(features) && Tools::Contains(features, L"NEPEREH");
}

void FeatureCalculatorRus::addPEREHfeature(
    vector<PredisambiguatedData>* predisambiguatedData) const
{
     // Left
	size_t length = predisambiguatedData->size();
    vector<int> distanceToNearestVerb_left(length);   
    vector<bool> maybeLeftVerbPEREH(length);
    vector<bool> maybeLeftVerbNEPEREH(length);
    for (size_t i = 0; i < length; i++)
	{
        if (mayBeVerb((*predisambiguatedData)[i].features))
        {
			distanceToNearestVerb_left[i] = 0;
            maybeLeftVerbPEREH[i] = mayBePEREH((*predisambiguatedData)[i].features);
            maybeLeftVerbNEPEREH[i] = mayBeNEPEREH((*predisambiguatedData)[i].features);
        }
        else if (i == 0)
        {
            distanceToNearestVerb_left[i] = MAX_VALUE;
            maybeLeftVerbPEREH[i] = false;
            maybeLeftVerbNEPEREH[i] = false;
		}
        else
        {
            distanceToNearestVerb_left[i] = distanceToNearestVerb_left[i - 1] == MAX_VALUE ? 
                MAX_VALUE : distanceToNearestVerb_left[i - 1] + 1;
            maybeLeftVerbPEREH[i] = maybeLeftVerbPEREH[i - 1];
            maybeLeftVerbNEPEREH[i] = maybeLeftVerbNEPEREH[i - 1];
        }
    }
    for (size_t i = 0; i < length; i++)
    {
        if (distanceToNearestVerb_left[i] == MAX_VALUE) {
			(*predisambiguatedData)[i].features.push_back(L"NO_LEFT_VERB");
        } else if (distanceToNearestVerb_left[i] > 0) {
            if (maybeLeftVerbPEREH[i]) 
            {
				(*predisambiguatedData)[i].features.push_back(L"P_LEFT_VERB");
            } 
            if (maybeLeftVerbNEPEREH[i]) 
            {
                (*predisambiguatedData)[i].features.push_back(L"NEP_LEFT_VERB");
            }
        }
	}
    // Right
    vector<int> distanceToNearestVerb_right(length);
    vector<bool> maybeRightVerbPEREH(length);
    vector<bool> maybeRightVerbNEPEREH(length);
    for (int i = length - 1; i >= 0; i--)
    {
        if (mayBeVerb((*predisambiguatedData)[i].features)) {
            distanceToNearestVerb_right[i] = 0;
            maybeRightVerbPEREH[i] = mayBePEREH((*predisambiguatedData)[i].features);
            maybeRightVerbNEPEREH[i] = mayBeNEPEREH((*predisambiguatedData)[i].features);
        } else if (i + 1 == static_cast<int>(length)) {
            distanceToNearestVerb_right[i] = MAX_VALUE;
            maybeRightVerbPEREH[i] = false;
            maybeRightVerbNEPEREH[i] = false;
        } else {
            distanceToNearestVerb_right[i] = distanceToNearestVerb_right[i + 1] == MAX_VALUE 
				? MAX_VALUE : distanceToNearestVerb_right[i + 1] + 1;
            maybeRightVerbPEREH[i] = maybeRightVerbPEREH[i + 1];
            maybeRightVerbNEPEREH[i] = maybeRightVerbNEPEREH[i + 1];
		}
    }
    for (size_t i = 0; i < length; i++)
    {
        if (distanceToNearestVerb_right[i] == MAX_VALUE) {
			(*predisambiguatedData)[i].features.push_back(L"NO_RIGHT_VERB");
        } else if (distanceToNearestVerb_right[i] > 0) {
            if (maybeRightVerbPEREH[i])
            {
				(*predisambiguatedData)[i].features.push_back(L"P_RIGHT_VERB");
            }
            if (maybeRightVerbNEPEREH[i])
            {
                (*predisambiguatedData)[i].features.push_back(L"NEP_RIGHT_VERB");
            }
		}
	}
    // Nearest;
    for (size_t i = 0; i < length; i++)
    {
        if (distanceToNearestVerb_left[i] < distanceToNearestVerb_right[i])
        {
            if (distanceToNearestVerb_left[i] > 0)
            {
				if (maybeLeftVerbPEREH[i]) (*predisambiguatedData)[i].features.push_back(L"P_NEAREST");
				if (maybeLeftVerbNEPEREH[i]) (*predisambiguatedData)[i].features.push_back(L"NEP_NEAREST");
            }
        } else if (distanceToNearestVerb_left[i] > distanceToNearestVerb_right[i]) {
            if (distanceToNearestVerb_right[i] > 0)
            {
				if (maybeRightVerbPEREH[i]) (*predisambiguatedData)[i].features.push_back(L"P_NEAREST");
				if (maybeRightVerbNEPEREH[i]) (*predisambiguatedData)[i].features.push_back(L"NEP_NEAREST");
            }
        } else if (distanceToNearestVerb_left[i] > 0) {
			if (maybeRightVerbPEREH[i] || maybeLeftVerbPEREH[i]) 
            {
               (*predisambiguatedData)[i].features.push_back(L"P_NEAREST");
            }
			if (maybeRightVerbNEPEREH[i] || maybeLeftVerbNEPEREH[i]) 
            {
                (*predisambiguatedData)[i].features.push_back(L"NEP_NEAREST");
            }
        }
    }
}

void FeatureCalculatorRus::addNERFeature(PredisambiguatedData* predisambiguatedData
    , const Token& token) const
{
    bool addedName = false;
    bool addedSurname = false;
    for (auto iter = predisambiguatedData->gramInfo.begin()
         ; iter != predisambiguatedData->gramInfo.end(); ++iter)
    {
        for (size_t gramIndex = 0; gramIndex < iter->second.size(); ++gramIndex)
        {
            const vector<shared_ptr<wstring> >& features = iter->second[gramIndex]->descriptions;
            for (size_t featIndex = 0; featIndex < features.size(); ++featIndex)
            {
                if (*(features[featIndex]) == L"\u0438\u043c\u044f") {//here was cyrrilic symbols: имя
                    if (!addedName)
                    {
                        predisambiguatedData->features.push_back(L"NAME");
                        addedName = true;
                    }
                } else if (*(features[featIndex]) == L"\u0444\u0430\u043c\u0438\u043b\u0438\u044f") {//here was cyrrilic symbols: фамилия
                    if (!addedSurname)
                    {
                        predisambiguatedData->features.push_back(L"SURNAME");
                        addedSurname = true;
                    }
                }
            }
        }
    }
}

}
