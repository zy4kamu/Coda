/** 
 * @file	Document.cpp
 * @brief	source file of the class containing descriptions of documents
 */

#include "Document.h"
#include "../Locale/LocaleSettings.hpp"
#include "fstream"
#include "iostream"

using std::wstring;
using std::string;
using std::wcout;
using std::wcin;
using std::wcerr;
using std::endl;
using std::shared_ptr;

namespace Tools
{
    DocumentPlain::DocumentPlain(const string& i_filename)
    {
        load(i_filename);
    }

    void DocumentPlain::load(const string &i_filename)
    {
        std::wifstream iStream(i_filename);
		Tools::SetLocale(iStream);
        if( !iStream )
        {
            wcerr<<L"Can\'t open file"<<endl;
            return;
        }
        wstring line;
		bool checked = false;
        while( std::getline(iStream, line) )
        {
			if (!checked)
			{
				if (line[0] == L'\uFEFF')
					line = wstring(line.begin() + 1, line.end());
				checked = true;
			}
            line = Tools::Trim(line);
            if (!line.empty())
                m_content.push_back(line);
        }
        iStream.close();
    }

    void DocumentPlain::save(const string &i_filename)
    {
        std::wofstream oStream(i_filename);
		Tools::SetLocale(oStream);
        if( !oStream )
        {
            wcerr<<L"Can\'t open file"<<endl;
            return;
        }
        for (size_t i = 0; i < m_content.size();i++)
            oStream<<m_content[i]<<std::endl;
        oStream.close();
    }

    DocumentExtended::DocumentExtended(const string& i_filename)
    {
        load(i_filename);
    }

    void DocumentExtended::load(const string &i_filename)
    {
        std::wifstream iStream(i_filename);

		Tools::SetLocale(iStream);
        if( !iStream )
        {
            wcerr<<L"Can\'t open file"<<endl;
            return;
        }
        wstring line;
        bool flag = false;
		bool checked = false;
        wstring *ptr = NULL;
        while( std::getline(iStream, line) )
        {
			if (!checked)
			{
				if (line[0] == L'\uFEFF')
					line = wstring(line.begin() + 1, line.end());
				checked = true;
			}
			line = Tools::Trim(line);
            if (line == L"!!!TITLE!!!")
            {
                ptr = &m_title;
                flag = true;
                continue;
            }
            else if (line == L"!!!TIME!!!")
            {
                ptr = &m_time;
                flag = true;
                continue;
            }
            else if (line == L"!!!URL!!!")
            {
                ptr = &m_url;
                flag = true;
                continue;
            }
            else if (line == L"!!!DESCRIPTION!!!")
            {
                ptr = &m_description;
                flag = true;
                continue;
            }
            else if (line == L"!!!CATEGORY!!!")
            {
                ptr = &m_category;
                flag = true;
                continue;
            }
            else if (line == L"!!!SUBCATEGORY!!!")
            {
                ptr = &m_subcategory;
                flag = true;
                continue;
            }
            else if (line == L"!!!TEXT!!!")
            {
                flag = false;
                continue;
            }

            if (flag)
                *ptr = line;
            else
            {
                if (!line.empty())
                    m_content.push_back(line);
            }
        }
        iStream.close();
    }

    void DocumentExtended::save(const string &i_filename)
    {
        std::wofstream oStream(i_filename);
		Tools::SetLocale(oStream);
        if( !oStream )
        {
            wcerr<<L"Can\'t open file"<<endl;
            return;
        }
        oStream<<L"!!!TITLE!!!"<<std::endl;
        oStream<<m_title<<std::endl;
        oStream<<L"!!!URL!!!"<<std::endl;
        oStream<<m_url<<std::endl;
        oStream<<L"!!!TIME!!!"<<std::endl;
        oStream<<m_time<<std::endl;
        oStream<<L"!!!CATEGORY!!!"<<std::endl;
        oStream<<m_category<<std::endl;
        oStream<<L"!!!SUBCATEGORY!!!"<<std::endl;
        oStream<<m_subcategory<<std::endl;
        oStream<<L"!!!DESCRIPTION!!!"<<std::endl;
        oStream<<m_description<<std::endl;
        oStream<<L"!!!TEXT!!!"<<std::endl;
        for (size_t i = 0; i < m_content.size();i++)
            oStream<<m_content[i]<<std::endl;
        oStream.close();
    }

    shared_ptr<Document> DocumentPlainCreator::loadDocument(const string& i_filename)
    {
        return shared_ptr<DocumentPlain>(new DocumentPlain(i_filename));
    }

    shared_ptr<Document> DocumentPlainCreator::createDocument()
    {
        return shared_ptr<DocumentPlain>(new DocumentPlain);
    }

    shared_ptr<Document> DocumentExtendedCreator::loadDocument(const string& i_filename)
    {
        return shared_ptr<DocumentExtended>(new DocumentExtended(i_filename));
    }

    shared_ptr<Document> DocumentExtendedCreator::createDocument()
    {
        return shared_ptr<DocumentExtended>(new DocumentExtended);
    }

}
