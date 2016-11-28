#include "StringTools.h"

#include <cwctype>
#include <cassert>
#include <locale>
#include <functional>
#include <algorithm> 
#include <functional> 
#include <cctype>

#include "Locale/RussianLocale.h"
#include "Locale/ToolsCyrillicSymbols.h"

namespace Tools
{

const wstring SpaceSymbols = L" \t"; /**<space symbols */
const wstring Punctuators = L".,!?:;"; /**<symbols of punctuation*/
const wstring SpaceAndCaretSymbols = L" \t\n"; /**<space and caret symbols*/
const wstring Digits = L"0123456789"; /**<digit symbols*/
const wstring RussianConsonants = L"\u0411\u0412\u0413\u0414\u0416\u0417\u0419\u041a\u041b\u041c\u041d\u041f\u0420\u0421\u0422\u0424\u0425\u0426\u0427\u0428\u0429\u042a\u042c\u0431\u0432\u0433\u0434\u0436\u0437\u0439\u043a\u043b\u043c\u043d\u043f\u0440\u0441\u0442\u0444\u0445\u0446\u0447\u0448\u0449\u044a\u044c";//here was cyrrilic symbols: БВГДЖЗЙКЛМНПРСТФХЦЧШЩЪЬбвгджзйклмнпрстфхцчшщъь
const wstring RussianVowels = L"\u0410\u0415\u0401\u0418\u041e\u0423\u042b\u042d\u042e\u042f\u0430\u0435\u0451\u0438\u043e\u0443\u044b\u044d\u044e\u044f";//here was cyrrilic symbols: АЕЁИОУЫЭЮЯаеёиоуыэюя

wstring Trim_Left(const wstring& s)
{
	wstring ss = s;
    ss.erase(
        ss.begin(), std::find_if(ss.begin(), ss.end()
        , std::not1(std::ptr_fun<int, int>(std::isspace))));
    return ss;
}

string Trim_Left(const string& s)
{
	string ss = s;
    ss.erase(
        ss.begin(), std::find_if(ss.begin(), ss.end()
        , std::not1(std::ptr_fun<int, int>(std::isspace))));
    return ss;
}

wstring Trim_Right(const wstring& s)
{
	wstring ss = s;
    ss.erase(
        std::find_if(ss.rbegin(), ss.rend()
        , std::not1(std::ptr_fun<wint_t, int>(std::iswspace))).base(), ss.end());
    return ss;
}

string Trim_Right(const string& s)
{
	string ss = s;
    ss.erase(
        std::find_if(ss.rbegin(), ss.rend()
        , std::not1(std::ptr_fun<int, int>(std::isspace))).base(), ss.end());
    return ss;
}

string Trim(string s)
{
    string trim_chars = " \t \r \n";
    string::size_type pos = s.find_last_not_of(trim_chars);
    if(pos != string::npos)
    {
        s.erase(pos + 1);
        pos = s.find_first_not_of(trim_chars);
        if(pos != string::npos) s.erase(0, pos);
    }
    else s.erase(s.begin(), s.end());
    return s;
}

wstring Trim(wstring s)
{
    wstring trim_chars = L" \t \r \n";
    wstring::size_type pos = s.find_last_not_of(trim_chars);
    if(pos != wstring::npos)
    {
        s.erase(pos + 1);
        pos = s.find_first_not_of(trim_chars);
        if(pos != wstring::npos) s.erase(0, pos);
    }
    else s.erase(s.begin(), s.end());
    return s;
}

wstring Replace(const wstring& str, const wstring& from, const wstring& to)
{
    wstring replaced = str;
    if(from.empty())
    {
        return replaced;
    }
    size_t start_pos = 0;
    while((start_pos = replaced.find(from, start_pos)) != wstring::npos)
    {
        replaced.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return replaced;
}

string Replace(const string& str, const string& from, const string& to)
{
    string replaced = str;
    if(from.empty())
    {
        return replaced;
    }
    size_t start_pos = 0;
    while((start_pos = replaced.find(from, start_pos)) != string::npos)
    {
        replaced.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return replaced;
}

bool ContainsSubstring(const string& str1, const string& str2)
{
    return std::string::npos != str1.find(str2);
}

bool ContainsSubstring(const wstring& str1, const wstring& str2)
{
    return std::wstring::npos != str1.find(str2);
}

int FindSubstring( const std::string& str, const std::string& substr )
{
    int position = str.find( substr );
    return position == (int)string::npos ? NOT_FOUND : position;
}

int FindSubstring(const std::wstring& str, const std::wstring& substr)
{
    int position = str.find( substr );
    return position == (int)wstring::npos ? NOT_FOUND : position;
}

wstring ToLower(const wstring& wstr)
{
    RussianLocale locale;
    wstring result = wstr;
    for (size_t i = 0; i < wstr.length(); ++i)
    {
        result[i] = towlower(result.c_str()[i]);
    }
    return result;
}

string ToLower(const string& str)
{
    string result = str;
    for (size_t i = 0; i < str.length(); ++i)
    {
        result[i] = towlower(result.c_str()[i]);
    }
    return result;
}

wstring ToUpper(const wstring& wstr)
{
    RussianLocale locale;
    wstring result = wstr;
    for (size_t i = 0; i < wstr.length(); ++i)
    {
        result[i] = towupper(result.c_str()[i]);
    }
    return result;
}

bool StartsWith(const string& str1, const string& str2)
{
    return str1.substr(0, str2.size()) == str2;
}

bool StartsWith(const wstring& str1, const wstring& str2)
{
    return str1.substr(0, str2.size()) == str2;
}

bool EndsWith(const string& str1, const string& str2)
{
    if (str1.size() < str2.size())
    {
        return false;
    }
    for (size_t letterIndex = 0; letterIndex < str2.size(); ++ letterIndex)
    {
        if (str1[str1.size() - 1 - letterIndex] != str2[str2.size() - 1 - letterIndex])
        {
            return false;
        }
    }
    return true;
}

bool EndsWith(const wstring& str1, const wstring& str2)
{
    if (str1.size() < str2.size())
    {
        return false;
    }
    for (size_t letterIndex = 0; letterIndex < str2.size(); ++ letterIndex)
    {
        if (str1[str1.size() - 1 - letterIndex] != str2[str2.size() - 1 - letterIndex])
        {
            return false;
        }
    }
    return true;
}

bool BothAreSpaces0(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

string ReplaceDuplicatedSpaces(const string& str)
{
    string retval = str;
    std::string::iterator new_end = std::unique(retval.begin(), retval.end(), BothAreSpaces0);
    retval.erase(new_end, retval.end());
    return retval;
}

bool BothAreSpaces1(wchar_t lhs, wchar_t rhs) { return (lhs == rhs) && (lhs == L' '); }

wstring ReplaceDuplicatedSpaces(const wstring& str)
{
    wstring retval = str;
    std::wstring::iterator new_end = std::unique(retval.begin(), retval.end(), BothAreSpaces1);
    retval.erase(new_end, retval.end());
    return retval;
}

void RemovePunctuation(wstring& str, vector<wchar_t>& punctuation)
{
    punctuation.clear();
    int punctIndex = str.size();
    for (;punctIndex >0; punctIndex--)
    {
        if (!IsPunctuation(str[punctIndex - 1]))
            break;
    }
    for (size_t i = punctIndex; i < str.size(); ++i)
        punctuation.push_back(str[punctIndex]);
    str = str.substr(0, punctIndex);
}

wstring RemoveAllPunctuation(const wstring& str)
{
    wstring retval = L"";
    for (size_t i = 0; i < str.size(); ++i)
        if (!IsPunctuation(str[i]))
            retval += str[i];
    return retval;
}

void RemoveAllPunctuation(wstring& str, wstring& punctuation)
{
    punctuation.clear();
    int punctIndex = str.size();
    for (;punctIndex >0; punctIndex--)
    {
        if (!IsPunctuation(str[punctIndex - 1]))
            break;
    }
    for (size_t i = punctIndex; i < str.size(); ++i)
        punctuation.push_back(str[punctIndex]);
    punctuation = str.substr(punctIndex);
    str = str.substr(0, punctIndex);
}

wstring PunctuationToString(const wstring& punctuation)
{
    if (punctuation.compare(L",") == 0)
        return L"COMMA";
    else if (punctuation.compare(L":") == 0)
        return L"COLON";
    else if (punctuation.compare(L"-") == 0)
        return L"DASH";
    else if (punctuation.compare(L".") == 0)
        return L"DOT";
    else if (punctuation.compare(L":") == 0)
        return L"COLON";
    else if (punctuation.compare(L"\"") == 0)
        return L"QUOTE";
    else if (punctuation.compare(L"?") == 0)
        return L"QUESTION";
    else if (punctuation.compare(L"!") == 0)
        return L"EXCLAMATION";
    else if (punctuation.compare(L"PUNCTUATION") == 0)
        return L"PUNCTUATION";
    else return L"";
}

wstring PunctuationToString(wchar_t punctuation)
{
    wstring s;
    std::wstringstream ss;
    ss << punctuation;
    ss >> s;
    return PunctuationToString(s);
}

bool HasUpperCaseLetter(const wstring& str)
{
    for (size_t i = 0;i < str.length(); i++ )
    {
        if ( iswupper(str.c_str()[i]) )
        {
            return true;
        }
    }
    return false;
}

bool HasLowerCaseLetter(const wstring& str)
{
    for (size_t i = 0;i < str.length(); i++ )
    {
        if ( iswlower(str.c_str()[i]) )
        {
            return true;
        }
    }
    return false;
}

bool HasPunctLetter(const wstring& str)
{
    for (size_t i = 0;i < str.length(); i++ )
    {
        if ( iswpunct(str.c_str()[i]) )
        {
            return true;
        }
    }
    return false;
}

bool HasDigitLetter(const wstring& str)
{
    for (size_t i = 0;i < str.length(); i++ )
    {
        if ( iswdigit(str.c_str()[i]) )
        {
            return true;
        }
    }
    return false;
}

bool HasAlphaLetter(const wstring& str)
{
    for (size_t i = 0;i < str.length(); i++ )
    {
        if ( iswalpha(str.c_str()[i]) )
        {
            return true;
        }
    }
    return false;
}

bool IsPunctuation(wchar_t ch)
{
    return ch == wchar_t(',') || ch == wchar_t(':') || ch == wchar_t('-') ||
           ch == wchar_t('.') || ch == wchar_t(':') || ch == wchar_t('\\') ||
           ch == wchar_t('?') || ch == wchar_t('!') || ch == wchar_t('(') ||
           ch == wchar_t(')') || ch == wchar_t('[') || ch == wchar_t(']') ||
           ch == wchar_t('{') || ch == wchar_t('}') || ch == wchar_t('"') ||
           ch == wchar_t('*') || ch == wchar_t('\'') || ch == wchar_t(';') ||
           ch == wchar_t('/') || ch == wchar_t('\\');
           //|| ch == wchar_t('«')||ch == wchar_t('»') || ch == wchar_t('„') || ch == wchar_t('“')
}

bool IsNumber(const wstring& str)
{
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (!(str[i] == wchar_t('0')) &&
            !(str[i] == wchar_t('1')) &&
            !(str[i] == wchar_t('2')) &&
            !(str[i] == wchar_t('3')) &&
            !(str[i] == wchar_t('4')) &&
            !(str[i] == wchar_t('5')) &&
            !(str[i] == wchar_t('6')) &&
            !(str[i] == wchar_t('7')) &&
            !(str[i] == wchar_t('8')) &&
            !(str[i] == wchar_t('9')) &&
            !IsPunctuation(str[i]))
            return false;
    }
    return true;
}

bool ContainsLatin(const wstring& data)
{
    for (size_t letterIndex = 0; letterIndex < data.size(); ++letterIndex)
    {
        for (size_t alphabetIndex = 0; alphabetIndex < 25; ++alphabetIndex)
        {
            if (data[letterIndex] == englishUpperAlphabet[alphabetIndex]
                || data[letterIndex] == englishLowerAlphabet[alphabetIndex])
            {
                return true;
            }
        }
    }
    return false;
}

bool IsUpper(wchar_t ch)
{
   #ifdef MSVC
       return isupper(ch, std::locale("Russian"));

   #else
       return isupper(ch, std::locale("ru_RU.utf8"));
   #endif
}

void ParseWstring(std::vector<std::wstring>& parsed, const std::wstring input, const std::wstring delimiter, bool addEmptyStrings = false )
{
    unsigned int pos = 0;
    if( input.empty() )
    {
        if( addEmptyStrings )
        {
            parsed.push_back( L"" );
        }
        return;
    }
    while( pos < input.length() || input.empty() )
    {
        size_t wordEnd = input.find_first_of( delimiter, pos );
        if( wordEnd == wstring::npos )
        {
            wordEnd = input.length();
        }
        std::wstring word = input.substr( pos, wordEnd - pos );
        if( addEmptyStrings || !word.empty() )
        {
            parsed.push_back( word );
        }
        pos = wordEnd + 1;
    }
    // if last character in the input string is delimiter
    if( addEmptyStrings && pos == input.length() )
    {
        parsed.push_back( L"" );
    }
}

std::wstring RemoveSurrounding( const std::wstring source, const std::wstring surrounding )
{
    int begin = source.find_first_not_of( surrounding );
    int end = source.find_last_not_of( surrounding );
    if( begin != NOT_FOUND)
    {
        assert( end >= begin );
        return source.substr( begin, end - begin + 1 );
    }
    return L"";
}

bool IsRussianConsonant(wchar_t letter)
{
    for (size_t index = 0; index < RussianConsonants.size(); ++index)
    {
        if (RussianConsonants[index] == letter)
        {
            return true;
        }
    }
    return false;
}

bool IsRussianVowel(wchar_t letter)
{
    for (size_t index = 0; index < RussianConsonants.size(); ++index)
    {
        if (RussianVowels[index] == letter)
        {
            return true;
        }
    }
    return false;
}

}

