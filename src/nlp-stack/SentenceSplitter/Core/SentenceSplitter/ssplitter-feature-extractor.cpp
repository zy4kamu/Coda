/**
 * @file    feature-extractor.cpp
 * @brief	implementations
 */

#include "ssplitter-feature-extractor.h"
#include <cwctype>
#include <functional>
#include <iostream>
#include <fstream>
#include "ssplitter-exceptions.h"
#include "Tools.h"

namespace _sentence_splitter
{

FeatureExtractor::FeatureExtractor(int i_left_context_length, int i_right_context_length)
    : m_left_context_length(i_left_context_length)
    , m_right_context_length(i_right_context_length)
{
}

FeatureExtractor::FeatureExtractor(int i_left_context_length, int i_right_context_length, std::string i_data)
    : m_left_context_length(i_left_context_length)
    , m_right_context_length(i_right_context_length)
{
    std::wifstream in(i_data);
    if(!in) {
        throw(_sentence_splitter::FileOpenException(i_data.c_str()));
    }

    std::vector<std::wstring> feats_from_file;

    int lineCounter = -1;
    std::wstring data;
    while(std::getline(in, data))
    {
        lineCounter++;
        data = Tools::Trim(data);

        if(data.empty())
        {
            continue;
        }

        feats_from_file = Tools::Split(data, L"\t");
        if(feats_from_file.size() != 3) {
            throw(_sentence_splitter::ModelException(i_data.c_str(), lineCounter));
        }
        std::pair<double, double> values;
        double value;
        std::wistringstream first_buffer(feats_from_file[1]);
        if(!(first_buffer >> value)) {
            throw(_sentence_splitter::ModelException(i_data.c_str(), lineCounter));
        }
        values.first = value;
        std::wistringstream second_buffer(feats_from_file[2]);
        if(!(second_buffer >> value)) {
            throw(_sentence_splitter::ModelException(i_data.c_str(), lineCounter));
        }
        values.first = value;

        wordsMap[ feats_from_file[0] ] = values;
    }
}

std::unordered_map<std::wstring, double> FeatureExtractor::getDoubleFeatures(size_t i_position, const std::wstring& i_line_to_split)
{
    std::unordered_map<std::wstring, double> result;
    getWordFeatures(i_position, i_line_to_split, result);
    return result;
}

void FeatureExtractor::getWordFeatures(size_t i_position, const std::wstring& i_line_to_split,
                                         std::unordered_map<std::wstring, double>& o_result)
{
    if(i_position == 0)
    {
        return;
    }
    int pos = i_position-1;
    for(; pos >= 0 && std::iswalnum(i_line_to_split[pos]); pos--) {};
    if(size_t(pos)==i_position-1)
    {
        return;
    }
    std::wstring word = i_line_to_split.substr(pos+1, i_position-pos-1);

    pos = i_position;
    for(; size_t(pos) < i_line_to_split.length() && std::iswpunct(i_line_to_split[pos]); pos++) {};
    if(size_t(pos)==i_position)
    {
        return;
    }
    std::wstring punct = i_line_to_split.substr(i_position, pos - i_position);

    std::wstring word_punct = word+punct;

//  model format: word; end count / 1000; total count / 1000
    std::pair<double,double> params;
    double delta = 0.001;
    params = ( wordsMap.find(word) != wordsMap.end() ) ?
                wordsMap[word] : std::make_pair(0., 0.);
    o_result[L"END_WORD"] = params.first + delta;
    o_result[L"TOTAL_WORD"] = params.second + delta;

    params = ( wordsMap.find(word_punct) != wordsMap.end() ) ?
                wordsMap[word_punct] : std::make_pair(0., 0.);
    o_result[L"END_WORD_PUNCT"] = params.first + delta;
    o_result[L"TOTAL_WORD_PUNCT"] = params.second + delta;

    params = ( wordsMap.find(punct) != wordsMap.end() ) ?
                wordsMap[punct] : std::make_pair(0., 0.);
    o_result[L"END_PUNCT"] = params.first + delta;
    o_result[L"TOTAL_PUNCT"] = params.second + delta;
}

std::vector<std::wstring> FeatureExtractor::getFeatures(size_t i_position, const std::wstring& i_line_to_split)
{
    int window_length = 1 + m_left_context_length + m_right_context_length;
    m_effective_left = 0;
    std::vector<std::wstring> resultTmp(window_length, L"");

    resultTmp[m_left_context_length].push_back(i_line_to_split[i_position]);

    //here we just implement regex: /([[:word:]]+|[[:punct:]]){2}/ to both sides of the found symbol. I didn't find the way to seek left to right in boost::regex.
    //so, here it is

    //two tokens to the right
    windowFstMatch_(resultTmp
                 , i_line_to_split
                 , i_position+1
                 , m_left_context_length + 1
                 , [&](int a, int b) -> bool {return a < (int)i_line_to_split.length() && b < window_length;}
                 , [](int& a){a++;}
                 , [&](std::wstring& str){return;});

    //two tokens to the left
    windowFstMatch_(resultTmp
                 , i_line_to_split
                 , i_position-1
                 , m_left_context_length - 1
                 , [&](int a, int b) -> bool {return a >= 0 && b >= 0;}
                 , [](int& a){a--;}
                 , [&](std::wstring& str){revertString_(str);});

    std::vector<std::wstring> result;

    bool startMark = false, endMark = false;

    for(int i = 0; i < window_length; i++)
    {
        if(!resultTmp[i].empty())
        {
            result.push_back(resultTmp[i]);
            if(i < m_left_context_length)
                m_effective_left++;
        }
        else if(i > m_left_context_length && !endMark)
        {
            endMark = true;
            result.push_back(L"<END>");
        }
        else if(i < m_left_context_length && !startMark)
        {
            startMark = true;
            m_effective_left++;
            result.push_back(L"<START>");
        }
    }

    result = getFeatureCodes_(result);
    if(i_position < i_line_to_split.length() - 2 && i_line_to_split[i_position + 1] == L' ')
    {
        result.push_back(L"NEXT_SPACE");
    }

    return result;
}

void FeatureExtractor::windowFstMatch_(std::vector<std::wstring>& io_resultTmp, const std::wstring& i_line_to_split, int i_start_position_in_line, int i_start_position_in_window, std::function<bool(int i, int j)> i_predicate,  std::function<void(int&)> i_iterate, std::function<void(std::wstring&)> i_postProcess)
{
    int positionInWindow = i_start_position_in_window;
    char state = 1; //1 = PREV_SPACE, 2 = PREV_CHARACTER, //3 = PREV_PUNCT

    for(int i = i_start_position_in_line; i_predicate(i, positionInWindow); i_iterate(i))
    {
        if(std::iswalnum(i_line_to_split[i]))
        {
            if(state == 1)
            {
                state = 2;
                io_resultTmp[positionInWindow].push_back(i_line_to_split[i]);
            }
            else if(state == 2)
            {
                io_resultTmp[positionInWindow].push_back(i_line_to_split[i]);
            }
            else if(state == 3)
            {
                state = 2;
                io_resultTmp[positionInWindow].push_back(i_line_to_split[i]);
            }
        }
        else if(std::iswpunct(i_line_to_split[i]))
        {
            if(state == 1)
            {
                state = 3;
                io_resultTmp[positionInWindow].push_back(i_line_to_split[i]);
                //i_iterate(positionInWindow);
            }
            else if(state == 2)
            {
                state = 3;
                i_postProcess(io_resultTmp[positionInWindow]);
                i_iterate(positionInWindow);
                if(i_predicate(i, positionInWindow))
                {
                    io_resultTmp[positionInWindow].push_back(i_line_to_split[i]);
                }
            }
            else if(state == 3)
            {
                i_iterate(positionInWindow);
                if(i_predicate(i, positionInWindow))
                {
                    io_resultTmp[positionInWindow].push_back(i_line_to_split[i]);
                }
            }
        }
        else if(!std::iswgraph(i_line_to_split[i]))
        {
            if(state == 2)
            {
                state = 1;
                i_postProcess(io_resultTmp[positionInWindow]);
                i_iterate(positionInWindow);
            }
            else if(state == 3)
            {
                state = 1;
                i_iterate(positionInWindow);
            }
        }
    }

    if(state == 2)
    {
        i_postProcess(io_resultTmp[positionInWindow]);
    }
}

void FeatureExtractor::revertString_(std::wstring& io_string_to_revert)
{
    int len = io_string_to_revert.length();

    for (int i = 0; i < len/2; i++)
    {
        std::swap(io_string_to_revert[i], io_string_to_revert[len-i-1]);
    }
}

bool FeatureExtractor::isNormalCharacter_( wchar_t i_character ) const
{
    return std::iswalpha(i_character) || i_character == L'-';
}

bool FeatureExtractor::isDigit_( wchar_t i_character ) const
{
    return std::iswdigit(i_character);
}

bool FeatureExtractor::isPunctuator_( wchar_t i_character ) const
{
    static std::wstring punctuators = L".,-!?%#;:»«()*[]{}\"";
    return( punctuators.find( i_character ) != std::wstring::npos );
}

bool FeatureExtractor::isQuotation_( wchar_t i_character ) const
{
    static std::wstring quotation = L"\"";
    return( quotation.find( i_character ) != std::wstring::npos );
}

bool FeatureExtractor::isCons_( wchar_t i_character ) const
{
    static std::wstring vowels = L"\u0411\u0412\u0413\u0414\u0416\u0417\u0419\u041a\u041b\u041c\u041d\u041f\u0420\u0421\u0422\u0424\u0425\u0426\u0427\u0428\u0429\u0431\u0432\u0433\u0434\u0436\u0437\u0439\u043a\u043b\u043c\u043d\u043f\u0440\u0441\u0442\u0444\u0445\u0446\u0447\u0448\u0449";//here was cyrrilic symbols: БВГДЖЗЙКЛМНПРСТФХЦЧШЩбвгджзйклмнпрстфхцчшщ
    return( vowels.find( i_character ) != std::wstring::npos );
}

bool FeatureExtractor::isUppercase_( wchar_t i_character ) const
{
    return std::iswupper(i_character);
}

bool FeatureExtractor::isSentenceEndPunctuator_( wchar_t i_character ) const
{
    static std::wstring sentenceEndPunctuators = L".!??;";
    return sentenceEndPunctuators.find( i_character ) != std::wstring::npos;
}

std::vector<std::wstring> FeatureExtractor::getFeatureCodes_(std::vector<std::wstring>& i_tokens)
{
    std::vector<std::wstring> result;

    static std::wstring feat_pref_last_letter_cons_ = L"E_LETTER_CONS";
    static std::wstring feat_punct = L"PUNCT";
    static std::wstring feat_quot = L"QUOT";
    static std::wstring feat_non_punct = L"NON_PUNCT";
    static std::wstring feat_endsym = L"SE_PUNCT";
    static std::wstring feat_upper_case = L"UPC";
    static std::wstring feat_length1 = L"LEN=1";
    static std::wstring feat_length2 = L"LEN=2";
    static std::wstring feat_length3 = L"LEN=3";
    static std::wstring feat_length4 = L"LONG";

    for(int i = 0; i < (int)i_tokens.size(); i++)
    {
        std::wstring distanceSuffix = (i - m_effective_left == 0) ? L"" : L"_" + std::to_wstring(i - m_effective_left);
        if(i_tokens[i] == L"<START>" || i_tokens[i] == L"<END>")
        {
            result.push_back(i_tokens[i] + distanceSuffix);
        }
        else if(isPunctuator_(i_tokens[i][0]) && isPunctuator_(i_tokens[i][i_tokens[i].size()-1]))
        {
            result.push_back(feat_punct + distanceSuffix);
            if (isSentenceEndPunctuator_(i_tokens[i][0])) result.push_back(feat_endsym + distanceSuffix);
            for(size_t j = 0; j < i_tokens[i].size(); j++)
            {
                if(isQuotation_(i_tokens[i][j]))
                {
                    result.push_back(feat_quot + distanceSuffix);
                }
            }
        }
        else
        {
            result.push_back(feat_non_punct + distanceSuffix);
            if(isCons_(*(i_tokens[i].end()-1))) result.push_back(feat_pref_last_letter_cons_ + distanceSuffix);


            for(size_t j = 0; j < i_tokens[i].size(); j++)
            {
                if(iswalnum(i_tokens[i][j]))
                {
                    if (isUppercase_(i_tokens[i][j])) result.push_back(feat_upper_case + distanceSuffix);
                    break;
                }
            }

            switch(i_tokens[i].length())
            {
            case 1:
                result.push_back(feat_length1+ distanceSuffix);
                break;
            case 2:
                result.push_back(feat_length2+ distanceSuffix);
                break;
            case 3:
                result.push_back(feat_length3+ distanceSuffix);
                break;
            default:
                result.push_back(feat_length4+ distanceSuffix);
                break;
            }
        }
    }

    return result;

}

} //_sentence_splitter
