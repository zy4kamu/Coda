/*! @file sql_table.h
 *  @section Copyright
 *  Object Retrieval Engine
 *  @brief base class to work with SQL tables
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


#ifndef SQL_TABLE_H
#define SQL_TABLE_H

#include <fstream>
#include <map>
#include <memory>
#include <stdint.h>
#include <stdio.h>
#include "sqlite_string_processor_base.h"
#include "sql_table_base.h"
#include "sqlite_helper.h"
#include "loki/Typelist.h"
#include "serializer.h"

template<int Size, class RowID>
class WriteRow {
    static const int64_t m_size = Size;
    std::shared_ptr<ISQLColumnDataQuery> m_table[m_size];
    bool m_fillSheme[m_size];
    const std::string& m_tableName;
    const std::vector<std::string>& m_columnNames;
    const RowID m_rowID;
    std::list<std::shared_ptr<ISQLTableExecutor> > m_dependence;
public:
    WriteRow(const std::string& i_tableNames,
            const std::vector<std::string>& i_columnNames, RowID i_rowID)
    : m_tableName(i_tableNames)
    , m_columnNames(i_columnNames)
    , m_rowID(i_rowID)
    {
        memset(m_table, 0, sizeof(m_table));
        memset(m_fillSheme, 0, sizeof(m_fillSheme));
    }

    ~WriteRow() {
        for(std::list<std::shared_ptr<ISQLTableExecutor> >::iterator i = m_dependence.begin();
                i != m_dependence.end(); ) {
            (*i)->execute();
            i = m_dependence.erase(i);
        }
    }
    template<class T>
    void add(int64_t i_columnID, T i_data) {
        WritedData field = writeData(m_tableName,
                m_rowID,
                m_columnNames[i_columnID],
                i_data);

        m_table[i_columnID] = field.get<CurrentRowData>();
        if (field.get<DependenceCall>() != std::shared_ptr<ISQLTableExecutor>()) {
            m_dependence.push_back(field.get<DependenceCall>());
        }
        m_fillSheme[i_columnID] = true;
    }

    bool isReady(int i_fieldID) {
        return m_fillSheme[i_fieldID];
    }

    bool isFull() {
        for (int64_t i = 0; i < m_size; ++i) {
            if (m_fillSheme[i] == false) {
                return false;
            }
        }
        return true;
    }
    const RowID& key() const {
        return m_rowID;
    }
    std::shared_ptr<ISQLColumnDataQuery> data() {
        int size = sizeof(m_table)/sizeof(std::shared_ptr<ISQLQuery>);
        SQLColumnDataQueryCollections<List> rowData(m_table, m_table + size);
        return 
			std::dynamic_pointer_cast<ISQLColumnDataQuery>(makeSharedPtr(rowData));
    }
};

/*! @class SqlTable
 *  @brief interface to SQL Table
 */
template<class ColumnTypeList, class PrimaryKeyTypeList>
class SqlTable<ColumnTypeList, DataTuple<PrimaryKeyTypeList> >: virtual public ISqlTable{

public:
    /// Type List for Table Columns
    typedef ColumnTypeList ColumnTypes;
    typedef DataTuple<ColumnTypes> RowData;
    typedef DataTuple<PrimaryKeyTypeList> PrimaryKey;
    enum { columnsTotal = Loki::TL::Length<ColumnTypes>::value };

protected :
    typedef WriteRow<columnsTotal, PrimaryKey> Row;
    typedef std::list<Row> RowCollection;

public:
    /*! @fn SqlTable(const std::string& i_tableName,
                std::vector<std::string> i_columnNames,
                std::vector<std::string> i_primaryKeyColumnsID)
     *  @brief constructor
     *  @param[in] i_tableName name of the table
     *  @param[in] i_columnNames set of columns names
     *  @param[in] i_primaryKeyColumnsID subset of i_columnNames which contains column names which shell be reference keys
     */
    SqlTable(const std::string& i_tableName,
            const std::vector<std::string>& i_columnNames,
            const std::vector<size_t>& i_primaryKeyColumnsID,
            const std::string i_constrain = std::string(),
            bool i_immediatelyFlushing = true)
    : m_tableName(i_tableName)
    , m_columnNames(prepareColumnName(i_columnNames))
    , m_primariKeyColumnsID(i_primaryKeyColumnsID)
    , m_tableConstrain(i_constrain)
    , m_helper(DataBaseSchema::instance().databaseName(),
            makeTable<ColumnTypes>(m_tableName,
                    m_columnNames,
                    getSubsetOfNames(m_columnNames, m_primariKeyColumnsID),
                    m_tableConstrain))
    , m_immediatelyFlushing(i_immediatelyFlushing)
    , m_primaryKeyMap2Columns(std::vector<bool>(columnsTotal, false)) {
        m_helper.execute();
        for (std::vector<size_t>::const_iterator i = m_primariKeyColumnsID.begin();
                i != m_primariKeyColumnsID.end(); ++i) {
            m_primaryKeyMap2Columns[*i] = true;
        }
        for(int i = 0; i < columnsTotal; ++i) {
            m_allColumnsID.push_back(i);
        }
    }

    ~SqlTable() {
        execute();
    }

    /// @see ISQLQuery::query()
    virtual std::string query() const {
       return makeTable<ColumnTypes>(m_tableName,
           m_columnNames,
           getSubsetOfNames(m_columnNames, m_primariKeyColumnsID),
           m_tableConstrain).query();
    }

    /// @see ISQLQuery::clone()
    virtual ISQLQuery* clone() const {
        return new SqlTable<ColumnTypeList, PrimaryKey>(*this);
    }

    /// @see ISqlTable::tableName()
    const std::string& tableName() const {
        return m_tableName;
    }

    /// @see ISqlTable::columnNames()
    const std::vector<std::string>& columnNames() const {
        return m_columnNames;
    }

    /// @see ISqlTable::primaryKeyNames()
    std::vector<std::string> primaryKeyNames() const {
        return getSubsetOfNames(m_columnNames, m_primariKeyColumnsID);
    }

    /// @see ISqlTable::tableConstrain()
    const std::string& tableConstrain() const {
        return m_tableConstrain;
    }

    /// @see ISqlTable::columnTotal()
    int64_t columnTotal() const {
        return columnsTotal;
    }

    /// @see ISqlTable::rowTotal()
    int64_t rowTotal() const {
        //SQLStringExpressionQuery("select count(_id) from sql_function_test")
        SQLSelectQuery select(SQLStringExpressionQuery("count(*)"),
            SQLFromQuery(SQLStringExpressionQuery(m_tableName)));
        m_helper.addQuery(select);
        SQLiteHelper::iterator iresult = m_helper.begin();
        if(iresult == m_helper.end()) {
            return 0;
        }
        SQLiteHelper::iterator::reference_type rezaltRow = *iresult;
        SQLiteHelper::iterator::reference_type::iterator idata = rezaltRow.begin();
        if(idata == rezaltRow.end()){
            throw SQLiteException();
        }
        std::string scount = readData<std::string, PrimaryKey>(rezaltRow, idata);
        return fromString<int64_t>(scount.c_str());
    }

    /// @see ISqlTable::deleteRow()
    void deleteRow(const SQLPredicateAscedColumnQuery& i_predicate){
        this->m_helper.addQuery(SQLDeleteQuery(
                SQLFromQuery(SQLStringExpressionQuery(m_tableName)),
                SQLWhereQuery(i_predicate)));
        this->m_helper.execute();
    }

    void clear() {
        this->m_helper.addQuery(SQLDeleteQuery(
            SQLFromQuery(SQLStringExpressionQuery(m_tableName))));
        this->m_helper.execute();
    }

    virtual bool isEmpty() const {
        return this->rowTotal() == 0;
    }
    /*! @fn template<class T> T get(RowID i_key, int64_t i_column)
     *  @brief read a data from the table
     *  @param i_key[in] data row index
     *  @param i_column[in] data column index
     *  @return data from the table field with row i_key and column i_column
     *      if a data was read and cast to type T otherwise throw exception SQLiteException()
     */
    template<class T>
    T get(PrimaryKey i_key,
            int64_t i_column) const {

        int64_t column = i_column;
        execute();

        SQLSelectQuery select(SQLStringExpressionQuery(m_columnNames[column]),
                SQLFromQuery(SQLStringExpressionQuery(m_tableName)),
                SQLWhereQuery(makePredicate(SQLPredicateAscedColumnQuery::SQLPredicateOperator::EQUAL,
                        m_primariKeyColumnsID,
                        i_key)));
        m_helper.addQuery(select);
        SQLiteHelper::iterator::reference_type rezaltRow = *(this->getResult());
        SQLiteHelper::iterator::reference_type::iterator idata = rezaltRow.begin();
        return readData<T>(rezaltRow, idata, m_tableName, i_key, m_columnNames[column]);
    }

    /*! @fn template<class T> T get(RowID i_key, int64_t i_column)
     *  @brief read a data from the table
     *  @param i_key[in] data row index
     *  @param i_column[in] data column index
     *  @return data from the table field with row i_key and column i_column
     *      if a data was read and cast to type T otherwise throw exception SQLiteException()
     */
    template<class TList>
    DataTuple<TList> get(PrimaryKey i_key,
            std::vector<size_t> i_column) const {

        assert(i_column.size() == DataTuple<TList>::Size );

        execute();
        SQLSelectQuery select(makeSQLStringExspresionCollection(
                getSubsetOfNames(m_columnNames, i_column)),
            SQLFromQuery(SQLStringExpressionQuery(m_tableName)),
            SQLWhereQuery(makePredicate(SQLPredicateAscedColumnQuery::SQLPredicateOperator::EQUAL,
                    m_primariKeyColumnsID,
                    i_key)));
        m_helper.addQuery(select);
        SQLiteHelper::iterator::reference_type rezaltRow = *(this->getResult());
        SQLiteHelper::iterator::reference_type::iterator idata = rezaltRow.begin();
        return readData(rezaltRow, idata, Loki::Type2Type<DataTuple<TList> >());
    }

    std::list<RowData > get() const {

        execute();
        try {
            SQLStringExpressionQuery select("SELECT * FROM " + tableName());
            m_helper.addQuery(select);
            SQLiteHelper::iterator idata = getResult();
            std::list<DataTuple<ColumnTypeList> >rezult;
            do {
                rezult.push_back(readData(
                        SQLiteHelper::iterator::reference_type(*idata),
                        SQLiteHelper::iterator::reference_type::iterator((*idata).begin()),
                        Loki::Type2Type<DataTuple<ColumnTypeList> >()));
                ++idata;
            } while(idata != m_helper.end());
            return rezult;
        }
        catch (SQLiteException e) {
            return std::list<DataTuple<ColumnTypeList> >();
        }
    }

    template<class TList>
    std::list<DataTuple<TList> > get (const std::string& i_selectQuery) {
        return ISqlTable::template get<TList>(i_selectQuery);
    }

    template<class OutTList, class AskTList>
    std::list<DataTuple<OutTList> > get(
            const DataTuple<AskTList>& i_key,
            std::vector<size_t> i_checkColumns,
            std::vector<size_t> i_requestColumns) const {
        return ISqlTable::template get<OutTList, AskTList>(i_key, i_checkColumns, i_requestColumns);
    }
    /*! @fn template<class T> std::list<RowID> find(const T& i_value, int64_t i_column) const
     *  @brief find data into the table column
     *  @param i_value[in] data to find
     *  @param i_column[in] data column index
     *  @return list which contain reference key of the data
     */
    template<class T>
    std::list<PrimaryKey> find(
            int64_t i_column,
            const T& i_value) const {
        try {
            int64_t column = i_column;
            execute();

            SQLSelectQuery select(makeSQLStringExspresionCollection(
                    getSubsetOfNames(m_columnNames, m_primariKeyColumnsID)),
                SQLFromQuery(SQLStringExpressionQuery(m_tableName)),
                SQLWhereQuery(SQLPredicateAscedColumnQuery(
                    SQLPredicateAscedColumnQuery(SQLStringExpressionQuery(m_columnNames[column]),
                        SQLPredicateAscedColumnQuery::SQLPredicateOperator::EQUAL,
                        SQLLiteralQuery<T>(i_value)))));
            m_helper.addQuery(select);
            SQLiteHelper::iterator idata = getResult();
            std::list<PrimaryKey>rezult;
            do {
                rezult.push_back(readData(
                        SQLiteHelper::iterator::reference_type(*idata),
                        SQLiteHelper::iterator::reference_type::iterator((*idata).begin()),
                        Loki::Type2Type<PrimaryKey>()));
                ++idata;
            } while(idata != m_helper.end());
            return rezult;
        }
        catch (SQLiteException e) {
            return std::list<PrimaryKey>();
        }
    }

    /*! @fn template<class T> std::list<RowID> find(const T& i_value, int64_t i_column) const
     *  @brief find data into the table column
     *  @param i_value[in] data to find
     *  @param i_column[in] data column index
     *  @return list which contain reference key of the data
     */
    template<class TList>
    std::list<PrimaryKey> find(
            std::vector<size_t> i_column,
            const DataTuple<TList>& i_value) const {
        try {
            execute();

            SQLSelectQuery select(makeSQLStringExspresionCollection(
                    getSubsetOfNames(m_columnNames, m_primariKeyColumnsID)),
                SQLFromQuery(SQLStringExpressionQuery(m_tableName)),
                SQLWhereQuery(makePredicate(SQLPredicateAscedColumnQuery::SQLPredicateOperator::EQUAL,
                    i_column,
                    i_value)));
            m_helper.addQuery(select);
            SQLiteHelper::iterator idata = getResult();
            std::list<PrimaryKey>rezult;
            do {
                rezult.push_back(readData(
                        SQLiteHelper::iterator::reference_type(*idata),
                        SQLiteHelper::iterator::reference_type::iterator((*idata).begin()),
                        Loki::Type2Type<PrimaryKey>()));
                ++idata;
            } while(idata != m_helper.end());
            return rezult;
        }
        catch (SQLiteException e) {
            return std::list<PrimaryKey>();
        }
    }

    /*! @fn template<class T> RowID insert(RowID i_key, int64_t i_column, const T& i_value)
     *  @brief write a data into table
     *  @param i_key[in] data row index
     *  @param i_column[in] data column index
     *  @param i_value[in] wrote data
     *  @return RowID which is key for wrote data
     *  @note result row will be insert into sql table only after all data of row will be got
     */
    template<class T>
    PrimaryKey insert(PrimaryKey i_key,
            int64_t i_column,
            const T& i_value) {
        int64_t column = i_column;
        PrimaryKey key = i_key;
        typename RowCollection::reverse_iterator row = find(key);
        if (row == m_rowCollection.rend()) {
            createNextRow(key);
            row = find(key);
        }
        row->add(column, i_value);
        if (m_immediatelyFlushing == true) {
            execute(row);
        }
        return key;
    }

    PrimaryKey insert(const RowData& i_value) {
        PrimaryKey key = getKey(i_value);
        typename RowCollection::reverse_iterator row =
                    insert(key, m_allColumnsID, i_value, Loki::Int2Type<DataTuple<ColumnTypes>::Size - 1>());
        if (m_immediatelyFlushing == true) {
            execute(row);
        }
        return key;
    }

    template<class TList>
    PrimaryKey insert(PrimaryKey i_key,
            std::vector<size_t> i_column,
            const DataTuple<TList>& i_value) {
        typename RowCollection::reverse_iterator row =
                    insert(i_key, i_column, i_value, Loki::Int2Type<DataTuple<TList>::Size - 1>());
        if (m_immediatelyFlushing == true) {
            execute(row);
        }
        return i_key;
    }

    bool execute(PrimaryKey i_key) const {
        PrimaryKey key = i_key;
        typename RowCollection::reverse_iterator row = find(key);
        if (row != m_rowCollection.rend()) {
           return execute(row);
        }
        return false;
    }

    bool execute() const {
        typename RowCollection::iterator row = m_rowCollection.begin();
        while (row != m_rowCollection.end()) {
            SQLQueryCollections<std::vector<std::shared_ptr<ISQLQuery> > > insertDataCollection;
            for (int i = 0; i < MaxRowForInsert && row != m_rowCollection.end(); ++i) {
                typename RowCollection::iterator cur = row;
                row++;
                if(cur->isFull() == false) {
                    continue;
                }
                insertDataCollection.push_back(std::shared_ptr<ISQLQuery>(new SQLStringExpressionQuery(
                        makeBrecetBlock(cur->data()->value()->query()))));
               m_rowCollection.erase(cur);
            }
            if(insertDataCollection.empty() == false) {
               std::string insertData = insertDataCollection.query();
                SQLStringExpressionQuery insertQuery(std::string("INSERT OR IGNORE INTO ") +
                    tableName() +
                    makeBrecetBlock(makeSQLStringExspresionCollection(columnNames()).query()) +
                    " VALUES " +
                    insertData);
                m_helper.addQuery( insertQuery);
                insertDataCollection.clear();
            }
        }
        bool rez = ! m_helper.isEmpty();
        m_helper.execute();
        return rez;
    }

    PrimaryKey getKey (const RowData& i_row) {
        return getSubTuple<typename PrimaryKey::TList>(i_row, m_primaryKeyMap2Columns.begin());
    }
protected:
    typename RowCollection::reverse_iterator find(const PrimaryKey& i_checkKey) const {
        for(typename RowCollection::reverse_iterator i = m_rowCollection.rbegin();
                i != m_rowCollection.rend(); ++i) {
            if(i->key() == i_checkKey){
                return i;
            }
        }
        return m_rowCollection.rend();
    }
    virtual SQLiteHelper& helper() const {
        return m_helper;
    }

    template<class TList>
    typename RowCollection::reverse_iterator insert(const PrimaryKey& i_key,
            const std::vector<size_t>& i_column,
            const DataTuple<TList>& i_value,
            Loki::Int2Type<0>) {
        typename RowCollection::reverse_iterator row = find(i_key);
        if (row == m_rowCollection.rend()) {
            bool f = createNextRow(i_key);
            row = find(i_key);
        }
        assert(row != m_rowCollection.rend());
        row->add(i_column[0], i_value.get<0>());
        return row;
    }

    template<class TList, int Count>
    typename RowCollection::reverse_iterator insert(const PrimaryKey& i_key,
            const std::vector<size_t>& i_column,
            const DataTuple<TList>& i_value,
            Loki::Int2Type<Count>) {
        typename RowCollection::reverse_iterator row =
            insert(i_key, i_column, i_value, Loki::Int2Type<Count - 1>());
        row->add(i_column[Count], i_value.get<Count>());
        return row;
    }

    /*! @fn void createNextRow(RowID i_key)
     *  @brief prepare place to save data before send them to data base
     *  @param i_key[in] key value to identify the row
     */
    bool createNextRow(PrimaryKey i_key) {
        m_rowCollection.push_back(Row(m_tableName, m_columnNames, i_key));
        return true;
    }

    bool execute(typename RowCollection::iterator i_row) const {

        if (i_row->isFull() == true) {
            m_helper.addQuery(SQLInsertQuery(SQLStringExpressionQuery(m_tableName),
                    *(i_row->data()),
                    SQLiteStringProcessorBase::_IGNORE));
            m_helper.execute();
            m_rowCollection.erase(i_row);
            return true;
        }
        return false;
    }

    bool execute(typename RowCollection::reverse_iterator i_row) const {
        return execute(--i_row.base());
    }
    std::vector<std::string> prepareColumnName(const std::vector<std::string>& i_columnNames) {
        std::vector<std::string> rez;
        for (std::vector<std::string>::const_iterator i = i_columnNames.begin();
                i != i_columnNames.end(); ++i) {
            rez.push_back(*i);
        }
        return rez;
    }
protected:
    const std::string m_tableName;
    const std::vector<std::string> m_columnNames;
    const std::vector<size_t> m_primariKeyColumnsID;
    const std::string  m_tableConstrain;
    mutable SQLiteHelper m_helper;
    mutable RowCollection m_rowCollection;
    bool m_immediatelyFlushing;
    static const int MaxRowForInsert = 500;
    std::vector<bool> m_primaryKeyMap2Columns;
    std::vector<size_t> m_allColumnsID;
};

/*! @class SqlTable
 *  @brief interface to SQL Table
 */
template<class ColumnTypeList, class RowID>
class SqlTable: virtual public ISqlTable,
    protected SqlTable<ColumnTypeList, DataTuple<Loki::Typelist<RowID, Loki::NullType> > >{
public:
    typedef RowID PrimaryKey;

protected:
    typedef DataTuple<Loki::Typelist<RowID, Loki::NullType> > KeyType;
    typedef SqlTable<ColumnTypeList, KeyType> Base;

public:
    typedef typename Base::ColumnTypes ColumnTypes;
    typedef typename Base::RowData RowData;

    enum { columnsTotal = Base::columnsTotal };
    /*! @fn SqlTable(const std::string& i_tableName,
                std::vector<std::string> i_columnNames,
                std::vector<std::string> i_primaryKeyColumnsID)
     *  @brief constructor
     *  @param[in] i_tableName name of the table
     *  @param[in] i_columnNames set of columns names
     *  @param[in] i_primaryKeyColumnsID subset of i_columnNames which contains column names which shell be reference keys
     */
    SqlTable(const std::string& i_tableName,
            const std::vector<std::string>& i_columnNames,
            size_t i_primariNameID,
            const std::string i_constrain = std::string(),
            bool i_immediatelyFlushing = true)
    : SqlTable<ColumnTypeList, DataTuple<Loki::Typelist<RowID, Loki::NullType> > >(i_tableName,
            i_columnNames,
            std::vector<size_t>(1, i_primariNameID),
            i_constrain,
            i_immediatelyFlushing){    }

    /// @see ISQLQuery::query()
    virtual std::string query() const {
       return Base::query();
    }

    /// @see ISQLQuery::clone()
    virtual ISQLQuery* clone() const {
        return new SqlTable<ColumnTypeList, RowID>(*this);
    }

    /// @see ISqlTable::tableName()
    const std::string& tableName() const {
        return Base::tableName();
    }

    /// @see ISqlTable::columnNames()
    const std::vector<std::string>& columnNames() const {
        return Base::columnNames();
    }

    /// @see ISqlTable::primaryKeyNames()
    std::vector<std::string> primaryKeyNames() const {
        return Base::primaryKeyNames();
    }

    /// @see ISqlTable::tableConstrain()
    const std::string& tableConstrain() const {
        return Base::tableConstrain();
    }

    /// @see ISqlTable::columnTotal()
    int64_t columnTotal() const {
        return Base::columnTotal();
    }

    /// @see ISqlTable::rowTotal()
    int64_t rowTotal() const {
       return Base::rowTotal();
    }

    /// @see ISqlTable::deleteRow()
    void deleteRow(const SQLPredicateAscedColumnQuery& i_predicate){
        Base::deleteRow(i_predicate);
    }

    void clear() {
        Base::clear();
    }

    virtual bool isEmpty() const {
        return Base::isEmpty();
    }
    /*! @fn template<class T> T get(RowID i_key, int64_t i_column)
     *  @brief read a data from the table
     *  @param i_key[in] data row index
     *  @param i_column[in] data column index
     *  @return data from the table field with row i_key and column i_column
     *      if a data was read and cast to type T otherwise throw exception SQLiteException()
     */
    template<class T>
    T get(RowID i_key,
            int64_t i_column) const {
        return Base::template get<T>(key(i_key), i_column);
    }

    template<class OutTList, class AskTList>
    std::list<DataTuple<OutTList> > get(
            const DataTuple<AskTList>& i_key,
            std::vector<size_t> i_checkColumns,
            std::vector<size_t> i_requestColumns) const {
       return Base::template get<OutTList, AskTList>(i_key, i_checkColumns, i_requestColumns );
    }
	template<class TList>
    std::list<DataTuple<TList> > get (const std::string& i_selectQuery) {
        return Base::template get<TList>(i_selectQuery);
    }
    std::list<DataTuple<ColumnTypeList> > get() const {
       return Base::get();
    }

    /*! @fn template<class T> std::list<RowID> find(const T& i_value, int64_t i_column) const
     *  @brief find data into the table column
     *  @param i_value[in] data to find
     *  @param i_column[in] data column index
     *  @return list which contain reference key of the data
     */
    template<class T>
    std::list<RowID> find(
            int64_t i_column,
            const T& i_value) const {
        std::list<RowID> rez;
        std::list<KeyType> midle = Base::find(i_column, i_value);
        for (typename std::list<KeyType>::const_iterator i = midle.begin(); i != midle.end(); ++i) {
            rez.push_back(i->get<0>());
        }
        return rez;
    }

    /*! @fn template<class T> RowID insert(RowID i_key, int64_t i_column, const T& i_value)
     *  @brief write a data into table
     *  @param i_key[in] data row index
     *  @param i_column[in] data column index
     *  @param i_value[in] wrote data
     *  @return RowID which is key for wrote data
     *  @note result row will be insert into sql table only after all data of row will be got
     */
    template<class T>
    RowID insert(RowID i_key,
            int64_t i_column,
            const T& i_value) {
        KeyType rezKey = Base::insert(key(i_key), i_column, i_value);
        return rezKey.get<0>();
    }

    bool execute(RowID i_key) const {
        return Base::execute(key(i_key));
    }

    bool execute() const {
        return Base::execute();
    }

protected:
    virtual SQLiteHelper& helper() const {
        return Base::helper();
    }

    static KeyType key(RowID i_value){
        KeyType rez;
        rez.KeyType::template set<0>(i_value);
        return rez;
    }

    bool createNextRow(RowID i_key) {
        return Base::createNextRow(key(i_key));
    }
};

/*! @class SqlTable
 *  @brief interface to SQL Table which is one primary key column
 */
template<class ColumnTypeList>
class SqlTableWithAutoIncrement: virtual public ISqlTable,
        protected SqlTable<Loki::Typelist<int64_t, ColumnTypeList>, int64_t>{
    typedef SqlTable<Loki::Typelist<int64_t, ColumnTypeList>, int64_t> BaseTable;
public:
    typedef int64_t PrimaryKey;
    /// Type List for Table Columns
    typedef typename BaseTable::ColumnTypes ColumnTypes;
    typedef typename BaseTable::RowData RowData;

protected:
    typedef typename BaseTable::RowCollection RowCollection;
    typedef typename BaseTable::Row Row;

public:
    enum { columnsTotal = BaseTable::columnsTotal };

    /*! @fn SqlTable(const std::string& i_tableName,
                std::vector<std::string> i_columnNames,
                std::vector<std::string> i_primaryKeyColumnsID)
     *  @brief constructor
     *  @param[in] i_tableName name of the table
     *  @param[in] i_columnNames set of columns names
     *  @param[in] i_primaryKeyColumnsID subset of i_columnNames which contains column names which shell be reference keys
     */
    SqlTableWithAutoIncrement(const std::string& i_tableName,
            const std::vector<std::string>& i_columnNames,
            const std::string i_constrain = std::string(),
            bool i_immediatelyFlushing = true)
    : BaseTable(i_tableName
    , addIDColumn(i_columnNames)
    , 0
    , i_constrain
    , i_immediatelyFlushing){
        updateDBMetrics();
    }

    /// @see ISqlTable::tableName()
    const std::string& tableName() const {
        return BaseTable::tableName();
    }

    /// @see ISqlTable::columnNames()
    const std::vector<std::string>& columnNames() const {
        return BaseTable::columnNames();
    }

    /// @see ISqlTable::primaryKeyNames()
    std::vector<std::string> primaryKeyNames() const {
        return BaseTable::primaryKeyNames();
    }

    /// @see ISqlTable::tableConstrain()
    const std::string& tableConstrain() const {
        return BaseTable::tableConstrain();
    }

    /// @see ISqlTable::columnTotal()
    int64_t columnTotal() const {
        return BaseTable::columnTotal();
    }

    /// @see ISqlTable::rowTotal()
    int64_t rowTotal() const {
        return BaseTable::rowTotal();
    }

    /// @see ISqlTable::deleteRow()
    void deleteRow(const SQLPredicateAscedColumnQuery& i_predicate) {
        BaseTable::deleteRow(i_predicate);
    }

    void clear() {
        BaseTable::clear();
    }

    virtual bool isEmpty() const {
        return BaseTable::isEmpty();
    }
    /// @see ISQLQuery::query()
    virtual std::string query() const {
       return BaseTable::query();
    }

    /// @see ISQLQuery::clone()
    virtual ISQLQuery* clone() const {
        return new SqlTableWithAutoIncrement<ColumnTypeList>(*this);
    }

    /*! @fn template<class T> T get(int64_t i_key, int64_t i_column)
     *  @brief read a data from the table
     *  @param i_key[in] data row index
     *  @param i_column[in] data column index
     *  @return data from the table field with row i_key and column i_column
     *      if a data was read and cast to type T otherwise throw exception SQLiteException()
     */
    template<class T>
    T get(int64_t i_key,
            int64_t i_column) const {
        return BaseTable::template get<T>(i_key, i_column);
    }

    template<class OutTList, class AskTList>
    std::list<DataTuple<OutTList> > get(
            const DataTuple<AskTList>& i_key,
            std::vector<size_t> i_checkColumns,
            std::vector<size_t> i_requestColumns) const {
       return BaseTable::template get<OutTList, AskTList>(i_key, i_checkColumns, i_requestColumns);
    }

    std::list<DataTuple<ColumnTypes> > get() const {
       return BaseTable::get();
    }
    /*! @fn template<class T> int64_t find(const T& i_value, int64_t i_column) const
     *  @brief find data into the table column
     *  @param i_value[in] data to find
     *  @param i_column[in] data column index
     *  @return id of the data
     */
    template<class T>
    std::list<int64_t> find(
            int64_t i_column,
            const T& i_value) const {
        return BaseTable::find(i_column, i_value);
    }
    /*! @fn template<class T> void insert(int64_t i_key, int64_t i_column, const T& i_value)
     *  @brief write a data into table
     *  @param i_key[in] data row index, this is recommended value for create new row
     *  @param i_column[in] data column index
     *  @param i_value[in] wrote data
     *  @return key value which definition row which contain the data
     *  @note result row will be insert into sql table only after all data of row will be got
     */
    template<class T>
    int64_t insert(int64_t i_key,
            int64_t i_column,
            const T& i_value) {
        int accessKey = i_key;
        typename RowCollection::reverse_iterator row = BaseTable::Base::find(BaseTable::key(accessKey));
        if (row == BaseTable::Base::m_rowCollection.rend() || row->isReady(i_column)) {
            accessKey = this->getNextIndex();
            row = BaseTable::Base::find(BaseTable::key(accessKey));
        }
        row->add(i_column, i_value);
        if (BaseTable::m_immediatelyFlushing == true) {
            this->execute(i_key);
        }
        return accessKey;
    }

    /*! @fn template<class T> int64_t insert(int64_t i_column, const T& i_value)
     *  @brief create new row into table
     *  @param i_column[in] data column index
     *  @param i_value[in] wrote data
     *  @return id of the inserted data
     */
    int64_t getNextIndex() {

        int64_t rezID = (this->m_dbMaxRowID == 0) ?
                BaseTable::m_rowCollection.size() + this->m_dbRowCont :
                this->m_dbMaxRowID + BaseTable::m_rowCollection.size() + 1;
        if (BaseTable::createNextRow(rezID) == false) {
            throw SQLiteException();
        }
        BaseTable::insert(rezID, 0, rezID);
        return rezID;
    }

    /*! @fn void deleteRow(int64_t i_key)
     *  @brief delete row according id the row
     *  @param i_key[in] id row which shell be deleted
     */
    void deleteRow(int64_t i_key){
        SQLDeleteQuery deleteRow(
            SQLFromQuery(SQLStringExpressionQuery(tableName())),
            SQLWhereQuery(SQLPredicateAscedColumnQuery(
                SQLPredicateAscedColumnQuery(SQLStringExpressionQuery(primaryKeyNames().operator [](0)),
                    SQLPredicateAscedColumnQuery::SQLPredicateOperator::EQUAL,
                    SQLLiteralQuery<int64_t>(i_key)))));
        BaseTable::m_helper.addQuery(deleteRow);
        BaseTable::m_helper.flush();
        this->updateDBMetrics();
    }

    bool execute(int64_t i_key) const {
        bool rez = BaseTable::execute(i_key);
        this->updateDBMetrics();
        return rez;
    }

    bool execute() const {
        bool rez = BaseTable::execute();
        this->updateDBMetrics();
        return rez;
    }

private:
    virtual SQLiteHelper& helper() const {
        return BaseTable::helper();
    }

    /*! @fn std::vector<std::string> addIDColumn(const std::vector<std::string>& i_columns)
     *  @brief add string "_id" in first place of input container
     *  @param i_columns[in] input name collection
     *  @return new collection with add "_id"
     */
    static std::vector<std::string> addIDColumn(const std::vector<std::string>& i_columns) {
        std::vector<std::string> columnsNames(1, "_id");
        for(std::vector<std::string>::const_iterator i = i_columns.begin(); i != i_columns.end(); ++i){
            columnsNames.push_back(*i);
        }
        return columnsNames;
    }

    void updateDBMetrics() const {
        BaseTable::m_helper.addQuery(SQLStringExpressionQuery("Select count(_id) count, max(_id) maxID FROM " + BaseTable::tableName()));
        SQLiteHelper::iterator id = BaseTable::getResult();
        SQLiteHelper::iterator::reference_type rezContainer(*id);
        SQLiteHelper::iterator::reference_type::iterator rezIterator = rezContainer.begin();
        std::string stringCountID = readData<std::string, int64_t>(
                rezContainer,
                rezIterator);
        m_dbRowCont = fromString<int64_t>(stringCountID.c_str());

        std::string stringMaxID = readData<std::string, int64_t>(
                rezContainer,
                ++rezIterator);
        m_dbMaxRowID = fromString<int64_t>(stringMaxID.c_str());
    }
private:
    mutable size_t m_dbRowCont;
    mutable size_t m_dbMaxRowID;
};

/*! @class template<class Table, int Column> struct ColumnType
 *  @brief get column type form SqlTable columns type collection
 *  @param Table Some of type which have a type collection which is named ColumnTypes
 *  @param Column number position asked Type.
 */
template<class Table, int Column>
struct ColumnType {
    typedef typename Loki::TL::TypeAt<
        typename Table::ColumnTypes,
            Column>::Result Type;
};

#endif //SQL_TABLE_H
