#include "SyntaxCreator.h"
#include "SyntaxParser/Core/Exceptions/LanguageNotRegisteredException.h"
#include "SyntaxSettingsEn.h"
#include "SyntaxSettingsRus.h"

namespace SyntaxParser
{

SyntaxCreator::SyntaxCreator()
{
}

SyntaxCreator& SyntaxCreator::GetSyntaxCreator()
{
    static SyntaxCreator creator;
    return creator;
}

bool SyntaxCreator::ContainsLanguage(Language language)
{
    return settings.find(language) != settings.end();
}

shared_ptr<ISyntaxParser>
ISyntaxParser::GetSyntaxParser(Language language)
{
    SyntaxCreator& creator = SyntaxCreator::GetSyntaxCreator();
    if (!creator.ContainsLanguage(language))
    {
        switch (language)
        {
            case Tools::RU:
                creator.Register<SyntaxSettingsRus>();
                break;
            case Tools::EN:
                creator.Register<SyntaxSettingsEn>();
                break;
            default:
                throw LanguageNotRegisteredException(language);
        }
    }
    return SyntaxCreator::GetSyntaxCreator().GetSyntaxParser(language);
}

shared_ptr<ISyntaxParser>
SyntaxCreator::GetSyntaxParser(Language language)
{
    auto found = settings.find(language);
    if (found == settings.end()) {
        throw LanguageNotRegisteredException(language);
    } else {
        return found->second;
    }
}


void SyntaxCreator::RemoveSyntaxParser(Language language)
{
    settings.erase(language);
}

}



//#include "SyntaxCreator.h"
//#include "SettingsRus.h"

//namespace SyntaxParser
//{

//shared_ptr<ISyntaxParser>
//ISyntaxParser::GetParser(Language language)
//{
//    return SyntaxCreator::GetSyntaxCreator().GetSyntaxParser(language);
//}

//SyntaxCreator& SyntaxCreator::GetSyntaxCreator()
//{
//    static SyntaxCreator creator;
//    return creator;
//}

//shared_ptr<ISyntaxParser>
//SyntaxCreator::ReloadSyntaxParser(Language language)
//{
//    switch (language) {
//    case Language::RU:
//        parsers[language] = std::make_shared<SettingsRus::Parser>(
//            string(SYNTAX_CONFIG_FOLDER) + "RU/Model_Output.txt"
//            , Language::RU);
//        return parsers[language];
//        break;
//    default:
//        return 0;
//        break;
//    }
//}

//shared_ptr<ISyntaxParser>
//SyntaxCreator::GetSyntaxParser(Language language)
//{
//    auto found = parsers.find(language);
//    if (found == parsers.end()) {
//        return ReloadSyntaxParser(language);
//    } else {
//        return found->second;
//    }
//}


//void SyntaxCreator::RemoveSyntaxParser(Language language)
//{
//    parsers.erase(language);
//}

//}
