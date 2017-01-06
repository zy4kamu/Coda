#include "Exceptions.h"
#include "DictionaryCreator.h"

namespace DictionaryOps
{

DictionaryCreator& DictionaryCreator::getDictionaryCreator()
{
    static DictionaryCreator creator;
    return creator;
}

DictionaryCreator::DictionaryCreator() {}

void DictionaryCreator::Register(
    std::shared_ptr<DictionaryFacade> i_toRegister)
{
    m_dictionaries[i_toRegister->getLanguage()] = i_toRegister;
}

shared_ptr<DictionaryFacade>
DictionaryCreator::getDictionary(Tools::Language i_language)
{
    if(!isSupported_(i_language))
    {
        throw Tools::LanguageNotFoundException();
    }
    auto found = m_dictionaries.find(i_language);
    if (found == m_dictionaries.end()) {
        Register(std::make_shared<DictionaryFacade>(i_language));
    }
    return m_dictionaries[i_language];
}


void DictionaryCreator::removeDictionary(Tools::Language i_language)
{
    m_dictionaries.erase(i_language);
}

bool DictionaryCreator::isSupported_(Tools::Language i_language)
{
    if(i_language != Tools::RU && i_language != Tools::EN)
    {
        return false;
    }
    return true;
}

}

