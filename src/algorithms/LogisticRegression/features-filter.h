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

#ifndef FEATURESFILTER_H
#define FEATURESFILTERE_H

#include <vector>
#include <string>
#include <memory>
#include "sample.h"
#include <climits>

namespace softmax_regression
{

class IFeaturesFilter
{
public:

    virtual void Filter(std::shared_ptr<TrainingSet> set) const = 0;
    virtual void SetMaxFeaturesCount(double value) = 0;
 };

class FeaturesFilter : public IFeaturesFilter
{
public:
    FeaturesFilter( double PMIThreshold = 0.3, size_t maxFeaturesCount = INT_MAX ) : m_PMIThreshold ( PMIThreshold ), m_maxFeaturesCount( maxFeaturesCount ) {};
    FeaturesFilter( const FeaturesFilter* other) : m_PMIThreshold( other->m_PMIThreshold ), m_maxFeaturesCount(other->m_maxFeaturesCount) {};

    void Filter(std::shared_ptr<TrainingSet> set) const;

    void SetPMIThreshold(double value) { m_PMIThreshold = value; };
    void SetMaxFeaturesCount(double value) { m_maxFeaturesCount = value; };


private:
    double m_PMIThreshold;
    size_t m_maxFeaturesCount;

    void fillFeatureStatistics( std::shared_ptr<TrainingSet> set, std::vector< std::vector<double> >& features_for_classes) const;
    std::vector<size_t> filterFeatures( const std::vector< std::vector<double> >& features_for_classes, size_t number) const;
    void refillSet(std::shared_ptr<TrainingSet> set, const std::vector<size_t>& filtered ) const;
};


}

#endif //FEATURESFILTER
