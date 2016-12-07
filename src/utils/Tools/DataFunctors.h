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

#ifndef TRAINING_AND_TEST_DATA_FUNCTORS
#define TRAINING_AND_TEST_DATA_FUNCTORS

#include <cstdlib>

namespace Tools
{

class DataFunctor
{
public:
    virtual bool operator()(size_t index) = 0;
};

template<size_t modNumber>
class ModFunctor : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        return index % modNumber == 0;
    }
};

template<size_t modNumber, size_t firstMod>
class ModFunctorOneParam : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        size_t mod = index % modNumber;
        return (mod == firstMod);
    }
};

template<size_t modNumber>
class NotModFunctor : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        return index % modNumber != 0;
    }
};

template<size_t modNumber, size_t firstMod>
class NotModFunctorOneParam : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        size_t mod = index % modNumber;
        return (mod != firstMod);
    }
};

template<size_t modNumber, size_t firstMod, size_t secondMod>
class NotModFunctorTwoParams : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        size_t mod = index % modNumber;
        return (mod != firstMod) && (mod != secondMod);
    }
};

template<size_t maxIndex>
class LessFunctor : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        return index < maxIndex;
    }
};

template<size_t maxIndex>
class MoreFunctor : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        return index >= maxIndex;
    }
};

template<size_t lowerBound, size_t upperBound>
class SegmentFunctor : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        return index >= lowerBound && index <= upperBound;
    }
};

}

#endif // TRAINING_AND_TEST_DATA_FUNCTORS
