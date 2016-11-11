/*! @file blob.h
 *  @section Copyright
 *  Object Retrieval Engine
 *  @brief definition Blob class which used to save serialized data.
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


#ifndef BLOB_H
#define BLOB_H

#include <fstream>
#include <cstdlib>
#include <stdint.h>
#include "sqlite_string_processor_base.h"

/*! @class Blob
 *  @brief wrapper of serialized data
 */
class Blob{
    size_t m_capacity;
    size_t m_size;
    char* m_data;
public:
    Blob(const Blob& i_other)
    : m_capacity(i_other.m_capacity)
    , m_size(i_other.m_size)
    , m_data(new char[m_capacity]) {
        memcpy(m_data, i_other.m_data, m_size);
    }

    /*! @brief constructor
     * @param i_size in start size of the Blob capacity
     */
    Blob(size_t i_size = 10)
    : m_capacity(i_size)
    , m_size(0)
    , m_data(new char[m_capacity]) {
    }

    /*! @brief constructor
     *  @param i_blob in pointer to input data
     *  @param i_size in size of input data
     */
    Blob(const void* i_blob, size_t i_size)
    : m_capacity(i_size)
    , m_size(i_size)
    , m_data(new char[i_size]) {
        memcpy(m_data, i_blob, m_size);
    }

    ~Blob(){
        delete[] m_data;
    }

    Blob& operator = (const Blob& i_other) {
        if (this != &i_other) {
            m_capacity = (i_other.m_capacity);
            m_size = (i_other.m_size);
            delete[] m_data;
            m_data = (new char[m_capacity]);
            memcpy(m_data, i_other.m_data, m_size);
        }
        return *this;
    }
    /*! @brief write into the blob next part of data
     *  @param i_blob in pointer to input data
     *  @param i_size in size of input data
     *  @param i_shift in set a position into the blob to start write the data
     */
    void set(const void* i_blob, size_t i_size, size_t i_shift = 0) {
        if(i_size + i_shift > m_capacity){
            char* oldData = m_data;
            m_capacity = i_size + i_shift;
            m_data = new char[m_capacity];
            memcpy(m_data, oldData, i_shift);
            delete[] oldData;
        }
        m_size = i_size + i_shift;
        memcpy(m_data + i_shift, i_blob, i_size);
    }

    /*! @brief get pointer to blob
     *  @return pointer to blob data
     */
    const char* data() const{
        return m_data;
    }

    /*! @brief get current size of blob
     *  @return size the blob
     */
    size_t size() const{
        return m_size;
    }
};

/*! @fn inline Blob operator + (const Blob& i_arg1, const Blob& i_arg2)
 *  @brief operator of blob concatenation
 *  @return new blob
 */
inline Blob operator + (const Blob& i_arg1, const Blob& i_arg2){
    Blob merge(i_arg1.size() + i_arg2.size());
    merge.set(i_arg1.data(), i_arg1.size());
    merge.set(i_arg2.data(), i_arg2.size(), i_arg1.size());
    return merge;
}

/*! @fn inline std::string toString(const Blob& i_data)
 *  @brief get string representation of the blob
 *  @return string representation in Hex format
 */
inline std::string toString(const Blob& i_data){
   std::string strBlob = SQLiteStringProcessorBase::Hex2String(i_data.data(), i_data.size());
   return "\"" + strBlob + "\"";
}

inline Blob writeBlob2File(const std::string& i_fileName, const Blob& i_data){
    std::ofstream dataFile(i_fileName.c_str(),
            std::ios_base::trunc | std::ios_base::out | std::ios_base::binary);
    for (size_t i = 0; i < i_data.size(); ++i) {
        dataFile.put(i_data.data()[i]);
    }
    dataFile.close();
    return Blob(i_fileName.c_str(), i_fileName.length());
}

inline Blob readBlobFromFile(const std::string& i_fileName, size_t i_size){
    std::ifstream dataFile(i_fileName.c_str());
    char* ch = new char[i_size];
    for (size_t i = 0; i < i_size; ++i) {
        dataFile.get(*(ch + i));
    }
    dataFile.close();
    Blob rezult(ch, i_size);
    delete[] ch;
    return rezult;
}

#endif
