#include "EmptyPossibleStateFinder.h"
#include "IPossibleStateFinder.h"
#include "FileReader.h"


namespace LinearCRF
{

EmptyPossibleStateFinder::EmptyPossibleStateFinder()
{
}

EmptyPossibleStateFinder::EmptyPossibleStateFinder(
    const string& tagsFile)
{
    FileManager::Read(tagsFile, &allTags);
}

void EmptyPossibleStateFinder::FindPossibleStates(
    Chain* chain)
{
    chain->SetPossibleStates(
        vector<vector<wstring> >(chain->GetSize(), allTags));
}

void EmptyPossibleStateFinder::ConnectWithModel(
    const CRF& crf)
{
    allTags = crf.GetOutputAlphabet().GetData();
}

}


