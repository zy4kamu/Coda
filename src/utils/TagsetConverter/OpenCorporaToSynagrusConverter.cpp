/** 
 * @file	OpenCorporaToSynagrusConverter.cpp
 * @brief	source file for converter from OpenCorpora properties to Synatgrus properties
 */

#include "OpenCorporaToSynagrusConverter.h"
#include "FileReader.h"
#include "Tools.h"

using namespace TagsetConverting;

OpenCorporaToSynagrusConverter::OpenCorporaToSynagrusConverter(
    const string& fileConverter)
{
	try
	{
		FileManager::Read(fileConverter, &opCorpPropertyTosyntagrusProperty, L',');
	}
	catch (...)
	{
		wstring message = L"OpenCorporaToSynagrusConverter: catch (...) file is corrupted";;
		throw (message);
	}
    vector<wstring> keys = Tools::Keys(opCorpPropertyTosyntagrusProperty);
	for (size_t i = 0; i < keys.size(); i++)
        opCorpPropertyTosyntagrusProperty[keys[i]] = Tools::Trim(opCorpPropertyTosyntagrusProperty[keys[i]]);
	
	vector<wstring> vec0;
	vec0.push_back(L"\u0415\u0414");//here was cyrrilic symbols: ЕД
	vec0.push_back(L"\u041c\u041d");//here was cyrrilic symbols: МН
	vec0.push_back(L"\u041c\u0423\u0416");//here was cyrrilic symbols: МУЖ
	vec0.push_back(L"\u0416\u0415\u041d");//here was cyrrilic symbols: ЖЕН
	vec0.push_back(L"\u0421\u0420\u0415\u0414");//here was cyrrilic symbols: СРЕД
	vec0.push_back(L"\u0418\u041c");//here was cyrrilic symbols: ИМ
	vec0.push_back(L"\u0420\u041e\u0414");//here was cyrrilic symbols: РОД
	vec0.push_back(L"\u0414\u0410\u0422");//here was cyrrilic symbols: ДАТ
	vec0.push_back(L"\u0412\u0418\u041d");//here was cyrrilic symbols: ВИН
	vec0.push_back(L"\u0422\u0412\u041e\u0420");//here was cyrrilic symbols: ТВОР
	vec0.push_back(L"\u041f\u0420");//here was cyrrilic symbols: ПР
	vec0.push_back(L"\u041f\u0410\u0420\u0422");//here was cyrrilic symbols: ПАРТ
	vec0.push_back(L"\u041c\u0415\u0421\u0422\u041d");//here was cyrrilic symbols: МЕСТН
	vec0.push_back(L"\u0417\u0412");//here was cyrrilic symbols: ЗВ
	vec0.push_back(L"\u041e\u0414");//here was cyrrilic symbols: ОД
	vec0.push_back(L"\u041d\u0415\u041e\u0414");//here was cyrrilic symbols: НЕОД
	pos_orders.insert(std::make_pair(L"S", vec0));

	vector<wstring> vec1;
	vec1.push_back(L"\u041a\u0420");//here was cyrrilic symbols: КР
	vec1.push_back(L"\u041f\u0420\u0415\u0412");//here was cyrrilic symbols: ПРЕВ
	vec1.push_back(L"\u0415\u0414");//here was cyrrilic symbols: ЕД
	vec1.push_back(L"\u041c\u041d");//here was cyrrilic symbols: МН
	vec1.push_back(L"\u041c\u0423\u0416");//here was cyrrilic symbols: МУЖ
	vec1.push_back(L"\u0416\u0415\u041d");//here was cyrrilic symbols: ЖЕН
	vec1.push_back(L"\u0421\u0420\u0415\u0414");//here was cyrrilic symbols: СРЕД
	vec1.push_back(L"\u0421\u0420\u0410\u0412");//here was cyrrilic symbols: СРАВ
	vec1.push_back(L"\u0421\u041c\u042f\u0413");//here was cyrrilic symbols: СМЯГ
	vec1.push_back(L"\u0418\u041c");//here was cyrrilic symbols: ИМ
	vec1.push_back(L"\u0420\u041e\u0414");//here was cyrrilic symbols: РОД
	vec1.push_back(L"\u0414\u0410\u0422");//here was cyrrilic symbols: ДАТ
	vec1.push_back(L"\u0412\u0418\u041d");//here was cyrrilic symbols: ВИН
	vec1.push_back(L"\u0422\u0412\u041e\u0420");//here was cyrrilic symbols: ТВОР
	vec1.push_back(L"\u041f\u0420");//here was cyrrilic symbols: ПР
	vec1.push_back(L"\u041f\u0410\u0420\u0422");//here was cyrrilic symbols: ПАРТ
	vec1.push_back(L"\u041c\u0415\u0421\u0422\u041d");//here was cyrrilic symbols: МЕСТН
	vec1.push_back(L"\u0417\u0412");//here was cyrrilic symbols: ЗВ
	vec1.push_back(L"\u041e\u0414");//here was cyrrilic symbols: ОД
	vec1.push_back(L"\u041d\u0415\u041e\u0414");//here was cyrrilic symbols: НЕОД
	pos_orders.insert(std::make_pair(L"A", vec1));

	vector<wstring> vec2;
	vec2.push_back(L"\u0421\u041e\u0412");//here was cyrrilic symbols: СОВ
	vec2.push_back(L"\u041d\u0415\u0421\u041e\u0412");//here was cyrrilic symbols: НЕСОВ
	vec2.push_back(L"\u0421\u0422\u0420\u0410\u0414");//here was cyrrilic symbols: СТРАД
	vec2.push_back(L"\u041f\u0420\u0418\u0427");//here was cyrrilic symbols: ПРИЧ
	vec2.push_back(L"\u0414\u0415\u0415\u041f\u0420");//here was cyrrilic symbols: ДЕЕПР
	vec2.push_back(L"\u0418\u0417\u042a\u042f\u0412");//here was cyrrilic symbols: ИЗЪЯВ
	vec2.push_back(L"\u041f\u041e\u0412");//here was cyrrilic symbols: ПОВ
	vec2.push_back(L"\u041f\u0420\u041e\u0428");//here was cyrrilic symbols: ПРОШ
	vec2.push_back(L"\u041d\u0415\u041f\u0420\u041e\u0428");//here was cyrrilic symbols: НЕПРОШ
	vec2.push_back(L"\u041a\u0420");//here was cyrrilic symbols: КР
	vec2.push_back(L"\u041c\u041d");//here was cyrrilic symbols: МН
	vec2.push_back(L"\u0415\u0414");//here was cyrrilic symbols: ЕД
	vec2.push_back(L"1-\u041b");//here was cyrrilic symbols: Л
	vec2.push_back(L"2-\u041b");//here was cyrrilic symbols: Л
	vec2.push_back(L"3-\u041b");//here was cyrrilic symbols: Л
	vec2.push_back(L"\u041c\u0423\u0416");//here was cyrrilic symbols: МУЖ
	vec2.push_back(L"\u0416\u0415\u041d");//here was cyrrilic symbols: ЖЕН
	vec2.push_back(L"\u0421\u0420\u0415\u0414");//here was cyrrilic symbols: СРЕД
	vec2.push_back(L"\u0418\u041d\u0424");//here was cyrrilic symbols: ИНФ
	vec2.push_back(L"\u0418\u041c");//here was cyrrilic symbols: ИМ
	vec2.push_back(L"\u0420\u041e\u0414");//here was cyrrilic symbols: РОД
	vec2.push_back(L"\u0414\u0410\u0422");//here was cyrrilic symbols: ДАТ
	vec2.push_back(L"\u0412\u0418\u041d");//here was cyrrilic symbols: ВИН
	vec2.push_back(L"\u0422\u0412\u041e\u0420");//here was cyrrilic symbols: ТВОР
	vec2.push_back(L"\u041f\u0420");//here was cyrrilic symbols: ПР
	vec2.push_back(L"\u041f\u0410\u0420\u0422");//here was cyrrilic symbols: ПАРТ
	vec2.push_back(L"\u041c\u0415\u0421\u0422\u041d");//here was cyrrilic symbols: МЕСТН
	vec2.push_back(L"\u0417\u0412");//here was cyrrilic symbols: ЗВ
	vec2.push_back(L"\u041e\u0414");//here was cyrrilic symbols: ОД
	vec2.push_back(L"\u041d\u0415\u041e\u0414");//here was cyrrilic symbols: НЕОД
	pos_orders.insert(std::make_pair(L"V", vec2));

	vector<wstring> vec3;
	vec3.push_back(L"\u0421\u0420\u0410\u0412");//here was cyrrilic symbols: СРАВ
	vec3.push_back(L"\u0421\u041c\u042f\u0413");//here was cyrrilic symbols: СМЯГ
	pos_orders.insert(std::make_pair(L"ADV", vec3));

	vector<wstring> vec4;
	vec4.push_back(L"\u0415\u0414");//here was cyrrilic symbols: ЕД
	vec4.push_back(L"\u041c\u041d");//here was cyrrilic symbols: МН
	vec4.push_back(L"\u041c\u0423\u0416");//here was cyrrilic symbols: МУЖ
	vec4.push_back(L"\u0416\u0415\u041d");//here was cyrrilic symbols: ЖЕН
	vec4.push_back(L"\u0421\u0420\u0415\u0414");//here was cyrrilic symbols: СРЕД
	vec4.push_back(L"\u0418\u041c");//here was cyrrilic symbols: ИМ
	vec4.push_back(L"\u0420\u041e\u0414");//here was cyrrilic symbols: РОД
	vec4.push_back(L"\u0414\u0410\u0422");//here was cyrrilic symbols: ДАТ
	vec4.push_back(L"\u0412\u0418\u041d");//here was cyrrilic symbols: ВИН
	vec4.push_back(L"\u0422\u0412\u041e\u0420");//here was cyrrilic symbols: ТВОР
	vec4.push_back(L"\u041f\u0420");//here was cyrrilic symbols: ПР
	vec4.push_back(L"\u041f\u0410\u0420\u0422");//here was cyrrilic symbols: ПАРТ
	vec4.push_back(L"\u041c\u0415\u0421\u0422\u041d");//here was cyrrilic symbols: МЕСТН
	vec4.push_back(L"\u0417\u0412");//here was cyrrilic symbols: ЗВ
	vec4.push_back(L"\u041e\u0414");//here was cyrrilic symbols: ОД
	vec4.push_back(L"\u041d\u0415\u041e\u0414");//here was cyrrilic symbols: НЕОД
	pos_orders.insert(std::make_pair(L"NUM", vec4));

	vector<wstring> vec5;
	pos_orders.insert(std::make_pair(L"PR", vec5));

	vector<wstring> vec6;
	pos_orders.insert(std::make_pair(L"CONJ", vec6));

	vector<wstring> vec7;
	pos_orders.insert(std::make_pair(L"PART", vec7));

	vector<wstring> vec8;
	pos_orders.insert(std::make_pair(L"INTJ", vec8));
}


OpenCorporaToSynagrusConverter::~OpenCorporaToSynagrusConverter(void)
{
}

vector<wstring> OpenCorporaToSynagrusConverter::PartialConvert(const wstring& openCorpString)
{
    vector<wstring> converted;
	// Checking for mistakes;
    if (Tools::ContainsSubstring(openCorpString, L"\u043e\u043f\u0435\u0447\u0430\u0442\u043a\u0430") || Tools::Trim(openCorpString) == L"UNKNOWN")//here was cyrrilic symbols: опечатка
	{
		converted.push_back(L"UNKNOWN");
	    return converted;
	}
	// Working with gender disambiguity;
    if (Tools::ContainsSubstring(openCorpString, L"\u0440\u043e\u0434 / \u0440\u043e\u0434 \u043d\u0435 \u0432\u044b\u0440\u0430\u0436\u0435\u043d"))//here was cyrrilic symbols: род,род,не,выражен
    {
		converted = PartialConvert(
            Tools::Replace(openCorpString, L"\u0440\u043e\u0434 / \u0440\u043e\u0434 \u043d\u0435 \u0432\u044b\u0440\u0430\u0436\u0435\u043d", L"\u043c\u0443\u0436\u0441\u043a\u043e\u0439 \u0440\u043e\u0434"));//here was cyrrilic symbols: род,род,не,выражен,мужской,род

        vector<wstring> toAppend1 = 
            PartialConvert(Tools::Replace(openCorpString, L"\u0440\u043e\u0434 / \u0440\u043e\u0434 \u043d\u0435 \u0432\u044b\u0440\u0430\u0436\u0435\u043d", L"\u0436\u0435\u043d\u0441\u043a\u0438\u0439 \u0440\u043e\u0434"));//here was cyrrilic symbols: род,род,не,выражен,женский,род
        Tools::Append(&converted, toAppend1);

        vector<wstring> toAppend2 = 
            PartialConvert(Tools::Replace(openCorpString, L"\u0440\u043e\u0434 / \u0440\u043e\u0434 \u043d\u0435 \u0432\u044b\u0440\u0430\u0436\u0435\u043d", L"\u0441\u0440\u0435\u0434\u043d\u0438\u0439 \u0440\u043e\u0434"));//here was cyrrilic symbols: род,род,не,выражен,средний,род
        Tools::Append(&converted, toAppend2);

        vector<wstring> distinct = Tools::Distinct(converted);
        return distinct;
    }
	// Working with animacy;
    if (Tools::ContainsSubstring(openCorpString, L"\u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0441\u0442\u044c / \u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0441\u0442\u044c \u043d\u0435 \u0432\u044b\u0440\u0430\u0436\u0435\u043d\u0430"))//here was cyrrilic symbols: одушевлённость,одушевлённость,не,выражена
    {
		converted = PartialConvert(
            Tools::Replace(openCorpString, L"\u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0441\u0442\u044c / \u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0441\u0442\u044c \u043d\u0435 \u0432\u044b\u0440\u0430\u0436\u0435\u043d\u0430", L"\u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0435"));//here was cyrrilic symbols: одушевлённость,одушевлённость,не,выражена,одушевлённое
        vector<wstring> toAppend = PartialConvert(
            Tools::Replace(openCorpString, L"\u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0441\u0442\u044c / \u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0441\u0442\u044c \u043d\u0435 \u0432\u044b\u0440\u0430\u0436\u0435\u043d\u0430", L"\u043d\u0435\u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0435"));//here was cyrrilic symbols: одушевлённость,одушевлённость,не,выражена,неодушевлённое
        Tools::Append(&converted, toAppend);
        vector<wstring> distinct = Tools::Distinct(converted);
        return distinct;
    }
	// Find possible syntagrus properties for openCorpString;
    vector<wstring> splitted = Tools::Split(openCorpString, L" - ");
	set<wstring> syntagrusProperties;
	wstring answerString = L"";
	for (size_t i = 0; i < splitted.size(); ++i)
    {
        wstring current = Tools::Trim(splitted[i]);
        if (Tools::ContainsKey(
            opCorpPropertyTosyntagrusProperty, current))
        {
            vector<wstring> splitted1 = Tools::Split(
                opCorpPropertyTosyntagrusProperty[current], L"@");
			if (answerString == L"") answerString = splitted1[0];
			for (size_t j = 0; j < splitted1.size(); ++j)
				syntagrusProperties.insert(splitted1[j]);
        }
    }
	if (syntagrusProperties.size() == 0) 
    {
        return converted;
    }
	// Get possible properties for current token;
    vector<wstring> posProp = pos_orders[answerString];
	// Find intersection between posProp and syntagrusProperties;
	// Preparing return value;
	for (size_t i = 0; i < posProp.size(); ++i)
    {
		if (syntagrusProperties.find(posProp[i]) 
            != syntagrusProperties.end())
        {
			answerString += L"@"; 
			answerString += posProp[i];
		}
    }
	converted.push_back(answerString);
	// Some special preparations;
    if (Tools::ContainsSubstring(openCorpString, L"\u043f\u0440\u0438\u0447\u0430\u0441\u0442\u0438\u0435")//here was cyrrilic symbols: причастие
        && Tools::ContainsSubstring(openCorpString, L"\u043d\u0430\u0441\u0442\u043e\u044f\u0449\u0435\u0435 \u0432\u0440\u0435\u043c\u044f"))//here was cyrrilic symbols: настоящее,время
    {
        wstring retvalString1 = Tools::Replace(answerString, L"\u041d\u0415\u041f\u0420\u041e\u0428", L"\u041f\u0420\u041e\u0428");//here was cyrrilic symbols: НЕПРОШ,ПРОШ
		if (answerString != retvalString1) converted.push_back(retvalString1);
    }
    // Transitivity
    if (Tools::ContainsSubstring(openCorpString, L"\u043d\u0435\u043f\u0435\u0440\u0435\u0445\u043e\u0434\u043d\u044b\u0439"))//here was cyrrilic symbols: непереходный
	{
		converted[0] += L"@NEPEREH";
	}
    if (Tools::ContainsSubstring(openCorpString, L"\u043f\u0435\u0440\u0435\u0445\u043e\u0434\u043d\u044b\u0439")//here was cyrrilic symbols: переходный
        && !Tools::ContainsSubstring(openCorpString, L"\u043d\u0435\u043f\u0435\u0440\u0435\u0445\u043e\u0434\u043d\u044b\u0439"))//here was cyrrilic symbols: непереходный
	{
		converted[0] += L"@PEREH";
	}
    return converted;
} 
