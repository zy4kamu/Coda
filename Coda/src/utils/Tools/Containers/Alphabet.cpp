/** 
 * @file	Alphabet.cpp
 * @brief	source file for alphabet structure for CRF-model
 */

#include "Alphabet.h"

namespace Tools
{

const wstring Alphabet::UNBELIEVABLE_ELEMENT = L"UNBELIEVABLE_ELEMENT";

Alphabet::Alphabet(void)
{
}

Alphabet::Alphabet(ifstream& in)
{
	Read(in);
}

Alphabet::Alphabet(wifstream& in)
{
	Read(in);
}

Alphabet::Alphabet(const Alphabet& alphabet)
    : _map(alphabet._map)
    , _vec(alphabet._vec)
{
}


Alphabet::~Alphabet()
{

}

void Alphabet::Read(ifstream& in)
{
	Tools::ReadWStringArray(in, _vec);
	int enumerator = 0;
	for(vector<wstring>::iterator it = _vec.begin(); it != _vec.end(); ++it)
	{
		_map.insert(std::make_pair(*it, enumerator));
		++enumerator;
	}
}

void Alphabet::Read(wifstream& in)
{
	wstring str;
	getline(in, str);
	int size = stoi(str);
	for (int i = 0; i < size; ++i)
	{
		getline(in, str);
		LookUpIndex_Adding(str);
	}
}

int Alphabet::LookUpIndex_Adding(const wstring& data)
{
	unordered_map<wstring, int>::iterator it = _map.find(data);
	if (it != _map.end())
		return _map[data];
	else
	{
		int size = _map.size();
		_map.insert(std::make_pair(data, size));
		_vec.push_back(data);
		return size;
	}
}

int Alphabet::LookUpIndex_NoAdding(const wstring& data) const
{
	unordered_map<wstring, int>::const_iterator it = _map.find(data);
	if (it != _map.end())
		return _map.at(data);
	else return -1;
}

wstring Alphabet::LookUpObject(int index) const
{
	if (index < 0 || index >= (int)_vec.size())
		throw "Alphabet::LookUpObject(int index) -- wrong argument: index";
	return _vec[index];
}


bool Alphabet::Contains(const wstring& feature) const
{
	return _map.find(feature) != _map.end();
}

Alphabet& Alphabet::operator=(const Alphabet& alphabet)
{
	_map = alphabet._map;
	_vec = alphabet._vec;
	return *this;
}

int Alphabet::Size() const
{
	return _vec.size();
}

void Alphabet::Print(ofstream& out) const
{
	Tools::WriteWStringArray_Binary(out, _vec);
}

const vector<wstring>& Alphabet::GetData() const
{
    return _vec;
}

void Alphabet::Clear()
{
    _map.clear();
    _vec.clear();
}

size_t Alphabet::GetByteSize() const
{
    size_t size = _vec.size() * (sizeof(double) + sizeof(int));
    for (auto iter = _map.begin(); iter != _map.end(); ++iter)
    {
        size += iter->first.size() * sizeof(wchar_t);
    }
    return size;
}

}



