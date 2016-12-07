/*! @file sqlite_helper.h
 *  @section Copyright
 *  Object Retrieval Engine
 *  @brief definition of SQLiteHelper
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

#pragma warning(disable: 4996)

#ifndef SQLITE_HELPER_H
#define SQLITE_HELPER_H

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <stddef.h>
#include "sqlite3_wrapper.h"
#include "sqlite_string_processor_base.h"

/*! @class List
 *  @brief adapter for std::list. It need to use the container such as template
 *  argument of SQLColumnDataQueryCollections
 */
template<class T>
struct List: public std::list<T>{
    List(): std::list<T>(){}
    List(const List& other): std::list<T>(other){}
    template<class InputIterator>
    List(InputIterator i_begin, InputIterator i_end)
    : std::list<T>(i_begin, i_end){}
};

/// @defgroup Group function for deMarshal data from DB into ISQLColumnDataQuery
/// @{
std::shared_ptr<ISQLColumnDataQuery> deMarshalColumnDataQuery(int i_columnIterator, const std::string& i_name,
        const std::string& i_type, sqlite3_stmt* i_statment );

inline std::shared_ptr<ISQLColumnDataQuery> deMarshalColumnDataQuery(int i_columnIterator, const std::string& i_name, sqlite3_stmt* i_statment ) {
    const char *columnType = sqlite3_column_decltype(i_statment, i_columnIterator);
    std::string typeName;
    if (columnType != NULL) {
        std::transform(columnType, columnType + strlen(columnType), std::back_inserter(typeName), ::toupper);
    }
    else {
        typeName = SQLiteStringProcessorBase::TEXT;
    }
    return deMarshalColumnDataQuery(i_columnIterator, i_name, typeName, i_statment);
}

inline std::shared_ptr<ISQLColumnDataQuery> deMarshalColumnDataQuery(int i_columnIterator, sqlite3_stmt* i_statment ){
    std::string columnName(sqlite3_column_name(i_statment, i_columnIterator));
    return deMarshalColumnDataQuery(i_columnIterator, columnName, i_statment);
}
/// @}

/*! @class Iterator
 *  @brief iterator for SQLiteHelper
 *  @details the class encapsulate sqlite3 pure C API.
 */
class Iterator: public std::output_iterator_tag {
public:
    typedef SQLColumnDataQueryCollections<List> reference_type;
    typedef SQLQueryCollections<std::list<std::shared_ptr<ISQLQuery> > > QueryCollections;
    Iterator(const Iterator& i_other)
    : m_database(i_other.m_database)
    , m_strQuery(NULL)
    , m_pzTail(NULL)
    , m_oldPzTail(NULL)
    , m_statment(NULL)
    //, m_rezData(i_other.m_rezData)
    , m_lastRet(0) {
        if (i_other.m_oldPzTail != NULL) {
            if (*(i_other.m_oldPzTail) != '\0') {
                int queriBuffereLength = strlen(i_other.m_oldPzTail) + 1;
                m_strQuery = new char[queriBuffereLength];
                strcpy(m_strQuery, i_other.m_oldPzTail);
                m_pzTail = m_strQuery;
                initial();
            }
        }
    }

    Iterator(const std::string& i_databaseName, const QueryCollections& i_query)
    : m_database(SQLiteDataBaseWrapper(i_databaseName))
    , m_strQuery(NULL)
    , m_pzTail(NULL)
    , m_oldPzTail(NULL)
    , m_statment(NULL)
    , m_lastRet(0) {
        if(!i_query.query().empty()) {
            //std::string tmp = "PRAGMA foreign_keys = ON; BEGIN TRANSACTION; " + i_query.query() + "; COMMIT;";
            std::string tmp = "PRAGMA foreign_keys = ON;" + i_query.query() + ";";
            m_strQuery = new char[tmp.size() + 1];
            strcpy(m_strQuery, tmp.c_str());
            m_pzTail = m_strQuery;
            initial();
        }
    }

    virtual ~Iterator() {
        flushQuery();
    }

    Iterator& operator = (const Iterator& other);

    bool operator == (const Iterator& i_other) const ;

    bool operator != (const Iterator& i_other) const {
        return !(*this == i_other);
    }

    Iterator& operator ++ () {
        initial();
        //next();
        return *this;
    }

    Iterator operator ++ (int) {
        Iterator tmp(*this);
        initial();
        //next();
        return tmp;
    }

    reference_type operator * () {
		if(m_rezData != std::shared_ptr<reference_type>()){
            return *m_rezData;
        }
        return reference_type();
    }

    reference_type operator -> () {
        return this->operator *();
    }

private:
    /// @brief process current statment
    void next();
    /// @brief get next statment
    void initial();
    /// @brief change sate of the iterator to end iterator
    void flushQuery();
    /// @brief read row
    void readData();

private:
    SQLiteDataBaseWrapper m_database;
    char* m_strQuery;
    char* m_pzTail;
    char* m_oldPzTail;
    sqlite3_stmt* m_statment;
    /// @brief row which is read from DB
    /// @details the container is empty only if the iterator don't have any query
    /// which DB can returned something
	std::shared_ptr<reference_type> m_rezData;
    template<class T>
    struct List: public std::list<T> {};
    int m_lastRet;
};

/*! @class SQLiteHelper
 *  @brief collect queries for DB and drive transfer the queries into DB.
 */
class SQLiteHelper{

public:
    typedef Iterator iterator;

    /*! @brief constructor
     * i_databaseName in table name
     * i_tableCreator in query for create the table
     */
    SQLiteHelper(const std::string& i_databaseName,
            const ISQLQuery& i_tableCreator)
    : m_dataBase(i_databaseName)
    , m_tableCreator(makeSharedPtr(i_tableCreator))
    , m_queryCollection(std::list<std::shared_ptr<ISQLQuery> >(), SQLiteStringProcessorBase::SEMICOLON)
    , m_emptyQueryCollection(std::list<std::shared_ptr<ISQLQuery> >(1, std::shared_ptr<ISQLQuery>(new SQLEmptyQuery()))){
        m_queryCollection.push_back(std::shared_ptr<ISQLQuery>(m_tableCreator->clone()));
    }

    /*! @brief destructor
     *  @details commit all queries which haven't been sent to DB yet
     */
    ~SQLiteHelper(){
        if(m_queryCollection.empty() == false){
            execute();
        }
    }

    /*! @brief register SQL query
     *  @brief i_query in query which shell be send to DB
     */
    void addQuery(const ISQLQuery& i_query){
        m_queryCollection.push_back(std::shared_ptr<ISQLQuery>(i_query.clone()));
    }

    /// @brief rest all registered queries which haven't been sent to DB yet
    const void reset(){
        m_queryCollection.clear();
        m_queryCollection.push_back(std::shared_ptr<ISQLQuery>(m_tableCreator->clone()));
    }

    /// @brief general query
    /// @return a string with all queries which haven't been sent to DB yet
    std::string query(){
        return m_queryCollection.query();
    }

    /*! @brief begin
     * @return iterator to begin
     * @note now registered queries will be sent to DB while DB doesn't return something
     */
    iterator begin(){
        if(!m_queryCollection.empty()){
            Iterator flushIterator = Iterator(m_dataBase, m_queryCollection);
            m_queryCollection.clear();
            return flushIterator;
        }
        return end();
    }

    iterator end(){
        return Iterator(m_dataBase, m_emptyQueryCollection);
    }

    /// @brief send all registered queries which haven't been sent to DB yet
    void execute(){
        Iterator flushIterator = begin();
        while(flushIterator != end()){
            ++flushIterator;
        }
    }

    /// @brief execute all added queries without returning results
    bool isEmpty() {
        return m_queryCollection.empty();
    }

private:
    std::string m_dataBase;
    std::shared_ptr<ISQLQuery> m_tableCreator;
    Iterator::QueryCollections m_queryCollection;
    Iterator::QueryCollections m_emptyQueryCollection;
};

#endif //SQLITE_HELPER_H
