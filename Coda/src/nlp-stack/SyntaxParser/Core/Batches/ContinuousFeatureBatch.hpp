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

#ifndef FEATURE_BATCH
#define FEATURE_BATCH

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

// !!! HEY, FUNCTION GET WEIGHT WORKS SLOWER THAN IT SHOULD !!!
template<class FeatureType>
class ContinuousFeatureBatch
{
public:
    ContinuousFeatureBatch();

    ContinuousFeatureBatch(
        vector<FeatureType>&& features
        , unordered_map<FeatureType, double>&& weights
            = unordered_map<FeatureType, double>());

    ContinuousFeatureBatch(ContinuousFeatureBatch&& batch);

    ContinuousFeatureBatch<FeatureType>& operator = (
        ContinuousFeatureBatch&& batch);

    size_t GetSize() const;

    const FeatureType& GetFeature(size_t index) const;

    double GetWeight(size_t index) const;

    void Add(FeatureType&& feature);

    void Add(FeatureType&& feature, double value);

    void Reserve(size_t vectorSize);

    const unordered_map<FeatureType, double>& GetWeights() const;

    template<class Feat>
    friend std::ostream& operator << (std::ostream& stream
        , const ContinuousFeatureBatch<Feat>& batch);

    template<class Feat>
    friend std::istream& operator >> (std::istream& stream
        , ContinuousFeatureBatch<Feat>& batch);
private:
    vector<FeatureType> features;
    unordered_map<FeatureType, double> weights;
};

ContinuousFeatureBatch<int> Translate(
    const ContinuousFeatureBatch<wstring>& batch,
    shared_ptr<Alphabet> alphabet, bool addingToAlphabet);

ContinuousFeatureBatch<wstring> Translate(
    const ContinuousFeatureBatch<int>& batch,
    shared_ptr<Alphabet> alphabet);

template<class FeatureType>
ContinuousFeatureBatch<FeatureType>::ContinuousFeatureBatch()
{
}

template<class FeatureType>
ContinuousFeatureBatch<FeatureType>::ContinuousFeatureBatch(
    vector<FeatureType>&& io_features
    , unordered_map<FeatureType, double>&& io_weights)
    : features(std::move(io_features))
    , weights(std::move(io_weights))
{
}

template<class FeatureType>
ContinuousFeatureBatch<FeatureType>::ContinuousFeatureBatch(
    ContinuousFeatureBatch&& batch)
    : features(std::move(batch.features))
    , weights(std::move(batch.weights))
{
}

template<class FeatureType>
ContinuousFeatureBatch<FeatureType>&
ContinuousFeatureBatch<FeatureType>::operator = (
    ContinuousFeatureBatch&& batch)
{
    this->features = std::move(batch.features);
    this->weights = std::move(batch.weights);
    return *this;
}

template<class FeatureType>
size_t ContinuousFeatureBatch<FeatureType>::GetSize() const
{
    return features.size();
}

template<class FeatureType>
const FeatureType& ContinuousFeatureBatch<FeatureType>::GetFeature(
    size_t index) const
{
    return features[index];
}

template<class FeatureType>
double ContinuousFeatureBatch<FeatureType>::GetWeight(
    size_t index) const
{
    auto found = weights.find(features[index]);
    if (found != weights.end()) {
        return found->second;
    } else {
        return 1;
    }
}

template<class FeatureType>
void ContinuousFeatureBatch<FeatureType>::Add(
    FeatureType&& feature)
{
    features.push_back(std::move(feature));
}

template<class FeatureType>
void ContinuousFeatureBatch<FeatureType>::Add(
    FeatureType&& feature, double value)
{
    weights[feature] = value;
    features.push_back(std::move(feature));
}

template<class FeatureType>
void ContinuousFeatureBatch<FeatureType>::Reserve(
    size_t vectorSize)
{
    features.reserve(vectorSize);
}

template<class FeatureType>
const unordered_map<FeatureType, double>&
ContinuousFeatureBatch<FeatureType>::GetWeights() const
{
    return weights;
}

template<class Feat>
std::ostream& operator << (std::ostream& stream
    , const ContinuousFeatureBatch<Feat>& batch)
{
    stream << batch.features.size() << std::endl;
    for (Feat& feat : batch.features)
    {
        stream << feat << std::endl;
    }
    stream << batch.weights.size() << std::endl;
    for (auto& weight : batch.weights)
    {
        stream << weight.first
               << std::endl
               << weight.second
               << std::endl;
    }
    return stream;
}

template<class Feat>
std::istream& operator >> (std::istream& stream
    , ContinuousFeatureBatch<Feat>& batch)
{
    size_t size;
    stream >> size;
    batch.features.resize(size);
    for (size_t index = 0; index < size; ++index)
    {
        stream >> batch.features[index];
    }
    stream >> size;
    batch.weights.reserve(size);
    for (size_t index = 0; index < size; ++index)
    {
        Feat feature;
        double value;
        stream >> feature >> value;
        batch.weights[feature] = value;
    }
    return stream;
}

}

#endif // FEATURE_BATCH
