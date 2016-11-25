#include "Core/Controller/Controller.h"
#include "DisambiguatorSettingsEn.h"
#include "DisambiguatorSettingsEnFast.h"
#include "DisambiguatorSettingsRus.h"

using Disambiguation::Controller;
using Disambiguation::DisambiguatorSettingsRus;
using Disambiguation::DisambiguatorSettingsEn;
using Disambiguation::DisambiguatorSettingsEnFast;
using Disambiguation::IDisambiguatorSettings;
using Tokenization::ITokenizer;

std::shared_ptr<IDisambiguatorSettings> GetSettings()
{
    std::wcout << L"Input language: ru, en, en_fast: ";
    wstring lang;
    std::wcin >> lang;
    if (lang == L"ru") {
        return std::make_shared<DisambiguatorSettingsRus>();
    } else if (lang == L"en") {
        return std::make_shared<DisambiguatorSettingsEn>();
    } else if (lang == L"en_fast") {
        return std::make_shared<DisambiguatorSettingsEnFast>();
    } else {
        std::wcout << L"Wrong language input..." << std::endl;
        exit(0);
    }
}

int main()
{ 
    Tools::PrepareConsole();
    std::shared_ptr<IDisambiguatorSettings> settings
        = GetSettings();
    shared_ptr<ITokenizer> tokenizer
        = ITokenizer::GetTokenizer(settings->GetLanguage());

    Controller controller(settings, tokenizer);
    wcout << L"Input learn, relearn, prepare, test_from_file, test, lemmatize: ";
    wstring input;
    wcin >> input;
    if (input == L"learn") {
        controller.Learn();
    } else if (input == L"relearn") {
        controller.Relearn();
    } else if (input == L"prepare") {
        controller.Prepare();
    } else if (input == L"test_from_file") {
        controller.TestFromFile();
    } else if (input == L"test") {
        controller.Test();
    } else if (input == L"lemmatize") {
        controller.Lemmatize();
    } else {
        wcout << L"Wrong input..." << std::endl;
    }
}
