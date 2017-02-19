/**
 * DictionaryTools.cpp
 */

#include "DictionaryTools.h"
#include "Dictionary.h"

wstring RussianAlphaSymbols = L" \u0430\u0431\u0432\u0433\u0434\u0435\u0451\u0436\u0437\u0438\u0439\u043a\u043b\u043c\u043d\u043e\u043f\u0440\u0441\u0442\u0443\u0444\u0445\u0446\u0447\u0448\u0449\u044a\u044b\u044c\u044d\u044e\u044f-0123456789,'\".;!?\nabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";//here was cyrrilic symbols: абвгдеёжзийклмнопрстуфхцчшщъыьэюя
wstring RussianAlphaCharacters = L"\u0430\u0431\u0432\u0433\u0434\u0435\u0451\u0436\u0437\u0438\u0439\u043a\u043b\u043c\u043d\u043e\u043f\u0440\u0441\u0442\u0443\u0444\u0445\u0446\u0447\u0448\u0449\u044a\u044b\u044c\u044d\u044e\u044f0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";//here was cyrrilic symbols: абвгдеёжзийклмнопрстуфхцчшщъыьэюя
wstring po = L"\u043f\u043e";//here was cyrrilic symbols: по

/**
 * Constructor of dictionary tools
 */
DictionaryTools::DictionaryTools(Dictionary* _dic)
{
    dic = _dic;
    initAlpha();
    initCharMap();
    // for Linux
    initLinuxCharMap();
}

/**
 * Constructor of dictionary tools
 */
DictionaryTools::DictionaryTools()
{
    dic = NULL;
    initAlpha();
    initCharMap();
    // for Linux
    initLinuxCharMap();
}

/**
 * Destructor of dictionary tools
 */
DictionaryTools::~DictionaryTools(void)
{
	// free memory
	dic = NULL;
	alpha.clear();
	alphabet.clear();
	rAlphabet.clear();
}

/**
 * Split string by delimiter
 */
vector<wstring> DictionaryTools::dictionarySplit(const wstring & str, const wstring & delimiter, bool splitByDelimiter)
{
	vector<wstring> tokens = vector<wstring>();
    size_t len = str.length();
    size_t i = splitByDelimiter ? str.find_first_not_of(delimiter) : str.find_first_of(delimiter);
	while (i < len)
	{
        size_t j = splitByDelimiter ? str.find_first_of(delimiter, i) : str.find_first_not_of(delimiter, i);
        if (j == string::npos)
		{
            j = len;
		}
		wstring tok = str.substr(i, j - i);
        tokens.push_back(tok);
        if (j < len)
        {
            i = splitByDelimiter ? str.find_first_not_of(delimiter, j) : str.find_first_of(delimiter, j);
        }
        else
        {
            i = len;
        }
	}
	return tokens;
}

wchar_t DictionaryTools::charToLowerCase(wchar_t ch)
{
    if (ch == L'\u0401')//here was cyrrilic symbols: Ё
    {
        return L'\u0451';//here was cyrrilic symbols: ё
    }
    else if (ch >= L'\u0410' && ch <= L'\u042f')//here was cyrrilic symbols: А,Я
    {
        return ch + L'\u0430' - L'\u0410';//here was cyrrilic symbols: а,А
    }
    else if (ch >= L'A' && ch <= L'Z')
    {
        return ch + L'a' - L'A';
    }
    return ch;
}

/**
 * Convert a string to lower case
 */
wstring DictionaryTools::toLowerCase(const wstring & _string)
{
	wstring _result;
	_result.clear();
	for (int i = 0; i < (int) _string.length(); ++i) 
	{
        _result.push_back(charToLowerCase(_string.at(i)));
	}
	return _result;
}

/**
 * Delete spaces
 */
wstring DictionaryTools::deleteSpaces(const wstring & _string)
{
	wstring _result;
	_result.clear();
	for (int i = 0; i < (int) _string.length(); ++i) 
	{
		if (_string.at(i) != L' ')
		{
			_result.push_back(_string.at(i));
		}
	}
	return _result;
}

vector<wstring> DictionaryTools::getModifications(const wstring & word)
{
	set<wstring> mod = set<wstring>();
	
	// replace 1 character
	for (int i = 0; i < (int) word.length(); ++i)
	{
		for (int j = 0; j < (int) alpha.size(); ++j)
		{
			wstring str = word.substr(0, i) + alpha.at(j) + word.substr(i + 1);
			mod.insert(str);
		}
	}
	
	// insert 1 character
	for (int i = 0; i <= (int) word.length(); ++i)
	{
		for (int j = 0; j < (int) alpha.size(); ++j)
		{
			wstring str = word.substr(0, i) + alpha.at(j) + word.substr(i);
			mod.insert(str);
		}
	}
	
	// delete 1 character
	for (int i = 0; i < (int) word.length(); ++i)
	{
		wstring str = word.substr(0, i) + word.substr(i + 1);
		mod.insert(str);
	}
	
	// Transpose 2 characters
	for (int i = 0; i < (int) word.length() - 1; ++i)
	{
		wstring str = word.substr(0, i);
		str.push_back(word.at(i+1));
		str.push_back(word.at(i));
		str = str + word.substr(i + 2);
		mod.insert(str);
	}

	vector<wstring> result = vector<wstring>();
	result.insert(result.end(), mod.begin(), mod.end());
	return result;
}

vector<wstring> DictionaryTools::getAllEeVariant(const wstring & word)
{
	vector<wstring> result = vector<wstring>();
	vector<int> ePos = vector<int>();
	int total = 1;
	for (int i = 0; i < (int) word.length(); ++i)
	{
		if (word.at(i) == L'\u0435' || word.at(i) == L'\u0451')//here was cyrrilic symbols: е,ё
		{
			ePos.push_back(i);
			total *= 2;
		}
	}
	int numberOfE = ePos.size();
	if (!numberOfE)
	{
		return result;
	}
	vector<int> eBin = vector<int>(numberOfE);
	for (int k = 0; k < total; ++k)
	{
		int tmp = k;
		for (int i = numberOfE - 1; i >= 0; --i)
		{
			eBin[i] = tmp % 2;
			tmp /= 2;
		}
		wstring _str = L"";
		int tr = -1;
		for (int i = 0; i < numberOfE; ++i)
		{
			_str += word.substr(tr + 1, ePos.at(i) - tr -1);
			tr = ePos.at(i);
			if (eBin[i])
			{
				_str.push_back(L'\u0435');//here was cyrrilic symbols: е
			}
			else
			{
				_str.push_back(L'\u0451');//here was cyrrilic symbols: ё
			}
		}
		_str += word.substr(tr + 1);
		if (_str.compare(word))
		{
			result.push_back(_str);
		}
	}
	return result;
}

vector<wstring> DictionaryTools::getAllWordVariant(const wstring & word, bool ignoreE, bool hyphenSplit)
{
	vector<wstring> wordVariants = vector<wstring>();
	if (hyphenSplit && word.find(L"-") >= 0)
	{
		vector<wstring> tokens = dic->getDictionaryTools()->dictionarySplit(word, L"-");
		wordVariants.insert(wordVariants.begin(), tokens.begin(), tokens.end());
		//wordVariants.push_back(word);
	}
	else
	{
		wordVariants.push_back(word);
	}

	vector<wstring> allWordVariant = vector<wstring>();

	for (int i = 0; i < (int) wordVariants.size(); ++i)
	{
		wstring wordVariant = wordVariants.at(i);
		if (ignoreE && (wordVariant.find(L"\u0435") != std::string::npos || wordVariant.find(L"\u0451") != std::string::npos))//here was cyrrilic symbols: е,ё
		{
			vector<wstring> tmp = getAllEeVariant(wordVariant);
			allWordVariant.insert(allWordVariant.end(), tmp.begin(), tmp.end());
		}
		else
		{
			allWordVariant.push_back(wordVariant);
		}
	}
	return allWordVariant;
}

/**
 * Refine string
 * Convert to lower-case
 * Delete unnecessary symbols
 */
wstring DictionaryTools::refineString(const wstring & _string)
{
	wstring _lower = toLowerCase(_string);
	wstring result = L"";
	for (int i = 0; i < (int) _lower.length(); ++i)
	{
		if (RussianAlphaSymbols.find_first_of(_lower.at(i)) != string::npos)
		{
			result.push_back(_lower.at(i));
		}
	}
	return result;
}

/**
 * Refine (filter) string by alphabet
 * Delete unnecessary symbols
 */
wstring DictionaryTools::refineStringByAlphabet(const wstring & _string, const wstring & _alphabet)
{
	wstring result = L"";
	for (int i = 0; i < (int) _string.length(); ++i)
	{
		if (_alphabet.find_first_of(_string.at(i)) != string::npos)
		{
			result.push_back(_string.at(i));
		}
	}
	return result;
}

/*
 * True if _str begins with Po
 */
bool DictionaryTools::stringBeginsWithPo(const wstring & _str)
{
	if (_str.length() >= 2) {
		wstring prefix = _str.substr(0, 2);
		if (prefix.compare(po) == 0) {
			return true;
		}
	}
	return false;
}

vector<unsigned char> DictionaryTools::intToCharVector1(int _number)
{
    vector<unsigned char> _result;
    _result.push_back((unsigned char) (_number));
	return _result;
}

vector<unsigned char> DictionaryTools::intToCharVector2(int _number)
{
    vector<unsigned char> _result;
    _result.push_back((unsigned char) (_number / 256));
    _result.push_back((unsigned char) (_number % 256));
	return _result;
}

vector<unsigned char> DictionaryTools::intToCharVector3(int _number)
{
	int _tmp = _number;
    vector<unsigned char> _result;
    _result.push_back((unsigned char) (_tmp / 65536));
	_tmp %= 65536;
    _result.push_back((unsigned char) (_tmp / 256));
    _result.push_back((unsigned char) (_tmp % 256));
	return _result;
}

vector<unsigned char> DictionaryTools::intToCharVector4(int _number)
{
	int _tmp = _number;
    vector<unsigned char> _result;
    _result.push_back((unsigned char) (_tmp / 16777216));
	_tmp %= 16777216;
    _result.push_back((unsigned char) (_tmp / 65536));
	_tmp %= 65536;
    _result.push_back((unsigned char) (_tmp / 256));
    _result.push_back((unsigned char) (_tmp % 256));
	return _result;
}

/**
 * Convert char to wchar_t
 */
wchar_t DictionaryTools::charToWchar(unsigned char _char)
{
	if (rAlphabet.count(_char) > 0)
	{
		return rAlphabet.at(_char);
	}
    return L'?';
}

unsigned char DictionaryTools::wcharToChar(wchar_t wchar)
{
    map<wchar_t, unsigned char>::iterator iter = alphabet.find(wchar);
    if (iter != alphabet.end())
	{
        return iter->second;
	}
	return 0;
}

vector<unsigned char> DictionaryTools::wstringToCharVector(const wstring & _str)
{
    vector<unsigned char> _result;
	for (int i = 0; i < (int) _str.length(); ++i)
	{
		_result.push_back(wcharToChar(_str.at(i)));
	}
	return _result;
}

vector<unsigned char> DictionaryTools::doubleToCharVector8(double _number)
{
    DoubleChar n;
    n.doubleValue = _number;
    vector<unsigned char> _result = vector<unsigned char>();
    for (int i = 0; i < 8; ++i)
    {
        _result.push_back(n.charArray[i]);
    }
    return _result;
}

double DictionaryTools::charVector8ToDouble(vector<unsigned char> _charVector)
{
    DoubleChar n;
    for (int i = 0; i < 8; ++i)
    {
        n.charArray[i] = _charVector.at(i);
    }
    return n.doubleValue;
}

/**
 * Init russian alphabet
 */
/*
void DictionaryTools::initAlphabet() {
	alphabetCase = map<wchar_t, wchar_t>();
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0410', '\u0430'));//here was cyrrilic symbols: А,а
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0411', '\u0431'));//here was cyrrilic symbols: Б,б
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0412', '\u0432'));//here was cyrrilic symbols: В,в
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0413', '\u0433'));//here was cyrrilic symbols: Г,г
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0414', '\u0434'));//here was cyrrilic symbols: Д,д
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0415', '\u0435'));//here was cyrrilic symbols: Е,е
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0401', '\u0451'));//here was cyrrilic symbols: Ё,ё
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0416', '\u0436'));//here was cyrrilic symbols: Ж,ж
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0417', '\u0437'));//here was cyrrilic symbols: З,з
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0418', '\u0438'));//here was cyrrilic symbols: И,и
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0419', '\u0439'));//here was cyrrilic symbols: Й,й
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u041a', '\u043a'));//here was cyrrilic symbols: К,к
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u041b', '\u043b'));//here was cyrrilic symbols: Л,л
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u041c', '\u043c'));//here was cyrrilic symbols: М,м
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u041d', '\u043d'));//here was cyrrilic symbols: Н,н
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u041e', '\u043e'));//here was cyrrilic symbols: О,о
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u041f', '\u043f'));//here was cyrrilic symbols: П,п
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0420', '\u0440'));//here was cyrrilic symbols: Р,р
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0421', '\u0441'));//here was cyrrilic symbols: С,с
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0422', '\u0442'));//here was cyrrilic symbols: Т,т
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0423', '\u0443'));//here was cyrrilic symbols: У,у
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0424', '\u0444'));//here was cyrrilic symbols: Ф,ф
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0425', '\u0445'));//here was cyrrilic symbols: Х,х
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0426', '\u0446'));//here was cyrrilic symbols: Ц,ц
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0427', '\u0447'));//here was cyrrilic symbols: Ч,ч
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0428', '\u0448'));//here was cyrrilic symbols: Ш,ш
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u0429', '\u0449'));//here was cyrrilic symbols: Щ,щ
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u042a', '\u044a'));//here was cyrrilic symbols: Ъ,ъ
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u042b', '\u044b'));//here was cyrrilic symbols: Ы,ы
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u042c', '\u044c'));//here was cyrrilic symbols: Ь,ь
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u042d', '\u044d'));//here was cyrrilic symbols: Э,э
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u042e', '\u044e'));//here was cyrrilic symbols: Ю,ю
	alphabetCase.insert(pair<wchar_t, wchar_t>('\u042f', '\u044f'));//here was cyrrilic symbols: Я,я
}
*/

/**
 * Init russian alphabet
 */
void DictionaryTools::initAlpha(void)
{
	alpha = vector<wstring>(33);
	alpha[0] = L"\u0430";//here was cyrrilic symbols: а
	alpha[1] = L"\u0431";//here was cyrrilic symbols: б
	alpha[2] = L"\u0432";//here was cyrrilic symbols: в
	alpha[3] = L"\u0433";//here was cyrrilic symbols: г
	alpha[4] = L"\u0434";//here was cyrrilic symbols: д
	alpha[5] = L"\u0435";//here was cyrrilic symbols: е
	alpha[6] = L"\u0451";//here was cyrrilic symbols: ё
	alpha[7] = L"\u0436";//here was cyrrilic symbols: ж
	alpha[8] = L"\u0437";//here was cyrrilic symbols: з
	alpha[9] = L"\u0438";//here was cyrrilic symbols: и
	alpha[10] = L"\u0439";//here was cyrrilic symbols: й
	alpha[11] = L"\u043a";//here was cyrrilic symbols: к
	alpha[12] = L"\u043b";//here was cyrrilic symbols: л
	alpha[13] = L"\u043c";//here was cyrrilic symbols: м
	alpha[14] = L"\u043d";//here was cyrrilic symbols: н
	alpha[15] = L"\u043e";//here was cyrrilic symbols: о
	alpha[16] = L"\u043f";//here was cyrrilic symbols: п
	alpha[17] = L"\u0440";//here was cyrrilic symbols: р
	alpha[18] = L"\u0441";//here was cyrrilic symbols: с
	alpha[19] = L"\u0442";//here was cyrrilic symbols: т
	alpha[20] = L"\u0443";//here was cyrrilic symbols: у
	alpha[21] = L"\u0444";//here was cyrrilic symbols: ф
	alpha[21] = L"\u0445";//here was cyrrilic symbols: х
	alpha[23] = L"\u0446";//here was cyrrilic symbols: ц
	alpha[24] = L"\u0447";//here was cyrrilic symbols: ч
	alpha[25] = L"\u0448";//here was cyrrilic symbols: ш
	alpha[26] = L"\u0449";//here was cyrrilic symbols: щ
	alpha[27] = L"\u044a";//here was cyrrilic symbols: ъ
	alpha[28] = L"\u044b";//here was cyrrilic symbols: ы
	alpha[29] = L"\u044c";//here was cyrrilic symbols: ь
	alpha[30] = L"\u044d";//here was cyrrilic symbols: э
	alpha[31] = L"\u044e";//here was cyrrilic symbols: ю
	alpha[32] = L"\u044f";//here was cyrrilic symbols: я
}

/**
 * Initialize char map
 */

void DictionaryTools::initCharMap(void)
{
    wstring _aaa = L" абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`!@#$%^&*()-+=:;\"'|,.?";
//    wcout << _aaa.length() << endl;
    alphabet.clear();
    rAlphabet.clear();
    for (unsigned char i = 0; i < (unsigned char) _aaa.size(); ++i)
    {
        //wcout << _aaa.at(i) << " >> " << i << endl;
        alphabet[_aaa.at(i)] = i;
        rAlphabet[i] = _aaa.at(i);
    }
//    wcout << "Count = " << alphabet.count(L'A');
}

void DictionaryTools::initCharMap_old(void)
{
    alphabet.clear();
    rAlphabet.clear();
    alphabet[L' '] = 0;
    alphabet[L'\u0430'] = 1;//here was cyrrilic symbols: а
    alphabet[L'\u0431'] = 2;//here was cyrrilic symbols: б
    alphabet[L'\u0432'] = 3;//here was cyrrilic symbols: в
    alphabet[L'\u0433'] = 4;//here was cyrrilic symbols: г
    alphabet[L'\u0434'] = 5;//here was cyrrilic symbols: д
    alphabet[L'\u0435'] = 6;//here was cyrrilic symbols: е
    alphabet[L'\u0451'] = 7;//here was cyrrilic symbols: ё
    alphabet[L'\u0436'] = 8;//here was cyrrilic symbols: ж
    alphabet[L'\u0437'] = 9;//here was cyrrilic symbols: з
    alphabet[L'\u0438'] = 10;//here was cyrrilic symbols: и
    alphabet[L'\u0439'] = 11;//here was cyrrilic symbols: й
    alphabet[L'\u043a'] = 12;//here was cyrrilic symbols: к
    alphabet[L'\u043b'] = 13;//here was cyrrilic symbols: л
    alphabet[L'\u043c'] = 14;//here was cyrrilic symbols: м
    alphabet[L'\u043d'] = 15;//here was cyrrilic symbols: н
    alphabet[L'\u043e'] = 16;//here was cyrrilic symbols: о
    alphabet[L'\u043f'] = 17;//here was cyrrilic symbols: п
    alphabet[L'\u0440'] = 18;//here was cyrrilic symbols: р
    alphabet[L'\u0441'] = 19;//here was cyrrilic symbols: с
    alphabet[L'\u0442'] = 20;//here was cyrrilic symbols: т
    alphabet[L'\u0443'] = 21;//here was cyrrilic symbols: у
    alphabet[L'\u0444'] = 22;//here was cyrrilic symbols: ф
    alphabet[L'\u0445'] = 23;//here was cyrrilic symbols: х
    alphabet[L'\u0446'] = 24;//here was cyrrilic symbols: ц
    alphabet[L'\u0447'] = 25;//here was cyrrilic symbols: ч
    alphabet[L'\u0448'] = 26;//here was cyrrilic symbols: ш
    alphabet[L'\u0449'] = 27;//here was cyrrilic symbols: щ
    alphabet[L'\u044a'] = 28;//here was cyrrilic symbols: ъ
    alphabet[L'\u044b'] = 29;//here was cyrrilic symbols: ы
    alphabet[L'\u044c'] = 30;//here was cyrrilic symbols: ь
    alphabet[L'\u044d'] = 31;//here was cyrrilic symbols: э
    alphabet[L'\u044e'] = 32;//here was cyrrilic symbols: ю
    alphabet[L'\u044f'] = 33;//here was cyrrilic symbols: я
    alphabet[L'@'] = 34;
    alphabet[L'#'] = 35;
    alphabet[L'&'] = 36;
    alphabet[L'-'] = 37;
    alphabet[L'a'] = 38;
    alphabet[L'b'] = 39;
    alphabet[L'c'] = 40;
    alphabet[L'd'] = 41;
    alphabet[L'e'] = 42;
    alphabet[L'f'] = 43;
    alphabet[L'g'] = 44;
    alphabet[L'h'] = 45;
    alphabet[L'i'] = 46;
    alphabet[L'j'] = 47;
    alphabet[L'k'] = 48;
    alphabet[L'l'] = 49;
    alphabet[L'm'] = 50;
    alphabet[L'n'] = 51;
    alphabet[L'o'] = 52;
    alphabet[L'p'] = 53;
    alphabet[L'q'] = 54;
    alphabet[L'r'] = 55;
    alphabet[L's'] = 56;
    alphabet[L't'] = 57;
    alphabet[L'u'] = 58;
    alphabet[L'v'] = 59;
    alphabet[L'w'] = 60;
    alphabet[L'x'] = 61;
    alphabet[L'y'] = 62;
    alphabet[L'z'] = 63;
    alphabet[L'A'] = 64;
    alphabet[L'B'] = 65;
    alphabet[L'C'] = 66;
    alphabet[L'D'] = 67;
    alphabet[L'E'] = 68;
    alphabet[L'F'] = 69;
    alphabet[L'G'] = 70;
    alphabet[L'H'] = 71;
    alphabet[L'I'] = 72;
    alphabet[L'J'] = 73;
    alphabet[L'K'] = 74;
    alphabet[L'L'] = 75;
    alphabet[L'M'] = 76;
    alphabet[L'N'] = 77;
    alphabet[L'O'] = 78;
    alphabet[L'P'] = 79;
    alphabet[L'Q'] = 80;
    alphabet[L'R'] = 81;
    alphabet[L'S'] = 82;
    alphabet[L'T'] = 83;
    alphabet[L'U'] = 84;
    alphabet[L'V'] = 85;
    alphabet[L'W'] = 86;
    alphabet[L'X'] = 87;
    alphabet[L'Y'] = 88;
    alphabet[L'Z'] = 89;
    alphabet[L'*'] = 90;
    alphabet[L'0'] = 91;
    alphabet[L'1'] = 92;
    alphabet[L'2'] = 93;
    alphabet[L'3'] = 94;
    alphabet[L'4'] = 95;
    alphabet[L'5'] = 96;
    alphabet[L'6'] = 97;
    alphabet[L'7'] = 98;
    alphabet[L'8'] = 99;
    alphabet[L'9'] = 100;
    alphabet[L'%'] = 101;
    alphabet[L'\u0412'] = 102; // for \u0412\u0430\u0440\u0438\u0430\u043d\u0442 \u043f\u0440\u0435\u0434\u043b\u043e\u0433\u0430//here was cyrrilic symbols: В,Вариант,предлога
    alphabet[L'('] = 103;
    alphabet[L')'] = 104;
    alphabet[L'/'] = 105;
    rAlphabet.clear();
    for (std::map<wchar_t, unsigned char>::iterator it=alphabet.begin(); it!=alphabet.end(); ++it)
    {
        rAlphabet[it->second] = it->first;
    }
}

void DictionaryTools::initLinuxCharMap()
{
    alphabetLinux = map<wchar_t, string>();

    alphabetLinux[L' '] = " ";

    alphabetLinux[L'\u0430'] = "\u0430";//here was cyrrilic symbols: а,а
    alphabetLinux[L'\u0431'] = "\u0431";//here was cyrrilic symbols: б,б
    alphabetLinux[L'\u0432'] = "\u0432";//here was cyrrilic symbols: в,в
    alphabetLinux[L'\u0433'] = "\u0433";//here was cyrrilic symbols: г,г
    alphabetLinux[L'\u0434'] = "\u0434";//here was cyrrilic symbols: д,д
    alphabetLinux[L'\u0435'] = "\u0435";//here was cyrrilic symbols: е,е
    alphabetLinux[L'\u0451'] = "\u0451";//here was cyrrilic symbols: ё,ё
    alphabetLinux[L'\u0436'] = "\u0436";//here was cyrrilic symbols: ж,ж
    alphabetLinux[L'\u0437'] = "\u0437";//here was cyrrilic symbols: з,з
    alphabetLinux[L'\u0438'] = "\u0438";//here was cyrrilic symbols: и,и
    alphabetLinux[L'\u0439'] = "\u0439";//here was cyrrilic symbols: й,й
    alphabetLinux[L'\u043a'] = "\u043a";//here was cyrrilic symbols: к,к
    alphabetLinux[L'\u043b'] = "\u043b";//here was cyrrilic symbols: л,л
    alphabetLinux[L'\u043c'] = "\u043c";//here was cyrrilic symbols: м,м
    alphabetLinux[L'\u043d'] = "\u043d";//here was cyrrilic symbols: н,н
    alphabetLinux[L'\u043e'] = "\u043e";//here was cyrrilic symbols: о,о
    alphabetLinux[L'\u043f'] = "\u043f";//here was cyrrilic symbols: п,п
    alphabetLinux[L'\u0440'] = "\u0440";//here was cyrrilic symbols: р,р
    alphabetLinux[L'\u0441'] = "\u0441";//here was cyrrilic symbols: с,с
    alphabetLinux[L'\u0442'] = "\u0442";//here was cyrrilic symbols: т,т
    alphabetLinux[L'\u0443'] = "\u0443";//here was cyrrilic symbols: у,у
    alphabetLinux[L'\u0444'] = "\u0444";//here was cyrrilic symbols: ф,ф
    alphabetLinux[L'\u0445'] = "\u0445";//here was cyrrilic symbols: х,х
    alphabetLinux[L'\u0446'] = "\u0446";//here was cyrrilic symbols: ц,ц
    alphabetLinux[L'\u0447'] = "\u0447";//here was cyrrilic symbols: ч,ч
    alphabetLinux[L'\u0448'] = "\u0448";//here was cyrrilic symbols: ш,ш
    alphabetLinux[L'\u0449'] = "\u0449";//here was cyrrilic symbols: щ,щ
    alphabetLinux[L'\u044a'] = "\u044a";//here was cyrrilic symbols: ъ,ъ
    alphabetLinux[L'\u044b'] = "\u044b";//here was cyrrilic symbols: ы,ы
    alphabetLinux[L'\u044c'] = "\u044c";//here was cyrrilic symbols: ь,ь
    alphabetLinux[L'\u044d'] = "\u044d";//here was cyrrilic symbols: э,э
    alphabetLinux[L'\u044e'] = "\u044e";//here was cyrrilic symbols: ю,ю
    alphabetLinux[L'\u044f'] = "\u044f";//here was cyrrilic symbols: я,я

    alphabetLinux[L'\u0410'] = "\u0410";//here was cyrrilic symbols: А,А
    alphabetLinux[L'\u0411'] = "\u0411";//here was cyrrilic symbols: Б,Б
    alphabetLinux[L'\u0412'] = "\u0412";//here was cyrrilic symbols: В,В
    alphabetLinux[L'\u0413'] = "\u0413";//here was cyrrilic symbols: Г,Г
    alphabetLinux[L'\u0414'] = "\u0414";//here was cyrrilic symbols: Д,Д
    alphabetLinux[L'\u0415'] = "\u0415";//here was cyrrilic symbols: Е,Е
    alphabetLinux[L'\u0401'] = "\u0401";//here was cyrrilic symbols: Ё,Ё
    alphabetLinux[L'\u0416'] = "\u0416";//here was cyrrilic symbols: Ж,Ж
    alphabetLinux[L'\u0417'] = "\u0417";//here was cyrrilic symbols: З,З
    alphabetLinux[L'\u0418'] = "\u0418";//here was cyrrilic symbols: И,И
    alphabetLinux[L'\u0419'] = "\u0419";//here was cyrrilic symbols: Й,Й
    alphabetLinux[L'\u041a'] = "\u041a";//here was cyrrilic symbols: К,К
    alphabetLinux[L'\u041b'] = "\u041b";//here was cyrrilic symbols: Л,Л
    alphabetLinux[L'\u041c'] = "\u041c";//here was cyrrilic symbols: М,М
    alphabetLinux[L'\u041d'] = "\u041d";//here was cyrrilic symbols: Н,Н
    alphabetLinux[L'\u041e'] = "\u041e";//here was cyrrilic symbols: О,О
    alphabetLinux[L'\u041f'] = "\u041f";//here was cyrrilic symbols: П,П
    alphabetLinux[L'\u0420'] = "\u0420";//here was cyrrilic symbols: Р,Р
    alphabetLinux[L'\u0421'] = "\u0421";//here was cyrrilic symbols: С,С
    alphabetLinux[L'\u0422'] = "\u0422";//here was cyrrilic symbols: Т,Т
    alphabetLinux[L'\u0423'] = "\u0423";//here was cyrrilic symbols: У,У
    alphabetLinux[L'\u0424'] = "\u0424";//here was cyrrilic symbols: Ф,Ф
    alphabetLinux[L'\u0425'] = "\u0425";//here was cyrrilic symbols: Х,Х
    alphabetLinux[L'\u0426'] = "\u0426";//here was cyrrilic symbols: Ц,Ц
    alphabetLinux[L'\u0427'] = "\u0427";//here was cyrrilic symbols: Ч,Ч
    alphabetLinux[L'\u0428'] = "\u0428";//here was cyrrilic symbols: Ш,Ш
    alphabetLinux[L'\u0429'] = "\u0429";//here was cyrrilic symbols: Щ,Щ
    alphabetLinux[L'\u042a'] = "\u042a";//here was cyrrilic symbols: Ъ,Ъ
    alphabetLinux[L'\u042b'] = "\u042b";//here was cyrrilic symbols: Ы,Ы
    alphabetLinux[L'\u042c'] = "\u042c";//here was cyrrilic symbols: Ь,Ь
    alphabetLinux[L'\u042d'] = "\u042d";//here was cyrrilic symbols: Э,Э
    alphabetLinux[L'\u042e'] = "\u042e";//here was cyrrilic symbols: Ю,Ю
    alphabetLinux[L'\u042f'] = "\u042f";//here was cyrrilic symbols: Я,Я

    alphabetLinux[L'a'] = "a";
    alphabetLinux[L'b'] = "b";
    alphabetLinux[L'c'] = "c";
    alphabetLinux[L'd'] = "d";
    alphabetLinux[L'e'] = "e";
    alphabetLinux[L'f'] = "f";
    alphabetLinux[L'g'] = "g";
    alphabetLinux[L'h'] = "h";
    alphabetLinux[L'i'] = "i";
    alphabetLinux[L'j'] = "j";
    alphabetLinux[L'k'] = "k";
    alphabetLinux[L'l'] = "l";
    alphabetLinux[L'm'] = "m";
    alphabetLinux[L'n'] = "n";
    alphabetLinux[L'o'] = "o";
    alphabetLinux[L'p'] = "p";
    alphabetLinux[L'q'] = "q";
    alphabetLinux[L'r'] = "r";
    alphabetLinux[L's'] = "s";
    alphabetLinux[L't'] = "t";
    alphabetLinux[L'u'] = "u";
    alphabetLinux[L'v'] = "v";
    alphabetLinux[L'w'] = "w";
    alphabetLinux[L'x'] = "x";
    alphabetLinux[L'y'] = "y";
    alphabetLinux[L'z'] = "z";

    alphabetLinux[L'A'] = "A";
    alphabetLinux[L'B'] = "B";
    alphabetLinux[L'C'] = "C";
    alphabetLinux[L'D'] = "D";
    alphabetLinux[L'E'] = "E";
    alphabetLinux[L'F'] = "F";
    alphabetLinux[L'G'] = "G";
    alphabetLinux[L'H'] = "H";
    alphabetLinux[L'I'] = "I";
    alphabetLinux[L'J'] = "J";
    alphabetLinux[L'K'] = "K";
    alphabetLinux[L'L'] = "L";
    alphabetLinux[L'M'] = "M";
    alphabetLinux[L'N'] = "N";
    alphabetLinux[L'O'] = "O";
    alphabetLinux[L'P'] = "P";
    alphabetLinux[L'Q'] = "Q";
    alphabetLinux[L'R'] = "R";
    alphabetLinux[L'S'] = "S";
    alphabetLinux[L'T'] = "T";
    alphabetLinux[L'U'] = "U";
    alphabetLinux[L'V'] = "V";
    alphabetLinux[L'W'] = "W";
    alphabetLinux[L'X'] = "X";
    alphabetLinux[L'Y'] = "Y";
    alphabetLinux[L'Z'] = "Z";

    alphabetLinux[L'0'] = "0";
    alphabetLinux[L'1'] = "1";
    alphabetLinux[L'2'] = "2";
    alphabetLinux[L'3'] = "3";
    alphabetLinux[L'4'] = "4";
    alphabetLinux[L'5'] = "5";
    alphabetLinux[L'6'] = "6";
    alphabetLinux[L'7'] = "7";
    alphabetLinux[L'8'] = "8";
    alphabetLinux[L'9'] = "9";
    alphabetLinux[L'*'] = "*";
    alphabetLinux[L'%'] = "%";
    alphabetLinux[L'('] = "(";
    alphabetLinux[L')'] = ")";
    alphabetLinux[L'/'] = "/";
    alphabetLinux[L'\''] = "\'";

/*
    linuxAlphabet = map<char, wstring>();
    for (map<wchar_t, string>::iterator iter = alphabetLinux.begin(); iter != alphabetLinux.end(); ++iter)
    {
        char _first = iter->second.at(0); // @TODO : Error !!!
        wstring _second = L"";
        _second.push_back(iter->first);
        cout << iter->second << "-" << iter->second.length() << endl;
        for (unsigned i = 0; i < iter->second.length(); ++i)
        {
            cout << iter->second.at(i) << endl;
        }
        //cout << endl;
        linuxAlphabet.insert(pair<char, wstring>(_first, _second));
    }
*/
}

string DictionaryTools::wchar2char(wchar_t wc)
{
    if (alphabetLinux.count(wc) > 0)
    {
        return alphabetLinux.at(wc);
    }
    return "?";
}

string DictionaryTools::wstring2string(const wstring & _wstr)
{
    string _str = "";
    for (int i = 0; i < (int) _wstr.length(); ++i)
    {
        _str += wchar2char(_wstr.at(i));
    }
    return _str;
}
/*
wstring DictionaryTools::char2wchar(char c)
{
    if (linuxAlphabet.count(c) > 0)
    {
        return linuxAlphabet.at(c);
    }
    return L"?";
}

wstring DictionaryTools::string2wstring(string _str)
{
    wstring _wstr = L"";
    for (int i = 0; i < (int) _str.length(); ++i)
    {
        _wstr += char2wchar(_str.at(i));
    }
    return _wstr;
}
*/

bool DictionaryTools::isSuffix(const wstring & s1, const wstring & s2)
{
    size_t l1 = s1.length();
    size_t l2 = s2.length();
    if (l1 > l2)
    {
        return false;
    }
    size_t i1 = 0;
    size_t i2 = l2 - l1;
    for (size_t j = 0; j < l1; ++j)
    {
        if (s1.at(i1) != s2.at(i2))
        {
            return false;
        }
        i1++;
        i2++;
    }
    return true;
}


