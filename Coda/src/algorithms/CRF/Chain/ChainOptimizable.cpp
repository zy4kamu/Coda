#include "ChainOptimizable.h"

namespace LinearCRF
{

template <>
void ChainOptimizable<USE_UNPRESENTED_IN_CLIQUES_FEATURES>::UpdateCliquePossibleFeatures(
    unordered_map<int, unordered_set<int> >* cliquePossibleFeatures)
{
    size_t length = labelIndexes.size();
    // Iterate over cliques in chain
    for (int cliqueIndex = 0; cliqueIndex < static_cast<int>(length) - 1; ++cliqueIndex)
    {
        const vector<int>& leftStates = possibleStates[cliqueIndex];
        const vector<int>& rightStates = possibleStates[cliqueIndex + 1];
        const vector<int>& features = featureIndexes[cliqueIndex];
        // Iterate over left possible states
        for (size_t leftNodeIndex = 0; leftNodeIndex < leftStates.size(); ++leftNodeIndex)
        {
            int leftNode = leftStates[leftNodeIndex];
            if (transitionConverter->find(leftNode) == transitionConverter->end())
            {
                continue;
            }
            const unordered_map<int, int>& currentTransitions = transitionConverter->at(leftNode);
            // Iterate over right possible states
            for (size_t rightNodeIndex  = 0; rightNodeIndex < rightStates.size(); ++rightNodeIndex)
            {
                int rightNode = rightStates[rightNodeIndex];
                if (currentTransitions.find(rightNode) == currentTransitions.end())
                {
                    continue;
                }
                // Insert new feature to clique
                int currentClique = currentTransitions.at(rightNode);
                for (size_t featureIndex = 0; featureIndex < features.size(); ++featureIndex)
                {
                    (*cliquePossibleFeatures)[currentClique].insert(features[featureIndex]);
                }
            }
        }
    }
}

template <>
void ChainOptimizable<DONT_USE_UNPRESENTED_IN_CLIQUES_FEATURES>::UpdateCliquePossibleFeatures(
    unordered_map<int, unordered_set<int> >* cliquePossibleFeatures)
{
    size_t length = labelIndexes.size();
    // Iterate over cliques in chain
    for (int cliqueIndex = 0; cliqueIndex < static_cast<int>(length) - 1; ++cliqueIndex)
    {
        int currentClique = cliqueIndexes[cliqueIndex];
        const vector<int>& features = featureIndexes[cliqueIndex];
        for (size_t featureIndex = 0; featureIndex < features.size(); ++featureIndex)
        {
            (*cliquePossibleFeatures)[currentClique].insert(features[featureIndex]);
        }
    }
}

}
