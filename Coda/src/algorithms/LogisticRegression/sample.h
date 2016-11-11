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

#ifndef TRAININGSAMPLE_H
#define TRAININGSAMPLE_H

#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <memory>
#include "features-reader.h"

namespace softmax_regression
{

class TrainingSet
{
public:
    TrainingSet(const std::shared_ptr<ISampleReader>& i_sampleReader);

    size_t getSize() const;
    size_t getLabelOfElement(size_t i_elementIndex) const;
    const std::unordered_map<size_t,double>& getElement(size_t i_elementIndex) const;
    size_t getNumberOfFeatures() const;
    size_t getNumberOfClasses() const;

    std::map<std::wstring,double> getFeaturesOfElement(size_t i_elementIndex) const;
    std::wstring getStringLabelOfElement(size_t i_elementIndex) const;

    const std::vector<std::wstring>& getFeatures() const {return m_indexToFeatureName;}
    const std::vector<std::wstring>& getLabels() const {return m_classIndexToLabel;}
    size_t getNumberOfElements() const {return m_trainingSet.size();}

    friend class FeaturesFilter;

private:
    std::vector<std::wstring> m_indexToFeatureName;
    std::map<std::wstring, size_t> m_featureNameToIndex;
    std::vector<std::unordered_map<size_t,double>> m_trainingSet;
    std::vector<size_t> m_trainLabels;
	std::map<std::wstring, int> m_classLabelToIndex;
    std::vector<std::wstring> m_classIndexToLabel;

    void fillLabels();

    void fillFeatureStatistics( std::vector< std::vector<size_t> >& features_for_classes) const;
    std::vector<size_t> filterFeatures( const std::vector< std::vector<size_t> >& features_for_classes, size_t number) const;
};

} //namespace _softmax_regression

#endif // ISAMPLE_H
