/** 
 * @file	Factors.cpp
 * @brief	source file of the class containing general settings of the CRF-model
 */

#include "Factors.h"

using namespace LinearCRF;


Factors::Factors(void)
    : weights(new vector<unordered_map<int, double> >)
{
}


Factors::~Factors(void)
{
}

const Alphabet& Factors::GetWeightAlphabet() const
{
	return *weightAlphabet;
}

shared_ptr<const Alphabet> Factors::GetWeightAlphabetPtr() const
{
    return weightAlphabet;
}

shared_ptr<Alphabet> Factors::GetWeightAlphabetPtr()
{
    return weightAlphabet;
}

const vector<unordered_map<int, double> >& Factors::GetWeights() const
{
	return *weights;
}

const vector<double>& Factors::GetDefaultWeights() const
{
	return *defaultWeights;
}

const vector<double>& Factors::GetInitialWeights() const
{
	return *initialWeights;
}
	
const vector<double>& Factors::GetFinalWeights() const
{
	return *finalWeights;
}


void Factors::SetWeightAlphabet(shared_ptr<Alphabet> _weightAlphabet)
{
	weightAlphabet = _weightAlphabet;
}

void Factors::SetDefaultWeights(shared_ptr<vector<double> > data)
{
	defaultWeights = data;
}

void Factors::SetInitialWeights(shared_ptr<vector<double> > data)
{
	initialWeights = data;
}

void Factors::SetFinalWeights(shared_ptr<vector<double> > data)
{
	finalWeights = data;
}

void Factors::SetWeights(shared_ptr<vector<unordered_map<int, double> > > data)
{
	weights = data;
}

shared_ptr<vector<double> > Factors::GetFinalWeightsPtr()
{
    return finalWeights;
}

shared_ptr<vector<unordered_map<int, double> > > Factors::GetWeightsPtr()
{
    return weights;
}

shared_ptr<vector<double> > Factors::GetDefaultWeightsPtr()
{
    return defaultWeights;
}

shared_ptr<vector<double> > Factors::GetInitialWeightsPtr()
{
    return initialWeights;
}


void Factors::Truncate(double truncationLevel)
{
    for (size_t cliqueIndex = 0; cliqueIndex < weights->size()
        ; ++cliqueIndex)
    {
        Tools::EraseMapValueLess(&(*weights)[cliqueIndex], truncationLevel);
    }
}
