#include "DisambiguatorCreator.h"
#include "LanguageNotRegisteredException.h"
#include "DisambiguatorSettingsEn.h"
#include "DisambiguatorSettingsRus.h"
#include "DisambiguatorSettingsEnFast.h"

namespace Disambiguation
{

DisambiguatorCreator::DisambiguatorCreator()
{
    this->Register(
        std::make_shared<DisambiguatorSettingsRus>());
    this->Register(
        std::make_shared<DisambiguatorSettingsEn>());
    this->Register(
        std::make_shared<DisambiguatorSettingsEnFast>());
}

shared_ptr<IDisambiguator>
IDisambiguator::GetDisambiguator(Language language)
{
    return DisambiguatorCreator::GetDisambiguatorCreator().GetDisambiguator(language);
}

DisambiguatorCreator& DisambiguatorCreator::GetDisambiguatorCreator()
{
    static DisambiguatorCreator creator;
    return creator;
}

void DisambiguatorCreator::Register(std::shared_ptr<IDisambiguatorSettings> toRegister)
{
    settings[toRegister->GetLanguage()]
        = std::make_shared<DisambiguatorSettingsHolder>(toRegister);
}

shared_ptr<IDisambiguator>
DisambiguatorCreator::GetDisambiguator(Language language)
{
    auto found = settings.find(language);
    if (found == settings.end()) {
        throw LanguageNotRegisteredException(language);
    } else {
        return found->second->GetDisambiguator();
    }
}

void DisambiguatorCreator::RemoveDisambiguator(Language language)
{
    settings.erase(language);
}

}
