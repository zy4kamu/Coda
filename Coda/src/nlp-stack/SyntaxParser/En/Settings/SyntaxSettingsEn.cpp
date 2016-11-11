#include "SyntaxSettingsEn.h"
#include "En/Settings/DisambiguatorSettingsEn.h"

namespace SyntaxParser
{

const Tools::Language SyntaxSettingsEn::language = Tools::Language::EN;
const string SyntaxSettingsEn::SYNTAX_MODEL_OUTPUT_PATH = string(SYNTAX_CONFIG_FOLDER) + "EN/Model_Output.txt";
const string SyntaxSettingsEn::SYNTAX_TEST_SET_PATH = string(SYNTAX_CONFIG_FOLDER) + "EN/TestSet.txt";
const string SyntaxSettingsEn::SYNTAX_TRAINING_SET_PATH = string(SYNTAX_CONFIG_FOLDER) + "EN/TrainingSet.txt";

const shared_ptr<SyntaxDataPreparer> SyntaxSettingsEn::dataPreparer
    = std::make_shared<SyntaxDataPreparer>(
      string(SYNTAX_CONFIG_FOLDER) + "EN/trees"
      , SyntaxSettingsEn::SYNTAX_TRAINING_SET_PATH
      , SyntaxSettingsEn::SYNTAX_TEST_SET_PATH
      , std::make_shared<Tools::SegmentFunctor<3914, 43745> >()
      , std::make_shared<Tools::SegmentFunctor<45446, 47861> >());

}
