#include "StresserRus.h"
#include "FileReader.h"
#include "Tools.h"

namespace Stresser
{

StresserRus::StresserRus(
    const string& folder)
{
	FileManager::Read(folder + "Tokens_NonAmbig.txt", &tokens_NonAmbig, L'\t');
	prepareAmbigByMorphologyTokenData(
		folder + "Tokens_AmbiguatedByMorphology.txt");
	prepareNonAmbigByMorphologyTokenData(
		folder + "Tokens_NonAmbiguatedByMorphology.txt");
}

vector<wstring> StresserRus::SetStress(
    const vector<DisambiguatedData>& disambiguatedData)
{
    vector<wstring> stress(disambiguatedData.size());
	for (size_t tokenIndex = 0; tokenIndex < disambiguatedData.size();
        ++tokenIndex)
	{
        const wstring& currenToken = disambiguatedData[tokenIndex].content;
        const wstring& currentag = disambiguatedData[tokenIndex].label;
        const wstring& previousToken = 
            (tokenIndex > 0) 
            ? disambiguatedData[tokenIndex - 1].content : L"";
        const wstring& previousTag = 
            (tokenIndex > 0) 
            ? disambiguatedData[tokenIndex - 1].label : L"";
        const wstring& nextToken = 
            (tokenIndex + 1 < disambiguatedData.size()) 
            ? disambiguatedData[tokenIndex + 1].content : L"";
        const wstring& nextTag = 
            (tokenIndex + 1 < disambiguatedData.size()) 
            ? disambiguatedData[tokenIndex + 1].label : L"";
		stress[tokenIndex] = setStress(
            currenToken
            , currentag
            , previousToken
            , previousTag
			, nextToken
            , nextTag);
	}
    return stress;
}

void StresserRus::prepareAmbigByMorphologyTokenData(
    const string& file)
{
	wifstream in(file);
	wstring s;
	wstring curToken;
	unordered_map<wstring, wstring>* morphem_Stress;
    vector<wstring> splitted;
	set<wchar_t> delimeters;
	delimeters.insert(L'\t'); 
	delimeters.insert(L';');

	getline(in, curToken);
	tokens_AmbigByMorpology[curToken] = unordered_map<wstring,wstring>();
	morphem_Stress = &tokens_AmbigByMorpology[curToken];
	while (getline(in, s))
	{
        s = Tools::Trim(s);
		if (s.size() == 0)
		{
			getline(in, curToken);
			tokens_AmbigByMorpology[curToken] = unordered_map<wstring,wstring>();
			morphem_Stress = &tokens_AmbigByMorpology[curToken];
		}
		else
		{
            splitted = Tools::Split(s, delimeters);
            for (size_t i = 1; i < splitted.size(); ++i)
            {
                (*morphem_Stress)[splitted[i]] = splitted[0];
            }
		}
	}
	in.close();
}

void StresserRus::prepareNonAmbigByMorphologyTokenData(
    const string& file)
{
	wifstream in(file);
	wstring s;
	wstring curToken;
    vector<wstring> splitted;
	set<wchar_t> delimeters;
	delimeters.insert(L'\t'); 
	delimeters.insert(L';');

	getline(in, curToken);
	exceptionToken_Stress.insert(std::make_pair(curToken, set<wstring>()));
	while (getline(in, s))
	{
        s = Tools::Trim(s);
		if (s.size() == 0)
		{
			getline(in, curToken);
			exceptionToken_Stress.insert(std::make_pair(curToken, set<wstring>()));
		}
		else
		{
            splitted = Tools::Split(s, delimeters);
            exceptionStress_Tags.insert(std::make_pair(splitted[0], set<wstring>()));
            set<wstring>* tags = &exceptionStress_Tags[splitted[0]];
            for (size_t i = 1; i < splitted.size(); ++i)
                tags->insert(splitted[i]);
		}
	}
	in.close();
}

wstring StresserRus::setStress(
    const wstring& token, const wstring& tag)
{
    wstring token_low = Tools::ToLower(token);
	if (tokens_NonAmbig.find(token_low) != tokens_NonAmbig.end())
    {
		return tokens_NonAmbig[token_low];
    }
	else if (tokens_AmbigByMorpology.find(token_low) != tokens_AmbigByMorpology.end())
	{
		unordered_map<wstring, wstring>& tag_Stress = tokens_AmbigByMorpology[token_low];
		if (tag_Stress.find(tag) == tag_Stress.end())
			return tag_Stress.begin()->second;
		return tag_Stress[tag];
	}
	else if (exceptionToken_Stress.find(token_low) != exceptionToken_Stress.end())
	{
		set<wstring>& stresses = exceptionToken_Stress[token_low];
		set<wstring> possibleStressForException;
		for (set<wstring>::const_iterator it = stresses.begin(); it != stresses.end(); ++it)
		{
			set<wstring>& tags = exceptionStress_Tags[*it];
			if (tags.find(tag) != tags.end())
				possibleStressForException.insert(*it);
		}
		if (possibleStressForException.size() == 1)
			return *possibleStressForException.begin();
	}
	return token;
}

wstring StresserRus::setStress(
		const wstring& token
        , const wstring& tag
        , const wstring& prevToken
        , const wstring& prevTag
        , const wstring& nextToken
        , const wstring& nextTag)
{
    wstring token_low = Tools::ToLower(token);
    wstring prevToken_low = Tools::ToLower(prevToken);
    wstring nextToken_low = Tools::ToLower(nextToken);

	if (tokens_NonAmbig.find(token_low) != tokens_NonAmbig.end())
    {
		return tokens_NonAmbig[token_low];
    }
	if (tokens_AmbigByMorpology.find(token_low) != tokens_AmbigByMorpology.end())
	{
		unordered_map<wstring, wstring>& tag_Stress = tokens_AmbigByMorpology[token_low];
		if (tag_Stress.find(tag) == tag_Stress.end())
			return tag_Stress.begin()->second;
		return tag_Stress[tag];
	}
	else if (exceptionToken_Stress.find(token_low) != exceptionToken_Stress.end())
	{
		set<wstring>& stresses = exceptionToken_Stress[token_low];
		set<wstring> possibleStressForException;
		for (set<wstring>::const_iterator it = stresses.begin(); it != stresses.end(); ++it)
		{
			set<wstring>& tags = exceptionStress_Tags[*it];
			if (tags.find(tag) != tags.end())
				possibleStressForException.insert(*it);
		}
		if (possibleStressForException.size() == 1)
			return *possibleStressForException.begin();
		return setStressForExceptions(
			token_low, tag, prevToken_low, prevTag, nextToken_low, nextTag);
	}
	return token;
}

wstring StresserRus::setStressForExceptions(
		const wstring& token
        , const wstring& tag
        , const wstring& prevToken
        , const wstring& prevTag
        , const wstring& nextToken
        , const wstring& nextTag)
{
    if (Tools::StartsWith(token, L"\u0431\u043e\u043b\u044c\u0448"))//here was cyrrilic symbols: больш
	{
		wstring ending = token.substr(5, token.size() - 5);
		if (nextToken == L"\u0447\u0435\u043c") {//here was cyrrilic symbols: чем
            return L"\u0431`\u043e\u043b\u044c\u0448" + ending;//here was cyrrilic symbols: б,ольш
        } else {
            return L"\u0431\u043e\u043b\u044c\u0448`" + ending;//here was cyrrilic symbols: больш
        }
	} else if (token == L"\u0431\u043e\u0440\u0442\u043e\u043c") {//here was cyrrilic symbols: бортом
		return L"\u0431`\u043e\u0440\u0442\u043e\u043c";//here was cyrrilic symbols: б,ортом
    } else if (Tools::StartsWith(token, L"\u0432\u0435\u0434\u0435\u043d")) {//here was cyrrilic symbols: веден
		wstring ending = token.substr(5, token.size() - 5);
		if (prevToken == L"\u0432") {//here was cyrrilic symbols: в
            return L"\u0432`\u0435\u0434\u0435\u043d" + ending;//here was cyrrilic symbols: в,еден
        } else {
            return L"\u0432\u0435\u0434`\u0435\u043d" + ending;//here was cyrrilic symbols: вед,ен
        }
    } else if (Tools::StartsWith(token, L"\u0432\u0438\u0434\u0443")) {//here was cyrrilic symbols: виду
		if (prevToken == L"\u0432") {//here was cyrrilic symbols: в
            return L"\u0432\u0438\u0434`\u0443";//here was cyrrilic symbols: вид,у
        } else {
            return L"\u0432`\u0438\u0434\u0443";//here was cyrrilic symbols: в,иду
        }
    } else if (Tools::StartsWith(token, L"\u0432\u0440\u0435\u043c\u0435\u043d\u043d")) {//here was cyrrilic symbols: временн
		wstring ending = token.substr(7, token.size() - 7);
        if (Tools::StartsWith(nextToken, L"\u0444\u043e\u0440\u043c")//here was cyrrilic symbols: форм
            || Tools::StartsWith(nextToken, L"\u044f\u043c")//here was cyrrilic symbols: ям
            || Tools::StartsWith(nextToken, L"\u043a\u043e\u043d\u0442\u0438\u043d\u0443\u0443\u043c")//here was cyrrilic symbols: континуум
            || Tools::StartsWith(nextToken, L"\u0440\u044f\u0434")//here was cyrrilic symbols: ряд
            || Tools::StartsWith(nextToken, L"\u043a\u043e\u043e\u0440\u0434\u0438\u043d\u0430\u0442")) {//here was cyrrilic symbols: координат
                return L"\u0432\u0440\u0435\u043c\u0435\u043d\u043d`" + ending;//here was cyrrilic symbols: временн
        } else {
            return L"\u0432\u0440`\u0435\u043c\u0435\u043d\u043d" + ending;//here was cyrrilic symbols: вр,еменн
        }
	} else if (token == L"\u0432\u0441\u0435") {//here was cyrrilic symbols: все
        if (Tools::ContainsSubstring(nextTag, L"ADV")
            || (Tools::ContainsSubstring(nextTag, L"A") && Tools::ContainsSubstring(nextTag, L"\u041a\u0420"))//here was cyrrilic symbols: КР
            || (Tools::ContainsSubstring(nextTag, L"V@") && Tools::ContainsSubstring(nextTag, L"3-\u041b")))//here was cyrrilic symbols: Л
        {
            return L"\u0432\u0441`\u0451";//here was cyrrilic symbols: вс,ё
        } else if (nextToken == L"\u043e") {//here was cyrrilic symbols: о
            return L"\u0432\u0441`\u0451";//here was cyrrilic symbols: вс,ё
        } else {
            return  L"\u0432\u0441`\u0435";//here was cyrrilic symbols: вс,е
        }
	} else if (token == L"\u0433\u043e\u043b\u043e\u0432\u0443") {//here was cyrrilic symbols: голову
		return L"\u0433`\u043e\u043b\u043e\u0432\u0443";//here was cyrrilic symbols: г,олову
	} else if (token == L"\u0434\u043e\u043b\u0436\u043d\u043e") {//here was cyrrilic symbols: должно
		return L"\u0434\u043e\u043b\u0436\u043d`\u043e";//here was cyrrilic symbols: должн,о
	} else if (token == L"\u0434\u043e\u043b\u0436\u043d\u044b") {//here was cyrrilic symbols: должны
		return L"\u0434\u043e\u043b\u0436\u043d`\u044b";//here was cyrrilic symbols: должн,ы
    } else if (token == L"\u0434\u0443\u0448\u0430\u043c") {//here was cyrrilic symbols: душам
		if (prevToken == L"\u043f\u043e") {//here was cyrrilic symbols: по
			return L"\u0434\u0443\u0448`\u0430\u043c";//here was cyrrilic symbols: душ,ам
        } else {
            return L"\u0434`\u0443\u0448\u0430\u043c";//here was cyrrilic symbols: д,ушам
        }
    } else if (Tools::StartsWith(token, L"\u043a\u043b\u0443\u0431")) {//here was cyrrilic symbols: клуб
		wstring ending = token.substr(4, token.size() - 4);
        if (Tools::StartsWith(nextToken, L"\u0434\u044b\u043c")//here was cyrrilic symbols: дым
            || Tools::StartsWith(nextToken, L"\u0433\u0440\u044f\u0437")//here was cyrrilic symbols: гряз
            || Tools::StartsWith(nextToken, L"\u043f\u0430\u0440")//here was cyrrilic symbols: пар
            || Tools::StartsWith(nextToken, L"\u0432\u043e\u0437\u0434\u0443\u0445")//here was cyrrilic symbols: воздух
            || Tools::StartsWith(nextToken, L"\u0433\u0430\u0437")) {//here was cyrrilic symbols: газ
                return L"\u043a\u043b\u0443\u0431`" + ending;//here was cyrrilic symbols: клуб
        } else {
            return L"\u043a\u043b`\u0443\u0431" + ending;//here was cyrrilic symbols: кл,уб
        }
	} else if (token == L"\u043a\u043e\u0441\u0438\u0442") {//here was cyrrilic symbols: косит
		if (prevToken == L"\u0442\u0440\u0430\u0432\u0443" //here was cyrrilic symbols: траву
            || nextToken == L"\u0442\u0440\u0430\u0432\u0443" //here was cyrrilic symbols: траву
            || prevToken == L"\u043f\u043e\u043b\u0435" //here was cyrrilic symbols: поле
            || nextToken == L"\u043f\u043e\u043b\u0435" //here was cyrrilic symbols: поле
            || prevToken == L"\u0433\u0430\u0437\u043e\u043d" //here was cyrrilic symbols: газон
            || nextToken == L"\u0433\u0430\u0437\u043e\u043d" //here was cyrrilic symbols: газон
            || nextToken == L"\u043e\u0442") {//here was cyrrilic symbols: от
			return L"\u043a`\u043ec\u0438\u0442";//here was cyrrilic symbols: к,о,ит
        } else {
            return L"\u043a\u043ec`\u0438\u0442";//here was cyrrilic symbols: ко,ит
        }
	} else if (token == L"\u043c\u0430\u043b\u043e") {//here was cyrrilic symbols: мало
        if (Tools::ContainsSubstring(prevTag, L"\u0420\u041e\u0414")//here was cyrrilic symbols: РОД
            || Tools::ContainsSubstring(nextTag, L"\u0420\u041e\u0414")//here was cyrrilic symbols: РОД
            || Tools::StartsWith(nextTag, L"V")) {
			return L"\u043c`\u0430\u043b\u043e";//here was cyrrilic symbols: м,ало
        } else {
            return L"\u043c\u0430\u043b`\u043e";//here was cyrrilic symbols: мал,о
        }
    } else if (Tools::StartsWith(token, L"\u043e\u0441\u043d\u043e\u0432\u043d")) {//here was cyrrilic symbols: основн
		wstring ending = token.substr(6, token.size() - 6);
		return L"\u043e\u0441\u043d\u043e\u0432\u043d`" + ending;//here was cyrrilic symbols: основн
    } else if (Tools::StartsWith(token, L"\u043e\u0441\u0442\u0440\u043e\u0442")) {//here was cyrrilic symbols: острот
		wstring ending = token.substr(6, token.size() - 6);
		return L"\u043e\u0441\u043d\u043e\u0432\u043d`" + ending;//here was cyrrilic symbols: основн
	} else if (token == L"\u043f\u043b\u0430\u0447\u0443") {//here was cyrrilic symbols: плачу
        if (Tools::ContainsSubstring(prevTag, L"\u0412\u0418\u041d")//here was cyrrilic symbols: ВИН
            || Tools::ContainsSubstring(nextTag, L"\u0412\u0418\u041d")//here was cyrrilic symbols: ВИН
            || (nextToken == L"\u0437\u0430")) {//here was cyrrilic symbols: за
                return L"\u043f\u043b\u0430\u0447`\u0443";//here was cyrrilic symbols: плач,у
        } else {
            return L"\u043f\u043b`\u0430\u0447\u0443";//here was cyrrilic symbols: пл,ачу
        }
	} else if (token == L"\u043f\u043e\u0432\u043e\u0434\u0443") {//here was cyrrilic symbols: поводу
		if (prevToken == L"\u043d\u0430") {//here was cyrrilic symbols: на
			return L"\u043f\u043e\u0432\u043e\u0434`\u0443";//here was cyrrilic symbols: повод,у
        } else {
            return L"\u043f`\u043e\u0432\u043e\u0434\u0443";//here was cyrrilic symbols: п,оводу
        }
    } else if (Tools::StartsWith(token, L"\u043f\u043e\u0434\u0432\u0438\u0436\u043d\u043e")) {//here was cyrrilic symbols: подвижно
		wstring ending = token.substr(8, token.size() - 8);
        if (Tools::StartsWith(nextToken, L"\u0441\u043e\u0441\u0442\u0430\u0432")) {//here was cyrrilic symbols: состав
            return L"\u043f\u043e\u0434\u0432\u0438\u0436\u043d\u043e`" + ending;//here was cyrrilic symbols: подвижно
        } else {
            return L"\u043f\u043e\u0434\u0432`\u0438\u0436\u043d\u043e" + ending;//here was cyrrilic symbols: подв,ижно
        }
    } else if (Tools::StartsWith(token, L"\u043f\u043e\u0434\u0432\u043e\u0434\u043d")) {//here was cyrrilic symbols: подводн
		wstring ending = token.substr(7, token.size() - 7);
		return L"\u043f\u043e\u0434\u0432`\u043e\u0434\u043d" + ending;//here was cyrrilic symbols: подв,одн
	} else if (token == L"\u043f\u043e\u043b\u043d\u043e") {//here was cyrrilic symbols: полно
		if (nextToken == L"\u0432\u0430\u043c" //here was cyrrilic symbols: вам
            || nextToken == L"\u0442\u0435\u0431\u0435") {//here was cyrrilic symbols: тебе
                return L"\u043f`\u043e\u043b\u043d\u043e";//here was cyrrilic symbols: п,олно
        } else {
            return L"\u043f\u043e\u043b\u043d`\u043e";//here was cyrrilic symbols: полн,о
        }
    } else if (Tools::StartsWith(token, L"\u0441\u0432\u0435\u0434\u0435\u043d")) {//here was cyrrilic symbols: сведен
		wstring ending = token.substr(6, token.size() - 6);
		return L"\u0441\u0432`\u0435\u0434\u0435\u043d\u0438" + ending;//here was cyrrilic symbols: св,едени
	} else if (token == L"\u0441\u0432\u043e\u0435\u043c\u0443") {//here was cyrrilic symbols: своему
		if (prevToken == L"\u043f\u043e") {//here was cyrrilic symbols: по
		    return L"\u0441\u0432`\u043e\u0435\u043c\u0443";//here was cyrrilic symbols: св,оему
        } else {
            return L"\u0441\u0432\u043e\u0435\u043c`\u0443";//here was cyrrilic symbols: своем,у
        }
	} else if (token == L"\u0441\u043b\u0430\u0431\u043e") {//here was cyrrilic symbols: слабо
		return L"\u0441\u043b`\u0430\u0431\u043e";//here was cyrrilic symbols: сл,або
	} else if (token == L"\u0441\u043b\u0435\u0434\u0430") {//here was cyrrilic symbols: следа
		return L"\u0441\u043b\u0435\u0434`\u0430";//here was cyrrilic symbols: след,а
	} else {
        return token;
    }
}

Tools::Language StresserRus::GetLanguage() const
{
    return Tools::Language::RU;
}

}
