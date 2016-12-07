//#include "dataTools.h"
//#include "windows.h"
//
//using std::vector;
//using std::wstring;
//using std::string;
//using std::unordered_set;
//
//namespace DataTools
//{
////Splits string with delimiter
//vector<wstring> split(const wstring& line, const wstring& delim)
//{
//	vector<wstring> res(0);
//	size_t current;
//	size_t next = -1;
//	do
//	{
//		current = next + 1;
//		next = line.find_first_of( delim, current );
//		res.push_back(line.substr( current, next - current ));
//	}
//	while (next != wstring::npos);
//	return res;
//}
//
////helper for set intersection
//unordered_set<wstring> set_intersect(const unordered_set<wstring>& a, const unordered_set<wstring>& b)
//{
//	unordered_set<wstring> c;
//
//	if(!b.empty())
//	{
//		for(auto ap = a.begin(); ap != a.end(); ap++)
//		{
//			if(b.find(*ap) != b.end())
//			{
//				c.insert(*ap);
//			}
//		}
//	}
//	return c;
//}
//
////helper for set difference
//unordered_set<wstring> set_difference(const unordered_set<wstring>& a, const unordered_set<wstring>& b)
//{
//	unordered_set<wstring> c;
//
//
//	for(auto ap = a.begin(); ap != a.end(); ap++)
//	{
//		if(b.empty() || b.find(*ap) == b.end())
//		{
//			c.insert(*ap);
//		}
//	}
//
//	return c;
//}
//
//bool set_equal(const unordered_set<wstring>& a, const unordered_set<wstring>& b)
//{
//	if(set_difference(a, b).empty())
//		return true;
//	return false;
//}
//
//bool DataTools::ConvertWstringToUTF8( const wstring& str, string& result )
//{
//	size_t bufferLength = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, 0, 0 );
//	char* buffer = (char*)malloc( bufferLength );
//	memset( buffer, 0, bufferLength );
//	if( !WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, buffer, bufferLength, 0, 0 ) )
//	{
//		printf( "%d\n", GetLastError() );
//		if( buffer )
//		{
//			free( buffer );
//		}
//		return false;
//	}
//	result = string( buffer );
//	if( buffer )
//	{
//		free( buffer );
//	}
//	return true;
//}
//
//std::wstring DataTools::ConvertUTF8StringToWstring( const char* str )
//{
//	size_t bufferLength = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0 );
//	wchar_t* buffer = (wchar_t*)malloc( sizeof( wchar_t ) * bufferLength );
//	memset( buffer, 0, bufferLength );
//	if( !MultiByteToWideChar( CP_UTF8, 0, str, -1, buffer, bufferLength ) )
//	{
//		printf( "%d\n", GetLastError() );
//		if( buffer )
//		{
//			free( buffer );
//		}
//		return wstring();
//	}
//	wstring result = wstring( buffer );
//	if( buffer )
//	{
//		free( buffer );
//	}
//	return result;
//
//}
//
//} //namespace Tools
