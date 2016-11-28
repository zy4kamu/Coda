

#include "SlotTransformer.h"

using std::unordered_map;
using std::string;
using std::wstring;
using std::vector;

SlotTransformer::SlotTransformer()
{
	//mapInt.insert(std::make_pair("convertHours", &SlotTransformer::convertHours));
	name_FUNC.insert(std::make_pair(L"normalize_time", &SlotTransformer::normalizeTime));
	name_FUNC.insert(std::make_pair(L"inflect_phrase", &SlotTransformer::inflectPhrase));
}

SlotTransformer::~SlotTransformer()
{
	// TODO Auto-generated destructor stub
}

wstring SlotTransformer::Transform(
		const wstring& function,
		const QUERY_MAP& fields,
		const vector<wstring>& specialParameters)
{
	unordered_map<wstring, FUNC>::iterator it = name_FUNC.find(function);
	if (it != name_FUNC.end())
	{
		return (this->*(it->second))(fields, specialParameters);
	}
	else
	{
		std::wcout << L"SlotTransformer::Transform: Wrong input function" << std::endl;
		throw "SlotTransformer::Transform: Wrong input function";
	}
}

wstring SlotTransformer::convertHours(const QUERY_MAP& fields, const vector<wstring>& specialParameters)
{
	int hour = getField<int>(0, fields);
	switch(hour)
	{
	case 0:
		return L"\u043f\u043e\u043b\u043d\u043e\u0447\u044c";//here was cyrrilic symbols: полночь
	case 1:
		return L"\u0447\u0430\u0441 \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: час,ноль,ноль
	case 2:
		return L"\u0434\u0432\u0430 \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: два,ноль,ноль
	case 3:
		return L"\u0442\u0440\u0438 \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: три,ноль,ноль
	case 4:
		return L"\u0447\u0435\u0442\u044b\u0440\u0435 \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: четыре,ноль,ноль
	case 5:
		return L"\u043f\u044f\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: пять,ноль,ноль
	case 6:
		return L"\u0448\u0435\u0441\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: шесть,ноль,ноль
	case 7:
		return L"\u0441\u0435\u043c\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: семь,ноль,ноль
	case 8:
		return L"\u0432\u043e\u0441\u0435\u043c\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: восемь,ноль,ноль
	case 9:
		return L"\u0434\u0435\u0432\u044f\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: девять,ноль,ноль
	case 10:
		return L"\u0434\u0435\u0441\u044f\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: десять,ноль,ноль
	case 11:
		return L"\u043e\u0434\u0438\u043d\u043d\u0430\u0434\u0446\u0430\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: одиннадцать,ноль,ноль
	case 12:
		return L"\u0434\u0432\u0435\u043d\u0430\u0434\u0446\u0430\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: двенадцать,ноль,ноль
	case 13:
		return L"\u0442\u0440\u0438\u043d\u0430\u0434\u0446\u0430\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: тринадцать,ноль,ноль
	case 14:
		return L"\u0447\u0435\u0442\u044b\u0440\u043d\u0430\u0434\u0446\u0430\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: четырнадцать,ноль,ноль
	case 15:
		return L"\u043f\u044f\u0442\u043d\u0430\u0434\u0446\u0430\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: пятнадцать,ноль,ноль
	case 16:
		return L"\u0448\u0435\u0441\u0442\u043d\u0430\u0434\u0446\u0430\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: шестнадцать,ноль,ноль
	case 17:
		return L"\u0441\u0435\u043c\u043d\u0430\u0434\u0446\u0430\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: семнадцать,ноль,ноль
	case 18:
		return L"\u0432\u043e\u0441\u0435\u043c\u043d\u0430\u0434\u0446\u0430\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: восемнадцать,ноль,ноль
	case 19:
		return L"\u0434\u0435\u0432\u044f\u0442\u043d\u0430\u0434\u0446\u0430\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: девятнадцать,ноль,ноль
	case 20:
		return L"\u0434\u0432\u0430\u0434\u0446\u0430\u0442\u044c \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: двадцать,ноль,ноль
	case 21:
		return L"\u0434\u0432\u0430\u0434\u0446\u0430\u0442\u044c \u043e\u0434\u0438\u043d \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: двадцать,один,ноль,ноль
	case 22:
		return L"\u0434\u0432\u0430\u0434\u0446\u0430\u0442\u044c \u0434\u0432\u0430 \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: двадцать,два,ноль,ноль
	case 23:
		return L"\u0434\u0432\u0430\u0434\u0446\u0430\u0442\u044c \u0442\u0440\u0438 \u043d\u043e\u043b\u044c \u043d\u043e\u043b\u044c";//here was cyrrilic symbols: двадцать,три,ноль,ноль
	default:
		std::wcout << L"SlotTransformer::convertHours: Wrong input fields" << std::endl;
		throw "SlotTransformer::convertHours: Wrong input fields";
	}
}

wstring SlotTransformer::normalizeTime(const QUERY_MAP& fields, const vector<wstring>& specialParameters)
{
	return CONVERT_TO_WSTRING(getField<string>(0, fields));
	//return L"Unimplemented: normalizeTime";
}

wstring SlotTransformer::inflectPhrase(const QUERY_MAP& fields, const vector<wstring>& specialParameters)
{
	return CONVERT_TO_WSTRING(getField<string>(0, fields));
	//return L"Unimplemented: inflectPhrase";
}
