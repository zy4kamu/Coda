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

#ifndef IFEATURESREADER_H
#define IFEATURESREADER_H

#include <unordered_map>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

namespace softmax_regression
{

class ISampleReader
{
public:
    virtual ~ISampleReader() {}
    virtual void load(std::vector<std::wstring>& io_indexToFeatureName
					, std::map<std::wstring, size_t>& io_featureNameToIndex
					, std::vector<std::unordered_map<size_t, double>>& io_elements
					, std::map<std::wstring, int>& io_classLabelToIndex
					, std::vector<size_t>& io_labels) const = 0;
};

class IModelReader
{
public:
    virtual ~IModelReader() {}
    virtual void load(std::vector<std::wstring>& io_indexToFeatureName
                    , std::vector<std::wstring>& io_indexToLabel
                    , std::vector<double>& io_classLabelToIndex) const = 0;
};

class ModelBinaryReader : public IModelReader
{
public:
    ModelBinaryReader(const std::string& path) : m_fileName(path) {}

    virtual void load(std::vector<std::wstring>& io_indexToFeatureName
                    , std::vector<std::wstring>& io_indexToLabel
                    , std::vector<double>& io_classLabelToIndex) const;
private:
    std::string m_fileName;
};

class TextSampleReader : public ISampleReader
{
public:
    TextSampleReader(const std::string& path) : m_fileName(path) {}

    void load(std::vector<std::wstring>& io_indexToFeatureName
                , std::map<std::wstring, size_t>& io_featureNameToIndex
                , std::vector<std::unordered_map<size_t, double>>& io_elements
				, std::map<std::wstring, int>& io_classLabelToIndex
                , std::vector<size_t>& io_labels) const;

private:
    std::string m_fileName;
};

} // namespace _softmax_regression

#endif // IFEATURESREADER_H
