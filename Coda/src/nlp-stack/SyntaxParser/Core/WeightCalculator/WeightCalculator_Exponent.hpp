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

#ifndef WEIGHT_CALCULATOR_EXP
#define WEIGHT_CALCULATOR_EXP

#include <memory>

#include "BatchTypeIdentifier.h"

namespace SyntaxParser
{

#define WEIGHT_CALCULATOR_EXP_TEMPLATE template < \
    bool areFeaturesDiscrete>

#define WEIGHT_CALCULATOR_EXP_TEMPLATE1 WeightCalculator_Exponent< \
    areFeaturesDiscrete>

WEIGHT_CALCULATOR_EXP_TEMPLATE
class WeightCalculator_Exponent
{
public:
    typedef typename BatchTypeIdentifier<areFeaturesDiscrete>::IntBatchType IntBatch;

    WeightCalculator_Exponent(void);
    ~WeightCalculator_Exponent(void);

    double CalculateLogWeight(
        const IntBatch& batch,
        const vector<double>& parameters);

    double CalculateLogWeightGradient(
        const IntBatch& batch,
        const vector<double>& parameters,
        int index);
};

WEIGHT_CALCULATOR_EXP_TEMPLATE
WEIGHT_CALCULATOR_EXP_TEMPLATE1::WeightCalculator_Exponent(void)
{
}

WEIGHT_CALCULATOR_EXP_TEMPLATE
WEIGHT_CALCULATOR_EXP_TEMPLATE1::~WeightCalculator_Exponent(void)
{
}

WEIGHT_CALCULATOR_EXP_TEMPLATE
double WEIGHT_CALCULATOR_EXP_TEMPLATE1::CalculateLogWeight(
    const IntBatch& batch,
    const vector<double>& parameters)
{
    double sum = 0;
    size_t size = batch.GetSize();
    for (size_t it = 0; it < size; ++it)
    {
        sum += parameters[batch.GetFeature(it)] * batch.GetWeight(it);
    }
    return sum;
}

WEIGHT_CALCULATOR_EXP_TEMPLATE
double WEIGHT_CALCULATOR_EXP_TEMPLATE1::CalculateLogWeightGradient(
    const IntBatch& batch,
    const vector<double>& parameters,
    int index)
{
    return batch.GetWeight(index);
}

};

#endif

