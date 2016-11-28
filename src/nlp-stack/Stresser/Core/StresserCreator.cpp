#include "StresserCreator.h"
#include "StresserRus.h"

namespace Stresser
{

shared_ptr<IStresser>
IStresser::GetStresser(Language language)
{
    return StresserCreator::GetStresserCreator().GetStresser(language);
}

StresserCreator& StresserCreator::GetStresserCreator()
{
    static StresserCreator creator;
    return creator;
}

shared_ptr<IStresser>
StresserCreator::ReloadStresser(Language language)
{
    switch (language)
    {
        case Tools::Language::RU: {
            stressers[language] = std::make_shared<StresserRus>(
                string(STRESSER_CONFIG_FOLDER) + "RU/");
            return stressers[language];
        }
        case Tools::Language::EN: {
            return 0;
        }
    }
    return 0;
}

shared_ptr<IStresser>
StresserCreator::GetStresser(Language language)
{
    auto found = stressers.find(language);
    if (found == stressers.end()) {
        return ReloadStresser(language);
    } else {
        return found->second;
    }
}


void StresserCreator::RemoveStresser(Language language)
{
    stressers.erase(language);
}

}


