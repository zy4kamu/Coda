/** 
 * @file	CRFBinaryWriter.cpp
 * @brief	source file of the CRF-binary writer
 */

#include "CRFBinaryWriter.h"
#include "Factors.h"

using namespace LinearCRF;

const double EPS = 1e-10;

CRFBinaryWriter::CRFBinaryWriter(const string& _file)
{
	file = _file;
}


CRFBinaryWriter::~CRFBinaryWriter(void)
{
}

void CRFBinaryWriter::Write(const CRF& crf) const
{
	std::ofstream out(file, std::ios::binary);
	Tools::SetLocale(out);
    if (!out)
    {
        std::wcout << L"Can't write to "
                   << Tools::ConvertStringToWstring(file)
                   << L"    " << file.size()
                   << std::endl;
        exit(1);
    }
	// Printing alphabets;
	crf.GetInputAlphabet().Print(out);
	crf.GetOutputAlphabet().Print(out);
	crf.GetWeightsAlphabet().Print(out);
	// Printing States;
	int stateSize = crf.GetNumStates();
	out.write((char*)&stateSize, sizeof(int));
	for (int stateIndex = 0; stateIndex < stateSize; stateIndex++)
	{
		const State& state = crf.GetStates()[stateIndex];
		double initialWeight = crf.GetInitialWeights()[stateIndex];
		out.write((char*)&initialWeight, sizeof(double));
		double finalWeight = crf.GetFinalWeights()[stateIndex];
		out.write((char*)&finalWeight, sizeof(double));
		int numDestinations = state.GetNumDestinations();
		out.write((char*)&numDestinations, sizeof(int));
		for (int destIndex = 0; destIndex < numDestinations; ++destIndex)
		{
			out.write((char*)&state.GetDestinations()[destIndex], sizeof(int));
			int weightIndexesLength = state.GetWeightsIndexes()[destIndex].size();
			out.write((char*)&weightIndexesLength, sizeof(int));
			for (int weightIndex = 0; weightIndex < weightIndexesLength; ++weightIndex)
			{
				out.write((char*)&state.GetWeightsIndexes()[destIndex][weightIndex], 
					sizeof(int));
			}
		}
	}
	// Printing weights;
	const Factors& parameters = crf.GetParameters();
	for (int widx = 0; widx < parameters.GetWeightAlphabet().Size(); ++widx)
	{
		double defaultWeight = parameters.GetDefaultWeights()[widx];
		out.write((char*)&defaultWeight, sizeof(double));
        const unordered_map<int, double>& transitionWeights = parameters.GetWeights()[widx];
        int size = transitionWeights.size();
        out.write((char*)&size, sizeof(int));
        for (auto item : transitionWeights)
        {
            int index = item.first;
            double value = item.second;
            out.write((char*)&index, sizeof(int));
            out.write((char*)&value, sizeof(double));
        }
	}
	out.close();
}
