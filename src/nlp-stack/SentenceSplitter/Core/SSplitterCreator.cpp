#include "LanguageNotRegisteredException.h"
#include "SSplitterCreator.h"
#include "sentence-splitter.h"

namespace _sentence_splitter
{

SSplitterCreator::SSplitterCreator()
{
    this->Register(std::make_shared<SentenceSplitter>(Tools::Language::RU));
    this->Register(std::make_shared<SentenceSplitter>(Tools::Language::EN));
    this->Register(std::make_shared<SentenceSplitter>(Tools::Language::EN_FAST));
}

shared_ptr<ISentenceSplitter>
ISentenceSplitter::GetSentenceSplitter(Language language)
{
    return SSplitterCreator::GetSSplitterCreator().GetSentenceSplitter(language);
}

SSplitterCreator& SSplitterCreator::GetSSplitterCreator()
{
    static SSplitterCreator creator;
    return creator;
}

void SSplitterCreator::Register(
    std::shared_ptr<ISentenceSplitter> toRegister)
{
    ssplitters[toRegister->GetLanguage()] = toRegister;
}

shared_ptr<ISentenceSplitter>
SSplitterCreator::GetSentenceSplitter(Language language)
{
    auto found = ssplitters.find(language);
    if (found == ssplitters.end()) {
        throw LanguageNotRegisteredException(language);
    } else {
        return found->second;
    }
}

void SSplitterCreator::RemoveSentenceSplitter(Language language)
{
    ssplitters.erase(language);
}


}
