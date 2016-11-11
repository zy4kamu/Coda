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

#ifndef IOPTIMIZABLE
#define IOPTIMIZABLE

#include <vector>

using std::vector;

namespace Optimization
{

class IOptimizable
{
public:
    IOptimizable();
    IOptimizable(int dimension);
    virtual ~IOptimizable(void);
    virtual double CalculateValue(const vector<double>& point) = 0;
    virtual void CalculateGradient(
        const vector<double>& point, vector<double>* gradient) = 0;
    inline int GetDimension() const { return dimension; }
protected:
    int dimension;
};

};

#endif

