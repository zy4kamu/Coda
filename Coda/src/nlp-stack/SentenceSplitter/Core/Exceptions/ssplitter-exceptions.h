/**
 * @file    ssplitter-exceptions.h
 * @brief	header file with exceptions for sentence splitter module
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


#ifndef SSPLITTEREXCEPTIONS_H
#define SSPLITTEREXCEPTIONS_H

#include <exception>
#include <sstream>
#include "Tools.h"

namespace _sentence_splitter
{

/**
 * @class	ModelException
 * @brief	exception for bad model of Sentence Splitter
 */
class ModelException : public std::exception
{
public:

    /**
     * @brief constructor
     * @param[in] i_path path to bad model
     * @param[in] i_line number of line where error occured
     */
    ModelException(const char* i_path, int i_line = -1)
    {
        std::stringstream s;
        s << "Bad model: path " << i_path << ", line " << i_line;
        message = s.str();
    }

    virtual const char* what() const NOEXCEPT { return message.c_str(); }

private:

    std::string message;
};

/**
 * @class	FileOpenException
 * @brief	exception for bad model of Sentence Splitter
 */
class FileOpenException : public std::exception
{
public:

    /**
     * @brief constructor
     * @param[in] i_path bad path
     */
    FileOpenException(const char* i_path)
    {
        std::stringstream s;
        s << "Bad path: " << i_path;
        message = s.str();
    }

    virtual const char* what() const NOEXCEPT { return message.c_str(); }

private:

    std::string message;
};


}


#endif // SSPLITTEREXCEPTIONS_H
