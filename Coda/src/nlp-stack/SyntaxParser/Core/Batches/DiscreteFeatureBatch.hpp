/**
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

#ifndef DEFAULT_FEATURE_BATCH
#define DEFAULT_FEATURE_BATCH

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "Tools.h"

using std::shared_ptr;
using std::unordered_map;
using std::vector;
using std::wstring;

namespace SyntaxParser
{

using Tools::Alphabet;

template<class FeatureType>
class DiscreteFeatureBatch
{
public:
    DiscreteFeatureBatch();

    DiscreteFeatureBatch(
        vector<FeatureType>&& features);

    DiscreteFeatureBatch(DiscreteFeatureBatch<FeatureType>&& batch);

    DiscreteFeatureBatch<FeatureType>& operator = (
        DiscreteFeatureBatch&& batch);

    size_t GetSize() const;

    const FeatureType& GetFeature(size_t index) const;

    double GetWeight(size_t index) const;

    void Add(FeatureType&& feature);

    void Add(FeatureType&& feature, double value);

    void Reserve(size_t vectorSize);

    const unordered_map<FeatureType, double>& GetWeights() const;

    template<class Feat>
    friend std::ostream& operator << (std::ostream& stream
        , const DiscreteFeatureBatch<Feat>& batch);

    template<class Feat>
    friend std::istream& operator >> (std::istream& stream
        , DiscreteFeatureBatch<Feat>& batch);

private:
    vector<FeatureType> features;
};

DiscreteFeatureBatch<int> Translate(
    const DiscreteFeatureBatch<wstring>& batch,
    shared_ptr<Alphabet> alphabet, bool addingToAlphabet);

DiscreteFeatureBatch<wstring> Translate(
    const DiscreteFeatureBatch<int>& batch,
    shared_ptr<Alphabet> alphabet);

template<class FeatureType>
DiscreteFeatureBatch<FeatureType>::DiscreteFeatureBatch()
{
}

template<class FeatureType>
DiscreteFeatureBatch<FeatureType>::DiscreteFeatureBatch(
    vector<FeatureType>&& io_features)
    : features(std::move(io_features))
{
}

template<class FeatureType>
DiscreteFeatureBatch<FeatureType>::DiscreteFeatureBatch(
    DiscreteFeatureBatch<FeatureType>&& batch)
    : features(std::move(batch.features))
{
}

template<class FeatureType>
DiscreteFeatureBatch<FeatureType>& DiscreteFeatureBatch<FeatureType>::operator = (
    DiscreteFeatureBatch&& batch)
{
    this->features = std::move(batch.features);
    return *this;
}

template<class FeatureType>
size_t DiscreteFeatureBatch<FeatureType>::GetSize() const
{
    return features.size();
}

template<class FeatureType>
const FeatureType& DiscreteFeatureBatch<FeatureType>::GetFeature(
    size_t index) const
{
    return features[index];
}

template<class FeatureType>
double DiscreteFeatureBatch<FeatureType>::GetWeight(
    size_t index) const
{
    return 1;
}

template<class FeatureType>
void DiscreteFeatureBatch<FeatureType>::Add(
    FeatureType&& feature)
{
    features.push_back(std::move(feature));
}

template<class FeatureType>
void DiscreteFeatureBatch<FeatureType>::Add(
    FeatureType&& feature, double value)
{
    std::wcout << L"Function Add(feature, value) is prohibited in DiscreteFeatureBatch" << std::endl;
    exit(0);
}

template<class FeatureType>
void DiscreteFeatureBatch<FeatureType>::Reserve(
    size_t vectorSize)
{
    features.reserve(vectorSize);
}

template<class Feat>
std::ostream& operator << (std::ostream& stream
    , const DiscreteFeatureBatch<Feat>& batch)
{
    stream << batch.features.size();
    stream << std::endl;
    for (const Feat& feat : batch.features)
    {
        stream << feat;
        stream << std::endl;
    }
    return stream;
}

template<class Feat>
std::istream& operator >> (std::istream& stream
    , DiscreteFeatureBatch<Feat>& batch)
{
    size_t size;
    stream >> size;
    batch.features.resize(size);
    for (size_t index = 0; index < size; ++index)
    {
        stream >> batch.features[index];
    }
    return stream;
}

}

#endif // DEFAULT_FEATURE_BATCH

