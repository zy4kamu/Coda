/*! @file make_queries.h
 *  @section Copyright
 *  Object Retrieval Engine
 *  @brief definition helpers function to generate SQL queries
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


#include <vector>
#include <string>
#include <functional>
#include <assert.h>
#include "loki/Typelist.h"
#include "loki/TypeManip.h"
#include "loki/NullType.h"
#include "loki/HierarchyGenerators.h"

#include "sqlite_string_processor_base.h"


#ifndef MAKE_QUERIES
#define MAKE_QUERIES

template<class Type>
struct Type2TypeList{
    typedef Loki::Typelist<Type, Loki::NullType> Result;
};

template<class Head, class Tail>
struct Type2TypeList<Loki::Typelist<Head, Tail> >{
    typedef Loki::Typelist<Head, Tail> Result;
};

template<class T>
class DataTuple {
public:
    typedef typename Type2TypeList<T>::Result TList;
    typedef Loki::Tuple<TList> DataCollecdtion;
    enum {Size = Loki::TL::Length<TList>::value};
private:
    DataCollecdtion m_data;
public:

    template<int I>
    typename Loki::TL::TypeAt<TList, I>::Result get() const {
        return Loki::Field<I>(m_data);
    }

    template<int I>
    void set(typename Loki::TL::TypeAt<TList, I>::Result value) {
        Loki::Field<I>(m_data) = value;
    }

    template<int I, class TList>
    void set(DataTuple<TList> value) {
        this -> template set<I>(value, Loki::Int2Type<DataTuple<TList>::Size - 1>());
    }

private:
    template<int I, class TList>
    void set(DataTuple<TList> value, Loki::Int2Type<0>) {
        Loki::Field<I>(m_data) = value.template get<0>();
    }

    template<int I, class TList, int Count>
    void set(DataTuple<TList> value, Loki::Int2Type<Count>) {
        this -> template set<I>(value, Loki::Int2Type<Count - 1>());
        Loki::Field<I + Count>(m_data) = value.template get<Count>();
    }

};
template<class Tuple, class Iterator, class SubTuple, int TotalIn, int TotalOut, int CountIn, int CountOut>
    struct GetSubTuple;

template<class Tuple, class Iterator, class SubTuple, int TotalIn, int TotalOut>
struct GetSubTuple<Tuple, Iterator, SubTuple, TotalIn, TotalOut, TotalIn, TotalOut> {
    static SubTuple foo(const Tuple& i_value, Iterator i_currentMask) {
        SubTuple rez;
        if( bool (*i_currentMask) == true) {
          rez . SubTuple:: template set<TotalOut - 1>(i_value . template get<TotalIn - 1>());
        }
        return rez;
    }
};

template<class Tuple, class Iterator, class SubTuple, int TotalIn, int TotalOut, int CountOut>
struct GetSubTuple<Tuple, Iterator, SubTuple, TotalIn, TotalOut, TotalIn, CountOut> {
    static SubTuple foo(const Tuple& i_value, Iterator& i_currentMask) {
        assert(0); // Asked Subtuple is longer than currnt tuple
        return  GetSubTuple<Tuple, Iterator, SubTuple, TotalIn, TotalOut, TotalIn, TotalOut>::foo(
                i_value, i_currentMask);
    }
};

template<class Tuple, class Iterator, class SubTuple, int TotalIn, int TotalOut, int CountIn>
struct GetSubTuple<Tuple, Iterator, SubTuple, TotalIn, TotalOut, CountIn, TotalOut> {
    static SubTuple foo(const Tuple& i_value, Iterator i_currentMask) {
        if( bool (*i_currentMask) != true) {
            return GetSubTuple<Tuple, Iterator, SubTuple, TotalIn, TotalOut, CountIn + 1, TotalOut>::foo(
                    i_value, ++i_currentMask);
        }
        SubTuple rez;
        rez . SubTuple:: template set<TotalOut - 1>(i_value . template get<CountIn - 1>());
        return rez;
    }
};

template<class Tuple, class Iterator, class SubTuple, int TotalIn, int TotalOut, int CountIn, int CountOut>
struct GetSubTuple {
    static SubTuple foo(const Tuple& i_value, Iterator i_currentMask) {SubTuple rez;
        if(bool(*i_currentMask) == true) {
            SubTuple rez =
                GetSubTuple<Tuple, Iterator, SubTuple, TotalIn, TotalOut, CountIn + 1, CountOut + 1>::foo(
                    i_value, ++i_currentMask);

            rez . SubTuple:: template set<CountOut - 1>(i_value . template get<CountIn - 1>());
            return rez;
        }
        return GetSubTuple<Tuple, Iterator, SubTuple, TotalIn, TotalOut, CountIn + 1, CountOut>::foo(
                i_value, ++i_currentMask);
    }
};


template<class SubTupleTList, class Tuple, class Iterator>
DataTuple<SubTupleTList> getSubTuple(const Tuple& i_value, Iterator i_currentMask) {
    return GetSubTuple<Tuple, Iterator, DataTuple<SubTupleTList>, Tuple::Size, DataTuple<SubTupleTList>::Size, 1, 1>::foo(i_value, i_currentMask);
}

template<class TList>
static bool tupleLess(const DataTuple<TList>& i_arg1,
        const DataTuple<TList>& i_arg2,
        Loki::Int2Type<0>){
    return i_arg1.template get<0>() < i_arg2.template get<0>();
}

template<class TList, int Count>
static bool tupleLess(const DataTuple<TList>& i_arg1,
        const DataTuple<TList>& i_arg2,
        Loki::Int2Type<Count>){
    if (i_arg1.template get<Count>() < i_arg2.template get<Count>()) {
        return true;
    }
    return tupleLess(i_arg1, i_arg2, Loki::Int2Type<Count - 1>());
}

template<class TList>
bool operator < (const DataTuple<TList>& i_arg1,
        const DataTuple<TList>& i_arg2) {
    return tupleLess(i_arg1, i_arg2, Loki::Int2Type<Loki::TL::Length<TList>::value - 1>());
}

template<class TList>
bool operator > (const DataTuple<TList>& i_arg1,
        const DataTuple<TList>& i_arg2) {
    return i_arg2 < i_arg1;
}

template<class TList>
bool operator != (const DataTuple<TList>& i_arg1,
        const DataTuple<TList>& i_arg2) {
    return (i_arg1 < i_arg2) || (i_arg1 > i_arg2);
}

template<class TList>
bool operator == (const DataTuple<TList>& i_arg1,
        const DataTuple<TList>& i_arg2) {
    return !(i_arg1 != i_arg2);
}

template<class TList>
bool operator <= (const DataTuple<TList>& i_arg1,
        const DataTuple<TList>& i_arg2) {
    return !(i_arg1 > i_arg2);
}

template<class TList>
bool operator >= (const DataTuple<TList>& i_arg1,
        const DataTuple<TList>& i_arg2) {
    return !(i_arg2 < i_arg1);
}

template<class Container, class TList>
void read(Container& o_rezult, const DataTuple<TList>& dataTuple,
        Loki::Int2Type<0>){
    o_rezult.push_back(std::shared_ptr<ISQLQuery>(new SQLLiteralQuery<typename Loki::TL::TypeAt<TList, 0>::Result>(
        dataTuple.template get<0>())));
}

template<class Container, class TList, int Count>
void read(Container& o_rezult, const DataTuple<TList>& dataTuple,
        Loki::Int2Type<Count>){
    read(o_rezult, dataTuple, Loki::Int2Type<Count - 1>());
    o_rezult.push_back(std::shared_ptr<ISQLQuery>(new SQLLiteralQuery<typename Loki::TL::TypeAt<TList, Count>::Result>(
       dataTuple.template get<Count>())));
}

template<class Container, class TList>
typename Container::const_iterator write(DataTuple<TList>& o_rezult,
        const Container& dataCollection,
        Loki::Int2Type<0>){
    typename Container::const_iterator currentPosition = dataCollection.begin();
    o_rezult . DataTuple<TList> ::template set<0>(std::static_pointer_cast<typename Loki::TL::TypeAt<TList, 0>::Result>(*currentPosition));
    return currentPosition++;
}

template<class Container, class TList, int Count>
DataTuple<TList> write(DataTuple<TList>& o_rezult,
        const Container& dataCollection,
        Loki::Int2Type<Count>){
    typename Container::const_iterator currentPosition = write(o_rezult, dataCollection, Loki::Int2Type<Count - 1>());
    o_rezult . DataTuple<TList>:: template set<0>(std::static_pointer_cast<typename Loki::TL::TypeAt<TList, Count>::Result>(*currentPosition));
    return currentPosition++;
}

template<class TList>
std::string toString(const DataTuple<TList> data, Loki::Int2Type<0>) {
    return toString(data.template get<0>());
}

template<class TList, int Count>
std::string toString(const DataTuple<TList> data, Loki::Int2Type<Count>) {
    return toString(data, Loki::Int2Type<Count - 1>()) + "_" + toString(data.template get<Count>());
}


template<class TList>
std::string toString(const DataTuple<TList> data) {
    return toString(data, Loki::Int2Type<DataTuple<TList>::Size - 1>());
}

template<class TypeList>
class MakeTuple;

template<class Head, class Tail>
class MakeTuple<Loki::Typelist<Head, Tail> >{
    MakeTuple<Tail> m_tailData;
public:

    template<class BaseTupleData, class RezCollection, class NamesCollectionIterator>
    RezCollection& columns(RezCollection& o_collect, NamesCollectionIterator i_columnsName) const {
        o_collect.push_back(std::shared_ptr<BaseTupleData>(new Head(*i_columnsName)));
        m_tailData.columns<BaseTupleData>(o_collect, ++i_columnsName);
        return o_collect;
    }
};

template<>
class MakeTuple<Loki::NullType >{
public:
    template<class BaseData, class RezCollection, class NamesCollectionIterator>
    RezCollection& columns(RezCollection& o_collect, NamesCollectionIterator i_columnsName) const {
        return o_collect;
    }
};

template<template<class> class Base, class TypeList>
class TypesPack;

template<template<class> class Base, class Head, class Tail>
class TypesPack<Base, Loki::Typelist<Head, Tail> >{
    typedef typename TypesPack<Base, Tail>::Rez m_rez;
public:
    typedef Loki::Typelist<Base<Head>, m_rez> Rez;
};

template<template<class> class Base>
class TypesPack<Base, Loki::NullType >{
public:
    typedef Loki::NullType Rez;
};

template<class TypeList>
class MakeColumnCreateSet{
    MakeTuple<typename TypesPack<SQLCreatedColumnQuery, TypeList>::Rez > m_tuple;
public:

    template<class BaseData, class RezCollection, class NamesCollectionIterator>
    RezCollection& columns(RezCollection& o_collect, NamesCollectionIterator i_columnsName) const {
        return m_tuple.template columns<BaseData>(o_collect, i_columnsName);
    }
};

template<class OutContainer, class InputData, class Sheme>
void filter(OutContainer &out,
        InputData i_tableBegin,
        InputData i_tableEnd,
        Sheme i_tableSchemeBegin) {
    for (InputData i = i_tableBegin; i != i_tableEnd; ++i, ++i_tableSchemeBegin) {
        if (*i_tableSchemeBegin == true) {
            out.push_back(*i);
        }
    }
}

template<class OutContainer, class InputData, class Sheme, class Prefix>
void addPrefix(OutContainer out,
        InputData i_tableBegin,
        InputData i_tableEnd,
        Sheme i_tableSchemeBegin,
        Prefix i_prefix) {
    for (InputData i = i_tableBegin; i != i_tableEnd; ++i, ++i_tableSchemeBegin, ++out) {
        if (*i_tableSchemeBegin == true) {
            *out = (i_prefix + *i);
        }
    }
}

template<template<class> class OutputCollection, class CollectionBase, class Input>
OutputCollection<CollectionBase> makeStringQueryCollection(Input initDataBegin, Input initDataEnd) {
    OutputCollection<CollectionBase> rez;
    while (initDataBegin != initDataEnd) {
        rez.push_back(makeSharedPtr(SQLStringExpressionQuery(*initDataBegin)));
        ++initDataBegin;
    }
    return rez;
}

inline SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > makeSQLStringExspresionCollection(
        const std::vector<std::string>& i_stringSet) {
    return makeStringQueryCollection<SQLQueryCollections, std::vector<std::shared_ptr<ISQLQuery> > >
                (i_stringSet.begin(), i_stringSet.end());
}

/*!
 *
 * @param i_foreignKey[in] set of foreign key
 */
template<class TypeList>
SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > >  makeStaticQueryCollections(
        const std::vector<std::string>& i_columnNames) {
    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > queryCollection;
    MakeColumnCreateSet<TypeList> columns;
    columns. template columns<ISQLQuery>(queryCollection, i_columnNames.begin());
    return queryCollection;
}
template<class BiseType, class DataType>
SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > >  makeDynamicQueryCollections(
        const std::vector<DataType>& i_columnNames, Loki::Type2Type<BiseType>);

template<class DataType>
SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > >  makeDynamicQueryCollections(
        const std::vector<DataType>& i_columnNames, Loki::Type2Type<SQLLiteralQuery<DataType> >) {
    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > queryCollection;
    for (typename std::vector<DataType>::const_iterator i = i_columnNames.begin();
            i != i_columnNames.end(); ++i) {
        queryCollection.push_back(std::shared_ptr<ISQLQuery>(new SQLLiteralQuery<DataType>(*i)));
    }
    return queryCollection;
}

inline SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > >  makeDynamicQueryCollections(
        const std::vector<std::string>& i_columnNames, Loki::Type2Type<SQLStringExpressionQuery>) {
    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > queryCollection;
    for (std::vector<std::string>::const_iterator i = i_columnNames.begin();
            i != i_columnNames.end(); ++i) {
        queryCollection.push_back(std::shared_ptr<ISQLQuery>(new SQLStringExpressionQuery(*i)));
    }
    return queryCollection;
}
inline std::shared_ptr<ISQLQuery> makeForeignKey(
        const std::vector<std::string>& i_columnNames,
        const std::string& i_foreignTableName,
        const std::vector<std::string>& i_refernceForeignKey) {
    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > columnCollection =
            makeSQLStringExspresionCollection(i_columnNames);

    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > foreignColumnCollection =
            makeSQLStringExspresionCollection(i_refernceForeignKey);
    return makeSharedPtr(SQLForeignKeyQuery(columnCollection, i_foreignTableName, foreignColumnCollection));
}

inline std::shared_ptr<ISQLQuery> makeReferenceKey(
        const std::vector<std::string>& i_refernceKeyColumnNames) {
    SQLQueryCollections< std::vector<std::shared_ptr<ISQLQuery> > > refernceKey =
                makeSQLStringExspresionCollection(i_refernceKeyColumnNames);
    return makeSharedPtr(SQLStringExpressionQuery(
                SQLiteStringProcessorBase::PRIMARY_KEY
                + makeBrecetBlock(refernceKey.query())));
}

inline std::shared_ptr<ISQLQuery> makeUnique(
        const std::vector<std::string>& i_columnNames) {
    SQLQueryCollections< std::vector<std::shared_ptr<ISQLQuery> > > columns =
                makeSQLStringExspresionCollection(i_columnNames);
    return makeSharedPtr(SQLUniqueSetQuery(columns));
}


/*!
 *
 * @param i_foreignKey[in] set of foreign key
 */
template<class TypeList>
SQLCreateTableQuery makeTable(const std::string& i_tableName,
        const std::vector<std::string>& i_columnNames,
        const std::vector<std::string>& i_primaryKeyColumnNames,
        const std::string i_constrain = std::string()) {
    assert(i_columnNames.size() == DataTuple<TypeList>::Size );
    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > queryCollection =
            makeStaticQueryCollections<TypeList>(i_columnNames);
    queryCollection.push_back(makeReferenceKey(i_primaryKeyColumnNames));
    if (! i_constrain.empty()) {
        queryCollection.push_back(std::shared_ptr<ISQLQuery>(new SQLStringExpressionQuery(i_constrain)));
    }
    return SQLCreateTableQuery(i_tableName, queryCollection);
}


/*!
 *
 * @param i_foreignKey[in] set of foreign key
 */
template<class TypeList, class Conatiner>
SQLCreateTableQuery makeTable(const std::string& i_tableName,
        const std::vector<std::string>& i_columnNames,
        const std::vector<std::string>& i_refernceKeyColumnNames,
        const SQLQueryCollections<Conatiner>& i_addrdConstrain =
                SQLQueryCollections<Conatiner>()
        ) {
    std::vector<std::shared_ptr<ISQLQuery> > baseCollection;
    SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > queryCollection;
    MakeColumnCreateSet<TypeList> columns;
    columns. template columns<ISQLQuery>(queryCollection, i_columnNames.begin());
    SQLQueryCollections< std::vector<std::shared_ptr<ISQLQuery> > > refernceKey =
            makeSQLStringExspresionCollection(i_refernceKeyColumnNames);
    queryCollection.push_back(makeSharedPtr(SQLStringExpressionQuery(
            SQLiteStringProcessorBase::PRIMARY_KEY
            + makeBrecetBlock(refernceKey.query()))));
    for (typename SQLQueryCollections<Conatiner>::const_iterator i = i_addrdConstrain.begin();
            i != i_addrdConstrain.end(); ++i) {
        queryCollection.push_back(*i);
    }
    return SQLCreateTableQuery(i_tableName, queryCollection);
}

#endif //MAKE_QUERIES
