/*! @file sqlite_string_processor_base.h
 *  @section Copyright
 *  Object Retrieval Engine
 *  @brief definition of compositor for SQLite queries generation
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


#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <list>
#include <vector>
#include <memory>
#include <string>
#include <cstring>
#include <string.h>

#include "loki/TypeManip.h"
#include "loki/TypeTraits.h"

//#include "sqlite/sqlite3.h"
#include "sqlite3_wrapper.h"
#include "sqlite_exceptions.h"

#ifndef SQLITE_STRING_PROCESSOR_BASE_H
#define SQLITE_STRING_PROCESSOR_BASE_H

/// @defgroup SQLiteStringProcesor Generator of SQLite Query
/// @{


/*! @class AddBlockToString
 *  @brief helper to add a block into string
 */
class AddBlockToString{
    std::string& m_string;
    std::string m_blockClose;
public:
    /*! @brief constructor
    *   @param i_blockOpen in symbol which opens the block
    *   @param i_blockClose in symbol which closes the block
    *   @param i_string in a string which will be increased the block
     */
    AddBlockToString(const std::string& i_blockOpen, const std::string& i_blockClose, std::string& i_string)
    : m_string(i_string)
    , m_blockClose(i_blockClose){
        m_string += i_blockOpen;
    }
    ~AddBlockToString(){
        m_string += m_blockClose;
    }
};

/*! @fn inline std::string makeBlock(const std::string& i_dst,
            const std::string& i_blockOpen,
            const std::string& i_blockClose)
 *  @brief wrap a string into block symbols
 *  @param i_dst in a string which will be wrapped
 *  @param i_blockOpen in symbol which opens the block
 *  @param i_blockClose in symbol which closes the block
 *  @return wrapped string.
 */
inline std::string makeBlock(const std::string& i_dst,
        const std::string& i_blockOpen, const std::string& i_blockClose){
    if(i_dst == ""){
        return "";
    }
    std::string rez;
    {
        AddBlockToString block(i_blockOpen, i_blockClose, rez);
        rez += i_dst;
    }
    return rez;
}

/*! @fn inline std::string makeBrecetBlock(const std::string& i_dst)
 *  @brief wrap a string into brecets
 *  @param i_dst in a string which will be wrapped
 *  @return wrapped string.
 */
inline std::string makeBrecetBlock(const std::string& i_dst){
    return makeBlock(i_dst, "(", ")");
}

/*! @fn inline std::string makeSquareBrecetBlock(const std::string& i_dst)
 *  @brief wrap a string into square brecets
 *  @param i_dst in a string which will be wrapped
 *  @return wrapped string.
 */
inline std::string makeSquareBrecetBlock(const std::string& i_dst){
    return makeBlock(i_dst, "[", "]");
}

/*! @fn inline std::string makeSpaceBlock(const std::string& i_dst)
 *  @brief wrap a string into space
 *  @param i_dst in a string which will be wrapped
 *  @return wrapped string.
 */
inline std::string makeSpaceBlock(const std::string& i_dst){
    return makeBlock(i_dst, " ", " ");
}

/// @defgroup ToString Functions get string for different data
/// @ingroup SQLiteStringProcesor
/// @{
/*! @fn template<class T>
        static std::string toString(const T& i_data)
 *  @brief definition template to get a string serialization from a data
 *  @param i_data in a data
 *  @return string serialization
 */
template<class T>
static std::string toString(const T& i_data);
/// @}
/*! @class SQLiteStringProcessorBase
 *  @brief contains string constant and string process function to form SQLite queries
 */
class SQLiteStringProcessorBase{
    /*! @brief see definition of toString before
     *  @note friend definition to use protected helper function
     */
    template<class T>
    friend
    std::string toString(const T& i_data);


public:
    /// @defgroup SQLiteStringConstatnt SQLite string constants
    /// @ingroup SQLiteStringProcesor
    /// @{
    static const std::string CREATE_TABLE;
    static const std::string CREATE;
    static const std::string ALERT_TABLE;
    static const std::string INSERT;
    static const std::string SELECT;
    static const std::string UPDATE;
    static const std::string FROM;
    static const std::string WHERE;
    static const std::string GROUP_BY;
    static const std::string HAVING;
    static const std::string ORDER_BY;
    static const std::string LIMIT;
    static const std::string OFFSET;
    static const std::string SET;
    static const std::string DELETE_;
    static const std::string DROP_TABLE;
    static const std::string DROP;
    static const std::string JOIN;
    static const std::string USING;
    static const std::string INDEX;

    static const std::string IF_NOT_EXISTS;
    static const std::string DEFAULT;
    static const std::string VALUES;
    static const std::string AUTOINCREMENT;
    static const std::string UNIQUE;
    static const std::string PRIMARY_KEY;
    static const std::string FOREIGN_KEY;
    static const std::string REFERENCES;

    static const std::string INTO;
    static const std::string OR;
    static const std::string ROLLBACK;
    static const std::string ABORT;
    static const std::string REPLACE;
    static const std::string FAIL;
    static const std::string _IGNORE;

    static const std::string BLOB;
    static const std::string TEXT;
    static const std::string INTEGER;
    static const std::string REAL;

    static const std::string SPACE;
    static const std::string COMMA;
    static const std::string SEMICOLON;
    static const std::string BRECET_OPEN;

    static const std::string CROSS;
    static const std::string INNER;
    static const std::string ON;
    static const std::string NATURAL;
    static const std::string LEFT;
    static const std::string RIGHT;
    /// @}

    /// @addtogroup ToString
    /// @{
    /*! @brief get string type name in SQLite format
     *  @param i_data in a data which type is asked
     *  @return string name of type in SQLite format
     *  @note any type which couldn't resolved in SQLite type will be typed as BLOB
     */
    template<class T>
    static std::string getSQLiteTypeDataName(const T& i_data){
        return SQLiteStringProcessorBase::getSQLiteTypeDataName(
                Loki::Int2Type<Loki::TypeTraits<T>::isFundamental >(), i_data);
    }


    /*! @brief get string with blob data in Hex format
     *  @param i_data in a pointer to blob data
     *  @param i_size in size of the blob
     *  @return a string which contains hex format of the blob
     *  @note we aren't allowed to work with isn't alignment data
     */
    static std::string Hex2String(const char* i_data, size_t i_size){
        //if(i_size  % sizeof(int) == 0){
        return Hex2String(Loki::Int2Type<1>(), reinterpret_cast<const unsigned char*>(i_data),
                i_size);
        /*}
        assert(0);
        return std::string();*/
    }

    /*! @brief get pointer to blob data  from string in Hex format of the data
     *  @param i_data in a pointer the string
     *  @param i_size in size of the string
     *  @return a pair there is first is pointer to the blob
     *          second is a size of the blob
     */
    static std::pair<char*, size_t> HexString2Blob(const char* i_data, size_t i_size){
        //if(i_size  % sizeof(int) == 0){
            char* rez = new char[i_size >> 1];
            char data[] = "0xff";
            char* dstIterator = rez;
            const char* srcIterator = i_data;
            for(unsigned int i = 0; i < (i_size  >> 1) ; ++i){
                memcpy(data + 2, srcIterator, sizeof(char) << 1);
                int transformedData =  strtol(data, NULL, 16);
                memcpy(dstIterator, &transformedData, sizeof(char));
                dstIterator += sizeof(char);
                srcIterator += sizeof(char) << 1;
            }
            return std::make_pair(rez, i_size >> 1);
        //}
        //assert(0);
        //return std::make_pair<char *, size_t>(NULL, 0);
    }
    /// @}
private:
    /// @defgroup SQLiteStringTYPE The functions get SQLite type name
    /// @ingroup SQLiteStringProcesor
    /// @{
    static std::string getSQLiteTypeDataName(Loki::Int2Type<0> isFundamental, const std::string&){
        return TEXT;
    }
    static std::string getSQLiteTypeDataName(Loki::Int2Type<1> isFundamental, const char*){
        return TEXT;
    }
    static std::string getSQLiteTypeDataName(Loki::Int2Type<1> isFundamental, const int&){
        return INTEGER;
    }
    static std::string getSQLiteTypeDataName(Loki::Int2Type<1> isFundamental, const long&){
        return INTEGER;
    }
    static std::string getSQLiteTypeDataName(Loki::Int2Type<1> isFundamental, const long long&){
        return INTEGER;
    }
    static std::string getSQLiteTypeDataName(Loki::Int2Type<1> isFundamental, const double&){
        return REAL;
    }
    template<class T>
    static std::string getSQLiteTypeDataName(Loki::Int2Type<0> noFundamental, const T&){
        return BLOB;
    }
    /// @}

    /// @addtogroup ToString
    /// @{
     static std::string toString(Loki::Int2Type<0> isFundamental, const std::string& i_data){
        return i_data;
    }

    static std::string toString(Loki::Int2Type<1> isFundamental, const char* i_data){
        return std::string(i_data);
    }

    static std::string toString(Loki::Int2Type<1> isFundamental, const char& i_data){
		std::ostringstream buff;
		buff.setf(std::ios_base::dec, std::ios_base::basefield);
        buff<<(int)i_data;
        return buff.str();
    }

    template<typename Fundamental>
    static std::string toString(Loki::Int2Type<1> isFundamental, const Fundamental& i_data){
        std::ostringstream buff;
        buff.setf(std::ios_base::dec, std::ios_base::basefield);
        buff.setf(std::ios_base::fixed, std::ios_base::floatfield);
        buff.precision(6);

        buff<<i_data;
        return buff.str();
    }

    template<class T>
    static std::string toString(Loki::Int2Type<0> noFundamental, const T& i_data){
         //std::string blob;
        const unsigned char* data = reinterpret_cast<const unsigned char*>(&i_data);
        return makeBlock(
                Hex2String(Loki::Int2Type<sizeof(T) % sizeof(*data) == 0>(), data, sizeof(T) / sizeof(*data))
                , "\"", "\"");
    }

    static std::string Hex2String(const char& i_data){
        std::ostringstream buff;
        buff.setf(std::ios_base::hex, std::ios_base::basefield);
        buff.width(8);
        buff.fill('0');
        buff<<(int)i_data;
        std::string rez(buff.str());
        return rez.substr(6, 2);
    }
    /*! @brief there is a part checker of alignment data
     *  @details the function is call if if data is alignment to 32
     *  @param i_data in a pointer the string
     *  @param i_size in size of the string
     *  @return a pair there is first is pointer to the blob
     *          second is a size of the blob
     */
    static std::string Hex2String(Loki::Int2Type<1>, const unsigned char* i_data, size_t i_size){
        std::ostringstream buff;
        for(unsigned int i = 0; i != i_size; ++i){
            std::string data = Hex2String(*i_data);
            buff << data;
            i_data++;
        }
        return buff.str();
    }

    /*! @brief there is a part checker of alignment data
     *  @details if some code try to call the function a compiler return a error
     *  because the function isn't implemented. But the function is tried to call
     *  only if data isn't alignment to 32
     */
    static std::string Hex2String(Loki::Int2Type<0>, const unsigned int* i_data);
    /// @}
};
/// @addtogroup ToString
/// @{
/*! @fn template<class T>
            static T fromString(const std::string& i_data)
 *  @brief template to serialization a data from string
 *  @param i_data in a data
 *  @return string serialization
 */
template<class T>
static T fromString(const std::string& i_data){
	if (i_data.empty()) {
		return 0;
	}
    std::istringstream istream(i_data);
    T rez;
    istream >> rez;
    return rez;
}
/*! @fn template<class T>
            static T toString(const std::string& i_data)
 *  @brief template to get a string serialization from a data
 *  @param i_data in a data
 *  @return string serialization
 */
template<class T>
static std::string toString(const T& i_data){
    return SQLiteStringProcessorBase::toString(Loki::Int2Type<Loki::TypeTraits<T>::isFundamental >(), i_data);
}
/// @}

/// @defgroup SQLiteQueryGenerato Generator of SQLite queries
/// @ingroup SQLiteStringProcesor
/// @{
/*! @interface ISQLQuery
 *  @brief base interface class for all SQL queries classes
 */
class ISQLQuery {
public:
    virtual ~ISQLQuery() {}
    /// @brief query
    /// @return SQL query for the current class
    virtual std::string query() const =0;
    /// @brief virtual constructor
    virtual ISQLQuery* clone() const =0;
};

/// @brief operator equal for queries
inline bool operator == (const ISQLQuery& i_query1, const ISQLQuery& i_query2){
    return i_query1.query() == i_query2.query();
}

/// @brief factory to create shared pointer from reference to query
inline std::shared_ptr<ISQLQuery> makeSharedPtr(const ISQLQuery& i_query){
    return std::shared_ptr<ISQLQuery>(i_query.clone());
}

/*! @class SQLEmptyQuery
 *  @brief implement empty sql query
 */
class SQLEmptyQuery: virtual public ISQLQuery {
public:
    virtual std::string query() const {
        return "";
    }
    virtual ISQLQuery* clone() const {
        return new SQLEmptyQuery();
    }
};

/*! @interface ISQLNamedQuery
 *  increase of ISQLQuery to save and get named queries
 */
class ISQLStringExpressionQuery: virtual public ISQLQuery {
public:
    virtual const std::string& name() const = 0;
};

/*! @class SQLNamedQuery
 *  @brief the class encapsulated string field
 *  this is used to set column name
 */
class SQLStringExpressionQuery: virtual public ISQLStringExpressionQuery {
    std::string m_columnName;
public:
    SQLStringExpressionQuery(const SQLStringExpressionQuery& i_other)
    : m_columnName(i_other.m_columnName) {}

    SQLStringExpressionQuery(const std::string& i_columnName = std::string(""))
    : m_columnName(i_columnName) {}

    virtual std::string query() const {
        return /*std::move*/(m_columnName);
    }

    virtual ISQLQuery* clone() const {
        return new SQLStringExpressionQuery(*this);
    }

    virtual const std::string& name() const {
        return m_columnName;
    }
protected:
    void set(const std::string& i_data) {
        m_columnName = i_data;
    }
};

/*! @class template<T> SQLLiteralQuery
 *  @brief the class encapsulated any data
 *  this is used to save data between SQL DB and Application.
 *  This is base to create tuple of data.
 */
template<typename T>
class SQLLiteralQuery: virtual public ISQLQuery, public Loki::Type2Type<T> {
    T m_data;
public:
    SQLLiteralQuery(const SQLLiteralQuery& i_other)
    : m_data(i_other.m_data) {}

    SQLLiteralQuery(const T& i_data = T())
    : m_data(i_data) {}

    virtual std::string query() const {
        return /*std::move*/(toString(m_data));
    }

    virtual ISQLQuery* clone() const {
        return new SQLLiteralQuery<T>(*this);
    }

    /// @brief getter for value
    const T& value() const {
        return m_data;
    }

    /// @brief setter for value
    void set(const SQLLiteralQuery<T>& i_data) {
        m_data = i_data.value();
    }
};

/*! @class SQLLiteralQuery<std::string>
 *  @brief specialize of template<T> SQLLiteralQuery for string type
 */
template<>
class SQLLiteralQuery<std::string>: virtual public ISQLQuery, public Loki::Type2Type<std::string> {
    std::string m_data;
public:
    SQLLiteralQuery(const SQLLiteralQuery& i_other)
    : m_data(i_other.m_data) {}

    SQLLiteralQuery(const std::string& i_data = "")
    : m_data(i_data) {}

    virtual std::string query() const {
        return /*std::move*/("\"" + (m_data) + "\"");
    }

    virtual ISQLQuery* clone() const {
        return new SQLLiteralQuery<std::string>(*this);
    }

    const std::string& value() const {
        return m_data;
    }

    void set(const SQLLiteralQuery<std::string>& i_data) {
        m_data = i_data.value();
    }
};

/*! @class template<T> SQLTypeDataQuery
 *  @brief the class encapsulated type of any data
 *  this is used to add SQL type into SQL query.
 */
template<typename T>
class SQLTypeDataQuery: virtual public ISQLQuery, public Loki::Type2Type<T> {
public:
    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::getSQLiteTypeDataName(T()));
    }

    virtual ISQLQuery* clone() const {
        return new SQLTypeDataQuery(*this);
    }
};

/*! @class template<T> SQLDefaultQuery
 *  @brief the class add default section into SQL CREATE query.
 */
template<typename T>
class SQLDefaultQuery: public SQLLiteralQuery<T> {
public:
    SQLDefaultQuery(const SQLDefaultQuery& i_other)
    : SQLLiteralQuery<T>(i_other) {}

    SQLDefaultQuery(const SQLLiteralQuery<T>& i_other)
    : SQLLiteralQuery<T>(i_other) {}

    SQLDefaultQuery(const T& i_data = T())
    : SQLLiteralQuery<T>(i_data) {}

    virtual std::string query() const {
        return /*std::move*/( SQLiteStringProcessorBase::DEFAULT
                + SQLiteStringProcessorBase::SPACE
                + SQLLiteralQuery<T>::query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLDefaultQuery<T>(*this);
    }
};

/*! @class template<Container> SQLQueryCollections
 *  @brief this is wrapper under a collection of queries to generate General query.
 */
template<class Container>
class SQLQueryCollections: virtual public ISQLQuery, public Container, public Loki::Type2Type<Container> {
    std::string m_separatorSimbol;
public:
    SQLQueryCollections(const SQLQueryCollections& i_other)
    : Container(i_other)
    , m_separatorSimbol(i_other.m_separatorSimbol) {}

    SQLQueryCollections()
    : m_separatorSimbol(SQLiteStringProcessorBase::COMMA) {}

    SQLQueryCollections(const Container& data,
		const std::string& i_separatorSimbol = std::string(SQLiteStringProcessorBase::COMMA))
    : Container(data)
    , m_separatorSimbol(i_separatorSimbol) {}

    template<class Iterator>
    SQLQueryCollections(Iterator i_begin, Iterator i_end,
        const std::string& i_separatorSimbol =
                SQLiteStringProcessorBase::COMMA)
    : Container(i_begin, i_end)
    , m_separatorSimbol(i_separatorSimbol) {}

    virtual std::string query() const {
        std::string rezQuery;
        // TODO moiseenko.a Shell definition a type for '*'.
        if (this->empty() == true) {
            return "*";
        }
        for(typename Container::const_iterator x = this->begin(); x != this->end(); ++x) {
            rezQuery += getQuery(*x).query() + m_separatorSimbol;
        }
        rezQuery.erase(rezQuery.size() - m_separatorSimbol.size(), m_separatorSimbol.size());
        return rezQuery;
    }

    virtual ISQLQuery* clone() const {
        return new SQLQueryCollections(*this);
    }

    /// @brief set a symbol which is printed between different queries
    void setSeparatorSimbol(const std::string& i_separatorSimbol) {
        m_separatorSimbol = i_separatorSimbol;
    }

    /// @defgroup group of function to get a reference to query from different
    /// variant of collected data
    /// @{
    template<typename Query>
    static ISQLQuery& getQuery(std::shared_ptr<Query> i_query) {
        return *i_query;
    }

    static ISQLQuery& getQuery(ISQLQuery& i_query) {
        return i_query;
    }

    template<typename KeyType, typename Query>
    static ISQLQuery& getQuery(std::pair<KeyType, Query>& i_query) {
       return i_query.second;
    }

    template<typename KeyType, typename Query>
    static ISQLQuery& getQuery(std::pair<KeyType, std::shared_ptr<Query> >& i_query) {
        return *(i_query.second);
    }

    static const ISQLQuery& getQuery(const ISQLQuery& i_query) {
        return i_query;
    }

    template<typename KeyType, typename Query>
    static const ISQLQuery& getQuery(const std::pair<KeyType, Query>& i_query) {
       return i_query.second;
    }

    template<typename KeyType, typename Query>
    static const ISQLQuery& getQuery(const std::pair<KeyType, std::shared_ptr<Query> >& i_query) {
        return *(i_query.second);
    }
    /// @}
};

/*! @fn template<class Base, class Filtered>
        void FilterArg(std::vector<SHARED_PTR<Base> >& o_vector, const Base& arg)
 *  @brief the function collect element into container only if type of the
 *  element can be casted to Filtered type.
 *  @param Base in type of checked elements
 *  @param Filtered in type which data shell be collected
 *  @param o_vector out a reference to vector where collected data
 *  @param arg in candidate to collect
 */
template<class Base, class Filtered>
void FilterArg(std::vector<std::shared_ptr<Base> >& o_vector, const Base& arg) {
    if(NULL == dynamic_cast<const Filtered*>(&arg)) {
        o_vector.push_back(makeSharedPtr(arg));
    }
}

/*! @interface ISQLColumnDataQuery
 *  @brief increase of ISQLNamedQuery to save and get named column and data of the column
 */
class ISQLColumnDataQuery: virtual public ISQLStringExpressionQuery {
public:
    /*! @brief getter of value for the column
     *  @return shared pointer to ISQLQuery which shell be allowed to cast to SQLliteral
     */
    virtual std::shared_ptr<ISQLQuery> value() const =0;
    /*! @brief setter of value for the column
     *  @param i_data a query which contain data
     */
    virtual void set(const ISQLQuery& i_data) =0;
};


/*! @class SQLColumnDataQuery
 *  @brief implement of ISQLColumnDataQuery
 *  the class is used to save data of a SQL DB row field
 */
template<typename T>
class SQLColumnDataQuery: virtual  public ISQLColumnDataQuery, public Loki::Type2Type<T>, public SQLStringExpressionQuery {
    std::shared_ptr<ISQLQuery> m_data;
public:
    SQLColumnDataQuery(const SQLColumnDataQuery<T>& i_other)
    : SQLStringExpressionQuery(i_other)
    , m_data(i_other.m_data) {}

    SQLColumnDataQuery(const SQLStringExpressionQuery& i_columnName, const T& i_data = T())
    : SQLStringExpressionQuery(i_columnName)
    , m_data(std::shared_ptr<ISQLQuery>(new SQLLiteralQuery<T>(i_data))) {}

    SQLColumnDataQuery(const std::string& i_columnName, const T& i_data = T())
    : SQLStringExpressionQuery(i_columnName)
    , m_data(std::shared_ptr<ISQLQuery>(new SQLLiteralQuery<T>(i_data))) {}

    virtual std::string query() const {
        return /*std::move*/(SQLStringExpressionQuery::query()
                + " = "
                + m_data->query());
    }

    virtual const std::string& name() const {
        return SQLStringExpressionQuery::name();
    }

    virtual std::shared_ptr<ISQLQuery> value() const {
        return this->m_data;
    }

    virtual void set(const ISQLQuery& i_data) {
        const SQLLiteralQuery<T>* literal = dynamic_cast<const SQLLiteralQuery<T>*>(&i_data);
        if (literal != NULL) {
            m_data = std::shared_ptr<ISQLQuery>(this->dataSet(literal));
            return;
        }
        const SQLColumnDataQuery<T>* columnData = dynamic_cast<const SQLColumnDataQuery<T>*>(&i_data);
        if (columnData != NULL) {
            m_data = std::shared_ptr<ISQLQuery>(this->dataSet(columnData));
            return;
        }
        throw SQLiteException(
			"set data into SQLColumnDataQuery could be only from SQLLiteralQuery<T> or SQLColumnDataQuery<T>");
    }

    virtual ISQLQuery* clone() const {
        return (new SQLColumnDataQuery(*this));
    }
private:
    SQLLiteralQuery<T>* dataSet(const SQLLiteralQuery<T>* i_data) {
        return dynamic_cast<SQLLiteralQuery<T>*>(i_data->clone());
    }

    static SQLLiteralQuery<T>*  dataSet(const SQLColumnDataQuery<T>* i_data) {
        return dynamic_cast<SQLLiteralQuery<T>*>(dynamic_cast<const SQLLiteralQuery<T>& >(*(i_data->value())).clone());
    }
};

/*! @class SQLColumnDataQueryCollections
 *  @brief implement of ISQLColumnDataQuery
 *  the class is used to save row of SQL DB table
 */
template<template<class> class Container, class ColumnDataType = std::shared_ptr<ISQLColumnDataQuery> >
class SQLColumnDataQueryCollections: virtual  public ISQLColumnDataQuery,
    public SQLQueryCollections<Container<ColumnDataType> > {
    typedef Container<ColumnDataType > ColumnData;
    mutable std::string m_genericName;

public:
    typedef ColumnDataType ValueType;
    SQLColumnDataQueryCollections(const SQLColumnDataQueryCollections& i_other)
    : SQLQueryCollections<ColumnData>(i_other) {}

    SQLColumnDataQueryCollections()
    : SQLQueryCollections<ColumnData>() {}

    SQLColumnDataQueryCollections(const ColumnData& data,
            const std::string& i_separatorSimbol =
                    SQLiteStringProcessorBase::COMMA)
    :  SQLQueryCollections<ColumnData>(data, i_separatorSimbol) {}

    template<class Iterator>
    SQLColumnDataQueryCollections(Iterator i_begin, Iterator i_end,
                const std::string& i_separatorSimbol =
                        SQLiteStringProcessorBase::COMMA)
        :  SQLQueryCollections<ColumnData>(i_begin, i_end, i_separatorSimbol) {}

    virtual std::string query() const {
        return SQLQueryCollections<ColumnData>::query();
    }

    virtual ISQLQuery* clone() const {
        return (new SQLColumnDataQueryCollections(*this));
    }

    virtual std::shared_ptr<ISQLQuery> value() const {
        SQLQueryCollections<Container<std::shared_ptr<ISQLQuery> > > rez;
        rez.setSeparatorSimbol(SQLiteStringProcessorBase::COMMA);
        for(typename ColumnData::const_iterator i = this->begin();
            i != this->end(); ++i){
            const ISQLColumnDataQuery& columnData =
                    dynamic_cast<const ISQLColumnDataQuery&>(SQLQueryCollections<ColumnData>::getQuery(*i));
            rez.push_back(columnData.value());
        }
        return makeSharedPtr(rez);
    }

    virtual void set(const ISQLQuery& i_data){
        const SQLQueryCollections<Container<std::shared_ptr<ISQLQuery> > >& data =
                dynamic_cast<const SQLQueryCollections<Container<std::shared_ptr<ISQLQuery> > >& >(i_data);
        typename ColumnData::iterator dst = this->begin();
        typename SQLQueryCollections<Container<std::shared_ptr<ISQLQuery> > >::const_iterator src = data.begin();
        while(dst != this->end() && src != data.end()){
            ISQLColumnDataQuery& columnData =
                    dynamic_cast<ISQLColumnDataQuery&>(SQLQueryCollections<ColumnData>::getQuery(*dst));
            columnData.set(SQLQueryCollections<ColumnData>::getQuery(*src));
            ++dst, ++src;
        }
    }

    virtual const std::string& name() const{
        this->m_genericName = "";
        for(typename ColumnData::const_iterator i = this->begin();
            i != this->end(); ++i){
            this->m_genericName += dynamic_cast<ISQLColumnDataQuery&>(SQLQueryCollections<ColumnData>::getQuery(*i)).name()
                + SQLiteStringProcessorBase::COMMA;
        }
        this->m_genericName.erase(this->m_genericName.size() - SQLiteStringProcessorBase::COMMA.size(), SQLiteStringProcessorBase::COMMA.size());
        return this->m_genericName;
    }
};

/*! @class SQLIndexColumnQuery
 *  @brief specialisation of SQLCreatedColumnQuery<long> to definition
 *  the primary key column
 */
class SQLIndexColumnQuery: public ISQLQuery {
public:
    SQLIndexColumnQuery(const SQLIndexColumnQuery& i_other)
    : m_collection(i_other.m_collection) {}

    SQLIndexColumnQuery(const ISQLQuery& i_columnName,
           const ISQLQuery& i_cllate = SQLEmptyQuery(),
           const ISQLQuery& i_orderType = SQLEmptyQuery()) {
        m_args.push_back(makeSharedPtr(i_columnName));
        m_args.push_back(makeSharedPtr(i_cllate));
        m_args.push_back(makeSharedPtr(i_orderType));

        m_collection =
            SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery>>>(m_args, SQLiteStringProcessorBase::SPACE);
    }

    virtual std::string query() const {
        return /*std::move*/(m_collection.query());
    }

    virtual ISQLQuery* clone() const {
        return (new SQLIndexColumnQuery(*this));
    }
protected:
    std::vector<std::shared_ptr<ISQLQuery>> m_args;
    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery>>> m_collection;

};

/*! @class SQLCreatedIndexQuery
 *  @brief specialisation of SQLCreatedColumnQuery<long> to definition
 *  the primary key column
 */
class SQLCreatedIndexQuery: public ISQLQuery {
public:
    SQLCreatedIndexQuery(const SQLCreatedIndexQuery& i_other)
    : m_columnNames(i_other.m_columnNames)
    , m_collection(i_other.m_collection) {}

    SQLCreatedIndexQuery(const ISQLQuery& i_indexName,
            const ISQLQuery& i_tableNames,
           const ISQLQuery& i_columnNames,
           const ISQLQuery& i_unique = SQLEmptyQuery(),
           const ISQLQuery& i_ifNotExists = SQLEmptyQuery())
    : m_columnNames(makeSharedPtr(i_columnNames)){
        m_args.push_back(makeSharedPtr(SQLStringExpressionQuery(SQLiteStringProcessorBase::CREATE)));
        m_args.push_back(makeSharedPtr(i_unique));
        m_args.push_back(makeSharedPtr(SQLStringExpressionQuery(SQLiteStringProcessorBase::INDEX)));
        m_args.push_back(makeSharedPtr(i_ifNotExists));
        m_args.push_back(makeSharedPtr(i_indexName));
        m_args.push_back(makeSharedPtr(SQLStringExpressionQuery(SQLiteStringProcessorBase::ON)));
        m_args.push_back(makeSharedPtr(i_tableNames));

        m_collection =
            SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > >(m_args, SQLiteStringProcessorBase::SPACE);
    }

    virtual std::string query() const {
        return /*std::move*/(m_collection.query()
                + makeBrecetBlock(m_columnNames->query()));
    }

    virtual ISQLQuery* clone() const {
        return (new SQLCreatedIndexQuery(*this));
    }

protected:
    std::shared_ptr<ISQLQuery> m_columnNames;
    std::vector<std::shared_ptr<ISQLQuery> > m_args;
    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > m_collection;
};

/*! @class SQLCreatedColumnQuery
 *  @brief implement of ISQLQuery
 *  the class is used to add a column definition into create table query
 */
template<typename T>
class SQLCreatedColumnQuery: virtual public ISQLQuery,
        protected SQLColumnDataQuery<T>,
        protected SQLTypeDataQuery<T>,
        protected SQLDefaultQuery<T> {
public:
    SQLCreatedColumnQuery(const SQLCreatedColumnQuery<T>& i_other)
    : SQLColumnDataQuery<T>(i_other)
    , SQLDefaultQuery<T>(i_other) {}

    SQLCreatedColumnQuery(const SQLColumnDataQuery<T>& i_other)
    : SQLColumnDataQuery<T>(i_other)
    , SQLDefaultQuery<T>(dynamic_cast<SQLLiteralQuery<T>& >(*i_other.value())) {}

    SQLCreatedColumnQuery(const std::string& i_columnName, const T& i_data = T())
    : SQLColumnDataQuery<T>(i_columnName, i_data)
    , SQLDefaultQuery<T>(i_data) {}

    virtual std::string query() const {
        return /*std::move*/(SQLColumnDataQuery<T>::name()
                + SQLiteStringProcessorBase::SPACE
                + SQLTypeDataQuery<T>::query()
                + SQLiteStringProcessorBase::SPACE
                + SQLDefaultQuery<T>::query()
        );
    }

    SQLColumnDataQuery<T> sqlColumnData() {
        return SQLColumnDataQuery<T>(*this);
    }

    virtual ISQLQuery* clone() const {
        return ( new SQLCreatedColumnQuery(*this));
    }
};

/*! @class SQLCreatedPrimaryKeyColumnQuery
 *  @brief specialisation of SQLCreatedColumnQuery<long> to definition
 *  the primary key column
 */
class SQLCreatedPrimaryKeyColumnQuery: public SQLCreatedColumnQuery<long> {
public:
    SQLCreatedPrimaryKeyColumnQuery(const SQLCreatedPrimaryKeyColumnQuery& i_other)
    : SQLCreatedColumnQuery<long>(i_other) {}

    SQLCreatedPrimaryKeyColumnQuery(const SQLColumnDataQuery<long>& i_other)
    : SQLCreatedColumnQuery<long>(i_other) {}

    SQLCreatedPrimaryKeyColumnQuery(const std::string& i_columnName)
    : SQLCreatedColumnQuery<long>(i_columnName) {}

    virtual std::string query() const {
        return /*std::move*/(SQLCreatedColumnQuery<long>::name()
        + SQLiteStringProcessorBase::SPACE
        + SQLTypeDataQuery<long>::query()
        + SQLiteStringProcessorBase::SPACE
        + SQLiteStringProcessorBase::PRIMARY_KEY);
    }

    virtual ISQLQuery* clone() const {
        return (new SQLCreatedPrimaryKeyColumnQuery(*this));
    }
};

/*! @class SQLCreatedUniqueColumnQuery
 *  @brief specialisation of SQLCreatedColumnQuery<T> to definition
 *  the column with unique values
 */
template<typename T>
class SQLCreatedUniqueColumnQuery: public SQLCreatedColumnQuery<T>{
public:
    SQLCreatedUniqueColumnQuery(const SQLCreatedUniqueColumnQuery& i_other)
    : SQLCreatedColumnQuery<T>(i_other){}
    SQLCreatedUniqueColumnQuery(const std::string& i_columnName)
    : SQLCreatedColumnQuery<T>(i_columnName){
    }
    SQLCreatedUniqueColumnQuery(const SQLColumnDataQuery<T>& i_other)
    : SQLCreatedColumnQuery<T>(i_other){
    }
    SQLCreatedUniqueColumnQuery(const SQLStringExpressionQuery& i_columnName, const T& i_data = T())
    : SQLCreatedColumnQuery<T>(i_columnName, i_data){
    }
    virtual std::string query() const {
        return /*std::move*/(SQLCreatedColumnQuery<T>::name()
        + SQLiteStringProcessorBase::SPACE
        + SQLTypeDataQuery<T>::query()
        + SQLiteStringProcessorBase::SPACE
        + SQLiteStringProcessorBase::UNIQUE);
    }

    virtual ISQLQuery* clone() const {
        return new SQLCreatedUniqueColumnQuery(*this);
    }
};

/*! @class SQLLimitOffsetQuery
 *  @brief implementation of ISQLQuery
 *  the class is base for classes which add limit/offset part into query
 */
class SQLLimitOffsetQuery: virtual public ISQLQuery{
    int m_limit;
public:
    SQLLimitOffsetQuery(const SQLLimitOffsetQuery& i_other)
    : m_limit(i_other.m_limit){
    }

    SQLLimitOffsetQuery(int i_limit)
    : m_limit(i_limit){
    }

    virtual std::string query() const {
        return /*std::move*/(toString(m_limit));
    }

    virtual ISQLQuery* clone() const {
        return new SQLLimitOffsetQuery(*this);
    }
};

/*! @class SQLOffsetdQuery
 *  @brief specialisation of SQLLimitOffsetQuery
 *  the class adds offset part into query
 */
class SQLOffsetdQuery: public SQLLimitOffsetQuery{
public:
    SQLOffsetdQuery(const SQLOffsetdQuery& i_other)
    : SQLLimitOffsetQuery(i_other){
    }

    SQLOffsetdQuery(int i_limit)
    : SQLLimitOffsetQuery(i_limit){
    }

    virtual std::string query() const {
        return /*std::move*/( SQLiteStringProcessorBase::OFFSET
               + SQLiteStringProcessorBase::SPACE
               + SQLLimitOffsetQuery::query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLOffsetdQuery(*this);
    }
};

/*! @class SQLOffsetdQuery
 *  @brief specialisation of SQLLimitOffsetQuery
 *  the class adds limit part into query
 */
class SQLLimitededQuery: public SQLLimitOffsetQuery{
    std::shared_ptr<ISQLQuery> m_offest;
public:
    SQLLimitededQuery(const SQLLimitededQuery& i_other)
    : SQLLimitOffsetQuery(i_other)
    , m_offest(std::shared_ptr<ISQLQuery>(i_other.m_offest->clone())){
    }

    //TODO moiseenko.a: Add static type check
    SQLLimitededQuery(int i_limit, const ISQLQuery& i_offset = SQLEmptyQuery())
    : SQLLimitOffsetQuery(i_limit)
    , m_offest(makeSharedPtr(i_offset)){
    }

    virtual std::string query() const {
        return /*std::move*/( SQLiteStringProcessorBase::LIMIT
               + makeSpaceBlock(SQLLimitOffsetQuery::query())
               + m_offest->query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLLimitededQuery(*this);
    }
};

/*! @class SQLOrderedColumnQuery
 *  @brief specialisation of SQLNamedQuery
 *  the class adds ordered path in to query
 */
class SQLOrderedColumnQuery: public SQLStringExpressionQuery{
    SQLStringExpressionQuery m_typeOredered;
public:
    SQLOrderedColumnQuery(const SQLOrderedColumnQuery& i_other)
    : SQLStringExpressionQuery(i_other)
    , m_typeOredered(i_other.m_typeOredered){
    }

    SQLOrderedColumnQuery(const SQLStringExpressionQuery& i_columnName,
            const SQLStringExpressionQuery& i_typeOredered =  SQLStringExpressionQuery("ASC"))
    : SQLStringExpressionQuery(i_columnName)
    , m_typeOredered(i_typeOredered){
    }

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::ORDER_BY
            + makeSpaceBlock(SQLStringExpressionQuery::query())
            + m_typeOredered.query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLOrderedColumnQuery(*this);
    }
};

/*! @class SQLCrossColumnQuery CROSS
 *  @brief specialisation of SQLJoinConstraintQuery
 *  the class adds join constraint path in to query
 */
class SQLJoinConstraintQuery: public SQLStringExpressionQuery{
    std::shared_ptr<ISQLQuery> m_exprecion;
public:
    SQLJoinConstraintQuery(const SQLJoinConstraintQuery& i_other)
    : SQLStringExpressionQuery(i_other)
    , m_exprecion(i_other.m_exprecion){
    }

    SQLJoinConstraintQuery(const SQLStringExpressionQuery& i_typeConstraint,
            const ISQLQuery& i_exprecion)
    : SQLStringExpressionQuery(i_typeConstraint)
    , m_exprecion(makeSharedPtr(i_exprecion)){
    }

    virtual std::string query() const {
        return /*std::move*/(SQLStringExpressionQuery::query()
            + makeSpaceBlock(m_exprecion->query()));
    }

    virtual ISQLQuery* clone() const {
        return new SQLJoinConstraintQuery(*this);
    }
};

/*! @class SQLJoinColumnQuery
 *  @brief specialisation of SQLJoinQuery
 *  the class adds join path in to query
 */
class SQLJoinQuery: public ISQLQuery{
public:
    SQLJoinQuery(const SQLJoinQuery& i_other)
    : m_collection(i_other.m_collection) {}

    SQLJoinQuery(const ISQLQuery& i_firstSingleSource,
            const SQLStringExpressionQuery& i_joinTypeOp,
            const ISQLQuery& i_secondSingleSource,
            const ISQLQuery& i_joinConstraint = SQLEmptyQuery()) {
        m_args.push_back(makeSharedPtr(i_firstSingleSource));
        m_args.push_back(makeSharedPtr(i_joinTypeOp));
        m_args.push_back(makeSharedPtr(SQLStringExpressionQuery(SQLiteStringProcessorBase::JOIN)));
        m_args.push_back(makeSharedPtr(i_secondSingleSource));
        FilterArg<ISQLQuery, SQLEmptyQuery>(m_args, i_joinConstraint);
        m_collection =
            SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > >(m_args, SQLiteStringProcessorBase::SPACE);
    }

    virtual std::string query() const {
        return /*std::move*/(m_collection.query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLJoinQuery(*this);
    }
protected:
    std::vector<std::shared_ptr<ISQLQuery> > m_args;
    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > m_collection;
};

/*! @class SQLFromQuery
 *  @brief the class adds from path in to query
 */
class SQLFromQuery: virtual public ISQLQuery {
    std::shared_ptr<ISQLQuery> m_tableNames;
public:
    SQLFromQuery(const SQLFromQuery& i_other)
    : m_tableNames(std::shared_ptr<ISQLQuery>(i_other.m_tableNames->clone())){
    }

    //TODO moiseenko.a: Add static type check
    SQLFromQuery(const ISQLQuery& i_joinSource)
    : m_tableNames(makeSharedPtr(i_joinSource)){
    }

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::FROM
                + SQLiteStringProcessorBase::SPACE
                + m_tableNames->query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLFromQuery(*this);
    }
};

/*! @class SQLFromQuery
 *  @brief the class adds from path in to query
 */
class SQLUniqueSetQuery: virtual public ISQLQuery {
    std::shared_ptr<ISQLQuery> m_columnsNames;
public:
    SQLUniqueSetQuery(const SQLUniqueSetQuery& i_other)
    : m_columnsNames(std::shared_ptr<ISQLQuery>(i_other.m_columnsNames->clone())){
    }

    //TODO moiseenko.a: Add static type check
    SQLUniqueSetQuery(const ISQLQuery& i_columnsNames)
    : m_columnsNames(makeSharedPtr(i_columnsNames)){
    }

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::UNIQUE
                + makeBrecetBlock(m_columnsNames->query()));
    }

    virtual ISQLQuery* clone() const {
        return new SQLUniqueSetQuery(*this);
    }
};

/*! @class SQLFromQuery
 *  @brief the class adds from path in to query
 */
class SQLForeignKeyQuery: virtual public ISQLQuery {
    std::shared_ptr<ISQLQuery> m_columnsNames;
    SQLStringExpressionQuery m_foreignTableName;
    std::shared_ptr<ISQLQuery> m_foreignColumnsNames;

public:
    SQLForeignKeyQuery(const SQLForeignKeyQuery& i_other)
    : m_columnsNames(std::shared_ptr<ISQLQuery>(i_other.m_columnsNames->clone()))
    , m_foreignTableName(i_other.m_foreignTableName)
    , m_foreignColumnsNames(i_other.m_foreignColumnsNames){
    }

    //TODO moiseenko.a: Add static type check
    SQLForeignKeyQuery(const ISQLQuery& i_columnsNames = SQLEmptyQuery()
            , const SQLStringExpressionQuery& i_foreignTableName = SQLStringExpressionQuery()
            , const ISQLQuery& i_foreignColumnsNames = SQLEmptyQuery())
    : m_columnsNames(makeSharedPtr(i_columnsNames))
    , m_foreignTableName(i_foreignTableName)
    , m_foreignColumnsNames(makeSharedPtr(i_foreignColumnsNames)){
    }

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::FOREIGN_KEY
                + makeBrecetBlock(m_columnsNames->query())
                + makeSpaceBlock(SQLiteStringProcessorBase::REFERENCES)
                + m_foreignTableName.query()
                + makeBrecetBlock(m_foreignColumnsNames->query())
                + " ON DELETE CASCADE"
                " ON UPDATE CASCADE");

    }

    virtual ISQLQuery* clone() const {
        return new SQLForeignKeyQuery(*this);
    }
};

/*! @class SQLPredicateAscedColumnQuery
 *  @brief the class definition predicate which can be used into where part of a query
 */
class SQLPredicateAscedColumnQuery: virtual public ISQLQuery {

public:
    struct SQLPredicateOperator {
        enum PredicateOperator{
            EQUAL,
            LESS,
            GREATER,
            LESS_EQUAL,
            GREATE_EQUAL,
            NO_EQUAL,
            IS,
            IS_NOT,
            AND,
            OR,
            IN_,
            NOT_IN,
            BETWEEN,
            LIMIT_OPER  ///< definition error expression
        };
        SQLPredicateOperator(PredicateOperator i_operator = EQUAL)
        : m_operator(i_operator){
        }
        operator int() const {
            return int(m_operator);
        }
    private:
        PredicateOperator m_operator;
    };
    /// @enum SQLOperator
    /// @brief definition compare operators
    SQLPredicateAscedColumnQuery(const SQLPredicateAscedColumnQuery& i_other)
    : m_firstArg(std::shared_ptr<ISQLQuery>(i_other.m_firstArg->clone()))
    , m_secondArg(std::shared_ptr<ISQLQuery>(i_other.m_secondArg->clone()))
    , m_operation(i_other.m_operation) {}

    //TODO moiseenko.a: Add static type check
    /*! @brief constructor
     *  @param i_firstArg in first query of predicate
     *  @param i_operation in compare operator
     *  @param i_secondArg in second query of predicate
     */
    SQLPredicateAscedColumnQuery(const ISQLQuery& i_firstArg, SQLPredicateOperator i_operation, const ISQLQuery& i_secondArg)
    : m_firstArg(makeSharedPtr(i_firstArg))
    , m_secondArg(makeSharedPtr(i_secondArg))
    , m_operation(SQLPredicateOperator(i_operation)) {
        if (int(m_operation) >= SQLPredicateOperator::LIMIT_OPER) {
            throw SQLiteException("Unknown operation");
        }
    }

    virtual std::string query() const {
        return makeBrecetBlock(
                makeBrecetBlock(m_firstArg->query())
                + makeSpaceBlock(operation())
                + makeBrecetBlock(m_secondArg->query()));
    }

    virtual ISQLQuery* clone() const {
        return new SQLPredicateAscedColumnQuery(*this);
    }

    SQLPredicateAscedColumnQuery& operator= (const SQLPredicateAscedColumnQuery& i_other) {
        m_firstArg = (std::shared_ptr<ISQLQuery>(i_other.m_firstArg->clone()));
        m_secondArg = (std::shared_ptr<ISQLQuery>(i_other.m_secondArg->clone()));
        m_operation = (i_other.m_operation);
        return *this;
    }
private:
    std::shared_ptr<ISQLQuery> m_firstArg;
    std::shared_ptr<ISQLQuery> m_secondArg;
    SQLPredicateOperator m_operation;
    static const std::string OperationCollection[SQLPredicateOperator::LIMIT_OPER];
    const std::string& operation() const {
        return OperationCollection[m_operation];
    }
};

/*! @class SQLWhereQuery
 *  @brief the class adds where in to query
 */
class SQLWhereQuery: virtual public ISQLQuery {
    std::shared_ptr<ISQLQuery> m_predicate;
public:
    SQLWhereQuery(const SQLWhereQuery& i_other)
		: m_predicate(i_other.m_predicate) {}

    SQLWhereQuery(const ISQLQuery& i_predicate)
    : m_predicate(makeSharedPtr(i_predicate)) {}

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::WHERE
                + SQLiteStringProcessorBase::SPACE
                + m_predicate->query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLWhereQuery(*this);
    }
};

/*! @class SQLWhereQuery
 *  @brief the class create query for create a table
 */
class SQLCreateTableQuery: virtual public ISQLQuery {
public:
    SQLCreateTableQuery(const SQLCreateTableQuery& i_other)
    : m_tableName(i_other.m_tableName)
    , m_tableDescriptions(std::shared_ptr<ISQLQuery>(i_other.m_tableDescriptions->clone())){}

    //TODO(moiseenko.a): Add static type check
    /*! @brief constructor
     *  @param i_tableName[i] name of the table which is packed into SQLNamedQuery obj
     *  @param i_columnsQuery[i] reference to collection of created query ojects (SQLCreatedColumnQuery<T>)
     *  @param i_tableConstrain[i] reference to Table Constrain
     */
    SQLCreateTableQuery(const SQLStringExpressionQuery& i_tableName,
        const ISQLQuery& i_tableDescriptions)
    : m_tableName(i_tableName)
    , m_tableDescriptions(makeSharedPtr(i_tableDescriptions)){}

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::CREATE_TABLE
            + makeSpaceBlock(m_tableName.query())
            + makeBrecetBlock(m_tableDescriptions->query()));
    }

    virtual ISQLQuery* clone() const {
        return new SQLCreateTableQuery(*this);
    }

private:
    SQLStringExpressionQuery m_tableName;
    std::shared_ptr<ISQLQuery> m_tableDescriptions;
};

/*! @class SQLSelectQuery
 *  @brief the class create query for select
 */
class SQLSelectQuery: virtual public ISQLQuery {
public:
    SQLSelectQuery(const SQLSelectQuery& i_other)
    : m_collection(i_other.m_collection) {}

    //  TODO(moiseenko.a): Add static type check
    /*! @brief constructor
     *  @param i_ascedQuery is collection of SQLNamedQuery or a SQLNamedQuery
     *  which contains names of columns which shell be read from DB
     *  @param i_fromQuery is collection of SQLFromQuery or a SQLFromQuery
     *  which contains names of asked tables
     *  @param i_predicateQuery shell contain a SQLWhereQuery or SQLEmptyQuery
     *  @param i_orderQuery shell contain SQLOrderedColumnQuery or SQLEmptyQuery
     *  @param i_limitQuery shell contain SQLLimitColumnQuery or SQLEmptyQuery
     */
    SQLSelectQuery(const ISQLQuery& i_ascedQuery,
        const ISQLQuery& i_fromQuery,
        const ISQLQuery& i_predicateQuery = SQLEmptyQuery(),
        const ISQLQuery& i_orderQuery = SQLEmptyQuery(),
        const ISQLQuery& i_limitQuery = SQLEmptyQuery()) {
        m_args.push_back(makeSharedPtr(i_ascedQuery));
        m_args.push_back(makeSharedPtr(i_fromQuery));
        FilterArg<ISQLQuery, SQLEmptyQuery>(m_args, i_predicateQuery);
        FilterArg<ISQLQuery, SQLEmptyQuery>(m_args, i_orderQuery);
        FilterArg<ISQLQuery, SQLEmptyQuery>(m_args, i_limitQuery);
        m_collection =
			SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > >(m_args, SQLiteStringProcessorBase::SPACE);
    }

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::SELECT
                + SQLiteStringProcessorBase::SPACE
                + m_collection.query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLSelectQuery(*this);
    }

private:
    std::vector<std::shared_ptr<ISQLQuery> > m_args;
    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > m_collection;
};

/*! @class SQLInsertQuery
 *  @brief the class create query for insert data
 */
class SQLInsertQuery: virtual public ISQLQuery {
public:
    SQLInsertQuery(const SQLInsertQuery& i_other)
    : m_tableName(i_other.m_tableName)
	, m_insertedQuery(std::dynamic_pointer_cast<ISQLColumnDataQuery>(makeSharedPtr(*i_other.m_insertedQuery)))
    , m_insertType(i_other.m_insertType){}

    /*! @brief constructor
     *  @param i_tableName contain table name
     *  @param i_insertedQuery contain a collection of obj or individual obj
     *  which implement of  ISQLColumnDataQuery.
     *  There are data which shell be set into DB.
     */
    SQLInsertQuery(const SQLStringExpressionQuery& i_tableName,
        const ISQLColumnDataQuery& i_insertedQuery,
        const std::string& i_insertType = std::string(""))
    : m_tableName(i_tableName)
    , m_insertedQuery(std::dynamic_pointer_cast<ISQLColumnDataQuery>(makeSharedPtr(i_insertedQuery))){
        if(!i_insertType.empty()){
            m_insertType = makeSpaceBlock(SQLiteStringProcessorBase::OR) +
                i_insertType + SQLiteStringProcessorBase::SPACE + SQLiteStringProcessorBase::INTO;
        }
        else {
            m_insertType = SQLiteStringProcessorBase::SPACE + SQLiteStringProcessorBase::INTO;
        }
    }

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::INSERT
                + m_insertType
                + makeSpaceBlock(m_tableName.query())
                + makeBrecetBlock(m_insertedQuery->name())
                + makeSpaceBlock(SQLiteStringProcessorBase::VALUES)
                + makeBrecetBlock(m_insertedQuery->value()->query()));
    }

    virtual ISQLQuery* clone() const {
        return new SQLInsertQuery(*this);
    }

private:
    SQLStringExpressionQuery m_tableName;
    std::shared_ptr<ISQLColumnDataQuery> m_insertedQuery;
    std::string m_insertType;
};

/*! @class SQLSetQuery
 *  @brief the class adds set part into query
 */
class SQLSetQuery: virtual public ISQLQuery {
public:
    SQLSetQuery(const SQLSetQuery& i_other)
    :m_ascedQuery(std::dynamic_pointer_cast<ISQLColumnDataQuery>(makeSharedPtr(*i_other.m_ascedQuery))) {}

    /*! @brief constructor
     *  @param i_insertedQuery contain a collection of obj or individual obj
     *  which implement of  ISQLColumnDataQuery.
     *  There are data which shell be set into DB.
     */
    SQLSetQuery(const ISQLColumnDataQuery& i_insertedQuery)
    : m_ascedQuery(std::dynamic_pointer_cast<ISQLColumnDataQuery>(makeSharedPtr(i_insertedQuery))) {}

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::SET
                + makeSpaceBlock(m_ascedQuery->query()));
    }

    virtual ISQLQuery* clone() const {
        return new SQLSetQuery(*this);
    }

private:
    std::shared_ptr<ISQLColumnDataQuery> m_ascedQuery;
};

/*! @class SQLSetQuery
 *  @brief the class creates query of update
 */
class SQLUpdateQuery: virtual public ISQLQuery {
public:
    SQLUpdateQuery(const SQLUpdateQuery& i_other)
    : m_tableName(i_other.m_tableName)
    , m_setQuery(i_other.m_setQuery)
    , m_predicateQuery(makeSharedPtr(*i_other.m_predicateQuery)) {}

    //TODO moiseenko.a: Add static type check
    /*! @brief constructor
     *  @param i_tableName contains table name
     *  @param i_setQuery contains a collection of obj or individual obj
     *  which implement of  ISQLColumnDataQuery.
     *  There are data which shell be set into DB.
     *  @param i_predicateQuery contains a SQLWhereQuery or SQLEmptyQuery
     */
    SQLUpdateQuery(const SQLStringExpressionQuery& i_tableName,
            const SQLSetQuery& i_setQuery,
            const ISQLQuery& i_predicateQuery = SQLEmptyQuery())
    : m_tableName(i_tableName)
    , m_setQuery(i_setQuery)
    , m_predicateQuery(makeSharedPtr(i_predicateQuery)) {}

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::UPDATE
                + makeSpaceBlock(m_tableName.query())
                + m_setQuery.query()
                + SQLiteStringProcessorBase::SPACE
                + m_predicateQuery->query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLUpdateQuery(*this);
    }

private:
    SQLStringExpressionQuery m_tableName;
    SQLSetQuery m_setQuery;
    std::shared_ptr<ISQLQuery> m_predicateQuery;
};

/*! @class SQLDeleteQuery
 *  @brief the class creates query of delete
 */
class SQLDeleteQuery: virtual public ISQLQuery {
public:
    SQLDeleteQuery(const SQLDeleteQuery& i_other)
    : m_args(i_other.m_args)
    , m_collection(i_other.m_collection) {}

    //TODO moiseenko.a: Add static type check
    SQLDeleteQuery(const SQLFromQuery& i_tableName,
            const ISQLQuery& i_predicateQuery = SQLEmptyQuery(),
            const ISQLQuery& i_orderQuery = SQLEmptyQuery(),
            const ISQLQuery& i_limitQuery = SQLEmptyQuery()) {
        m_args.push_back(makeSharedPtr(i_tableName));
        FilterArg<ISQLQuery, SQLEmptyQuery>(m_args, i_predicateQuery);
        FilterArg<ISQLQuery, SQLEmptyQuery>(m_args, i_orderQuery);
        FilterArg<ISQLQuery, SQLEmptyQuery>(m_args, i_limitQuery);
        m_collection =
			SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > >(m_args, SQLiteStringProcessorBase::SPACE);
    }

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::DELETE_
                + SQLiteStringProcessorBase::SPACE
                + m_collection.query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLDeleteQuery(*this);
    }

private:
    std::vector<std::shared_ptr<ISQLQuery> > m_args;
    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > m_collection;
};

/*! @class SQLDropTableQuery
 *  @brief the class creates query of drop table index
 */
class SQLDropIndexQuery: virtual public ISQLQuery {
public:
    SQLDropIndexQuery(const SQLDropIndexQuery& i_other)
    : m_tableName(i_other.m_tableName) {}

    //TODO moiseenko.a: Add static type check
    SQLDropIndexQuery(const SQLStringExpressionQuery& i_tableName)
    : m_tableName(i_tableName) {}

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::DROP
                + SQLiteStringProcessorBase::SPACE
                + m_tableName.query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLDropIndexQuery(*this);
    }

private:
    SQLStringExpressionQuery m_tableName;
};

/*! @class SQLDropTableQuery
 *  @brief the class creates query of drop table
 */
class SQLDropTableQuery: virtual public ISQLQuery {
public:
    SQLDropTableQuery(const SQLDropTableQuery& i_other)
    : m_indexName(i_other.m_indexName) {}

    //TODO moiseenko.a: Add static type check
    SQLDropTableQuery(const SQLStringExpressionQuery& i_indexName)
    : m_indexName(i_indexName) {}

    virtual std::string query() const {
        return /*std::move*/(SQLiteStringProcessorBase::DROP
                + makeSpaceBlock(SQLiteStringProcessorBase::INDEX)
                + m_indexName.query());
    }

    virtual ISQLQuery* clone() const {
        return new SQLDropTableQuery(*this);
    }

private:
    SQLStringExpressionQuery m_indexName;
};
/// @}

/// @defgroup FACTORYES Factories of query
/// @{
template<typename ColumnType>
inline std::shared_ptr<ISQLQuery> makeColumnQuery(const std::string& i_columnName, Loki::Type2Type<ColumnType> arg) {
    return std::shared_ptr<ISQLQuery>(new SQLCreatedColumnQuery<ColumnType>(std::string(i_columnName)));
}

inline std::shared_ptr<ISQLQuery> makeColumnQuery(const std::string& i_columnName) {
    return std::shared_ptr<ISQLQuery>(new SQLCreatedColumnQuery<std::string>(std::string(i_columnName)));
}

template<typename ColumnType>
inline std::shared_ptr<ISQLStringExpressionQuery> makeColumnQuery(const std::string& i_columnName,
        Loki::Type2Type<SQLCreatedUniqueColumnQuery<ColumnType> > arg) {
    return std::shared_ptr<ISQLStringExpressionQuery>(new SQLCreatedUniqueColumnQuery<ColumnType>(std::string(i_columnName)));
}

template<class SQLQueryType, template<class> class SQLContainer, template<class> class Container>
SQLContainer<SQLQueryType> makeSQLCollections(Container<SQLQueryType> i_container) {
    return SQLContainer<SQLQueryType>(i_container);
}

template<class SQLQueryType, template<class> class SQLContainer, class Arg1, template<class, class> class Container>
SQLContainer<SQLQueryType> makeSQLCollections(Container<SQLQueryType, Arg1> i_container) {
    return SQLContainer<SQLQueryType>(i_container);
}

template<class SQLQueryType, template<class> class SQLContainer, class Arg1, class Arg2,
	template<class, class, class> class Container>
SQLContainer<SQLQueryType> makeSQLCollections(Container<SQLQueryType, Arg1, Arg2> i_container) {
    return SQLContainer<SQLQueryType>(i_container);
}

template<class SQLQueryType, template<class> class SQLContainer, class Arg1, class Arg2, class Arg3,
	template<class, class, class, class> class Container>
SQLContainer<SQLQueryType> makeSQLCollections(Container<SQLQueryType, Arg1, Arg2, Arg3> i_container) {
    return SQLContainer<SQLQueryType>(i_container);
}

template<class SQLQueryType, template<class> class SQLContainer, class Key, class Arg1, class Arg2,
	template<class, class, class, class> class Container>
SQLContainer<SQLQueryType> makeSQLCollections(Container<Key, SQLQueryType, Arg1, Arg2> i_container) {
    return SQLContainer<SQLQueryType>(i_container);
}
/// @}
/// @}
#endif //SQLITE_STRING_PROCESSOR_BASE_H
