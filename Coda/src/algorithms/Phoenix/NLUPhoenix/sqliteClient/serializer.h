/*! @file serializer.h
 *  @section Copyright
 *  Object Retrieval Engine
 *  @brief definition templates method to save blob into sql bd.
 *  @author Moiseenko Andrey <moiseenko.a@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *
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


#include <functional>
#include <memory>
#include "loki/TypeTraits.h"
#include "blob.h"
#include "sqlite_helper.h"
#include "make_queries.h"
#include "data_base_schema.h"

#ifndef SERIALIZER_H
#define SERIALIZER_H

template<class ColumnTypeList, class RowID>
class SqlTable;

typedef Loki::TL::MakeTypelist<std::string, std::string, int64_t, Blob>::Result BaseSerializationTableColumnsTypes;

template<class ForeignKey>
struct SerializationTableColumnsTypes {
    typedef typename Type2TypeList<ForeignKey>::Result ForeignKeyTypes;
    typedef typename Loki::TL::Append<ForeignKeyTypes,
            BaseSerializationTableColumnsTypes>::Result ColumnTypes;
    enum SerialisationColumn{
        ReferencKey = 0,
        KeyDataName = Loki::TL::Length<typename SerializationTableColumnsTypes<ForeignKey>::ForeignKeyTypes>::value,
        TypeName,
        BlobDataSize,
        BlobData
    };
};

template<class ForeignKey>
struct SerializationTable {
    typedef SqlTable<typename SerializationTableColumnsTypes<ForeignKey>::ColumnTypes,
            std::string> Table;

};

struct ISQLTableExecutor {
    virtual ~ISQLTableExecutor() {}
    virtual void execute() const =0;
};
template<class Table>
class SQLTableExecutor: public ISQLTableExecutor{
    std::shared_ptr<Table> m_table;
    typename Table::PrimaryKey m_key;
public:
    SQLTableExecutor(std::shared_ptr<Table> i_table, typename Table::PrimaryKey i_key)
    : m_table(i_table)
    , m_key(i_key) {}
    virtual void execute() const {
        m_table->execute(m_key);
    }
};
typedef DataTuple<Loki::TL::MakeTypelist<std::shared_ptr<ISQLQuery>, std::shared_ptr<ISQLTableExecutor> >::Result > WritedData;

enum WritedDataFields{
    CurrentRowData,
    DependenceCall
};
//class SQLiteException{};

/*! @fn template<class T>
 *      std::string serializationTypeName(Loki::Type2Type<T>)
 *  @brief getter for string with type name
 *  @param [in] a obj of Loki::Type2Type with ascked Type
 *  @return string with type name
 *  @note this function shell be definition for every class which is needed to  serializing
 */
template<class T>
std::string serializationTypeName(Loki::Type2Type<T>);

/*! @fn template<class T>
        Blob serialization(const T& i_value)
 *  @brief serializing Mat into Blob
 *  @param i_value[in] Mat which need to serializing
 *  @return blob which contain serializing result
 */
template<class T>
Blob serialization(const T& i_value);

/*! @fn template<class T>
        T deserialization(const Blob& i_value, Loki::Type2Type<T>)
 *  @brief deserialization Blob to Mat
 *  @param i_value[in] Blob with data to deserialize
 *  @param [in] a obj of Loki::Type2Type with ascked Type
 *  @return deserialized value
 */
template<class T>
T deserialization(const Blob& i_value, Loki::Type2Type<T>);

template<class ForeignKey>
struct SerialisationData {
    typedef DataTuple<typename SerializationTableColumnsTypes<ForeignKey>::ColumnTypes> ColumnData;
    static
    typename SerialisationData<ForeignKey>::ColumnData
    get(std::shared_ptr<typename SerializationTable<ForeignKey>::Table > table,
            const std::string& i_keyName,
            Loki::Int2Type<0>){
        typename SerialisationData<ForeignKey>::ColumnData res;
        typedef typename Loki::TL::TypeAt<typename SerializationTableColumnsTypes<ForeignKey>::ColumnTypes ,
                0>::Result Data;
        Data currentData = table->get<Data>(i_keyName, 0);
        res.set<0>(currentData);
        return res;
    }

    template<int Count>
    static typename SerialisationData<ForeignKey>::ColumnData
    get(std::shared_ptr<typename SerializationTable<ForeignKey>::Table > table,
            const std::string& i_keyName,
            Loki::Int2Type<Count>){
        typename SerialisationData<ForeignKey>::ColumnData res =
                get(table, i_keyName, Loki::Int2Type<Count - 1>());
        typedef typename Loki::TL::TypeAt<typename SerializationTableColumnsTypes<ForeignKey>::ColumnTypes ,
                Count>::Result Data;
        Data currentData = table->template get<Data>(i_keyName, Count);
        res.template set<Count>(currentData);
        return res;
    }

    static void set(std::shared_ptr<typename SerializationTable<ForeignKey>::Table > table,
            const std::string& i_keyName,
            const typename SerialisationData<ForeignKey>::ColumnData& i_data,
            Loki::Int2Type<0>){
        table->insert(i_keyName, 0, i_data.get<0>());
    }

    template<int Count>
    static void set(std::shared_ptr<typename SerializationTable<ForeignKey>::Table> table,
            const std::string& i_keyName,
            const typename SerialisationData<ForeignKey>::ColumnData& i_data,
            Loki::Int2Type<Count>){
        set(table, i_keyName, i_data, Loki::Int2Type<Count - 1>());
        table->insert(i_keyName, Count, i_data.get<Count>());
    }

};
/*! @fn std::shared_ptr<SerializationTable > getSerializationTable(const std::string& i_tableName)
 *  @brief get pointer to table which contains serialised data
 *  @param i_tableName[in] name of table.
 *  @return shared pointer to data table which is contained into DataBaseSchema
 */
template<class ForeignKey>
std::shared_ptr<typename SerializationTable<ForeignKey>::Table >
getSerializationTable(const std::string& i_tableName){
    typedef typename SerializationTable<ForeignKey>::Table Table;
    const std::string SerializationTableColumns[] = {
        "key_name",
        "blob_type_name",
        "blob_data_size",
        "BLOB_column"
    };
    std::string serializeTableName = DataBaseSchema::instance().serializerTablePrefix() + i_tableName;

    std::shared_ptr<Table> EXIFTable =
        DataBaseSchema::instance().getTable<Table>(serializeTableName);

    if ( EXIFTable == std::shared_ptr<Table>()) {
        std::shared_ptr<ISqlTable> parentTable = DataBaseSchema::instance().getTable(i_tableName);
        bool no_immmediatelyFlushing = false;
        std::vector<std::string> ForegnKeyNames = parentTable->primaryKeyNames();
        std::vector<std::string> columnNames(ForegnKeyNames.begin(), ForegnKeyNames.end());
        for (int i = 0; i < sizeof(SerializationTableColumns)/ sizeof(std::string); ++i) {
            columnNames.push_back(SerializationTableColumns[i]);
        }
        std::shared_ptr<ISQLQuery> foreignKey =
        makeForeignKey(
            ForegnKeyNames,
            parentTable->tableName(),
            ForegnKeyNames);
        std::string costrain = foreignKey->query();

        DataBaseSchema::instance().registrationTable(
                Table(
                serializeTableName,
                columnNames,
                size_t(SerializationTableColumnsTypes<ForeignKey>::KeyDataName),
                costrain,
                no_immmediatelyFlushing));
        EXIFTable = DataBaseSchema::instance().getTable<Table>(serializeTableName);
    }
    return EXIFTable;
}
/*! @fn bool readGlobalFeature(const std::string& i_tableName, int64_t i_imageID, cv::Mat& o_value)
 *  @brief read global feature from data base
 *  @param i_tableName[in] table name
 *  @param i_imageID[in] id of image which global feature is asked
 *  @param o_value[out] Mat which contains global feature
 *  @return true in case success read
 */
template<class ForeignKey>
bool readSerializationData(
        const std::string& i_tableName,
        const std::string& i_keyName,
        typename SerialisationData<ForeignKey>::ColumnData& o_value){
    try {
        std::shared_ptr<typename SerializationTable<ForeignKey>::Table> featureTable = getSerializationTable<ForeignKey>(i_tableName);
        static const int LastColumn= SerialisationData<ForeignKey>::ColumnData::Size - 1;
        o_value = SerialisationData<ForeignKey>::get(featureTable, i_keyName, Loki::Int2Type<LastColumn>());
    }
    catch (const SQLiteException&) {
        return false;
    }
    return true;
}

/*! @fn bool writeGlobalFeature(const std::string& i_tableName, int64_t i_imageID, const cv::Mat& i_value)
 *  @brief write global feature into DB
 *  @param i_tableName[in] table name
 *  @param i_imageID[in] id of image which global feature is saved
 *  @param i_value[in] global feature which shell be saved
 *  @return true in case success write
 */
template<class ForeignKey>
bool writeSerializationData(
        const std::string& i_tableName,
        const std::string& i_keyName,
        const typename SerialisationData<ForeignKey>::ColumnData& i_value) {
    try {
        std::shared_ptr<typename SerializationTable<ForeignKey>::Table> featureTable =
                getSerializationTable<ForeignKey>(i_tableName);
        SerialisationData<ForeignKey>::set(featureTable, i_keyName, i_value, Loki::Int2Type<SerialisationData<ForeignKey>::ColumnData::Size - 1>());
    }
    catch (const SQLiteException&) {
        return false;
    }
    return true;
}

const int gc_blobSizeThreshold = 100000;

/*! @fn template<class T>
        void serialization(const std::string& i_tableName, const std::string& i_keyName, const T& i_value)
 *  @brief template of writing serialized data into DB
 *  @param i_tableName[in] table name
 *  @param i_keyName[in] unique for the table name which identifies the data (for Ex. value name)
 *  @param i_value in value which need to serializing
*/
template<class T, class ForeignKey>
std::shared_ptr<ISQLTableExecutor> serialization(const std::string& i_tableName,
        DataTuple<ForeignKey> i_parentID,
        const std::string& i_keyName,
        const T& i_value)
{
    std::string typeName = serializationTypeName(Loki::Type2Type<T>());
    Blob blob = serialization(i_value);
    int64_t sizeData = blob.size();
    if (sizeData >= gc_blobSizeThreshold) {
        blob = writeBlob2File(i_keyName + ".bld", blob);
    }
    typename SerialisationData<ForeignKey>::ColumnData serialiseData;
    serialiseData.SerialisationData<ForeignKey>::ColumnData::template set<SerializationTableColumnsTypes<ForeignKey>::ReferencKey>(i_parentID);
    serialiseData.SerialisationData<ForeignKey>::ColumnData::template set<SerializationTableColumnsTypes<ForeignKey>::KeyDataName>(i_keyName);
    serialiseData.SerialisationData<ForeignKey>::ColumnData::template set<SerializationTableColumnsTypes<ForeignKey>::TypeName>(typeName);
    serialiseData.SerialisationData<ForeignKey>::ColumnData::template set<SerializationTableColumnsTypes<ForeignKey>::BlobDataSize>(sizeData);
    serialiseData.SerialisationData<ForeignKey>::ColumnData::template set<SerializationTableColumnsTypes<ForeignKey>::BlobData>(blob);
    writeSerializationData<ForeignKey>(i_tableName, i_keyName, serialiseData);
    std::shared_ptr<typename SerializationTable<ForeignKey>::Table> featureTable =
                    getSerializationTable<ForeignKey>(i_tableName);
    return std::shared_ptr<ISQLTableExecutor>(
            new SQLTableExecutor<typename SerializationTable<ForeignKey>::Table>(featureTable, i_keyName));
}

template<class T>
std::shared_ptr<ISQLTableExecutor> serialization(const std::string& i_tableName,
        int64_t i_parentID,
        const std::string& i_keyName,
        const T& i_value)
{
    DataTuple<typename Type2TypeList<int64_t>::Result > parentID;
    parentID.template set<0>(i_parentID);
    return serialization(i_tableName,
        parentID,
        i_keyName,
        i_value);
}

template<class IdentificationData>
inline std::string uniqueDataNameGen(const std::string& i_tableName,
        IdentificationData i_dataID,
        const std::string& i_columnName){
    return i_tableName + std::string("_") + i_columnName + std::string("_") + toString(i_dataID);
}

template<int NotIntegral, int NotReal>
void UndefineDeserializationMethod(std::shared_ptr<ISQLQuery> i_value,
        Loki::Int2Type<NotIntegral> IsIntegral,
        Loki::Int2Type<NotReal> isReal);

template<class Return>
Return UndefineDeserializationMethod(std::shared_ptr<ISQLQuery> i_value, Loki::Int2Type<1>IsIntegral, Loki::Int2Type<0> isReal){
    std::shared_ptr<SQLLiteralQuery<int64_t> > blob = std::dynamic_pointer_cast<SQLLiteralQuery<int64_t> >(i_value);
    return Return(blob->value());
}

template<class Return>
Return UndefineDeserializationMethod(std::shared_ptr<ISQLQuery> i_value, Loki::Int2Type<0>IsIntegral, Loki::Int2Type<1> isReal){
    std::shared_ptr<SQLLiteralQuery<double> > blob = std::static_pointer_cast<SQLLiteralQuery<double> >(i_value);
    return Return(blob->value());
}

/*! @fn template<class T>
    T deserialization(const T& i_value, Loki::Int2Type<1> isFundamental)
 *  @brief overload deserialization data for integral type
 *  @param i_value[in] reference to a integral data
 *  @param [in] indicate of integral type
 *  @return deserialized value
 */
template<class T, class RowID>
T deserialization(const std::string& i_tableName,
        RowID i_dataID,
        const std::string& i_columnName,
        std::shared_ptr<ISQLQuery> i_value,
        Loki::Type2Type<T>,
        Loki::Int2Type<1> isFundamental){
    return UndefineDeserializationMethod<T>(i_value,
            Loki::Int2Type<Loki::TypeTraits<T>::isIntegral >(),
            Loki::Int2Type<Loki::TypeTraits<T>::isFloat >());
}

/*! @fn template<class T>
T deserialization(const std::string& i_value, Loki::Int2Type<0> isFundamental)
 *  @brief doverload deserialization string data
 *  @param i_value[in] Blob with data to deserialize
 *  @param [in] indicate of no integral type
 *  @return deserialized value
 */
template<class RowID>
inline std::string deserialization(const std::string& i_tableName,
        RowID i_dataID,
        const std::string& i_columnName,
        std::shared_ptr<ISQLQuery> i_value,
        Loki::Type2Type<std::string>,
        Loki::Int2Type<0> isFundamental){
			std::shared_ptr<SQLLiteralQuery<std::string> > blob = std::dynamic_pointer_cast<SQLLiteralQuery<std::string> >(i_value);
    return blob->value();
}

/*! @fn template<class T>
T deserialization(const std::string& i_value, Loki::Int2Type<0> isFundamental)
 *  @brief doverload deserialization string data
 *  @param i_value[in] Blob with data to deserialize
 *  @param [in] indicate of no integral type
 *  @return deserialized value
 */
template<class RowID>
inline Blob deserialization(const std::string& i_tableName,
        RowID i_dataID,
        const std::string& i_columnName,
        std::shared_ptr<ISQLQuery> i_value,
        Loki::Type2Type<Blob>, Loki::Int2Type<0> isFundamental){
    std::shared_ptr<SQLLiteralQuery<Blob> > blob = std::static_pointer_cast<SQLLiteralQuery<Blob> >(i_value);
    return blob->value();
}

/*! @fn template<class T>
    T deserialization(const T& i_value, Loki::Int2Type<1> isFundamental)
 *  @brief overload deserialization data for integral type
 *  @param i_value[in] reference to a integral data
 *  @param [in] indicate of integral type
 *  @return deserialized value
 */

template<class T>
T deserialization(std::shared_ptr<ISQLQuery> i_value, Loki::Type2Type<T>, Loki::Int2Type<1> isFundamental){
    std::shared_ptr<SQLLiteralQuery<T> > blob = std::static_pointer_cast<SQLLiteralQuery<T>>(i_value);
    return blob->value();
}

/*! @fn template<class T>
T deserialization(const Blob& i_value, Loki::Int2Type<0> isFundamental)
 *  @brief deserialization Blob to T
 *  @param i_value[in] Blob with data to deserialize
 *  @param [in] indicate of no integral type
 *  @return deserialized value
 */

template<class T>
T deserialization(std::shared_ptr<ISQLQuery> i_value, Loki::Type2Type<T>, Loki::Int2Type<0> isFundamental){
    std::shared_ptr<SQLLiteralQuery<Blob> > blob = std::static_pointer_cast<SQLLiteralQuery<Blob> >(i_value);
    return deserialization(blob->value(), Loki::Type2Type<T>());
}


/*! @fn template<class T>
T deserialization(const std::string& i_value, Loki::Int2Type<0> isFundamental)
 *  @brief doverload deserialization string data
 *  @param i_value[in] Blob with data to deserialize
 *  @param [in] indicate of no integral type
 *  @return deserialized value
 */
inline std::string deserialization(std::shared_ptr<ISQLQuery> i_value, Loki::Type2Type<std::string>, Loki::Int2Type<0> isFundamental){
    std::shared_ptr<SQLLiteralQuery<std::string> > blob = std::dynamic_pointer_cast<SQLLiteralQuery<std::string> >(i_value);
    return blob->value();
}

/*! @fn template<class T>
T deserialization(const std::string& i_value, Loki::Int2Type<0> isFundamental)
 *  @brief doverload deserialization string data
 *  @param i_value[in] Blob with data to deserialize
 *  @param [in] indicate of no integral type
 *  @return deserialized value
 */
inline Blob deserialization(std::shared_ptr<ISQLQuery> i_value, Loki::Type2Type<Blob>, Loki::Int2Type<0> isFundamental){
    std::shared_ptr<SQLLiteralQuery<Blob> > blob = std::dynamic_pointer_cast<SQLLiteralQuery<Blob> >(i_value);
    return blob->value();
}

/*! @fn template<class T>
        T readData(SQLiteHelper::iterator::reference_type& i_value,
        SQLiteHelper::iterator::reference_type::iterator& i_valueIterator)
 *  @brief the template gets data from tuple which returns from DB
 *  @param i_value[in] tuple with data
 *  @param i_valueIterator[in] iterator to got data
 *  @return data
 */
template<class T, class RowID>
T readData(const SQLiteHelper::iterator::reference_type& i_value,
        const SQLiteHelper::iterator::reference_type::iterator& i_valueIterator,
        const std::string& i_tableName = "",
        RowID i_dataID = RowID(),
        const std::string& i_columnName = "") {
    if(i_valueIterator == i_value.end()) {
        throw SQLiteException();
    }
    return deserialization(i_tableName,
            i_dataID,
            i_columnName,
            (*i_valueIterator)->value(),
            Loki::Type2Type<T>(),
            Loki::Int2Type<Loki::TypeTraits<T>::isFundamental >());
}

/*! @fn template<class T>
        T readData(SQLiteHelper::iterator::reference_type& i_value,
        SQLiteHelper::iterator::reference_type::iterator& i_valueIterator)
 *  @brief the template gets data from tuple which returns from DB
 *  @param i_value[in] tuple with data
 *  @param i_valueIterator[in] iterator to got data
 *  @return data
*/
template<class T>
T readData(const SQLiteHelper::iterator::reference_type& i_value,
        const SQLiteHelper::iterator::reference_type::iterator& i_valueIterator,
        const std::string& i_tableName = "",
        int64_t i_dataID = 0,
        const std::string& i_columnName = "") {
    if(i_valueIterator == i_value.end()) {
        throw SQLiteException();
    }

    return deserialization(i_tableName,
            i_dataID,
            i_columnName,
            (*i_valueIterator)->value(),
            Loki::Type2Type<T>(),
            Loki::Int2Type<Loki::TypeTraits<T>::isFundamental >());
}

template<class TList>
DataTuple<TList> readData(const SQLiteHelper::iterator::reference_type& i_value,
        SQLiteHelper::iterator::reference_type::iterator& i_valueIterator,
        Loki::Int2Type<0>) {
    DataTuple<TList> rez;
    typedef typename Loki::TL::TypeAt<TList, 0>::Result Data;
    rez.DataTuple<TList>::template set<0>(readData<Data>(
            i_value,
            i_valueIterator));
    i_valueIterator++;
    return rez;
}

template<class TList, int Count>
DataTuple<TList> readData(const SQLiteHelper::iterator::reference_type& i_value,
        SQLiteHelper::iterator::reference_type::iterator& i_valueIterator,
        Loki::Int2Type<Count>) {
    DataTuple<TList> rez = (readData<TList >(
                i_value,
                i_valueIterator,
                Loki::Int2Type<Count - 1>()));
    typedef typename Loki::TL::TypeAt<TList, Count>::Result Data;
    Data curData =
        readData<typename Loki::TL::TypeAt<TList, Count>::Result>(
            i_value,
            i_valueIterator);
    rez.DataTuple<TList>::template set<Count>(curData);
    i_valueIterator++;
    return rez;
}


/*! @fn template<class T>
        T readData(SQLiteHelper::iterator::reference_type& i_value,
        SQLiteHelper::iterator::reference_type::iterator& i_valueIterator)
 *  @brief the template gets data from tuple which returns from DB
 *  @param i_value[in] tuple with data
 *  @param i_valueIterator[in] iterator to got data
 *  @return data
 */
template<class TList>
DataTuple<TList> readData(const SQLiteHelper::iterator::reference_type& i_value,
        const SQLiteHelper::iterator::reference_type::iterator& i_valueIterator,
        Loki::Type2Type<DataTuple<TList> >) {
    if(i_valueIterator == i_value.end()) {
        throw SQLiteException();
    }
    SQLiteHelper::iterator::reference_type::iterator iteratorData(i_valueIterator);
    return readData<TList >(i_value,
            iteratorData,
            Loki::Int2Type<DataTuple<TList>::Size - 1>());
}

/*! @fn template<class T>
        std::shared_ptr<ISQLQuery> serialization(const std::string& i_dataName,
            const T& i_value,
            Loki::Int2Type<1> isFundamental)
 *  @brief overload of serialization for fundamental data
 *  @param i_value[in] Mat which need to serializing
 *  @return blob which contain serializing result
 */
template<class T, class RowID>
WritedData serialization(const std::string& i_tableName,
        RowID dataID,
        const std::string& i_columnName,
        const T& i_value,
        Loki::Int2Type<1> isFundamental){
    WritedData rez;
    rez.set<CurrentRowData>(new SQLColumnDataQuery<T>(i_columnName, i_value));
    return rez;
}

/*! @fn template<class T>
        std::shared_ptr<ISQLQuery> serialization(const std::string& i_dataName,
            const T& i_value,
            Loki::Int2Type<1> isFundamental)
 *  @brief overload of serialization for string data
 *  @param i_value[in] Mat which need to serializing
 *  @return blob which contain serializing result
 */
template<class RowID>
WritedData serialization(const std::string& i_tableName,
        RowID dataID,
        const std::string& i_columnName,
        const std::string& i_value,
        Loki::Int2Type<0> isFundamental){
    WritedData rez;
    rez.WritedData::template set<CurrentRowData>(std::shared_ptr<ISQLQuery>(new SQLColumnDataQuery<std::string>(i_columnName, i_value)));
    return rez;
}

/*! @fn template<class T>
        std::shared_ptr<ISQLQuery> serialization(const std::string& i_dataName,
            const T& i_value,
            Loki::Int2Type<1> isFundamental)
 *  @brief overload of serialization for Blob data
 *  @param i_value[in] Mat which need to serializing
 *  @return blob which contain serializing result
 */
template<class RowID>
WritedData serialization(const std::string& i_tableName,
        RowID dataID,
        const std::string& i_columnName,
        const Blob& i_value,
        Loki::Int2Type<0> isFundamental){
    WritedData rez;
    rez.WritedData::template set<CurrentRowData>(std::shared_ptr<ISQLQuery>(new SQLColumnDataQuery<Blob>(i_columnName, i_value)));
    return rez;
}

/*! @fn template<class T>
        std::shared_ptr<ISQLQuery> serialization(const std::string& i_dataName,
            const T& i_value,
            Loki::Int2Type<1> isFundamental)
 *  @brief overload of serialization for Blob data
 *  @param i_value[in] Mat which need to serializing
 *  @return blob which contain serializing result
 */
template<class OtherDataType, class RowID>
WritedData serialization(const std::string& i_tableName,
        RowID i_dataID,
        const std::string& i_columnName,
        const OtherDataType& i_value,
        Loki::Int2Type<0> isFundamental){
    WritedData rez;
    rez.WritedData::template set<DependenceCall>(serialization(i_tableName,
            i_dataID,
            uniqueDataNameGen(i_tableName, i_dataID, i_columnName),
            i_value));
    rez.WritedData::template set<CurrentRowData>(std::shared_ptr<ISQLQuery>(
            new SQLColumnDataQuery<std::string>(i_columnName, uniqueDataNameGen(i_tableName, i_dataID, i_columnName))));
    return rez;
}

/*! @fn template<class T>
T writeData(const T& i_value)
 *  @brief the template create data to insert in DB
 *  @param i_dataName[in] column name of the data
 *  @param i_value[in] data to save
 *  @return sharet pinter to SQLColumnDataQuery
 */
template<class T, class RowID>
WritedData writeData(const std::string& i_tableName,
        RowID i_dataID,
        const std::string& i_columnName,
        const T& i_value) {
    return serialization(i_tableName, i_dataID, i_columnName, i_value,
            Loki::Int2Type<Loki::TypeTraits<T>::isFundamental >());
}

/*! @fn template<class T>
        T deserialization(const std::string& i_tableName, const std::string& keyName, Loki::Type2Type<T>)
 *  @brief the template reads a blob from DB and deserializes of the blob
 *  @param i_tableName[in] table name. The name will be increased by prefix
 *  @param i_keyName[in] unique for the table name which identifies the data (for Ex. value name)
 *  @param [in] a obj of Loki::Type2Type with ascked Type
 *  @return deserialized read data
 */
template<class T, class TList>
T deserialization(const std::string& i_tableName,
        DataTuple<TList> i_parentID,
        const std::string& i_keyName,
        Loki::Type2Type<T>) {
    typename SerialisationData<TList>::ColumnData serialisData;
    if (readSerializationData<TList>(i_tableName, i_keyName, serialisData) == false) {
        return T();
    }
    int64_t blobSize = serialisData.template get<SerializationTableColumnsTypes<TList>::BlobDataSize>();
    Blob blob = serialisData.template get<SerializationTableColumnsTypes<TList>::BlobData>();
    if (blobSize > gc_blobSizeThreshold) {
        blob = readBlobFromFile(i_keyName + ".bld", blobSize);
    }
    return deserialization(blob, Loki::Type2Type<T>());
}

template<class T>
T deserialization(const std::string& i_tableName,
        int64_t i_parentID,
        const std::string& i_keyName,
        Loki::Type2Type<T>){
    DataTuple<typename Type2TypeList<int64_t>::Result > parentID;
    parentID.template set<0>(i_parentID);
    return deserialization(i_tableName, parentID, i_keyName, Loki::Type2Type<T>());
}

/*! @fn template<class T>
T deserialization(const Blob& i_value, Loki::Int2Type<0> isFundamental)
 *  @brief deserialization Blob to T
 *  @param i_value[in] Blob with data to deserialize
 *  @param [in] indicate of no integral type
 *  @return deserialized value
 */
template<class T, class RowID>
T deserialization(const std::string& i_tableName,
        RowID i_dataID,
        const std::string& i_columnName,
        std::shared_ptr<ISQLQuery> i_value,
        Loki::Type2Type<T>,
        Loki::Int2Type<0> isFundamental){
    /*if (deserialization(i_tableName,
        i_dataID,
        i_columnName,
        i_value,
        Loki::Type2Type<std::string>(),
        isFundamental) != std::string("acacacac")){
        throw SQLiteException();
    }*/
    return deserialization<T>(i_tableName,
            i_dataID,
            uniqueDataNameGen(i_tableName, i_dataID, i_columnName),
            Loki::Type2Type<T>());
}

/*! @fn template<class T>
        void readSqlData(const std::string& i_tableName,
            const std::string& i_keyName,
            T& i_value)
 *  @brief read a data from DB
 *  @param i_tableName[in] table name
 *  @param i_keyName[in] the table unique name of the data
 *  @param o_value[out] read data
 */
template<class T>
void readSqlData(const std::string& i_tableName,
        const std::string& i_keyName,
        T& o_value) {
    std::string tableName = DataBaseSchema::instance().serializerTablePrefix() + i_tableName;
    std::shared_ptr<ISQLQuery> table[] = {
        makeSharedPtr(SQLCreatedPrimaryKeyColumnQuery("_id")),
        makeSharedPtr(SQLCreatedUniqueColumnQuery<std::string>("key_name")),
        makeSharedPtr(SQLCreatedColumnQuery<T>("data_column"))
    };
    static const int ColumnTotal = sizeof(table) / sizeof(table[0]);
    std::list<std::shared_ptr<ISQLQuery> >columnList(table, &table[ColumnTotal]);
    SQLQueryCollections<std::list<std::shared_ptr<ISQLQuery> > >rowCreator(columnList);

    SQLCreateTableQuery createQuery(SQLStringExpressionQuery(tableName), rowCreator);
    SQLiteHelper helper(DataBaseSchema::instance().databaseName(), createQuery);
    SQLSelectQuery select(SQLStringExpressionQuery("data_column"), SQLFromQuery(SQLStringExpressionQuery(tableName)),
            SQLWhereQuery(SQLPredicateAscedColumnQuery(
                    SQLPredicateAscedColumnQuery(SQLStringExpressionQuery("key_name"),
                            SQLPredicateAscedColumnQuery::SQLPredicateOperator::EQUAL,
                            SQLLiteralQuery<std::string>(i_keyName)))));
    helper.addQuery(select);
    SQLiteHelper::iterator iresult = helper.begin();
    if(iresult == helper.end()) {
        throw SQLiteException();
    }
    SQLiteHelper::iterator::reference_type rezaltRow = *iresult;
    SQLiteHelper::iterator::reference_type::iterator idata = rezaltRow.begin();
    if(idata == rezaltRow.end()){
        throw SQLiteException();
    }

    std::shared_ptr<SQLLiteralQuery<T> > blob =
    		std::static_pointer_cast<SQLLiteralQuery<T> >(
                    (*idata)->value());
    o_value = blob->value();
}

/*! @fn template<class T>
        void writeSqlData(const std::string& i_tableName,
            const std::string& i_keyName,
            const T& i_value)
 *  @brief write a data to DB
 *  @param i_tableName[in] table name
 *  @param i_keyName[in] the table unique name of the data
 *  @param i_value[in] wrote data
 */
template<class T>
void writeSqlData(const std::string& i_tableName,
        const std::string& i_keyName,
        const T& i_value) {
    std::string tableName = DataBaseSchema::instance().serializerTablePrefix() + i_tableName;
    std::shared_ptr<ISQLQuery> table[] = {
        makeSharedPtr(SQLCreatedPrimaryKeyColumnQuery("_id")),
        makeSharedPtr(SQLCreatedUniqueColumnQuery<std::string>("key_name")),
        makeSharedPtr(SQLCreatedColumnQuery<T>("data_column"))
    };
    static const int ColumnTotal = sizeof(table) / sizeof(table[0]);
    std::list<std::shared_ptr<ISQLQuery> >columnList(table, &table[ColumnTotal]);
    SQLQueryCollections<std::list<std::shared_ptr<ISQLQuery> > >rowCreator(columnList);
    SQLCreateTableQuery createQuery(SQLStringExpressionQuery(tableName), rowCreator);
    SQLiteHelper helper(DataBaseSchema::instance().databaseName(), createQuery);
    SQLColumnDataQueryCollections<List> rowData;
    SQLColumnDataQueryCollections<List>::ValueType keyName(
            new SQLColumnDataQuery<std::string>("key_name", i_keyName));
    rowData.push_back(SQLColumnDataQueryCollections<List>::ValueType(
                new SQLColumnDataQuery<T>("data_column", i_value)));
    SQLSelectQuery select(SQLStringExpressionQuery("data_column"), SQLFromQuery(SQLStringExpressionQuery(tableName)),
                SQLWhereQuery(SQLPredicateAscedColumnQuery(
                        SQLPredicateAscedColumnQuery(SQLStringExpressionQuery("key_name"),
                                SQLPredicateAscedColumnQuery::SQLPredicateOperator::EQUAL,
                                SQLLiteralQuery<std::string>(i_keyName)),
                        SQLPredicateAscedColumnQuery::SQLPredicateOperator::AND,
                        SQLPredicateAscedColumnQuery(SQLStringExpressionQuery("data_column"),
                                SQLPredicateAscedColumnQuery::SQLPredicateOperator::EQUAL,
                                SQLLiteralQuery<T>(i_value)))));
    helper.addQuery(select);
    SQLiteHelper::iterator iresult = helper.begin();
    if(iresult == helper.end()){
        rowData.push_front(keyName);
        helper.addQuery(SQLInsertQuery(SQLStringExpressionQuery(tableName), rowData));
    }
    else{
        helper.addQuery(SQLUpdateQuery(SQLStringExpressionQuery(tableName), rowData,
                SQLWhereQuery(SQLPredicateAscedColumnQuery(SQLStringExpressionQuery("key_name"),
                                                SQLPredicateAscedColumnQuery::SQLPredicateOperator::EQUAL,
                                                SQLLiteralQuery<std::string>(i_keyName)))));
    }
}

#endif //SERIALIZER_H
