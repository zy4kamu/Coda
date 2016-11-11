#include "LanguageNotRegisteredException.h"
#include "TokenizerCreator.h"
#include "TokenizerRus.h"
#include "TokenizerEn.h"
#include "TokenizerEnFast.h"

namespace Tokenization
{

TokenizerCreator::TokenizerCreator()
{
    this->Register(std::make_shared<TokenizerRus>());
    this->Register(std::make_shared<TokenizerEn>());
    this->Register(std::make_shared<TokenizerEnFast>());
}

shared_ptr<ITokenizer>
ITokenizer::GetTokenizer(Language language)
{
    return TokenizerCreator::GetTokenizerCreator().GetTokenizer(language);
}

TokenizerCreator& TokenizerCreator::GetTokenizerCreator()
{
    static TokenizerCreator creator;
    return creator;
}

void TokenizerCreator::Register(
    std::shared_ptr<ITokenizer> toRegister)
{
    tokenizers[toRegister->GetLanguage()] = toRegister;
}

shared_ptr<ITokenizer>
TokenizerCreator::GetTokenizer(Language language)
{
    auto found = tokenizers.find(language);
    if (found == tokenizers.end()) {
        throw LanguageNotRegisteredException(language);
    } else {
        return found->second;
    }
}


void TokenizerCreator::RemoveTokenizer(Language language)
{
    tokenizers.erase(language);
}

}

