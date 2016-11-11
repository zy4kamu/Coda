#include "TokenizerRus.h"
#include "FileReader.h"
#include "Tools.h"

#include <cwctype>
#include <stack>

namespace Tokenization
{

TokenizerRus::TokenizerRus(
    const string& muliplePartTokensFile,
    const string& dashTokensFile)
{
    FileManager::Read(muliplePartTokensFile, &muliplePartTokens);
    FileManager::Read(dashTokensFile, &dashTokens);
}

wstring TokenizerRus::replaceSpecialSymbols(const wstring& sentence) const
{
    wstring replaced = Tools::Replace(sentence, L"«", L"\"");
    replaced = Tools::Replace(replaced, L"”", L"\"");
    replaced = Tools::Replace(replaced, L"=", L" = ");
    replaced = Tools::Replace(replaced, L"»", L"\"");
    replaced = Tools::Replace(replaced, L"—", L"-");
    replaced = Tools::Replace(replaced, L"–", L"-");
    replaced = Tools::Replace(replaced, L"…", L".");
    replaced = Tools::Replace(replaced, L"№", L"\u043d\u043e\u043c\u0435\u0440 ");//here was cyrrilic symbols: номер
    return replaced;
}

vector<Token> TokenizerRus::Tokenize(
		const wstring& sentence) const
{
    wstring replaced = replaceSpecialSymbols(sentence);
    wstring alignedSentence = alignSentence(replaced);
    wstring correctedSentece = applyMultipleTokensRule(alignedSentence);
    vector<wstring> splitted = Tools::Split(correctedSentece, L" ", L"\u00A0", L"\t");
    vector<Token> tokens;
    for (size_t splittedIndex = 0; splittedIndex < splitted.size();
        ++splittedIndex)
    {
        wstring current = splitted[splittedIndex];
        vector<wstring> punctuation;
        removePunctuation(&current, &punctuation);
        Token token(current, punctuation);
        tokens.push_back(token);
    }
    applyFirstSymbolPunctuationRule(&tokens);
    return tokens;
}

Tools::Language TokenizerRus::GetLanguage() const
{
    return Tools::Language::RU;
}

void TokenizerRus::applyFirstSymbolPunctuationRule(
    vector<Token>* tokens) const
{
    if (tokens->size() <= 1 
        || tokens->at(0).content.size() != 0)
    {
        return;
    }
    const vector<wstring>& firstPunctuation 
        = tokens->at(0).punctuation;
    for (size_t punctuationIndex = 0
        ; punctuationIndex < firstPunctuation.size()
        ; ++punctuationIndex)
    {
        (*tokens)[1].punctuation.push_back(
            L"F" + firstPunctuation[punctuationIndex]);
    }
    tokens->erase(tokens->begin());
}

wstring TokenizerRus::alignSentence(const wstring& sentence) const
{
    wstring alginedSentence = L"";
	bool shouldMakeSpace = false;
	for (size_t letterIndex = 0; letterIndex < sentence.size(); ++letterIndex)
	{
		if (sentence[letterIndex] == L' ') {
			shouldMakeSpace = true;
        } else if (Tools::IsPunctuation(sentence[letterIndex])) {
            if ((sentence[letterIndex] != L',' &&
                sentence[letterIndex] != L'.' &&
                sentence[letterIndex] != L'-' &&
                sentence[letterIndex] != L':') ||
                letterIndex == 0 || letterIndex + 1 >= sentence.size() || // Working with numbers: 12,6
                !std::iswdigit(sentence[letterIndex - 1]) ||
                !std::iswdigit(sentence[letterIndex + 1]))
            {
                shouldMakeSpace = shouldMakeSpace || (sentence[letterIndex] != L'-');
            }
			alginedSentence += sentence[letterIndex];
        } else if (std::iswdigit(sentence[letterIndex]) && // Deal with 2%
            letterIndex + 1 < sentence.size() &&
            sentence[letterIndex + 1] == L'%') {
               shouldMakeSpace = true;
               alginedSentence += sentence[letterIndex];
        } else {
			if (shouldMakeSpace) {
				alginedSentence += L" ";
            }
			alginedSentence += sentence[letterIndex];
			shouldMakeSpace = false;
		}
	}
    return alginedSentence;
}

wstring TokenizerRus::inverseTrim(const wstring& sentence) const
{
    wstring untrimmed(sentence.size() + 2, L' ');
    untrimmed.replace(1, sentence.size(), sentence);
    return untrimmed;
}

wstring TokenizerRus::applyMultipleTokensRule(const wstring& sentence) const
{
    wstring changedSentence = Tools::Replace(sentence, L" ", L"_");
    wstring correctedSentence = inverseTrim(sentence);
    wstring punctuation = L",;.?!";
	for (auto exceptionIterator = muliplePartTokens.begin(); 
        exceptionIterator != muliplePartTokens.end(); ++exceptionIterator)
	{
        if (!Tools::ContainsSubstring(changedSentence, *exceptionIterator))
        {
            continue;
        }
        wstring complexTokenWithoutSpaces = inverseTrim(*exceptionIterator);
        wstring complexTokenWithSpaces = complexTokenWithoutSpaces;
        std::replace(complexTokenWithSpaces.begin(), complexTokenWithSpaces.end(), L'_', L' ');
        if (Tools::ContainsSubstring(correctedSentence, complexTokenWithSpaces))
        {
            correctedSentence = Tools::Replace(correctedSentence,
                complexTokenWithSpaces, complexTokenWithoutSpaces);
        }
        for (size_t punctuationIndex = 0; punctuationIndex < punctuation.size();
            ++punctuationIndex)
        {
            complexTokenWithoutSpaces[complexTokenWithoutSpaces.size() - 1] 
                = punctuation[punctuationIndex];
            complexTokenWithSpaces[complexTokenWithSpaces.size() - 1]
                = punctuation[punctuationIndex];
            if (Tools::ContainsSubstring(correctedSentence, complexTokenWithSpaces))
            {
                correctedSentence = Tools::Replace(correctedSentence,
                    complexTokenWithSpaces, complexTokenWithoutSpaces);
            }
        }
	}
    return correctedSentence.substr(1, correctedSentence.size() - 2);
}

void TokenizerRus::removePunctuation(
    wstring* token, vector<wstring>* punctuation) const
{
    std::stack<wstring> buffer;
    int numDots = 0;
    int enumerator = 0;
    for (auto pointer = token->rbegin(); pointer != token->rend(); ++pointer)
    {
        const wchar_t currentLetter = *pointer;
        if (Tools::IsPunctuation(currentLetter)) {
            ++enumerator;
            if (currentLetter == L'.') {
                ++numDots;
                if (numDots == 3) {
                    buffer.push(L"...");
                    numDots = 0;
                }
            } else {
                for (int dotEnumerator = 0; dotEnumerator < numDots; ++dotEnumerator)
                {
                    buffer.push(L".");
                }
                numDots = 0;
                wstring toPush;
                toPush += currentLetter;
                buffer.push(toPush);
            }
        } else {
            break;
        }
    }
    for (int dotEnumerator = 0; dotEnumerator < numDots; ++dotEnumerator)
    {
        buffer.push(L".");
    }
    *token = token->substr(0, token->size() - enumerator);
    while(!buffer.empty())
    {
        punctuation->push_back(buffer.top());
        buffer.pop();
    }
}

}

