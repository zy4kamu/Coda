#include "serializer.h"

#include <iostream>

#include "loki/TypeManip.h"

//#include "logger.h"
#include "sqlite_exceptions.h"

#if 0
std::shared_ptr<SerializtionTable > getSerializationTable(const std::string& i_tableName){
    const std::string serializtionTableColumns[] = {
        "image_id",
        "key_name",
        "blob_type_name",
        "blob_data_size",
        "BLOB_column"
    };
    std::string serializeTableName = DataBaseSchema::instance().serializerTablePrefix() + i_tableName;

    std::shared_ptr<SerializtionTable> EXIFTable =
        DataBaseSchema::instance().getTable<SerializtionTable>(serializeTableName);

    if ( EXIFTable == std::shared_ptr<SerializtionTable>()) {
        std::shared_ptr<ISqlTable> parentTable = DataBaseSchema::nstance().getTable(i_tableName);
        std::shared_ptr<ISQLQuery> foreignKey =
            makeForeignKey(
                std::vector<std::string>(1, serializtionTableColumns[0]),
                parentTable->tableName(),
                std::vector<std::string>(1, parentTable->columnNames()[0]));
        std::string costrain = foreignKey->query();
        bool no_immmediatelyFlushing = false;
        DataBaseSchema::instance().registrationTable(
            SerializtionTable(
                serializeTableName,
                std::vector<std::string>(serializtionTableColumns,
                    serializtionTableColumns + sizeof(serializtionTableColumns)/sizeof(std::string)),
                size_t(1),
                costrain,
                no_immmediatelyFlushing));
        EXIFTable = DataBaseSchema::instance().getTable<SerializtionTable>(serializeTableName);
    }
    return EXIFTable;
}
static SerialisationData get(std::shared_ptr<SerializtionTable> table,
        const std::string& i_keyName,
        Loki::Int2Type<0>){
    SerialisationData res;
    ColumnType<SerializtionTable, 0>::Type currentData =
        table->get<ColumnType<SerializtionTable, 0>::Type>(
                    i_keyName, 0);
    res.set<0>(currentData);
    return res;
}

template<int Count>
static SerialisationData get(std::shared_ptr<SerializtionTable> table,
        const std::string& i_keyName,
        Loki::Int2Type<Count>){
    SerialisationData res = get(table, i_keyName, Loki::Int2Type<Count - 1>());
    typename ColumnType<SerializtionTable, Count>::Type currentData =
        table->template get<typename
            ColumnType<SerializtionTable, Count>::Type>(
                    i_keyName, Count);
    res.template set<Count>(currentData);
    return res;
}

static void set(std::shared_ptr<SerializtionTable> table,
        const std::string& i_keyName,
        const SerialisationData& i_data,
        Loki::Int2Type<0>){
    table->insert(i_keyName, 0, i_data.get<0>());
}

template<int Count>
static void set(std::shared_ptr<SerializtionTable> table,
        const std::string& i_keyName,
        const SerialisationData& i_data,
        Loki::Int2Type<Count>){
    set(table, i_keyName, i_data, Loki::Int2Type<Count - 1>());
    table->insert(i_keyName, Count, i_data.get<Count>());
}

bool readSerializationData(const std::string& i_tableName, const std::string& i_keyName, SerialisationData& o_value){
    try {
        std::shared_ptr<SerializtionTable> featureTable = getSerializationTable(i_tableName);
        static const int LastColumn= SerialisationData::Size - 1;
        o_value = get(featureTable, i_keyName, Loki::Int2Type<LastColumn>());
    }
    catch (const SQLiteException&) {
        return false;
    }
    return true;
}

bool writeSerializationData(const std::string& i_tableName, const std::string& i_keyName, const SerialisationData& i_value){
    try {
        std::shared_ptr<SerializtionTable> featureTable = getSerializationTable(i_tableName);
        if(featureTable.empty()) {
            featureTable = getSerializationTable(i_tableName);
        }
        set(featureTable, i_keyName, i_value, Loki::Int2Type<SerialisationData::Size - 1>());
    }
    catch (const SQLiteException&) {
        return false;
    }
    return true;
}
#endif
