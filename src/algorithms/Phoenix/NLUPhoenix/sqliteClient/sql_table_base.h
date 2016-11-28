/*! @file sql_table_base.h
 *  @section Copyright
 *  Object Retrieval Engine
 *  @brief defenition base class for all implementation of SqlTable.
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


#ifndef SQL_TABLE_BASE_H
#define SQL_TABLE_BASE_H

#include "sqlite_string_processor_base.h"
#include "sqlite_helper.h"
#include "make_queries.h"
#include <memory>


class ISqlTable: public virtual ISQLQuery {
public:
    /*! @fn virtual const std::string& tableName() const
     *  @brief getter for table name
     *  @return reference to std::string which contain of table name
     */
    virtual const std::string& tableName() const =0;

    /*! @fn virtual const std::vector<std::string>& columnNames() const
     *  @brief getter for column name collection
     *  @return reference to std::vector<std::string> which contain of names of columns
     */
    virtual const std::vector<std::string>& columnNames() const =0;

    /*! @fn virtual const std::vector<std::string>& primaryKeyNames() const
     *  @brief getter for primary key name collection
     *  @return reference to std::vector<std::string> which contain of names of primary key columns
     */
    virtual std::vector<std::string> primaryKeyNames() const =0;

    /*! @fn virtual const std::string& tableConstrain() const
     *  @brief getter for table constrain
     *  @return reference to std::string which contain of other table constrain
     */
    virtual const std::string& tableConstrain() const =0;

    /*! @fn static int64_t columnTotal()
     *  @brief columns total
     *  @return count of columns
     */
    virtual int64_t columnTotal() const =0;

    /*! @fn int64_t rowTotal() const
     *  @brief row total
     *  @return count of row
     */
    virtual int64_t rowTotal() const =0;

    virtual void clear() =0;

    virtual bool isEmpty() const =0;

    /*! @fn virtual void deleteRow(const SQLPredicateAscedColumnQuery& i_predicate)
     *  @brief send queri into db to delete all row which safity to this predicate
     *  @param i_predicate[in] predicate which describe row to delete
     */
    virtual void deleteRow(const SQLPredicateAscedColumnQuery& i_predicate) =0;

    /*! @fn virtual void flush(RowID i_key)
     *  @brief commit all ready rows into DB
     */
    virtual bool execute() const =0;

    template<class TList>
    std::list<DataTuple<TList> > get (const std::string& i_selectQuery) {
        execute();
        try {
            SQLStringExpressionQuery select(i_selectQuery);
            this->helper().addQuery(select);
            SQLiteHelper::iterator idata = getResult();
            std::list<DataTuple<TList> >rezult;
			do {
                SQLiteHelper::iterator::reference_type rezData = (*idata);
				SQLiteHelper::iterator::reference_type::iterator it = (rezData.begin());
				rezult.push_back(readData(
                       rezData,
					   it,
                       Loki::Type2Type<DataTuple<TList> >()));
                ++idata;
            } while(idata != helper().end());
            return rezult;
        }
        catch (SQLiteException e) {
            return std::list<DataTuple<TList> >();
        }
    }

    template<class OutTList, class AskTList>
    std::list<DataTuple<OutTList> > get(
            const DataTuple<AskTList>& i_key,
            std::vector<size_t> i_checkColumns,
            std::vector<size_t> i_requestColumns) const {

        assert(i_requestColumns.size() == DataTuple<OutTList>::Size );
        assert(i_checkColumns.size() == DataTuple<AskTList>::Size );

        execute();
        try {
            SQLSelectQuery select(makeSQLStringExspresionCollection(
                   getSubsetOfNames(columnNames(), i_requestColumns)),
               SQLFromQuery(SQLStringExpressionQuery(tableName())),
               SQLWhereQuery(makePredicate(SQLPredicateAscedColumnQuery::SQLPredicateOperator::EQUAL,
                       i_checkColumns,
                   i_key)));
            this->helper().addQuery(select);
            SQLiteHelper::iterator idata = getResult();
            std::list<DataTuple<OutTList> >rezult;
            do {
				SQLiteHelper::iterator::reference_type rezCollect = (*idata);
				SQLiteHelper::iterator::reference_type::iterator it = (rezCollect.begin());
                rezult.push_back(readData(
                        rezCollect,
                        it,
                        Loki::Type2Type<DataTuple<OutTList> >()));
                ++idata;
            } while(idata != helper().end());
            return rezult;
        }
        catch (SQLiteException e) {
            return std::list<DataTuple<OutTList> >();
        }
    }

protected:
    virtual SQLiteHelper& helper() const =0;

    template<class TList>
    SQLQueryCollections<std::vector<SQLPredicateAscedColumnQuery> > makePredicate(
            SQLPredicateAscedColumnQuery::SQLPredicateOperator i_operator,
            const std::vector<size_t>& columnsID,
            const DataTuple<TList>& i_key)  const {
        std::vector<std::shared_ptr<ISQLQuery> > key;
        read(key, i_key, Loki::Int2Type<DataTuple<TList>::Size - 1>());
        SQLQueryCollections<std::vector<SQLPredicateAscedColumnQuery> > result;
        for(int i = 0; i < key.size(); ++i) {
            result.push_back(SQLPredicateAscedColumnQuery(
                    SQLStringExpressionQuery(columnNames()[columnsID[i]]),
                    i_operator,
                    *(key[i])));
        }
        result.setSeparatorSimbol(" AND ");
        return result;
    }

    /*! @fn static std::vector<std::string> getSubsetOfNames(const std::vector<std::string>& i_columns,
           const std::vector<size_t>& i_IDs)
    *  @brief form vector names of primary key columns
    *  @param i_columns[in] input name collection
    *  @param i_IDs[in] ID names which shell be included into resalt collection
    *  @return new name collection
    */
   static std::vector<std::string> getSubsetOfNames(const std::vector<std::string>& i_columns,
           const std::vector<size_t>& i_IDs) {
       std::vector<std::string> columnsNames;
       for(std::vector<size_t>::const_iterator i = i_IDs.begin(); i != i_IDs.end(); ++i){
           columnsNames.push_back(i_columns[*i]);
       }
       return columnsNames;
   }
   /*! @fn SQLiteHelper::iterator::reference_type getResult() const
    *  @brief get copy of current read data from SQLLiteHelper
    *  @return copy to container which save current row
    */
   SQLiteHelper::iterator getResult() const {
       SQLiteHelper::iterator iresult = helper().begin();
       if(iresult == helper().end()) {
           throw SQLiteException();
       }
       SQLiteHelper::iterator::reference_type rezaltRow = *iresult;
       SQLiteHelper::iterator::reference_type::iterator idata = rezaltRow.begin();
       if(idata == rezaltRow.end()){
           throw SQLiteException();
       }
       helper().execute();
       return iresult;
   }
};


#endif //SQL_TABLE_BASE_H
