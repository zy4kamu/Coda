/** 
 * @file	CRFTextReader.cpp
 * @brief	source file of the CRF-reader in text format
 */

#include "CRFTextReader.h"

#include <locale>
#include <iostream>

#include "Tools.h"

using namespace LinearCRF;

CRFTextReader::CRFTextReader(const string& file)
	: file(file), inputAlphabet(new Alphabet), 
	  outputAlphabet(new Alphabet), weightAlphabet(new Alphabet), 
	  initialWeights(new vector<double>), finalWeights(new vector<double>),
	  defaultWeights(new vector<double>), states(new vector<State>),
      weights(new vector<unordered_map<int, double> >)
{
}

CRFTextReader::~CRFTextReader(void)
{
}

void CRFTextReader::Read(CRF& crf) const
{
	wifstream in(file);
	Tools::SetLocale(in);
	wstring str; 
	getline(in, str);
	inputAlphabet->Read(in);
	getline(in, str);
	outputAlphabet->Read(in);
	getline(in, str);
	weightAlphabet->Read(in);
	getline(in, str);
	readStates(in, crf);
	readWeights(in);
	in.close();
	fillContent(crf);
}

void CRFTextReader::readStates(wifstream& in, CRF& crf) const
{
	wstring s;
	vector<wstring> splitted;
	while (true)
    {
		// Get name of state and number of transitions;
		getline(in, s);
        s = Tools::Trim(s);
		if (s.size() == 0) break;
        splitted = Tools::Split(s, L"\"");
		wstring stateName = splitted[1];
        splitted = Tools::Split(s, L"(");
        splitted = Tools::Split(splitted[1], L" ");
		int numTransitions = stoi(splitted[0]);
		// Read initialWeight;
		getline(in, s);
        splitted = Tools::Split(s, L"=");
        s = Tools::Trim(splitted[1]);
		double initialWeight = (s == L"-Infinity") ? -1e+10 : stod(s);
		initialWeights->push_back(initialWeight);
		//Read finalWeight;
		getline(in, s);
        splitted = Tools::Split(s, L"=");
        s = Tools::Trim(splitted[1]);
		double finalWeight = (s == L"-Infinity") ? -1e+10 : stod(s);
		finalWeights->push_back(finalWeight);
		// Read transitions;
		shared_ptr<vector<vector<int>>> curWeightIndices(new vector<vector<int>>);
		shared_ptr<vector<int>> curDestinations(new vector<int>);
		shared_ptr<vector<wstring>> curDestinationNames(new vector<wstring>);
		curDestinations->resize(numTransitions);
		curWeightIndices->resize(numTransitions);
		curDestinationNames->resize(numTransitions);
		getline(in, s);
		getline(in, s);
        s = Tools::Trim(s);
	    for (int i = 0; i < numTransitions; ++i)
		{
			// Read destination name;
			wstring transition = s;
            s = Tools::Trim(s);
            splitted = Tools::Split(s, L"->");
            splitted[1] = Tools::Trim(splitted[1]);
			curDestinationNames->at(i) = splitted[1];
			curDestinations->at(i) = outputAlphabet->LookUpIndex_NoAdding(splitted[1]);
			// Read destination cliques;
		    while (true)
		    {
				getline(in, s);
                s = Tools::Trim(s);
                if (s.size() == 0 || s.find(L"\"") == std::wstring::npos)
					break;
                splitted = Tools::Split(s, L"\"");
				wstring clique = splitted[1];
				int indexClique = weightAlphabet->LookUpIndex_Adding(clique);
				curWeightIndices->at(i).push_back(indexClique);
		    }
		}
		// Emplace new state;
		states->emplace_back(State(
			stateName,
			outputAlphabet->LookUpIndex_NoAdding(stateName), 
			curDestinations, curWeightIndices, curDestinationNames,
			&crf));
	}
}

void CRFTextReader::readWeights(wifstream& in) const
{
	wstring s;
	vector<wstring> splitted;
	set<wchar_t> specialSet;
	specialSet.insert('['); specialSet.insert(':');
	specialSet.insert('='); specialSet.insert(']');
	defaultWeights->resize(weightAlphabet->Size());

	getline(in, s);
	getline(in, s);
	getline(in, s);
	int numCliques = weightAlphabet->Size();
	for (int cliqueIndex = 0; cliqueIndex < numCliques; ++cliqueIndex)
	{
		// Get clique name;
        s = Tools::Trim(s);
        splitted = Tools::Split(s, L"=");
        wstring clique = Tools::Trim(splitted[1]);
		// Get default value;
		getline(in, s);
        splitted = Tools::Split(s, L"=");
		defaultWeights->at(cliqueIndex) = (s == L"-Infinity") ? -1e+10 : stod(splitted[1]);
		// Get clique data;
       weights->emplace_back();
       unordered_map<int, double>& cliqueData = weights->back();
       while (true)
       {
           if(!getline(in, s)) break;
           s = Tools::Trim(s);
           if (s.size() == 0) break;
           if (!(s[0] == ':')) break;
           splitted = Tools::Split(s, specialSet);
           wstring feature = Tools::Trim(splitted[1]);
           for (size_t i = 2; i < splitted.size() - 1; ++i)
           {
               feature += L"=" + splitted[i];
           }
           feature = Tools::Trim(feature);
           int index = inputAlphabet->LookUpIndex_NoAdding(feature);
           wstring stringVal = Tools::Trim(splitted[splitted.size() - 1]);
           double value = (s == L"-Infinity") ? -1e+10 : stod(stringVal);
           cliqueData[index] = value;
       }
	}
}

void CRFTextReader::fillContent(CRF& crf) const
{
	crf.SetInputAlphabet(inputAlphabet);
	crf.SetOutputAlphabet(outputAlphabet);
	crf.SetWeightAlphabet(weightAlphabet);
    crf.SetStates(states);
	crf.SetInitialWeights(initialWeights);
	crf.SetDefaultWeights(defaultWeights);
	crf.SetFinalWeights(finalWeights);
	crf.SetWeights(weights);
}
