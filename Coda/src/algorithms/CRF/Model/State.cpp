/** 
 * @file	States.cpp
 * @brief	source file of the class containing state of the CRF-model
 */

#include "State.h"

using namespace LinearCRF;

State::State(CRF* _crf)
    : crf(_crf)
{
}

State::State(const wstring& _name, int _index, 
		  shared_ptr<vector<int>> _destinations, 
		  shared_ptr<vector<vector<int>>> _weightsIndexes,
		  shared_ptr<vector<wstring>> _destinationNames, CRF* _crf)
{
	name = _name;
	index = _index;
	destinations = _destinations;
	weightsIndexes = _weightsIndexes;
	crf = _crf;
	destinationNames = _destinationNames;
}

int State::GetNumDestinations() const
{
	return destinationNames->size();
}

const vector<int>& State::GetDestinations() const
{
	return *destinations;
}

const vector<vector<int>>& State::GetWeightsIndexes() const
{
	return *weightsIndexes;
}

State::~State(void)
{
}

wstring State::GetName() const
{
	return name;
}

void State::SetWeightIndexes(shared_ptr<vector<vector<int>>> data)
{
	weightsIndexes = data;
}

int State::GetIndex() const
{
	return index;
}
	
