/** 
 * @file	FixedHashMap.h
 * @brief	Fast map container for fixed key value set. 
 *          Realizes two level hash function
 *
 * .. invisible:
 *     _   _ _____ _     _____ _____
 *
 *    | | | |  ___| |   |  ___/  ___|
 *
 *    | | | | |__ | |   | |__ \ `--.
 *
 *    | | | |  __|| |   |  __| `--. \
 *
 *    \ \_/ / |___| |___| |___/\__/ /
 *
 *     \___/\____/\_____|____/\____/
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License
 *
 */

#ifndef FIXED_HASH_MAP
#define FIXED_HASH_MAP

#include <cassert>
#include <memory>

#include "LinearHashFunction.h"

using std::shared_ptr;

template <class ValueType>
class Bucket;

/** 
 * @class	FixedHashMap
 * @brief	Fast map container for fixed key value set.
 *          Realizes two level hash function.
 */
template <class ValueType>
class FixedHashMap
{
public:
    /**
	 * @brief	Constructor
	 */
    FixedHashMap()
    {}

    /**
	 * @brief	Constructor. Should take iterators like map<int, ValueType>::iterator
	 * @param[in]	begin - iterator on the begin
     * @param[in]	end - iterator on the end
	 */
    template <class MapIterator>
    FixedHashMap(MapIterator begin, MapIterator end);

    /**
	 * @brief	Constructor
	 * @param[in]	keys - keys
     * @param[in]	values - values
	 */
    FixedHashMap(const vector<int>& keys, 
        const vector<ValueType>& values);

    /**
	 * @brief	Initializes map
	 * @param[in]	keys - keys
     * @param[in]	values - values
	 */
    void Initialize(const vector<int>& keys, 
        const vector<ValueType>& values);

    /**
	 * @brief	Checks if map contains key
	 * @param[in]	keys - element to search
	 */
    bool Contains(int key) const;

    /**
	 * @brief	Finds value by key
	 * @param[in]	keys - element to search
	 */
    const ValueType& operator[] (int key) const;

    /**
	 * @brief	Finds value by key
     * @param[in]	key - element to search
	 */
    ValueType& operator[] (int key);

    /**
     * @brief	Gets keys and values from container
     * @param[out]	keys - keys
     * @param[out]	values - values
     */
    void GetData(
        shared_ptr<vector<int> > keys,
        shared_ptr<vector<ValueType> > values) const;
private:
    size_t firstLayerHashTableSize_; /**< size of first layer hash function size*/
    LinearHashFunction firstLayerHashFunction_; /**< function that splitts elements into buckets*/
    vector<Bucket<ValueType> > secondLayerBuckets_; /**< buckets that contain elemets splitted on the first stage*/

    /**
	 * @brief	Calculates sum of squared numbers
	 * @param[in]	keys - element to search
	 */
    size_t calculateSumSquares(const vector<int>& numbers);

    /**
	 * @brief	Finds hash function such that the sum of squared hash parts is about O(N)
	 * @param[in]	keys - element to search
	 */
    LinearHashFunction findHashFunctionWithUniformDistribution(
        const vector<int>& numbers);

    /**
	 * @brief	Applyes hash function to split keys and values
	 * @param[in]	keys - keys of the map
     * @param[in]	values - values of the map
     * @param[in]	hashFunction - splitting hash function
     * @param[in]	splittedKeys - set into which we insert splitted keys
     * @param[in]	splittedValues - set into which we insert splitted values
	 */
    void makePartition(const vector<int>& keys, 
        const vector<ValueType>& values, 
        const LinearHashFunction& hashFunction, 
        vector<vector<int> >* splittedKeys, 
        vector<vector<ValueType> >* splittedValues);
};

/** 
 * @class	Bucket
 * @brief	Enables to find elements using only one hash 
 *          function but takes O(N^2) memory
 */
template <class ValueType>
class Bucket
{
public:
    /**
	 * @brief	Constructor
	 */
    Bucket();

    /**
	 * @brief	Initializes bucket
	 * @param[in]	keys - keys
     * @param[in]	values - values
	 */
    void Initialize(const vector<int>& keys, 
        const vector<ValueType>& values);

    /**
	 * @brief	Checks if map contains key
	 * @param[in]	keys - element to search
	 */
    bool Contains(int key) const;

    /**
	 * @brief	Finds value by key
	 * @param[in]	keys - element to search
	 */
    ValueType& operator[] (int key);

    /**
	 * @brief	Finds value by key
	 * @param[in]	keys - element to search
	 */
    const ValueType& operator[] (int key) const;

    /**
     * @brief	Returns values in bucket
     */
    const vector<ValueType>& GetValues() const {return cells_;}

    /**
     * @brief	Returns keys in bucket
     */
    const vector<int>& GetKeys() const {return keyCells_;}
private:
    int CalculateBucketIndex(int number) const; /**< finds element in cells by its index*/

    vector<ValueType> cells_; /**< values contained in the bucket (here is O(N^2) elements)*/
    vector<int> keyCells_; /**< keys contained in the bucket (here is O(N^2) elements) */
    LinearHashFunction hashFunction_; /**< hash function that finds element in cells by its index*/
};


template <class ValueType>
Bucket<ValueType>::Bucket()
{
}

template <class ValueType>
void Bucket<ValueType>::Initialize(const vector<int>& keys,
    const vector<ValueType>& values)
{
    std::default_random_engine generator;
    cells_.clear();

    if (keys.size() == 0)
    {
        cells_.resize(0);
        return;
    }
    bool hashFound;

    vector<int> cellIndexes(keys.size() * keys.size(), UNDEFINED_NUMBER);
    do
    {
        cellIndexes.assign(cellIndexes.size(), UNDEFINED_NUMBER);
        hashFunction_ = RandomizeHashFunction(generator);
        hashFound = true;
        for (size_t it = 0; it < keys.size(); ++it)
        {
            int hash = hashFunction_(keys[it]) % cellIndexes.size();
            if (cellIndexes[hash] != UNDEFINED_NUMBER)
            {
                hashFound = false;
                break;
            }
            else
            {
                cellIndexes[hash] = it;
            }
        }
    } while (!hashFound);

    cells_.clear();
    cells_.resize(keys.size() * keys.size());
    keyCells_.resize(keys.size() * keys.size(), UNDEFINED_NUMBER);
    for (size_t hash = 0; hash < cellIndexes.size(); ++hash)
    {
        if (cellIndexes[hash] != UNDEFINED_NUMBER)
        {
            cells_[hash] = values[cellIndexes[hash]];
            keyCells_[hash] = keys[cellIndexes[hash]];
        }
    }
}

template <class ValueType>
bool Bucket<ValueType>::Contains(int key) const
{
    if (cells_.empty())
    {
        return false;
    } else {
        int index = CalculateBucketIndex(key);
        return keyCells_[index] == key;
    }
}

template <class ValueType>
ValueType& Bucket<ValueType>::operator[] (int key)
{
    int index = CalculateBucketIndex(key);
    return cells_[index];
}

template <class ValueType>
const ValueType& Bucket<ValueType>::operator[] (int key) const
{
    int index = CalculateBucketIndex(key);
    return cells_[index];
}

template <class ValueType>
int Bucket<ValueType>::CalculateBucketIndex(int key) const
{
    return hashFunction_(key) % cells_.size();
}

template <class ValueType>
size_t FixedHashMap<ValueType>::calculateSumSquares(const vector<int>& numbers)
{
    size_t sumSquares = 0;
    for (size_t it = 0; it < numbers.size(); ++it)
    {
        sumSquares += static_cast<size_t>(numbers[it]) * 
            static_cast<size_t>(numbers[it]);
    }
    return sumSquares;
}

template <class ValueType>
LinearHashFunction FixedHashMap<ValueType>::findHashFunctionWithUniformDistribution(
    const vector<int>& numbers)
{
    std::default_random_engine generator;
    LinearHashFunction hashFunction;
    vector<int> partition(numbers.size(), 0);
    do
    {
        partition.assign(numbers.size(), 0);
        hashFunction = RandomizeHashFunction(generator);
        for (size_t it = 0; it < numbers.size(); ++it)
        {
            int bucket = hashFunction(numbers[it]) % numbers.size();
            ++partition[bucket];
        }
    } while (calculateSumSquares(partition) >= 4 * numbers.size());
    return hashFunction;
}

template <class ValueType>
void FixedHashMap<ValueType>::makePartition(
    const vector<int>& keys, 
    const vector<ValueType>& values, 
    const LinearHashFunction& hashFunction, 
    vector<vector<int> >* splittedKeys, 
    vector<vector<ValueType> >* splittedValues)
{
    splittedKeys->resize(keys.size());
    splittedValues->resize(values.size());
    for (size_t it = 0; it < keys.size(); ++it)
    {
        int bucket = hashFunction(keys[it]) % keys.size();
        (*splittedKeys)[bucket].push_back(keys[it]);
        (*splittedValues)[bucket].push_back(values[it]);
    }
}

template <class ValueType>
template <class MapIterator>
FixedHashMap<ValueType>::FixedHashMap(
    MapIterator begin, MapIterator end)
{
    vector<int> keys;
    vector<ValueType> values;
    for (MapIterator iterator = begin; iterator != end; ++iterator)
    {
        keys.push_back(iterator->first);
        values.push_back(iterator->second);
    }
    if (!keys.empty())
    {
        Initialize(keys, values);
    }
}

template <class ValueType>
FixedHashMap<ValueType>::FixedHashMap(const vector<int>& keys, 
        const vector<ValueType>& values)
{
    Initialize(keys, values);
}

template <class ValueType>
void FixedHashMap<ValueType>::Initialize(const vector<int>& keys, 
        const vector<ValueType>& values)
{
    assert(keys.size() == values.size());
    secondLayerBuckets_.clear();

    firstLayerHashTableSize_ = keys.size();
    firstLayerHashFunction_ = findHashFunctionWithUniformDistribution(keys);
    vector<vector<int> > splittedKeys;
    vector<vector<ValueType> > splittedValues;
    makePartition(keys, values, firstLayerHashFunction_, 
        &splittedKeys, &splittedValues);

    secondLayerBuckets_.resize(keys.size());
    for (size_t it = 0; it < keys.size(); ++it)
    {
        secondLayerBuckets_[it].Initialize(
            splittedKeys[it], splittedValues[it]);
    }
}

template <class ValueType>
bool FixedHashMap<ValueType>::Contains(int key) const
{
    if (secondLayerBuckets_.size() == 0)
    {
        return false;
    }
    int bucketIndex = firstLayerHashFunction_(key) % firstLayerHashTableSize_;
    return secondLayerBuckets_[bucketIndex].Contains(key);
}

template <class ValueType>
const ValueType& FixedHashMap<ValueType>::operator[] (int key) const
{
    int bucketIndex = firstLayerHashFunction_(key) % firstLayerHashTableSize_;
    return secondLayerBuckets_[bucketIndex][key];
}

template <class ValueType>
ValueType& FixedHashMap<ValueType>::operator[] (int key)
{
    int bucketIndex = firstLayerHashFunction_(key) % firstLayerHashTableSize_;
    return secondLayerBuckets_[bucketIndex][key];
}

template <class ValueType>
void FixedHashMap<ValueType>::GetData(
    shared_ptr<vector<int> > keys,
    shared_ptr<vector<ValueType> > values) const
{
    for (size_t bucketIndex = 0; bucketIndex < secondLayerBuckets_.size();
         ++bucketIndex)
    {
        const Bucket<ValueType>& bucket = secondLayerBuckets_[bucketIndex];
        const vector<int>& bucketKeys = bucket.GetKeys();
        const vector<ValueType>& bucketValues = bucket.GetValues();
        for (size_t indexInBucket = 0; indexInBucket < bucketKeys.size(); ++indexInBucket)
        {
            if (bucketKeys[indexInBucket] != UNDEFINED_NUMBER)
            {
                keys->push_back(bucketKeys[indexInBucket]);
                values->push_back(bucketValues[indexInBucket]);
            }
        }
    }
}

#endif // FIXED_HASH_MAP
