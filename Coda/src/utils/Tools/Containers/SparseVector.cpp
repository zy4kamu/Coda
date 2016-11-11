/** 
 * @file	SparseVector.cpp
 * @brief	source file of the class containing sequence of labeled doubles 
 */

#include "SparseVector.h"
#include <cmath>

namespace Tools
{

SparseVector::SparseVector()
    : indexes(new vector<int>())
    , values(new vector<double>())
{
}

SparseVector::SparseVector(const map<int, double>& data)
    : indexes(new vector<int>())
    , values(new vector<double>())
{
    indexes->resize(data.size());
    values->resize(data.size());
    size_t enumerator = 0;
    for (auto iter = data.begin(); iter != data.end(); ++iter)
    {
        (*indexes)[enumerator] = iter->first;
        (*values)[enumerator] = iter->second;
        ++enumerator;
    }
}

SparseVector::SparseVector(shared_ptr<vector<int>> _indexes
    , shared_ptr<vector<double>> _values)
{
	indexes = _indexes;
	values = _values;
}

SparseVector::SparseVector(const SparseVector& data)
    : indexes(data.indexes), values(data.values)
{
}

SparseVector::SparseVector(SparseVector&& data)
    : indexes(std::move(data.indexes))
    , values(std::move(data.values))
{
}

SparseVector& SparseVector::operator = (const SparseVector& data)
{
    indexes = data.indexes;
    values = data.values;
    return *this;
}

SparseVector& SparseVector::operator = (SparseVector&& data)
{
    indexes = std::move(data.indexes);
    values = std::move(data.values);
    return *this;
}

SparseVector::~SparseVector(void)
{
}

int SparseVector::Size() const
{
	return indexes->size();
}

double SparseVector::GetValue(int index) const
{
	return values->at(index);
}

int SparseVector::Contains(int index) const
{
	int begin = 0;
	int end = indexes->size() - 1;
	while (end >= begin)
	{
		int middle = (begin + end) / 2;
		if (indexes->at(middle) > index)
		{
			end = middle - 1;
		}
		else if (indexes->at(middle) < index)
		{
			begin = middle + 1;
		}
		else return middle;
	}
	return -1;
}

double SparseVector::GetIndex(int index) const
{
	return indexes->at(index);
}

const vector<int>& SparseVector::GetIndexes() const
{
	return *indexes;
}

const vector<double>& SparseVector::GetValues() const
{
	return *values;
}

void SparseVector::Truncate(double truncationLevel)
{
    vector<int> newIndexes;
    vector<double> newValues;
    for (size_t index = 0; index < indexes->size(); ++index)
    {
        if (std::abs((*values)[index]) > truncationLevel)
        {
            newIndexes.push_back((*indexes)[index]);
            newValues.push_back((*values)[index]);
        }
    }
    *indexes = newIndexes;
    *values = newValues;
}

map<int, double> SparseVector::ConvertToMap()
{
    map<int, double> converted;
    for (size_t index = 0; index < indexes->size(); ++index)
    {
        converted[indexes->at(index)] = values->at(index);
    }
    return converted;
}

}
