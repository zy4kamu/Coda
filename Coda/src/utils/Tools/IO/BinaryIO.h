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

#ifndef BINARY_IO
#define BINARY_IO

#include <string>
#include <vector>
#include <fstream>
#include <locale>
#include <unordered_map>
#include <iostream>

#include "Strings/StringConvertions.h"

using std::string;
using std::wstring;
using std::vector;
using std::ofstream;
using std::wofstream;
using std::ifstream;
using std::wifstream;
using std::locale;
using std::unordered_map;
using std::ios;
using std::ifstream;
using std::wcout;
using std::endl;

namespace Tools {

/**
* @brief	Writes vector of string to binary file
* @param[in]	fname - name of the output file
* @param[in]	data - data to write
*/
void WriteStringArray_Binary(const string &fname, const vector<string>& data);

/**
* @brief	Writes vector of string to binary stream
* @param[in]	out - name of the stream
* @param[in]	data - data to write
*/
void WriteStringArray_Binary(ofstream& out, const vector<string>& data);

/**
* @brief	Writes wide string to binary stream
* @param[in]	i_out - name of the stream
* @param[in]	i_wstring - data to write
*/
void WriteWString_Binary(ofstream& i_out, const wstring& i_wstring, locale loc = std::locale());

/**
* @brief	Writes string to binary stream
* @param[in]	i_out - name of the stream
* @param[in]	i_string - data to write
*/
void WriteString_Binary(ofstream& i_out, const string& i_string);

/**
* @brief	Writes vector of wstring to binary file
* @param[in]	fname - name of the output file
* @param[in]	data - data to write
*/
void WriteWStringArray_Binary(const string &fname, const vector<wstring>& data);

/**
* @brief	Writes vector of wstring to binary stream
* @param[in]	out - name of the stream
* @param[in]	data - data to write
*/
void WriteWStringArray_Binary(ofstream& out, const vector<wstring>& data);

/**
* @brief	Reads file line by line
* @param[in]	fname - file to read
* @param[out]	data - lines of the file
*/
void ReadStringArray(const string &fname, vector<string>& data);

/**
* @brief	Reads stream line by line
* @param[in]	in - stream to read
* @param[out]	data - lines of the file
*/
void ReadStringArray(ifstream& in, vector<string>& data);

/**
* @brief	Reads file line by line in wstring-format
* @param[in]	fname - file to read
* @param[out]	data - lines of the file
*/
void ReadWStringArray(const string &fname, vector<wstring>& data);

/**
* @brief	Reads stream line by line in wstring format
* @param[in]	in - stream to read
* @param[out]	data - lines of the file
*/
void ReadWStringArray(ifstream& in, vector<wstring>& data);

/**
* @brief	Reads stream in wstring format
* @param[in]	i_in - stream to read
* @param[out]	o_wstring - place for storing result
*/
void ReadWString(ifstream& i_in, wstring& o_wstring, locale loc = std::locale());

/**
* @brief	Reads stream in string format
* @param[in]	i_in - stream to read
* @param[out]	o_string - place for storing result
*/
void ReadString(ifstream& i_in, string& o_string);

/**
 * Saving unordered_map<class T1, class T2> in binary format
 */
template<class T1, class T2>
void
save_map_binary(ofstream& i_oStream, unordered_map<T1, T2>& i_map)
{
    int size = (int)i_map.size();
    int size_T1, size_T2;
    size_T1 = (int)sizeof(T1);
    size_T2 = (int)sizeof(T2);
    i_oStream.write((char*)& size,4);
    i_oStream.write((char*)& size_T1,4);
    i_oStream.write((char*)& size_T2,4);
    for (auto itr = i_map.begin(); itr != i_map.end(); itr++)
    {
        i_oStream.write((char*)& itr->first,size_T1);
        i_oStream.write((char*)& itr->second,size_T2);
    }
}

/**
 * Saving unordered_map<wstring, class T> in binary format
 */
template<class T>
void
save_map_binary(ofstream& i_oStream, unordered_map<wstring, T>& i_map)
{
    int size = (int)i_map.size();
    i_oStream.write((char*)& size,4);
#ifdef MSVC
    locale loc = std::locale();
#else
    locale loc = std::locale("ru_RU.UTF-8");
#endif
    for (auto itr = i_map.begin(); itr != i_map.end(); itr++)
    {
        string str;
        Tools::ConvertWstringToUTF8(itr->first,str,loc);
        Tools::WriteString_Binary(i_oStream,str);
        i_oStream.write((char*)& itr->second,(sizeof(T) == sizeof(double))? 8 : 4);
    }
}

template<class T1, class T2>
void
save_map_binary(const string& i_filename, unordered_map<T1, T2>& i_map)
{
    ofstream oStream(i_filename,ios::binary|ios::out);
    if (!oStream)
    {
        std::wcout<< L"Error:save_map_binary: can\'t create stream" << std::endl;
        return;
    }
    save_map_binary(oStream,i_map);
    oStream.close();
}


/**
 * Loading unordered_map<wstring, class T> in binary format
 */
template<class T1, class T2>
void
load_map_binary(ifstream& i_iStream, unordered_map<T1, T2>& i_map)
{
    int size;
    int size_T1, size_T2;
    T1 value;
    T2 key;
    i_iStream.read((char*)& size, 4);
    i_iStream.read((char*)& size_T1, 4);
    i_iStream.read((char*)& size_T2, 4);
    for (int i = 0; i < size; i++)
    {
        i_iStream.read((char*)& key,size_T1);
        i_iStream.read((char*)& value,size_T2);
        i_map[key] = value;
    }
}

/**
 * Loading unordered_map<wstring, class T> in binary format
 */
template<class T>
void
load_map_binary(ifstream& i_iStream, unordered_map<wstring, T>& i_map)
{
    int size;
    wstring empty_str = L"empty_string";
    i_iStream.read((char*)& size, 4);
#ifdef MSVC
    locale loc = std::locale();
#else
    locale loc = std::locale("ru_RU.UTF-8");
#endif
    for (int i = 0; i < size; i++)
    {
        string str;
        T value;
        Tools::ReadString(i_iStream,str);
        wstring line = Tools::ConvertUTF8StringToWstring(str,loc);
        i_iStream.read((char*)& value,(sizeof(T) == sizeof(double))? 8 : 4);
        if (line.empty())
            line = empty_str+std::to_wstring(i);
        while (i_map.count(line))
        {
            line += std::to_wstring(i);
            wcout<<line<<std::endl;
        }
        i_map[line] = value;
    }
}

/**
 * Loading unordered_map<wstring, class T> in binary format
 */
template<class T1, class T2>
void
load_map_binary(const string& i_filename, unordered_map<T1, T2>& i_map)
{
    ifstream iStream(i_filename,ios::in|ios::binary);
    if (!iStream.is_open())
    {
        wcout<<L"Can\'t open file for reading\n.Filename:"<<Tools::ConvertUTF8StringToWstring(i_filename)<<endl;
        return;
    }
    i_map.clear();
    load_map_binary(iStream,i_map);
    iStream.close();
}

/**
 * Saving vector of unordered_map<wstring, class T> in binary format
 */
template<class T>
void
save_vmap_binary(const string& i_filename, vector<unordered_map<wstring, T> >& i_vmap)
{
    ofstream oStream(i_filename,ios::binary|ios::out);
    if (!oStream)
    {
        wcout<< L"Error:save_map_binary: can\'t create stream" << endl;
        return;
    }
    int size = i_vmap.size();
    oStream.write((char*)& size,4);
    for (size_t i = 0; i < i_vmap.size(); i++)
    {
        save_map_binary(oStream,i_vmap[i]);
    }
    oStream.close();
}

/**
 * Loading vector of unordered_map<wstring, class T> in binary format
 */
template<class T>
void
load_vmap_binary(const string& i_filename, vector<unordered_map<wstring, T> >& i_vmap)
{
    ifstream iStream(i_filename,ios::in|ios::binary);
    if (!iStream.is_open())
    {
        wcout<<L"Can\'t open file for reading\n.Filename:"<<Tools::ConvertUTF8StringToWstring(i_filename)<<endl;
        return;
    }
    i_vmap.clear();
    int size;
    iStream.read((char*)& size, 4);
    for (int i = 0; i < size; i++)
    {
        unordered_map<wstring, T> map;
        load_map_binary(iStream,map);
        i_vmap.push_back(map);
    }
    iStream.close();
}


}

#endif // BINARY_IO
