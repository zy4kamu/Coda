/*! @file sqlite_exceptions.h
 *  @section Copyright
 *  Object Retrieval Engine
 *  @brief definition C++ interface for CQLite
 *  @author Moiseenko Andrey <moiseenko.a@samsung.com>
 *  @version 1.0
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


#include <string>

#ifndef SQLITE_EXCEPTIONS_H
#define SQLITE_EXCEPTIONS_H
/*! @class SerializingException
 *  @brief base class for SQL wrapper exceptions
 */
class SQLiteException{
    std::string m_message;
public:
    SQLiteException(const std::string& i_errorMessage = "")
    : m_message(i_errorMessage){}

    const std::string& message() const {
        return m_message;
    }
};

#endif //SQLITE_EXCEPTIONS_H
