#include "DbTables.h"
#include <memory>
#include <string>

namespace Phoenix
{
//Afisha table***************************************************************

std::shared_ptr<ProgrammesTable > getProgrammesTable(const std::string& i_tableName)
{
static const std::string columnNames[] = {"Id",
		"channel_name",
		"Time",
		"Overview",
		"Detail"
};

std::shared_ptr<ProgrammesTable> table =
			DataBaseSchema::instance().getTable<ProgrammesTable>(i_tableName);

if ( table == std::shared_ptr<ProgrammesTable>()) {
		DataBaseSchema::instance().registrationTable(
				ProgrammesTable(
						i_tableName,
						std::vector<std::string>(columnNames, columnNames + sizeof(columnNames)/sizeof(std::string)),
						size_t(0)));
		table = DataBaseSchema::instance().getTable<ProgrammesTable>(i_tableName);
	}
	return table;
}

//***************************************************************************

} //namespace Phoenix

