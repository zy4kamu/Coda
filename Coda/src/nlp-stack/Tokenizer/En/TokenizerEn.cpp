#include "TokenizerEn.h"
#include "FileReader.h"
#include "Tools.h"

#include <boost/regex.hpp>

#include <cwctype>
#include <stack>

namespace Tokenization
{

TokenizerEn::TokenizerEn()
{
}

vector<Token> TokenizerEn::Tokenize(
        const wstring& sentence) const
{
    vector<size_t> positions;
    return TokenizeWithAlignment(sentence, positions);
}

vector<Token> TokenizerEn::TokenizeWithAlignment(
        const wstring& sentence, vector<size_t>& o_positions) const
{
    vector<wstring> splitted;
    o_positions.clear();
    vector<Token> tokens = processSentence(sentence, splitted, o_positions);

/*    for(size_t index = 0; index < tokens.size(); index++) {
        std::wcout << tokens[index].content << L" ";
    }
    std::wcout << endl;
    */
    return tokens;
}

Tools::Language TokenizerEn::GetLanguage() const
{
    return Tools::Language::EN;
}

vector<Token> TokenizerEn::processSentence(const wstring& i_sentence, vector<wstring>& o_content, vector<size_t>& o_positions) const
{
    o_content.clear();
    o_positions.clear();
    splitLine(i_sentence, o_content, o_positions);

    vector<Token> tokens = fillSourceTokens(i_sentence, o_content, o_positions);

    for(size_t index = 0; index < o_content.size(); index++)
    {
        o_content[index] = replaceBracketsAndSpecialCharacters(o_content[index]);
    }
    replaceQuotations(i_sentence, o_positions, o_content );
    for(size_t index = 0; index < tokens.size(); index++)
    {
        tokens[index].content = o_content[index];
    }
    return tokens;
}

void TokenizerEn::splitLine(const wstring& i_sentence, vector<wstring>& o_content, vector<size_t>& o_positions) const
{
    // find last non-punctuator
    int lastLetter = (int)i_sentence.length()-1;
    for(; lastLetter >= 0; --lastLetter)
    {
        if(!isSpace(i_sentence[lastLetter]) && !isPunct(i_sentence[lastLetter]))
        {
            break;
        }
    }

    o_content.clear();
    o_positions.clear();
    size_t wordBegin = i_sentence.length();
    for(size_t pos = 0; pos <= i_sentence.length(); pos++)
    {
        if(pos == i_sentence.length()
            || isSpace(i_sentence[pos]) )
        {
            if(wordBegin < pos)
            {
                wstring word = i_sentence.substr(wordBegin, pos - wordBegin);
                vector<wstring> newContent;
                vector<size_t> newPos;
                splitSpecialSymbols(word, newContent, newPos, (pos > lastLetter) );
                o_content.insert(o_content.end(), newContent.begin(), newContent.end());
                for(auto pos_iter = newPos.begin(); pos_iter != newPos.end(); ++pos_iter)
                {
                    (*pos_iter)+=wordBegin;
                }
                o_positions.insert(o_positions.end(), newPos.begin(), newPos.end());
                wordBegin = i_sentence.length();
            }
        } else if(wordBegin > pos) {
            wordBegin = pos;
        }
    }
}

void TokenizerEn::splitSpecialSymbols(const wstring& i_word, vector<wstring>& o_content, vector<size_t>& o_positions, bool isSentenceEnd) const
{
    o_content.clear();
    o_positions.clear();
    // i_word should not contain spaces
    wstring result = i_word;
    //splitting punctuations
    static const boost::wregex punktPattern(L"(\\w|[[.period.]])((?!\"|[[.period.]])[[:punct:]]+)(?=\\s|$)");
    static const boost::wregex apostrophPairPattern(L"(^|\\s)(')([^\"'\\s]+)(')(\\s|$|\\W|[[:punct:]])");
    static const boost::wregex n_tPattern(L"n't");
    static const boost::wregex modalPattern(L"'(s|m|(re)|(ll)|d|(ve))(?=\\W|$)");
    static const boost::wregex rightApostrophPattern(L"'(\\W)");
    static const boost::wregex wonnaPattern(L"\\b(won|gon)(na)\\b");
    static const boost::wregex bracketsPattern(L"([\\(\\[\\{\\)\\]\\}\"])");
    static const boost::wregex specialCharPattern(L"([&%$])");

    static const wstring apostrophPairReplacement(L"$1 $2 $3 $4 $5");
    static const wstring n_tReplacement(L" n't");
    static const wstring modalReplacement(L" '$1");
    static const wstring rightApostrophReplacement(L" '$1");
    static const wstring wonnaReplacement(L"$1 $2");
    static const wstring bracketsReplacement(L" $1 ");
    static const wstring specialCharReplacement(L" $1 ");

    static const wstring replacement = L"$1 $2 ";
    result = boost::regex_replace(result, punktPattern, replacement);
    //left apostroph
    result = boost::regex_replace(result, apostrophPairPattern, apostrophPairReplacement),
    // children's --> children 's; won't --> wo n't etc
    result = boost::regex_replace(result, n_tPattern, n_tReplacement);
    result = boost::regex_replace(result, modalPattern, modalReplacement);
    result = boost::regex_replace(result, rightApostrophPattern, rightApostrophReplacement);
    // split special words
    // gonna --> gon na wonna --> won na
    result = boost::regex_replace(result, wonnaPattern, wonnaReplacement);
    // split brackets and quotations
    result = boost::regex_replace(result, bracketsPattern, bracketsReplacement);
    // split special characters
    result = boost::regex_replace(result, specialCharPattern, specialCharReplacement);
    if(isSentenceEnd)
    {
        result = splitEndingPeriod(result);
    }
    result = normalizeSpaces(result);
    o_content = Tools::Split(result, L" ");
    size_t pos = 0;
    o_positions.resize(o_content.size());
    for(size_t index = 0; index < o_content.size(); index++) {
        o_positions[index] = pos;
        pos += o_content[index].length();
    }
}

wstring TokenizerEn::splitEndingPeriod(const wstring& i_word) const
{
    static const boost::wregex endPeriodPattern(L"([[.period.]]+\\W*$)");

    return boost::regex_replace(i_word, endPeriodPattern, L" $1");
}

vector<Token> TokenizerEn::fillSourceTokens(const wstring& i_sentence, const vector<wstring>& i_content, const vector<size_t>& i_positions) const
{
    vector<Token> tokens;
    for (size_t index = 0; index < i_content.size();
        ++index)
    {
        Token token(i_content[index], vector<wstring>() );
        wstring source = i_sentence.substr(i_positions[index], i_content[index].length());
        token.source = source;
        size_t next = i_positions[index] + i_content[index].length();
        token.isNextSpace = true;
        if(next  == i_sentence.length()
                || !isSpace(i_sentence[next]))
        {
            token.isNextSpace = false;
        }
        tokens.push_back(token);
    }
    return tokens;
}

wstring TokenizerEn::replaceBracketsAndSpecialCharacters(const wstring& i_word) const
{
    static const boost::wregex lrbPattern(L"\\(");
    static const boost::wregex rrbPattern(L"\\)");
    static const boost::wregex lsbPattern(L"\\[");
    static const boost::wregex rsbPattern(L"\\]");
    static const boost::wregex lcbPattern(L"\\{");
    static const boost::wregex rcbPattern(L"\\}");
    static const boost::wregex slashPattern(L"/");
    static const boost::wregex commaPattern(L"^,$");
    static const boost::wregex numberPattern(L"^([0-9]*,)*[[.period.]]?[0-9]+$");

    static const wstring lrbReplacement(L"-LRB-");
    static const wstring rrbReplacement(L"-RRB-");
    static const wstring lsbReplacement(L"-LSB-");
    static const wstring rsbReplacement(L"-RSB-");
    static const wstring lcbReplacement(L"-LCB-");
    static const wstring rcbReplacement(L"-RCB-");
    static const wstring slashReplacement(L"\\\\/");
//    static const wstring commaReplacement(L"COMMA");
    static const wstring numberReplacement(L"NUMBER");

    //bracket are replaced to secial notation (acronims): ( ) [ ] { } --> -LRB- -RRB- -RSB- -RSB- -LCB- -RCB- ((Left|Right) (Round|Square|Curly) Bracket)
    wstring result = i_word;
    result = boost::regex_replace(result, lrbPattern, lrbReplacement);
    result = boost::regex_replace(result, rrbPattern, rrbReplacement);
    result = boost::regex_replace(result, lsbPattern, lsbReplacement);
    result = boost::regex_replace(result, rsbPattern, rsbReplacement);
    result = boost::regex_replace(result, lcbPattern, lcbReplacement);
    result = boost::regex_replace(result, rcbPattern, rcbReplacement);
//    result = boost::regex_replace(result, commaPattern, commaReplacement);
    result = boost::regex_replace(result, numberPattern, numberReplacement);
    //'/' is replaced by '\/'
    result = boost::regex_replace(result, slashPattern, slashReplacement);
    return result;
}

bool TokenizerEn::isSpace(wchar_t i_char) const
{
    return iswspace(i_char);
}

bool TokenizerEn::isPunct(wchar_t i_char) const
{
    return iswpunct(i_char);
}

bool TokenizerEn::isWord(const wstring& word) const
{
    static const boost::wregex punctWord = boost::wregex(L"[[:punct:]]+");
    return !boost::regex_match(word.begin(), word.end(), punctWord);
}

wstring TokenizerEn::normalizeSpaces(const wstring& str) const
{
    wstring result = Tools::Trim(str);
    static const boost::wregex spaces(L"(\\s)+");
    return boost::regex_replace(result, spaces, L" ");
}

void TokenizerEn::replaceQuotations(const wstring& i_sentence, const vector<size_t>& i_positions, vector<wstring>& o_content) const
{
    // at first, we replace sure quotation pairs like "word", which, in theory, could occure inside long qoutationed phrase,
    // e.g. "I read "Times" every day," -- she said.
    wstring str = i_sentence;
    static const boost::wregex sureQuotationPair(L"(^|\\s)(\")([^\"\\s]+)(\")(\\s|$|\\W|[[:punct:]])");
    static const boost::wregex apostrophPair(L"(^|\\s)(')([^\"'\\s]+)(')(\\s|$|\\W|[[:punct:]])");
    static const boost::wregex quotationPair(L"(^|\\s)(\")([^\"]*[^\"\\s][^\"]*)(\")");
    static const boost::wregex left(L"(^|\\s)(\")");
    static const boost::wregex right(L"(\")(\\s|$|\\W|[[:punct:]])");

    vector<size_t> left_single;
    vector<size_t> right_single;
    vector<size_t> left_double;
    vector<size_t> right_double;

 //   const int groups[] = {2,3,4};
    std::wstring::const_iterator str_begin = str.begin();
    std::wstring::const_iterator end = str.end();
    wchar_t tmp_replacement = L'_';
    boost::wsmatch what;

    std::wstring::const_iterator begin = str.begin();
    while(boost::regex_search(begin, end, what, sureQuotationPair, boost::match_default))
    {
        left_double.push_back(std::distance(str_begin, what[2].first));
        right_double.push_back(std::distance(str_begin, what[4].first));
        str[*(left_double.end()-1)] = tmp_replacement;
        str[*(right_double.end()-1)] = tmp_replacement;
        begin = what[4].second+1;
        if(std::distance(begin, end) <= 0 ) {
            break;
        }
    }

    begin = str.begin();
    while(boost::regex_search(begin, end, what, apostrophPair, boost::match_default))
    {
        left_single.push_back(std::distance(str_begin, what[2].first));
        right_single.push_back(std::distance(str_begin, what[4].first));
        str[*(left_single.end()-1)] = tmp_replacement;
        str[*(right_single.end()-1)] = tmp_replacement;
        begin = what[4].second+1;
        if(std::distance(begin, end) <= 0 ) {
            break;
        }
    }

    begin = str.begin();
    while(boost::regex_search(begin, end, what, quotationPair, boost::match_default))
    {
        left_double.push_back(std::distance(str_begin, what[2].first));
        right_double.push_back(std::distance(str_begin, what[4].first));
        str[*(left_double.end()-1)] = tmp_replacement;
        str[*(right_double.end()-1)] = tmp_replacement;
        begin = what[4].second+1;
        if(std::distance(begin, end) <= 0 ) {
            break;
        }
    }

    begin = str.begin();
    while(boost::regex_search(begin, end, what, left, boost::match_default))
    {
        left_double.push_back(std::distance(str_begin, what[2].first));
        str[*(left_double.end()-1)] = tmp_replacement;
        begin = what[2].second+1;
        if(std::distance(begin, end) <= 0 ) {
            break;
        }
    }

    begin = str.begin();
    while(boost::regex_search(begin, end, what, right, boost::match_default))
    {
        right_double.push_back(std::distance(str_begin, what[1].first));
        begin = what[1].second+1;
        if(std::distance(begin, end) <= 0 ) {
            break;
        }
    }

    std::sort(left_double.begin(), left_double.end());
    std::sort(right_double.begin(), right_double.end());

    size_t left_double_pos = 0;
    size_t right_double_pos = 0;
    size_t left_single_pos = 0;
    size_t right_single_pos = 0;
    for(size_t word_index = 0; word_index < o_content.size(); ++word_index) {
        if( left_double_pos < left_double.size() && i_positions[word_index] == left_double[left_double_pos] )
        {
            o_content[word_index] = L"``";
            left_double_pos++;
            continue;
        }
        if( right_double_pos < right_double.size() && i_positions[word_index] == right_double[right_double_pos] )
        {
            o_content[word_index] = L"''";
            right_double_pos++;
            continue;
        }
        if( left_single_pos < left_single.size() && i_positions[word_index] == left_single[left_single_pos] )
        {
            o_content[word_index] = L"`";
            left_single_pos++;
            continue;
        }
        if( right_single_pos < right_single.size() && i_positions[word_index] == right_single[right_single_pos] )
        {
            o_content[word_index] = L"'";
            right_single_pos++;
            continue;
        }
        if(o_content[word_index].compare(L"\"") == 0) {
            o_content[word_index] = L"''";
        }
    }
}


}
