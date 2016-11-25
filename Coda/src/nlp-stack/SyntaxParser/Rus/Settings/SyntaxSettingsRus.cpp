#include "SyntaxSettingsRus.h"
#include "Disambiguator/Rus/Settings/DisambiguatorSettingsRus.h"

namespace SyntaxParser
{

const Tools::Language SyntaxSettingsRus::language = Tools::Language::RU;
const string SyntaxSettingsRus::SYNTAX_MODEL_OUTPUT_PATH = string(SYNTAX_CONFIG_FOLDER) + "RU/Model_Output.txt";
const string SyntaxSettingsRus::SYNTAX_TEST_SET_PATH = string(SYNTAX_CONFIG_FOLDER) + "RU/TestSet.txt";
const string SyntaxSettingsRus::SYNTAX_TRAINING_SET_PATH = string(SYNTAX_CONFIG_FOLDER) + "RU/TrainingSet.txt";

const shared_ptr<SyntaxDataPreparer> SyntaxSettingsRus::dataPreparer
    = std::make_shared<SyntaxDataPreparerRus>(
      string(SYNTAX_CONFIG_FOLDER) + "RU/SyntaxTreesWithPEREH.txt"
      , SyntaxSettingsRus::SYNTAX_TRAINING_SET_PATH
      , SyntaxSettingsRus::SYNTAX_TEST_SET_PATH
      , std::make_shared<Disambiguation::DisambiguatorSettingsRus::TrainingFunctor>()
      , std::make_shared<Disambiguation::DisambiguatorSettingsRus::TestFunctor>());


}
