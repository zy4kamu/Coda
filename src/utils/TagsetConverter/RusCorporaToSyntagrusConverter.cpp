#include "RusCorporaToSyntagrusConverter.h"
#include "FileReader.h"
#include "Tools.h"

using namespace TagsetConverting;


RusCorporaToSyntagrusConverter::RusCorporaToSyntagrusConverter(
    const string& file)
{
	FileManager::Read(file, &mapConverter, L',');

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


RusCorporaToSyntagrusConverter::~RusCorporaToSyntagrusConverter(void)
{
}

vector<wstring> RusCorporaToSyntagrusConverter::PartialConvert(const wstring& tag)
{
	vector<wstring> convertedTags;
	vector<wstring> splitted;
	set<wchar_t> delimeters;
	delimeters.insert(L',');
	delimeters.insert(L'=');
    splitted = Tools::Split(tag, delimeters);
	for (size_t i = 0; i < splitted.size(); ++i)
    {
        splitted[i] = Tools::Trim(splitted[i]);
    }

	wstring retval = mapConverter[splitted[0]];
	vector<wstring>& orders = pos_orders[retval];

	set<wstring> syntagProps;
	for (size_t i = 1; i < splitted.size(); ++i)
	{
		if (mapConverter.find(splitted[i]) != mapConverter.end())
        {
			syntagProps.insert(mapConverter[splitted[i]]);
        }
	}

	for (size_t i = 0; i < orders.size(); ++i)
	{
		if (syntagProps.find(orders[i]) != syntagProps.end())
        {
			retval += L"@" + orders[i];
        }
	}
	convertedTags.push_back(retval);
	
    if (Tools::StartsWith(retval, L"A@") && Tools::ContainsSubstring(retval, L"\u0412\u0418\u041d") && Tools::ContainsSubstring(retval, L"@\u041d\u0415\u041e\u0414"))//here was cyrrilic symbols: ВИН,НЕОД
    {
        convertedTags.push_back(Tools::Replace(retval, L"@\u041d\u0415\u041e\u0414", L""));//here was cyrrilic symbols: НЕОД
    }
    if (Tools::StartsWith(retval, L"A@") && Tools::ContainsSubstring(retval, L"\u0412\u0418\u041d") && Tools::ContainsSubstring(retval, L"@\u041e\u0414"))//here was cyrrilic symbols: ВИН,ОД
    {
        convertedTags.push_back(Tools::Replace(retval, L"@\u041e\u0414", L""));//here was cyrrilic symbols: ОД
    }
    if (Tools::StartsWith(retval, L"A@") && Tools::ContainsSubstring(retval, L"\u0412\u0418\u041d") &&//here was cyrrilic symbols: ВИН
        !Tools::ContainsSubstring(retval, L"@\u041e\u0414") && !Tools::ContainsSubstring(retval, L"@\u041d\u0415\u041e\u0414"))//here was cyrrilic symbols: ОД,НЕОД
	{
		convertedTags.push_back(retval + L"@\u041e\u0414");//here was cyrrilic symbols: ОД
		convertedTags.push_back(retval + L"@\u041d\u0415\u041e\u0414");//here was cyrrilic symbols: НЕОД
	}

    if (Tools::StartsWith(retval, L"NUM@") && Tools::ContainsSubstring(retval, L"\u0412\u0418\u041d") && Tools::ContainsSubstring(retval, L"@\u041d\u0415\u041e\u0414"))//here was cyrrilic symbols: ВИН,НЕОД
    {
        convertedTags.push_back(Tools::Replace(retval, L"@\u041d\u0415\u041e\u0414", L""));//here was cyrrilic symbols: НЕОД
    }
    if (Tools::StartsWith(retval, L"NUM@") && Tools::ContainsSubstring(retval, L"\u0412\u0418\u041d") && Tools::ContainsSubstring(retval, L"@\u041e\u0414"))//here was cyrrilic symbols: ВИН,ОД
    {
        convertedTags.push_back(Tools::Replace(retval, L"@\u041e\u0414", L""));//here was cyrrilic symbols: ОД
    }
    if (Tools::StartsWith(retval, L"NUM@") && Tools::ContainsSubstring(retval, L"\u0412\u0418\u041d") &&//here was cyrrilic symbols: ВИН
        !Tools::ContainsSubstring(retval, L"@\u041e\u0414") && !Tools::ContainsSubstring(retval, L"@\u041d\u0415\u041e\u0414"))//here was cyrrilic symbols: ОД,НЕОД
	{
		convertedTags.push_back(retval + L"@\u041e\u0414");//here was cyrrilic symbols: ОД
		convertedTags.push_back(retval + L"@\u041d\u0415\u041e\u0414");//here was cyrrilic symbols: НЕОД
	}

    if (Tools::StartsWith(retval, L"V@") && Tools::ContainsSubstring(retval, L"\u0412\u0418\u041d") && Tools::ContainsSubstring(retval, L"\u041f\u0420\u0418\u0427") && Tools::ContainsSubstring(retval, L"@\u041d\u0415\u041e\u0414"))//here was cyrrilic symbols: ВИН,ПРИЧ,НЕОД
    {
        convertedTags.push_back(Tools::Replace(retval, L"@\u041d\u0415\u041e\u0414", L""));//here was cyrrilic symbols: НЕОД
    }
    if (Tools::StartsWith(retval, L"V@") && Tools::ContainsSubstring(retval, L"\u0412\u0418\u041d")  && Tools::ContainsSubstring(retval, L"\u041f\u0420\u0418\u0427") && Tools::ContainsSubstring(retval, L"@\u041e\u0414"))//here was cyrrilic symbols: ВИН,ПРИЧ,ОД
    {
        convertedTags.push_back(Tools::Replace(retval, L"@\u041e\u0414", L""));//here was cyrrilic symbols: ОД
    }
    if (Tools::StartsWith(retval, L"V@") && Tools::ContainsSubstring(retval, L"\u0412\u0418\u041d") &&//here was cyrrilic symbols: ВИН
        !Tools::ContainsSubstring(retval, L"@\u041e\u0414") && !Tools::ContainsSubstring(retval, L"@\u041d\u0415\u041e\u0414"))//here was cyrrilic symbols: ОД,НЕОД
	{
		convertedTags.push_back(retval + L"@\u041e\u0414");//here was cyrrilic symbols: ОД
		convertedTags.push_back(retval + L"@\u041d\u0415\u041e\u0414");//here was cyrrilic symbols: НЕОД
	}
    return convertedTags;
}

vector<wstring> RusCorporaToSyntagrusConverter::PartialConvert(
    const wstring& token, const wstring& tag)
{
    vector<wstring> convertedTags;
    wstring tokenToLower = Tools::ToLower(token);
    convertedTags = PartialConvert(tag);
	correctPredictedLabel(tokenToLower, convertedTags);
    return convertedTags;
}

void RusCorporaToSyntagrusConverter::correctPredictedLabel(
    const wstring& token, vector<wstring>& convertedTags)
{
	for (size_t i = 0; i < convertedTags.size(); ++i)
	{
		wstring& label = convertedTags[i];

		if (token == L"\u043e\u043d" || token == L"\u041e\u043d") {//here was cyrrilic symbols: он,Он
				label = L"S@\u0415\u0414@\u041c\u0423\u0416@\u0418\u041c@\u041e\u0414";//here was cyrrilic symbols: ЕД,МУЖ,ИМ,ОД
        } else if (token == L"\u043e\u043d\u0430" || token == L"\u041e\u043d\u0430") {//here was cyrrilic symbols: она,Она
			label = L"S@\u0415\u0414@\u0416\u0415\u041d@\u0418\u041c@\u041e\u0414";//here was cyrrilic symbols: ЕД,ЖЕН,ИМ,ОД
        } else if (token == L"\u043c\u043d\u043e\u0439" || token == L"\u041c\u043d\u043e\u0439") {//here was cyrrilic symbols: мной,Мной
			label = L"S@\u0415\u0414@\u0422\u0412\u041e\u0420@\u041e\u0414";//here was cyrrilic symbols: ЕД,ТВОР,ОД
		} else if (token == L"\u042f" || token == L"\u044f") {//here was cyrrilic symbols: Я,я
			label = L"S@\u0415\u0414@\u0418\u041c@\u041e\u0414";//here was cyrrilic symbols: ЕД,ИМ,ОД
		} else if (token == L"\u041c\u044b" || token == L"\u043c\u044b") {//here was cyrrilic symbols: Мы,мы
			label = L"S@\u041c\u041d@\u0418\u041c@\u041e\u0414";//here was cyrrilic symbols: МН,ИМ,ОД
		} else if (token == L"\u041e\u043d\u0438" || token == L"\u043e\u043d\u0438") {//here was cyrrilic symbols: Они,они
			label = L"S@\u041c\u041d@\u0418\u041c@\u041e\u0414";//here was cyrrilic symbols: МН,ИМ,ОД
		} else if (token == L"\u0422\u044b" || token == L"\u0442\u044b") {//here was cyrrilic symbols: Ты,ты
			label = L"S@\u0415\u0414@\u0418\u041c@\u041e\u0414";//here was cyrrilic symbols: ЕД,ИМ,ОД
		} else if (token == L"\u0412\u044b" || token == L"\u0432\u044b") {//here was cyrrilic symbols: Вы,вы
			label = L"S@\u041c\u041d@\u0418\u041c@\u041e\u0414";//here was cyrrilic symbols: МН,ИМ,ОД
		} else if (token == L"\u043d\u0435\u0442") {//here was cyrrilic symbols: нет
			label = L"PART";
		} else if (token == L"\u041d\u0435\u0442") {//here was cyrrilic symbols: Нет
			label = L"PART";
		} else if (token == L"\u043d\u0435\u043c\u043d\u043e\u0433\u043e") {//here was cyrrilic symbols: немного
			label = L"ADV";
        }

		if (token == L"\u0434\u0432\u0435" && label == L"NUM@\u0416\u0415\u041d@\u0412\u0418\u041d") {//here was cyrrilic symbols: две,ЖЕН,ВИН
			label = L"NUM@\u0416\u0415\u041d@\u0412\u0418\u041d@\u041d\u0415\u041e\u0414";//here was cyrrilic symbols: ЖЕН,ВИН,НЕОД
        } else if (token == L"\u0414\u0432\u0435" && label == L"NUM@\u0416\u0415\u041d@\u0412\u0418\u041d") {//here was cyrrilic symbols: Две,ЖЕН,ВИН
			label = L"NUM@\u0416\u0415\u041d@\u0412\u0418\u041d@\u041d\u0415\u041e\u0414";//here was cyrrilic symbols: ЖЕН,ВИН,НЕОД
		} else if (token == L"\u0441\u0435\u0431\u044f" && label == L"A@\u0412\u0418\u041d") {//here was cyrrilic symbols: себя,ВИН
			label = L"S@\u0415\u0414@\u0412\u0418\u041d@\u041e\u0414";//here was cyrrilic symbols: ЕД,ВИН,ОД
		} else if (token == L"\u0441\u0435\u0431\u044f" && label == L"A@\u0420\u041e\u0414") {//here was cyrrilic symbols: себя,РОД
			label = L"S@\u0415\u0414@\u0420\u041e\u0414@\u041e\u0414";//here was cyrrilic symbols: ЕД,РОД,ОД
		} else if (token == L"\u0447\u0442\u043e" && label == L"PART") {//here was cyrrilic symbols: что
			label = L"ADV";
		} else if (token == L"\u0447\u0442\u043e" && label == L"A@\u0415\u0414@\u0421\u0420\u0415\u0414@\u0418\u041c") {//here was cyrrilic symbols: что,ЕД,СРЕД,ИМ
			label = L"S@\u0415\u0414@\u0421\u0420\u0415\u0414@\u0418\u041c@\u041e\u0414";//here was cyrrilic symbols: ЕД,СРЕД,ИМ,ОД
		} else if (token == L"\u0447\u0442\u043e" && label == L"A@\u0415\u0414@\u0421\u0420\u0415\u0414@\u0412\u0418\u041d") {//here was cyrrilic symbols: что,ЕД,СРЕД,ВИН
			label = L"S@\u0415\u0414@\u0421\u0420\u0415\u0414@\u0412\u0418\u041d@\u041e\u0414";//here was cyrrilic symbols: ЕД,СРЕД,ВИН,ОД
		} else if (token == L"\u043d\u0438\u0447\u0435\u0433\u043e" && label == L"A@\u0415\u0414@\u0421\u0420\u0415\u0414@\u0420\u041e\u0414") {//here was cyrrilic symbols: ничего,ЕД,СРЕД,РОД
			label = L"A@\u0415\u0414@\u0421\u0420\u0415\u0414@\u0420\u041e\u0414@\u041e\u0414";//here was cyrrilic symbols: ЕД,СРЕД,РОД,ОД
		} else if (token == L"\u0432\u0441\u0451" && label == L"A@\u0415\u0414@\u0421\u0420\u0415\u0414@\u0412\u0418\u041d@\u041d\u0415\u041e\u0414") {//here was cyrrilic symbols: всё,ЕД,СРЕД,ВИН,НЕОД
			label = L"A@\u0415\u0414@\u0421\u0420\u0415\u0414@\u0412\u0418\u041d";//here was cyrrilic symbols: ЕД,СРЕД,ВИН
        }

		if (token == L"\u0435\u043c\u0443" || token == L"\u0438\u043c" || token == L"\u043c\u043d\u0435" ||//here was cyrrilic symbols: ему,им,мне
			token == L"\u043c\u0435\u043d\u044f" || token == L"\u0442\u0435\u0431\u044f" || token == L"\u0447\u0442\u043e-\u043d\u0438\u0431\u0443\u0434\u044c" ||//here was cyrrilic symbols: меня,тебя,что,нибудь
			token == L"\u043d\u0438\u0447\u0435\u0433\u043e" || token == L"\u0447\u0435\u043c" || token == L"\u0435\u0439" ||//here was cyrrilic symbols: ничего,чем,ей
			token == L"\u043d\u0430\u043c" || token == L"\u043d\u0451\u043c" || token == L"\u043d\u0430\u0441" || token == L"\u043d\u0435\u0451" ||//here was cyrrilic symbols: нам,нём,нас,неё
			token == L"\u043a\u0442\u043e" || token == L"\u043d\u0438\u0445" || token == L"\u0432\u0430\u0441")//here was cyrrilic symbols: кто,них,вас
		{
            label = Tools::Replace(label, L"A@", L"S@");
            if (!Tools::ContainsSubstring(label, L"@\u041e\u0414")) label += L"@\u041e\u0414";//here was cyrrilic symbols: ОД,ОД
		}
	}
}
