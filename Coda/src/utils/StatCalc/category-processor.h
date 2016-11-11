/**
 * @file    category-processor.h
 * @brief	header file with class and functions for converting documents to bag of word form
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


#ifndef _CATEGORY_PROCESSOR_
#define _CATEGORY_PROCESSOR_

#include "statcalc-enums.h"
#include <functional>
#include <algorithm>
#include <fstream>
#include "Tools.h"

namespace statcalc
{

/**
 * @class	DocumentProcessor
 * @brief	Class for converting documents to bag-of-words format and storing others good statistics...
 */
template<class T>
class LabelBase
{
public:
    LabelBase() : m_label(0), m_isEnum(false){}

    LabelBase(T label)
    {
        m_label = label;
    }

    LabelBase(const wstring &label)
    {
        m_label = this->fromWstring(label);
    }

    T& label()
    {
        return m_label;
    }

    const T& label() const
    {
        return m_label;
    }

    bool& isEnum()
    {
        return m_isEnum;
    }

    const bool& isEnum() const
    {
        return m_isEnum;
    }

    virtual void saveBinary(std::ofstream& i_oStream) = 0;
    virtual void loadBinary(std::ifstream& i_oStream) = 0;
    virtual vector<int> labelToVec() = 0;

//    virtual bool operator==(LabelBase& other)
//    {
//        return (m_label == other.label())? true : false;
//    }

    std::function<T(wstring)> fromWstring;
    virtual ~LabelBase(){}
private:
    T m_label;
    bool m_isEnum;
};

template<class T = int>
class EnumLabel : public LabelBase<T>
{
public:
    EnumLabel() : LabelBase<T>(){ this->isEnum() = true; }

    virtual void saveBinary(std::ofstream& i_oStream)
    {
        i_oStream.write((char*)& this->label(),4);
    }

    virtual void loadBinary(std::ifstream& i_iStream)
    {
        i_iStream.read((char*)& this->label(),4);
    }

    virtual vector<int> labelToVec()
    {
        return vector<int>(1,(int)this->label());
    }
};

class WikiCategoryFullLabel : public EnumLabel<>
{
public:
    WikiCategoryFullLabel() : EnumLabel<>()
    {
        this->fromWstring = statcalc::wikicategoryfullWstringToEnum;
    }
};

class WikiCategoryLabel : public EnumLabel<>
{
public:
    WikiCategoryLabel() : EnumLabel<>()
    {
        fromWstring = statcalc::wikicategoryWstringToEnum;
    }
};

class GoogleAppsLabel : public EnumLabel<>
{
public:
    GoogleAppsLabel() : EnumLabel<>()
    {
        fromWstring = statcalc::googlecategoryWstringToEnum;
    }
};

class GoogleAppsShortLabel : public EnumLabel<>
{
public:
    GoogleAppsShortLabel() : EnumLabel<>()
    {
        fromWstring = statcalc::googlecategoryshortWstringToEnum;
    }
};

class DomainLabel : public EnumLabel<>
{
    DomainLabel() : EnumLabel<>()
    {
        fromWstring = statcalc::domainWstringToEnum;
    }
};

template<class T = Tools::TrieDictionary<vector<wstring>,int> >
class HierarchicalLabel : public LabelBase<T>
{
public:
    HierarchicalLabel() : LabelBase<T>(){ this->isEnum() = false; }

    virtual void saveBinary(std::ofstream& i_oStream)
    {
        int size = (int)this->label().size();
        i_oStream.write((char*)& size,4);
		wstring sep(L"|");
        for (auto itr = this->label().begin(); itr != this->label().end(); itr++)
        {
            int num = itr->second;
            i_oStream.write((char*)& num,4);
            string str;
#ifdef MSVC
            locale loc = std::locale();
#else
            locale loc = std::locale("ru_RU.UTF-8");
#endif
            Tools::ConvertWstringToUTF8( Tools::Concatenate(itr->first,sep) , str , loc);
            Tools::WriteString_Binary(i_oStream,str);
        }
    }

    virtual void loadBinary(std::ifstream& i_iStream)
    {
#ifdef MSVC
    locale loc = std::locale();
#else
    locale loc = std::locale("ru_RU.UTF-8");
#endif
        int path_size;
        i_iStream.read((char*)& path_size, 4);
        for (int i = 0; i < path_size; i++)
        {
            int value;
            i_iStream.read((char*)& value, 4);
            string str;
            Tools::ReadString(i_iStream,str);
            wstring line = Tools::ConvertUTF8StringToWstring(str,loc);
            vector<wstring> path = Tools::Split(line,L"|");
            this->label()[path] = value;
        }
    }

    virtual vector<int> labelToVec()
    {
        return vector<int>();
    }
};

class PatentLabel : public HierarchicalLabel<>
{
public:
    PatentLabel() : HierarchicalLabel<>()
    {
        fromWstring = statcalc::patentLabeltoTrie;
    }
};

template<class T = vector<int> >
class VecIntLabel : public LabelBase<T>
{
public:
    VecIntLabel() : LabelBase<T>(){ this->isEnum() = false; }

    virtual void saveBinary(std::ofstream& i_oStream)
    {
        int num = this->label().size();
        i_oStream.write((char*)& num,4);
        for (size_t i = 0; i < this->label().size(); i++)
        {
            num = this->label()[i];
            i_oStream.write((char*)& num,4);
        }
    }

    virtual void loadBinary(std::ifstream& i_iStream)
    {
        int num;
        i_iStream.read((char*)& num,4);
        this->label() = vector<int>(num,0);
        for (size_t i = 0; i < this->label().size(); i++)
        {
            i_iStream.read((char*)& num,4);
            this->label()[i] = num;
        }
    }

    virtual vector<int> labelToVec()
    {
        return this->label();
    }
};

class PatentLabelVec : public VecIntLabel<>
{
public:
    PatentLabelVec() : VecIntLabel<>()
    {
        fromWstring = statcalc::patentLabeltoVec;
    }
};


}
#endif //_CATEGORY_PROCESSOR_
