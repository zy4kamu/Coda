/** 
 * @file	Document.h
 * @brief	header file of the class containing description of documents
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


#ifndef DOCUMENT
#define DOCUMENT

#include <vector>
#include <memory>

#include "Strings/StringTools.h"

using std::vector;
using std::shared_ptr;
using std::wstring;
using std::string;

namespace Tools
{

/** 
 * @class	Document
 * @brief	Base parent class for others types of documents
 */
class Document
{
public:

    const vector<wstring>& content(void) const
    {
        return m_content;
    }

    vector<wstring>& content(void)
    {
        return m_content;
    }

    wstring title(void)
    {
        return m_title;
    }

    wstring category(void)
    {
        return m_category;
    }

    virtual void load(const string& i_filename) = 0;
    virtual void save(const string& i_filename) = 0;

    /**
     * @brief	Destructor
     */
    virtual ~Document(){}

protected:
    vector<wstring> m_content;
    wstring m_title;
    wstring m_category;
};


class DocumentPlain : public Document
{
public:
    DocumentPlain(){}
    DocumentPlain(const string& i_filename);

    void load(const string& i_filename);
    void save(const string& i_filename);
};


class DocumentExtended : public Document
{
public:
    DocumentExtended(){}
    DocumentExtended(const string& i_filename);

    wstring url(void)
    {
        return m_url;
    }

    wstring subcategory(void)
    {
        return m_subcategory;
    }

    wstring time(void)
    {
        return m_time;
    }

    wstring description(void)
    {
        return m_description;
    }


    void load(const string& i_filename);
    void save(const string& i_filename);

private:
    wstring m_subcategory;
    wstring m_time;
    wstring m_url;
    wstring m_description;
};


class DocumentCreator{
public:
    virtual shared_ptr<Document> loadDocument(const string& i_filename) = 0;
    virtual shared_ptr<Document> createDocument() = 0;
};

class DocumentPlainCreator : public DocumentCreator
{
public:
    virtual shared_ptr<Document> loadDocument(const string& i_filename);
    virtual shared_ptr<Document> createDocument();
};

class DocumentExtendedCreator : public DocumentCreator
{
public:
    virtual shared_ptr<Document> loadDocument(const string& i_filename);
    virtual shared_ptr<Document> createDocument();
};


}

#endif // DOCUMENT

