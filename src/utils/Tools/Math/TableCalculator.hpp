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

#include <functional>
#include <memory>

namespace Tools
{

template<class InputType = double, class OutputType = double>
class TableCalculator
{
public:
    TableCalculator(const std::function<OutputType(InputType)>& funct,
        InputType start, InputType end, InputType step);

    OutputType Get(InputType input);
private:
    InputType start;
    InputType end;
    InputType step;
    unordered_map<size_t, OutputType> table;
};


template<class InputType, class OutputType>
TableCalculator<InputType, OutputType>::TableCalculator(
    const std::function<OutputType(InputType)>& funct,
    InputType start, InputType end, InputType step)
    : start(start), end(end), step(step)
{
    size_t enumerator = 0;
    for (InputType iter = start; iter < end; iter += step)
    {
        table[enumerator++] = funct(iter);
    }
}

template<class InputType, class OutputType>
OutputType TableCalculator<InputType, OutputType>::Get(InputType input)
{
    if (input < start) {
        return table[0]; }
    else if (input >= end) {
        return table[table.size() - 1];
    } else {
        size_t index = static_cast<size_t>((input - start) / step);
        return table[index];
    }
}

}
