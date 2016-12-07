/** 
 * @file	CRFBinaryReader.cpp
 * @brief	source file for CRF-reader from binary file
 */

#include "CRFBinaryReader.h"

using namespace LinearCRF;

CRFBinaryReader::CRFBinaryReader(const string& file)
	: file(file), inputAlphabet(new Alphabet), 
	  outputAlphabet(new Alphabet), weightAlphabet(new Alphabet), 
	  initialWeights(new vector<double>), finalWeights(new vector<double>),
	  defaultWeights(new vector<double>), states(new vector<State>),
      weights(new vector<unordered_map<int, double> >)
{
}


CRFBinaryReader::~CRFBinaryReader(void)
{
}

void CRFBinaryReader::Read(CRF& crf)
{
	try
	{
		ifstream in(file, std::ios::in | std::ios::binary);
        if (!in)
        {
            std::wcout << L"Can't read from "
                       << Tools::ConvertStringToWstring(file)
                       << std::endl;
            exit(1);
        }
		inputAlphabet->Read(in);
		outputAlphabet->Read(in);
		weightAlphabet->Read(in);
		readStates(in, crf);
		readWeights(in);
		in.close();
		fillContent(crf);
	}
	catch (...)
	{
		std::cout << "CRFBinaryReader: file with CRF-model is corrupted";
	}
}

void CRFBinaryReader::readStates(ifstream& in, CRF& crf)
{
	int numStates;
	in.read((char*)&numStates, sizeof(int));
	initialWeights->resize(numStates);
	finalWeights->resize(numStates);
	states->reserve(numStates);
	for (int state = 0; state < numStates; ++state)
	{
		shared_ptr<vector<vector<int>>> curWeightIndexes(new vector<vector<int>>);
		shared_ptr<vector<int>> curDestinations(new vector<int>);
		shared_ptr<vector<wstring>> curDestinationNames(new vector<wstring>);
		in.read((char*)&initialWeights->at(state), sizeof(double));
		in.read((char*)&finalWeights->at(state), sizeof(double));
		int numDestinations;
		in.read((char*)&numDestinations, sizeof(int));
		curDestinations->resize(numDestinations);
		curWeightIndexes->resize(numDestinations);
		curDestinationNames->resize(numDestinations);
		for (int dest = 0; dest < numDestinations; ++dest)
		{
			in.read((char*)&curDestinations->at(dest), sizeof(int));
			curDestinationNames->at(dest) = outputAlphabet->LookUpObject(curDestinations->at(dest));
			int nWeights;
			in.read((char*)&nWeights, sizeof(int));
			curWeightIndexes->at(dest).resize(nWeights);
			for (int weightIndex = 0; weightIndex < nWeights; ++weightIndex)
				in.read((char*)&curWeightIndexes->at(dest)[weightIndex], sizeof(int));
		}
		states->emplace_back(State(
			outputAlphabet->LookUpObject(state),
			state, 
			curDestinations, curWeightIndexes, curDestinationNames,
			&crf));
	}
}

void CRFBinaryReader::readWeights(ifstream& in)
{
	int numCliques = weightAlphabet->Size();
    weights->resize(numCliques);
	defaultWeights->resize(numCliques);
	for (int cliqueIndex = 0; cliqueIndex < numCliques; ++cliqueIndex)
    {
        in.read((char*)&defaultWeights->at(cliqueIndex), sizeof(double));
        int cliqueSize;
        in.read((char*)&cliqueSize, sizeof(int));
        int index;
        double value;
        for (int i = 0; i < cliqueSize; ++i)
        {
            in.read((char*)&index, sizeof(int));
            in.read((char*)&value, sizeof(double));
            (*weights)[cliqueIndex][index] = value;
        }
	}
	in.close();
}

void CRFBinaryReader::fillContent(CRF& crf) const
{
	crf.SetInputAlphabet(inputAlphabet);
	crf.SetOutputAlphabet(outputAlphabet);
    crf.SetStates(states);
	crf.SetWeightAlphabet(weightAlphabet);
	crf.SetInitialWeights(initialWeights);
	crf.SetDefaultWeights(defaultWeights);
	crf.SetFinalWeights(finalWeights);
	crf.SetWeights(weights);
}

