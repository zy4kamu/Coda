/*! @file data_base_schema.h
 *  @section Copyright
 *  Object Retrieval Engine
 *  @brief definition singleton which definition a schema of data base
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


#ifndef DATA_BASE_SCHEMA_H
#define DATA_BASE_SCHEMA_H

#include <fstream>
#include <cstdlib>
#include <stdint.h>
#include <map>
#include <string>
#include "sql_table_base.h"

/*! @class DataBaseSchema
 *  @brief a singleton to save a schema of data base
 */
class DataBaseSchema{
    DataBaseSchema(const DataBaseSchema& other){}
    DataBaseSchema()
    : m_databaseName("RetrivalApp.db")
    , m_serializerTablePrefix("serializing_"){}
    std::string m_databaseName;
    std::string m_serializerTablePrefix;
    std::map<std::string, std::shared_ptr<ISqlTable> > m_tablesCollections;
public:
    typedef std::map<std::string, std::shared_ptr<ISqlTable> >::iterator iterator;
    typedef std::map<std::string, std::shared_ptr<ISqlTable> >::const_iterator const_iterator;

    /*! @fn static DataBaseSchema& instance()
     *  @brief getter for instance of singleton
     *  @return reference to singleton
     */
    static DataBaseSchema& instance(){
        static DataBaseSchema singleton;
        return singleton;
    }

    /*! @fn const std::string& databaseName() const
     *  @brief getter for data base name
     */
    const std::string& databaseName() const {
        return m_databaseName;
    }

    /*! @fn const std::string& serializerTablePrefix() const
     *  @brief getter for prefix of table which contain serialized Blob data
     */
    const std::string& serializerTablePrefix() const {
        return m_serializerTablePrefix;
    }

    /*! @fn void setDatabaseName(const std::string& i_databaseName)
     *  @brief setter for data base name
     */
    void setDatabaseName(const std::string& i_databaseName){
        m_databaseName = i_databaseName;
    }

    /*! @fn void setSerializerTablePrefix(const std::string& i_serializerTableName)
     *  @brief setter for prefix of table which contain serialized Blob data
     */
    void setSerializerTablePrefix(const std::string& i_serializerTableName){
        m_serializerTablePrefix = i_serializerTableName;
    }

    std::string localFeaturesTableName(const std::string& i_detectorName) const {
        return "feature_" + i_detectorName;
    }
    std::string keyPointsTableName(const std::string& i_detectorName) const {
        return "key_point_" + i_detectorName;
    }
    std::string bindFeatureWithKeyPointTableName(const std::string& i_detectorName) const {
        return "bind_feature_and_key_points_" + i_detectorName;
    }
    /*! @fn bool registrationTable(const ISqlTable& table)
     *  @brief add new table into tables collection
     *  @param table[in] reference to added table
     *  @return true in case success
     */
    bool registrationTable(const ISqlTable& table) ;

    /*! @fn std::shared_ptr<ISqlTable> getTable(const std::string& i_tableName) const
     *  @brief get table from tables collection
     *  @param i_tableName[in] reference to string with the table name
     *  @return return shared pointer to asked data
     */
    std::shared_ptr<ISqlTable> getTable(const std::string& i_tableName) const ;

    /*! @fn template<class Table> std::shared_ptr<Table> getTable(const std::string& i_tableName) const
     *  @brief get table from tables collection
     *  @param Table type of Table
     *  @param i_tableName[in] reference to string with the table name
     *  @return return shared pointer to asked data which was casted to asked type
     */
    template<class Table>
    std::shared_ptr<Table> getTable(const std::string& i_tableName) const ;

    iterator begin() {
        return m_tablesCollections.begin();
    }

    iterator end() {
        return m_tablesCollections.end();
    }
    const_iterator begin() const {
        return m_tablesCollections.begin();
    }

    const_iterator end() const {
        return m_tablesCollections.end();
    }
};

inline bool DataBaseSchema::registrationTable(const ISqlTable& table) {
    //ISQLQuery* SQLTableQuery = (table.clone());
    ISqlTable* tableClone = dynamic_cast<ISqlTable*>(table.clone());//
    std::string fullTableName = databaseName() + "." + tableClone->tableName();
    std::pair<std::map<std::string, std::shared_ptr<ISqlTable> >::iterator, bool> insertRezult =
        m_tablesCollections.insert(std::make_pair(fullTableName,
            std::shared_ptr<ISqlTable>(tableClone)));
    return insertRezult.second;
}
inline std::shared_ptr<ISqlTable> DataBaseSchema::getTable(const std::string& i_tableName) const {
    std::string fullTableName = databaseName() + "." + i_tableName;
    std::map<std::string, std::shared_ptr<ISqlTable> >::const_iterator table =
        m_tablesCollections.find(fullTableName);
    if(table != m_tablesCollections.end()) {
        return table->second;
    }
    return std::shared_ptr<ISqlTable>();
}
template<class Table>
std::shared_ptr<Table> DataBaseSchema::getTable(const std::string& i_tableName) const {
    return std::dynamic_pointer_cast<Table>(getTable(i_tableName));
}

#endif //DATA_BASE_SCHEMA_H
