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

#ifndef RELEARN_REGULARIZATION
#define RELEARN_REGULARIZATION

#include "IRegularization.h"
#include "VectorFactorsExtractor.h"

#include <cstdlib>

namespace LinearCRF
{

class RelearnRegularization : public IRegularization
{
public:
    RelearnRegularization(double regularizationFactor);

    void Initialize(const vector<double>& stableValues);

    void UpdateValue(const vector<double>& point
        , size_t numberOfPoints, double* value);

    void UpdateGradient(const vector<double>&point
        , size_t numberOfPoints, vector<double>* gradient);

    void SetRegularizationFactor(double factor);

    double GetRegularizationFactor();
private:
    double regularizationFactor;
    vector<double> stableValues;
};

}

#endif // RELEARN_REGULARIZATION
