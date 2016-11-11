#include "DisambiguatorRelearner.h"

#include "CRF.h"
#include "CRFBinaryWriter.h"
#include "DisambiguatorTester.h"

namespace Disambiguation
{

using LinearCRF::CRF;
using LinearCRF::CRFBinaryWriter;

static int NUMBER_OF_ITERATIONS_IN_ONE_RELEARNER_STEPS = 25;
static int TOTAL_NUMBER_OF_RELEARNER_ITERATIONS = 25;

/**** PUBLIC FUNCTIONS ****/

DisambiguatorRelearner::DisambiguatorRelearner(
    shared_ptr<DisambiguatorSettingsHolder> settings
    , shared_ptr<ITokenizer> tokenizer)
    : settings(settings)
    , language(settings->GetLanguage())
    ,  dictionary(settings->GetDictionary())
    , featureCalculator(settings->GetFeatureCalculator())
    , tokenizer(tokenizer)
    , disambiguator(settings->GetDisambiguator())
{
}

void DisambiguatorRelearner::RelearnDialog()
{
    wstring sentence;

    bool fillFileOrTrainOne = fillFileOrTrainOneDialog();
    bool useRelearnedModel = false;

    while (true)
    {
        if (useRelearnedModel)
        {
            disambiguator = settings->ReloadDisambiguator();
        }
        wcout << L"\nInput sentence that should be corrected..." << std::endl;
        wcin.clear();
        getline(wcin, sentence);
        if (sentence == L"exit") {
            return;
        }
        wcout << std::endl;
        vector<Token> tokens = tokenizer->Tokenize(sentence);
        vector<PredisambiguatedData> predisambiguated
            = featureCalculator->CalculateFeatures(tokens);
        vector<DisambiguatedData> disambiguated
            = disambiguator->Disambiguate(tokens);

        printDisambiguatedDataToConsole(disambiguated);
        if (fillFileOrTrainOne) {
            if (!printRelearnDataToFillFile(tokens, disambiguated)) {
                continue;
            }
            correctFillFile();
            if (!relearnDialogFromFile()) {
                continue;
            } else if (!moveRelearnedModel()) {
                continue;
            }
            testCorrectedModelDialog();
            useRelearnedModel = useRelearnedModelDialog();
        } else {
            useRelearnedModel = false;
            printRelearnDataToFileForCorrections(tokens
                , predisambiguated, disambiguated);
            if (!runCorrectionDialog()) {
                continue;
            } else if (!relearnDialog()) {
                continue;
            } else if (!moveRelearnedModel()) {
                continue;
            }
            testCorrectedModelDialog();
            useRelearnedModel = useRelearnedModelDialog();
        }
    }
}

void DisambiguatorRelearner::RelearnFromFile(const string& fileToFit)
{
    // Reading file and preparing data for relearning
    wifstream in(fileToFit);
	Tools::SetLocale(in);
    wofstream out(settings->GetRelearnSetFile());
	Tools::SetLocale(out);
    wstring sentence;
    vector<wstring> labels;
    bool fileNotEnded = true;
    while (fileNotEnded)
    {
        fileNotEnded = readRelearnSentence(in, &sentence, &labels);
        vector<Token> tokens = tokenizer->Tokenize(sentence);
        vector<PredisambiguatedData> predisambiguated
            = featureCalculator->CalculateFeatures(tokens);
        for (size_t tokenIndex = 0; tokenIndex < tokens.size(); ++tokenIndex)
        {
            out << tokens[tokenIndex].content << L" ";
            const vector<wstring>& features = predisambiguated[tokenIndex].features;
            if (features.size() > 0)
            {
                out << features[0];
                for (size_t featureIndex = 1; featureIndex < features.size(); ++featureIndex)
                {
                    out << L"," << features[featureIndex];
                }
            }
            out << L" " << labels[tokenIndex] << std::endl;
        }
        out << std::endl;
    }
    in.close();
    out.close();

    relearnFromFileWithFeatures(settings->GetRelearnSetFile());
}

/**** PRIVATE FUNCTIONS ****/

void DisambiguatorRelearner::relearnFromFileWithFeatures(
    const string& fileToFit)
{
    shared_ptr<IChainTransformer> chainTransformer
        = settings->GetChainTransformer();
    shared_ptr<IPossibleStateFinder> possibleStateFinder
        = settings->GetPossibleStateFinder();
    shared_ptr<RelearnRegularization> regularizator
        = settings->GetRelearnRegularizator();
    shared_ptr<CRF> crf = std::make_shared<CRF>(
        settings->GetModelFile(), true);
    ReLearner relearner(
        crf
        , fileToFit
        , fileToFit
        , settings->GetIntermidiateOutputFile()
        , chainTransformer,
        possibleStateFinder
        , regularizator
        , TOTAL_NUMBER_OF_RELEARNER_ITERATIONS);
    CRF relearnedCRF = relearner.Learn(NUMBER_OF_ITERATIONS_IN_ONE_RELEARNER_STEPS, 1, 1);
    CRFBinaryWriter writer(settings->GetIntermidiateOutputFile());
    writer.Write(relearnedCRF);
}

bool DisambiguatorRelearner::fillFileOrTrainOneDialog()
{
    wstring input;
    while (input != L"y" && input != L"n")
    {
        wcin.ignore();
        wcout << L"Fill file or relearn one sentence? (y/n)" << std::endl;
        getline(wcin, input);
    }
    return input == L"y";
}

void DisambiguatorRelearner::printDisambiguatedDataToConsole(
    const vector<DisambiguatedData>& disambiguated)
{
    for (size_t tokenIndex = 0; tokenIndex < disambiguated.size(); ++tokenIndex)
    {
        wcout << tokenIndex << L": "
            << disambiguated[tokenIndex].content << L" "
            << disambiguated[tokenIndex].lemma << L" "
            << disambiguated[tokenIndex].label << L" "
            << disambiguated[tokenIndex].weight << L" "
            << disambiguated[tokenIndex].lemmaId << L" "
            << endl;
    }
    wcout << std::endl;
}

void DisambiguatorRelearner::printRelearnDataToFileForCorrections(
    const vector<Token>& tokens
    , const vector<PredisambiguatedData>& predisambiguated
    , const vector<DisambiguatedData>& disambiguated)
{
    wofstream out(settings->GetRelearnSetFile());
	Tools::SetLocale(out);
    for (size_t tokenIndex = 0; tokenIndex < tokens.size(); ++tokenIndex)
    {
        out << tokens[tokenIndex].content << L" ";
        const vector<wstring>& features = predisambiguated[tokenIndex].features;
        if (features.size() > 0)
        {
            out << features[0];
            for (size_t featureIndex = 1; featureIndex < features.size(); ++featureIndex)
            {
                out << L"," << features[featureIndex];
            }
        }
        out << L" " << disambiguated[tokenIndex].label << std::endl;
    }
    out << std::endl;
    out.close();
}

bool DisambiguatorRelearner::printRelearnDataToFillFile(
        const vector<Token>& tokens
        , const vector<DisambiguatedData>& disambiguated)
{
    wstring input;
    while (input != L"y" && input != L"n")
    {
        wcout << L"\nPrint this sentence to file? (y/n)" << std::endl;
        wcin.clear();
        getline(wcin, input);
    }
    if (input == L"y") {
        wofstream out(settings->GetRelearnInputSetFile(), std::wofstream::app);
		Tools::SetLocale(out);
        for (size_t tokenIndex = 0; tokenIndex < tokens.size(); ++tokenIndex)
        {
            out << tokens[tokenIndex].content;
            for (size_t punctIndex = 0; punctIndex < tokens[tokenIndex].punctuation.size()
                 ; ++punctIndex)
            {
                out << tokens[tokenIndex].punctuation[punctIndex];
            }
            out << L"\t" << disambiguated[tokenIndex].label
                << std::endl;
        }
        out << std::endl;
        out.close();
        return true;
    } else {
        return false;
    }
}

bool DisambiguatorRelearner::runCorrectionDialog()
{
    wstring input;
    while (input != L"y" && input != L"n")
    {
        wcout << L"Should correct? (y/n)" << std::endl;
        wcin.clear();
        getline(wcin, input);
    }
    if (input == L"y") {
        string script = "gedit ";
        script += settings->GetRelearnSetFile();
        if (system(script.c_str())) {
            std::wcout << L"Script " << Tools::ConvertStringToWstring(script)
                       << L" has interrupted unexpectidly..." << std::endl;
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

void DisambiguatorRelearner::correctFillFile()
{
    string script = "gedit ";
    script += settings->GetRelearnInputSetFile();
    if (system(script.c_str())) {
        std::wcout << L"Script " << Tools::ConvertStringToWstring(script)
                   << L" has interrupted unexpectidly..." << std::endl;
    }
}

bool DisambiguatorRelearner::relearnDialog()
{
    wstring input;
    while (input != L"y" && input != L"n")
    {
        wcout << L"Start relearning? (y/n)" << std::endl;
        wcin.clear();
        getline(wcin, input);
    }
    if (input == L"y") {
        relearnFromFileWithFeatures(settings->GetRelearnSetFile());
        return true;
    } else {
        return false;
    }
}

bool DisambiguatorRelearner::relearnDialogFromFile()
{
    wstring input;
    while (input != L"y" && input != L"n")
    {
        wcout << L"Start relearning? (y/n)" << std::endl;
        wcin.clear();
        getline(wcin, input);
    }
    if (input == L"y") {
        RelearnFromFile(settings->GetRelearnInputSetFile());
        return true;
    } else {
        return false;
    }
}

bool DisambiguatorRelearner::moveRelearnedModel()
{
    string script = "cp ";
    script += settings->GetModelFile();
    script += " ";
    script += settings->GetModelFile();
    script += "_reserve";
    if (system(script.c_str())) {
        std::wcout << L"Script " << Tools::ConvertStringToWstring(script)
                   << L" has interrupted unexpectidly..." << std::endl;
        return false;
    }

    script = "cp ";
    script += settings->GetIntermidiateOutputFile();
    script += " ";
    script += settings->GetModelFile();
    if (system(script.c_str())) {
        std::wcout << L"Script " << Tools::ConvertStringToWstring(script)
                   << L" has interrupted unexpectidly..." << std::endl;
        return false;
    }

    return true;
}

void DisambiguatorRelearner::testCorrectedModelDialog()
{
    wstring input;
    while (input != L"y" && input != L"n")
    {
        wcin.ignore();
        wcout << L"Test corrected model? (y/n)" << std::endl;
        getline(wcin, input);
    }
    if (input == L"y")
    {
        DisambiguatorTester tester(settings, tokenizer);
        tester.TestFromFile("REAL_TEST_FILE.txt");
    }
}

bool DisambiguatorRelearner::useRelearnedModelDialog()
{
    wstring input;
    while (input != L"y" && input != L"n")
    {
        wcout << L"\nUse corrected model? (y/n)" << std::endl;
        wcin.clear();
        getline(wcin, input);
    }
    if (input == L"n") {
        string script = "cp ";
        script += settings->GetModelFile();
        script += "_reserve ";
        script += settings->GetModelFile();
        if (system(script.c_str())) {
            std::wcout << L"Script " << Tools::ConvertStringToWstring(script)
                       << L" has interrupted unexpectidly..." << std::endl;
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

bool DisambiguatorRelearner::readRelearnSentence(
    wifstream& in
    , wstring* sentence
    , vector<wstring>* labels)
{
    *sentence = L"";
    *labels = vector<wstring>();
    wstring line;
    while (getline(in, line))
    {
        line = Tools::Trim(line);
        if (line.size() == 0) {
            return true;
        } else {
            vector<wstring> splitted = Tools::Split(line, L"\t");
            *sentence += splitted[0] + L" ";
            labels->emplace_back(std::move(splitted[1]));
        }
    }
    return false;
}


}
