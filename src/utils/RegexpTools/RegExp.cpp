/** 
 * @file	RegExp.cpp
 * @brief	source file for RegExpTools class for processing regular expressions
 */


#include <fstream>
#include <iostream>

#include "RegExp.h"
#include "Tools.h"


using namespace Tools;
using namespace boost;

const wstring RegExpDictionaryFileName = L".\\Dictionaries\\RegExp.txt"; /**<Name of filename with list of regexps*/

void RegExpTools::loadRegularExpressions()
{
	wstring filename = RegExpDictionaryFileName;
	loadRegularExpressions(filename);
}

void RegExpTools::loadRegularExpressions(wstring filename)
{
#ifdef MSVC
	std::wifstream inputStream( filename );
#else
	string name;
	Tools::ConvertToString(filename,name);
	wifstream inputStream(name.c_str());
//	inputStream.open( name.c_str() , std::fstream::in);
#endif

	if( !inputStream ) 
	{
        std::wcout << L"Report open file error " << filename << std::endl;
		return;
	}
	vector<CTokenWord> variables;
	bool isVariableDefinition = false; // variables definition section is surrounded by PartOfFileDelimiter 
	regExpDictionary.clear();
	while( inputStream.good() ) 
	{
		wchar_t line[1000];
		inputStream.getline( line, 1000 );
		std::wstring newWord( line );
		if( newWord == Final ) 
		{
			return;
		}
		if( Tools::FindSubstring( newWord, CommentStringIndicator ) == 0 ) 
		{
			continue;
		}
		if ( newWord == PartOfFileDelimiter ) 
		{
			isVariableDefinition = !isVariableDefinition;
			continue;
		}
		if ( isVariableDefinition ) 
		{
			readRegExpVariable(newWord,variables);
			continue;
		}
		std::vector<std::wstring> words;
		Tools::ParseWstring( words, newWord, RegExpDelimiter,false);
		if( words.size() != 2 ) 
		{
            std::wcout << L"Bad RegExp preprocessing dictionary!" << std::endl;
			return;
		}
		replaceRegExpVariable(words[0],variables);
		CTokenWord token = { words[0], words[1] };
		regExpDictionary.push_back( token );
	}
	inputStream.close();
}

void RegExpTools::getRegExpDictionary(std::vector<CTokenWord>& regExpDictionary_)
{
	regExpDictionary_ = regExpDictionary;
}

void RegExpTools::loadRegexpReplacementList(const string& i_filename)
{
    wifstream inputStream(i_filename.c_str());

    if( !inputStream )
    {
        std::wcout<< L"Can\t open file with regexps" << std::endl;
        return;
    }
    m_regexpReplacementList.clear();
    wstring line;
    while( std::getline(inputStream, line) )
    {
        if ((*line.c_str()) == L'#' || line.empty())
        {
            continue;
        }
        vector<wstring> splitted = Tools::Split(line,L"\t");
        if (splitted.size() != 2)
        {
            continue;
        }
        wregex re(splitted[0]);
        if (re.empty())
            continue;
        pair<wregex,wstring> p;
        p.first = re;
        p.second = splitted[1];
        m_regexpReplacementList.push_back( p );
    }
    inputStream.close();
}

wstring RegExpTools::applyRegexReplacementList(wstring& i_wstring)
{
    wstring wstr = i_wstring;
    wstring wstr_ = wstr;
    int count = 0;
    for (size_t i = 0; i < m_regexpReplacementList.size();)
    {
        wstr_ = Tools::ReplaceRegExp(wstr,m_regexpReplacementList[i].first,m_regexpReplacementList[i].second,true);
        if (wstr_!=wstr)
        {
            wstr = wstr_;
            count ++;
            i--;
            if (count == 10000)
            {
                std::wcout << L"May be we apply regexp in circle" << std::endl;
                i++;
            }
        }
        else
        {
            count = 0;
        }
        i++;
    }
    return wstr;
}

bool Tools::MatchStrOnRegExp(const wstring& str, const wregex& xRegEx)
{
	boost::wsmatch xResults;
	return boost::regex_match(str,  xResults, xRegEx);
}

bool Tools::CheckStrOnRegExp(const wstring& str, const boost::wregex& xRegEx, vector<wstring>& result, vector<int>& idx)
{

	result.clear();
	idx.clear();
	boost::wsmatch xResults;
	wstring::const_iterator xItStart = str.begin();
	wstring::const_iterator xItEnd = str.end();
	bool hasRegExp = false;
	int len = str.length();
	while( boost::regex_search(xItStart, xItEnd, xResults, xRegEx) )
	{
		idx.push_back(len - xResults.suffix().length() - xResults[0].length());
		result.push_back(xResults[0]);
		xItStart = xResults.suffix().first;
		hasRegExp = true;
	}
	return hasRegExp;
}

bool Tools::CheckStrOnRegExp(const wstring& str, const boost::wregex& xRegEx, vector<wstring>& substr, wstring& result)
{
	boost::wsmatch xResults;
	wstring::const_iterator xItStart = str.begin();
	wstring::const_iterator xItEnd = str.end();

	if ( boost::regex_search(xItStart, xItEnd, xResults, xRegEx) )
	{
		result = xResults[0];
		int size = xResults.size();
		for (int i = 1; i < size; i++)
		{
			substr.push_back(xResults[i]);
		}
		return true;
	}
	
	return false;
}

bool Tools::CheckStrOnRegExp(const wstring& str, const boost::wregex& xRegEx, vector<wstring>& result)
{
	boost::wsmatch xResults;
	wstring::const_iterator xItStart = str.begin();
	wstring::const_iterator xItEnd = str.end();
	bool flag = false;

	while( boost::regex_search(xItStart, xItEnd, xResults, xRegEx) )
	{
		result.push_back(xResults[0]);
		xItStart = xResults.suffix().first;
		flag = true;
	}
	
	return flag;
}

bool Tools::CheckStrOnRegExp(const wstring& str, const boost::wregex& xRegEx)
{
	boost::wsmatch xResults;
	wstring::const_iterator xItStart = str.begin();
	wstring::const_iterator xItEnd = str.end();
	return boost::regex_search(xItStart, xItEnd, xResults, xRegEx) ;
}

bool Tools::CheckStrOnRegExp(const wstring& str, const boost::wregex& xRegEx, wstring& result)
{
	boost::wsmatch xResults;
	wstring::const_iterator xItStart = str.begin();
	wstring::const_iterator xItEnd = str.end();
	if( boost::regex_search(xItStart, xItEnd, xResults, xRegEx) )
	{
		result = xResults[0];
		return true;
	}
	
	return false;
}

wstring Tools::ReplaceRegExp(const wstring& str, const boost::wregex& xRegEx, const wstring& formatString, bool with_underscore)
{
	wstring res = boost::regex_replace(str, xRegEx, formatString, boost::format_first_only | boost::format_perl);
	size_t index=0;
    if (!with_underscore)
    {
        while ( (index=res.find('_') ) != std::string::npos )
        {
            res.erase(index, 1);
        }
    }
	return res;
}

bool Tools::MatchStrOnRegExp(const wstring& str, const wstring& regex)
{
	boost::wregex xRegEx(regex);
	return MatchStrOnRegExp(str, xRegEx);
}

bool Tools::CheckStrOnRegExp(const wstring& str, const wstring& regex, vector<wstring>& result, vector<int>& idx)
{
	boost::wregex xRegEx(regex);
	return CheckStrOnRegExp(str, xRegEx, result, idx);
}

bool Tools::CheckStrOnRegExp(const wstring& str, const wstring& regex, vector<wstring>& substr, wstring& result)
{
	boost::wregex xRegEx(regex);
	return CheckStrOnRegExp(str, xRegEx, substr, result);
}

bool Tools::CheckStrOnRegExp(const wstring& str, const wstring& regex, vector<wstring>& result)
{
	boost::wregex xRegEx(regex);
	return CheckStrOnRegExp(str, xRegEx, result);
}

bool Tools::CheckStrOnRegExp(const wstring& str, const wstring& regex)
{
	boost::wregex xRegEx(regex);
	return CheckStrOnRegExp(str, xRegEx);
}

bool Tools::CheckStrOnRegExp(const wstring& str, const wstring& regex, wstring& result)
{
	boost::wregex xRegEx(regex);
	return CheckStrOnRegExp(str, xRegEx, result);
}

wstring Tools::ReplaceRegExp(const wstring& str, const wstring& regex, const wstring& formatString)
{
	boost::wregex xRegEx(regex);
	return ReplaceRegExp(str, xRegEx, formatString);

}

// read string with variable and add into vector of variables name and value
void RegExpTools::readRegExpVariable(const wstring& str, vector<CTokenWord>& variables) const
{
	int delimiterPos = Tools::FindSubstring( str, PartOfRegExpVarDelimiter );
	if( delimiterPos == NOT_FOUND ) 
	{
        std::wcout << L"Bad variableString: " <<  str << std::endl;
		return;
	}
	int valuePos = delimiterPos + PartOfRegExpVarDelimiter.length();
	CTokenWord var;
	var.Token = Tools::RemoveSurrounding( str.substr( 0, delimiterPos ), Tools::SpaceSymbols );
	var.Replacement = Tools::RemoveSurrounding( str.substr( valuePos ), Tools::SpaceAndCaretSymbols );
	variables.push_back(var);
}

void RegExpTools::replaceRegExpVariable(wstring& str, const vector<CTokenWord>& variables) const
{
	std::vector<std::wstring> parsed;
	Tools::ParseWstring(parsed, str, RegExpHighLightSign, true);
	if( parsed.size() == 0 ) 
	{
		return;
	}
	wstring newString;
	std::vector<bool> wasReplace;
	wasReplace.assign( parsed.size(), false );
	for ( unsigned int i = 1; i < parsed.size()-1; i++) 
	{
		if (wasReplace[i-1] == true) 
		{
			continue;
		}
		int varIndex = findToken( variables, parsed[i] );
		if( varIndex != NOT_FOUND ) 
		{
			wasReplace[i] = true;
			parsed[i] = variables[varIndex].Replacement;
		}
	}
	newString += parsed[0];
	for ( unsigned int i = 1; i < parsed.size(); i++)
	{
		if(wasReplace[i] || wasReplace[i-1] ) 
		{
			newString += parsed[i];
		} 
		else
		{
			newString += RegExpHighLightSign;
			newString += parsed[i];
		}
	}
	str = newString;
}

// returns found token index, or NotFound
int RegExpTools::findToken( const vector<CTokenWord>& tokens, const wstring tokenToFind ) const
{
	for( unsigned int i = 0; i < tokens.size(); i++ ) 
	{
		if( tokens[i].Token == tokenToFind ) 
		{
			return i; 
		}
	}
	return NOT_FOUND;
}
