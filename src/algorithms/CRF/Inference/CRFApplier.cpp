#include "CRFApplier.h"
#include "ChainCollectionTester.h"

namespace LinearCRF
{

CRFApplier::CRFApplier(
    const string& crfFile
    , shared_ptr<IChainTransformer> chainTransformer
    , shared_ptr<IPossibleStateFinder> possibleStateFinder
    , bool isCRFfileBinary)
    : chainTransformer(chainTransformer)
    , possibleStateFinder(possibleStateFinder)
    , crf(std::make_shared<CRF>(crfFile, isCRFfileBinary))
{
    inputAlphabet = shared_ptr<Alphabet>(
        new Alphabet(crf->GetInputAlphabet()));
    outputAlphabet = shared_ptr<Alphabet>(
        new Alphabet(crf->GetOutputAlphabet()));
    possibleStateFinder->ConnectWithModel(*crf);
}

CRFApplier::CRFApplier(
    shared_ptr<CRF> crf
   , shared_ptr<IChainTransformer> chainTransformer
   , shared_ptr<IPossibleStateFinder> possibleStateFinder)
    : chainTransformer(chainTransformer)
    , possibleStateFinder(possibleStateFinder)
    , crf(crf)
{
    if (crf != 0)
    {
        inputAlphabet = shared_ptr<Alphabet>(
            new Alphabet(crf->GetInputAlphabet()));
        outputAlphabet = shared_ptr<Alphabet>(
            new Alphabet(crf->GetOutputAlphabet()));
        possibleStateFinder->ConnectWithModel(*crf);
    }
}

void CRFApplier::Apply(
    Chain& chain
    , vector<wstring>* bestSequence
    , vector<double>* bestSequenceWeights)
{
    chain = chainTransformer->ForwardTransform(chain);
    possibleStateFinder->FindPossibleStates(&chain);
    // Find encoded sequence
    EncodedCRFData encodedData;
    encodedData.intLabels = getLabelIndexes(chain);
    encodedData.intFeatures = getFeatureIndexes(chain);
    encodedData.possibleStates = getPossibleStates(chain);
    // Find best sequence
    vector<int> bestDigitalSequence;
    crf->FindBestSequence_Digital(
        encodedData.intFeatures
        , encodedData.possibleStates
        , &bestDigitalSequence, bestSequenceWeights);
    bestDigitalSequence = chainTransformer->BackwardTransform(bestDigitalSequence);
    bestSequence->resize(bestDigitalSequence.size());
    for (size_t chainIndex = 0; chainIndex < bestSequence->size(); ++chainIndex)
    {
        (*bestSequence)[chainIndex] = outputAlphabet->LookUpObject(
            bestDigitalSequence[chainIndex]);
    }
    *bestSequenceWeights = chainTransformer->BackwardTransform(*bestSequenceWeights);
}

void CRFApplier::Apply(Chain& chain
    , size_t numberOfHypothesis
    , vector<vector<wstring> >* bestSequences
    , vector<vector<double> >* bestSequenceWeights
    , vector<double>* hypothesisDistribution)
{
    chain = chainTransformer->ForwardTransform(chain);
    possibleStateFinder->FindPossibleStates(&chain);
    // Find encoded sequence
    EncodedCRFData encodedData;
    encodedData.intLabels = getLabelIndexes(chain);
    encodedData.intFeatures = getFeatureIndexes(chain);
    encodedData.possibleStates = getPossibleStates(chain);
    // Find best sequence
    vector<vector<int> > bestDigitalSequences;
    double partitionFunction;
    crf->FindNBestSequences_Digital(
        encodedData.intFeatures
        , encodedData.possibleStates
        , &bestDigitalSequences
        , bestSequenceWeights
        , &partitionFunction
        , numberOfHypothesis);

    bestSequences->resize(bestDigitalSequences.size());
    hypothesisDistribution->resize(bestDigitalSequences.size());
    for (size_t chainIndex = 0; chainIndex < bestSequences->size()
         ; ++chainIndex)
    {
        (*hypothesisDistribution)[chainIndex]
            = std::exp((*bestSequenceWeights)[chainIndex].back()) / partitionFunction;
        (*bestSequenceWeights)[chainIndex]
            = chainTransformer->BackwardTransform((*bestSequenceWeights)[chainIndex]);
        vector<int> bestDigitalSequence = chainTransformer->BackwardTransform(
            bestDigitalSequences[chainIndex]);
        (*bestSequences)[chainIndex].resize(bestDigitalSequence.size());
        for (size_t tokenIndex = 0; tokenIndex < bestDigitalSequence.size(); ++tokenIndex)
        {
            (*bestSequences)[chainIndex][tokenIndex]
                = outputAlphabet->LookUpObject(bestDigitalSequence[tokenIndex]);
        }
    }
}

void CRFApplier::Apply(Chain& chain
    , double percentage
    , size_t maxNumberOfPaths
    , vector<vector<wstring> >* bestSequences
    , vector<vector<double> >* bestSequenceWeights
    , vector<double>* hypothesisDistribution)
{
    chain = chainTransformer->ForwardTransform(chain);
    possibleStateFinder->FindPossibleStates(&chain);
    // Find encoded sequence
    EncodedCRFData encodedData;
    encodedData.intLabels = getLabelIndexes(chain);
    encodedData.intFeatures = getFeatureIndexes(chain);
    encodedData.possibleStates = getPossibleStates(chain);
    // Find best sequence
    vector<vector<int> > bestDigitalSequences;
    double partitionFunction;
    crf->FindPercentageBestSequences_Digital(
        encodedData.intFeatures
        , encodedData.possibleStates
        , &bestDigitalSequences
        , bestSequenceWeights
        , &partitionFunction
        , percentage
        , maxNumberOfPaths);

    bestSequences->resize(bestDigitalSequences.size());
    hypothesisDistribution->resize(bestDigitalSequences.size());
    for (size_t chainIndex = 0; chainIndex < bestSequences->size()
         ; ++chainIndex)
    {
        (*hypothesisDistribution)[chainIndex]
            = std::exp((*bestSequenceWeights)[chainIndex].back()) / partitionFunction;
        (*bestSequenceWeights)[chainIndex]
            = chainTransformer->BackwardTransform((*bestSequenceWeights)[chainIndex]);
        vector<int> bestDigitalSequence = chainTransformer->BackwardTransform(
            bestDigitalSequences[chainIndex]);
        (*bestSequences)[chainIndex].resize(bestDigitalSequence.size());
        for (size_t tokenIndex = 0; tokenIndex < bestDigitalSequence.size(); ++tokenIndex)
        {
            (*bestSequences)[chainIndex][tokenIndex]
                = outputAlphabet->LookUpObject(bestDigitalSequence[tokenIndex]);
        }
    }
}

double CRFApplier::GetPartFunction(Chain& chain)
{
    chain = chainTransformer->ForwardTransform(chain);
    possibleStateFinder->FindPossibleStates(&chain);
    // Find encoded sequence
    EncodedCRFData encodedData;
    encodedData.intFeatures = getFeatureIndexes(chain);
    encodedData.possibleStates = getPossibleStates(chain);
    return crf->GetPartitionFunction_Digital(
        encodedData.intFeatures
        , encodedData.possibleStates);
}

vector<vector<int> > CRFApplier::getFeatureIndexes(
    const Chain& chain)
{
    size_t numberOfNodes = chain.GetSize();
    const vector<vector<wstring> >& features = chain.GetFeatures();
    vector<vector<int> > intFeatures(numberOfNodes);
    for (size_t nodeIndex = 0; nodeIndex < numberOfNodes; ++nodeIndex)
    {
        const vector<wstring>& currentFeatures = features[nodeIndex];
        for (size_t featureIndex = 0; featureIndex < currentFeatures.size()
            ; ++featureIndex)
        {
            int toAddFeature = inputAlphabet->LookUpIndex_NoAdding(
                currentFeatures[featureIndex]);
            if (toAddFeature >= 0)
            {
                intFeatures[nodeIndex].push_back(toAddFeature);
            }
        }
    }
    return intFeatures;
}

vector<int> CRFApplier::getLabelIndexes(
    const Chain& chain)
{
    const vector<wstring>& labels = chain.GetLabels();
    vector<int> labelIndexes(labels.size(), -1);
    for (size_t iter = 0; iter < labels.size(); ++iter)
    {
        labelIndexes[iter] = outputAlphabet->LookUpIndex_NoAdding(labels[iter]);
    }
    return labelIndexes;
}

vector<vector<int> > CRFApplier::getPossibleStates(
    const Chain& chain)
{
    const vector<vector<wstring> >& possibleStatesString
        = chain.GetPossibleStates();
    vector<vector<int> > possibleStates(possibleStatesString.size());
    for (size_t stateIndex = 0; stateIndex < possibleStates.size()
        ; ++stateIndex)
    {
        for (size_t featIndex = 0; featIndex < possibleStatesString[stateIndex].size()
            ; ++featIndex)
        {
            int featureIndex = outputAlphabet->LookUpIndex_NoAdding(
                possibleStatesString[stateIndex][featIndex]);
            if (featureIndex >= 0)
            {
                possibleStates[stateIndex].push_back(featureIndex);
            }
        }
    }
    return possibleStates;
}


}

