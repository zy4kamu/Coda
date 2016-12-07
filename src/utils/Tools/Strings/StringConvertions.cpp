#include "StringConvertions.h"

#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include <iomanip>

#ifdef MSVC
    #include "windows.h"
#endif

using std::map;
using std::ofstream;
using std::wofstream;
using std::ifstream;
using std::wifstream;
using std::wcout;

namespace Tools
{

wstring ConvertWCharToWString(wchar_t ch)
{
    wstringstream ss;
    wstring s;
    ss << ch;
    ss >> s;
    return s;
}

wstring ConvertIntToWstring(int number)
{
    #ifdef MSVC
        if (number == 0) return L"0";
        if (number < 0) return L"-" + ConvertIntToWstring(-number);
        wstring retval = L"";
        while (number > 0)
        {
            int ost = number % 10;
            switch (ost)
            {
                case 0:
                {
                    retval = L"0" + retval;
                    break;
                }
                case 1:
                {
                    retval = L"1" + retval;
                    break;
                }
                case 2:
                {
                    retval = L"2" + retval;
                    break;
                }
                case 3:
                {
                    retval = L"3" + retval;
                    break;
                }
                case 4:
                {
                    retval = L"4" + retval;
                    break;
                }
                case 5:
                {
                    retval = L"5" + retval;
                    break;
                }
                case 6:
                {
                    retval = L"6" + retval;
                    break;
                }
                case 7:
                {
                    retval = L"7" + retval;
                    break;
                }
                case 8:
                {
                    retval = L"8" + retval;
                    break;
                }
                case 9:
                {
                    retval = L"9" + retval;
                    break;
                }
            }
            number /= 10;
        }
        return retval;
    #else
        return std::to_wstring(number);
    #endif
}

wstring ConvertDoubleToWstring(double number)
{
    return std::to_wstring(number);
}

wstring ConvertDoubleToWstring(double number, size_t precision)
{
    std::wostringstream out;
    out << std::setprecision(precision) << number;
    return out.str();
}

string ConvertIntToString(int number)
{
    #ifdef MSVC
        if (number == 0) return "0";
        if (number < 0) return "-" + ConvertIntToString(-number);
        string retval = "";
        while (number > 0)
        {
            int ost = number % 10;
            switch (ost)
            {
                case 0:
                {
                    retval = "0" + retval;
                    break;
                }
                case 1:
                {
                    retval = "1" + retval;
                    break;
                }
                case 2:
                {
                    retval = "2" + retval;
                    break;
                }
                case 3:
                {
                    retval = "3" + retval;
                    break;
                }
                case 4:
                {
                    retval = "4" + retval;
                    break;
                }
                case 5:
                {
                    retval = "5" + retval;
                    break;
                }
                case 6:
                {
                    retval = "6" + retval;
                    break;
                }
                case 7:
                {
                    retval = "7" + retval;
                    break;
                }
                case 8:
                {
                    retval = "8" + retval;
                    break;
                }
                case 9:
                {
                    retval = "9" + retval;
                    break;
                }
            }
            number /= 10;
        }
        return retval;
    #else
        return std::to_string(number);
    #endif
}

string ConvertDoubleToString(double number)
{
    return std::to_string(number);
}

string ConvertDoubleToString(double number, size_t precision)
{
    std::ostringstream out;
    out << std::setprecision(precision) << number;
    return out.str();
}

wstring ConvertStringToWstring(const string& str)
{
/*	wstring wstr;
    for(size_t i = 0; i < str.length(); ++i)
        wstr += wchar_t(str[i]);
    return wstr;*/
    #ifdef MSVC
        // set locale
        RussianLocale locale;

        std::wstring result;

        size_t bufferLength;
        mbstowcs_s( &bufferLength, NULL, 0, str.c_str(), -1 );
        wchar_t* resultBuffer = (wchar_t*)malloc( bufferLength * sizeof( wchar_t ) );
        memset( resultBuffer, 0, bufferLength );

        if( mbstowcs_s( &bufferLength, resultBuffer, bufferLength, str.c_str(), -1 ) )
        {
//			ReportError( L"Error while converting: " + wstring( resultBuffer ) );
            if( resultBuffer )
            {
                free( resultBuffer );
            }
            return false;
        }

        result = wstring( resultBuffer );
        if( resultBuffer )
        {
            free( resultBuffer );
        }
        return result;
    #else
        const size_t sz = str.length();
        if(sz == 0)
        {
            return std::wstring();
        }
        mbstate_t state = std::mbstate_t();
//        mbrlen(NULL,0,&state);
        const char *cnext;
        wchar_t *wnext;
        const char *cstr = str.c_str();
        wchar_t *buffer = new wchar_t[sz + 1];
        std::uninitialized_fill(buffer, buffer + sz + 1, 0);
        locale loc = std::locale("ru_RU.UTF-8");
        typedef std::codecvt<wchar_t, char, mbstate_t> cvt;
        cvt::result res;
        res = std::use_facet<cvt>(loc).in(state, cstr, cstr + sz, cnext, buffer, buffer + sz, wnext);
        std::wstring result(buffer);
        delete [] buffer;
        if(res == cvt::error)
        {
            return std::wstring();
        }
        return result;
    #endif
}

wstring ConvertCharPtrToWstring(const char* cstr)
{
#ifdef MSVC
    wcout << L"ConvertCharPtrToWstring: Not implemented exception";
    throw "ConvertCharPtrToWstring: Not implemented exception";
#else
    const size_t sz = strlen(cstr);
    if(sz == 0) return std::wstring();
    mbstate_t state;
    const char *cnext;
    wchar_t *wnext;
    wchar_t *buffer = new wchar_t[sz + 1];
    std::uninitialized_fill(buffer, buffer + sz + 1, 0);
    locale loc = std::locale("ru_RU.UTF-8");
    typedef std::codecvt<wchar_t, char, mbstate_t> cvt;
    cvt::result res;
    res = std::use_facet<cvt>(loc).in(state, cstr, cstr + sz, cnext, buffer, buffer + sz, wnext);
    std::wstring result(buffer);
    delete [] buffer;
    if(res == cvt::error) return std::wstring();
    else return result;
#endif
}

bool ConvertWstringToString( const std::wstring wstr, std::string& result,locale loc  )
{
    #ifdef MSVC
		size_t outputSize = wstr.length() + 1; // +1 for null terminator
		char * outputString = new char[outputSize];
		size_t charsConverted = 0;
		const wchar_t * inputW = wstr.c_str();
		wcstombs_s(&charsConverted, outputString, outputSize, inputW, wstr.length());
		result = string( outputString );
		delete [] outputString;
		return true;
    #else

	const size_t sz = wstr.length();
	if(sz == 0)
	{
		result = std::string();
	}

	//locale loc = std::locale("ru_RU.UTF-8");

	//typedef std::codecvt<wchar_t, char, mbstate_t> cvt;

	//    mbstate_t state;
	//    memset (&state,0,sizeof(state));
	mbstate_t state = std::mbstate_t();
	char *cnext;
	const wchar_t *wnext;
	const wchar_t *wcstr = wstr.c_str();


	std::codecvt<wchar_t, char, mbstate_t>::result res;
	size_t resLength = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >(loc).max_length() * sz;
	// extract current locale??
	char *buffer = new char[resLength + 1];
	std::uninitialized_fill(buffer, buffer + resLength, 0);
	res = std::use_facet< std::codecvt<wchar_t, char, mbstate_t> >(loc).out(state, wcstr, wcstr + sz, wnext, buffer, buffer + resLength, cnext);

	result = string(buffer);
	delete[] (buffer);
	if(res == std::codecvt<wchar_t, char, mbstate_t>::error)
	{
		return false;
	}
	return true;
#endif
}

string ToString(int number)
{
    #ifdef MSVC
        return std::to_string(static_cast<long double>(number));
    #else
        return std::to_string(number);
    #endif
}

wstring ToWstring(int number)
{
    #ifdef MSVC
        return std::to_wstring(static_cast<long double>(number));
    #else
        std::wstringstream ss;
        ss << number;
        return ss.str();
    #endif
}

wstring ConvertUTF8StringToWstring(const string& str, locale loc)
{
#ifdef MSVC
    size_t bufferLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0 );
    wchar_t* buffer = (wchar_t*)malloc( sizeof( wchar_t ) * bufferLength );
    memset( buffer, 0, bufferLength );
    if( !MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, buffer, bufferLength ) )
    {
        printf( "%d\n", GetLastError() );
        if( buffer )
        {
            free( buffer );
        }
        return wstring();
    }
    wstring result = wstring( buffer );
    if( buffer )
    {
        free( buffer );
    }
    return result;
#else
    const size_t sz = str.length();
    if(sz == 0)
    {
        return std::wstring();
    }
//    mbstate_t state;
//    memset (&state,0,sizeof(state));
    mbstate_t state = std::mbstate_t();
    //mbrlen(NULL,0,&state);
    const char *cnext;
    wchar_t *wnext;
    const char *cstr = str.c_str();
    wchar_t *buffer = new wchar_t[sz + 1];
    std::uninitialized_fill(buffer, buffer + sz + 1, 0);
    //locale loc = std::locale("ru_RU.UTF-8");

    typedef std::codecvt<wchar_t, char, mbstate_t> cvt;
    cvt::result res;
    res = std::use_facet<cvt>(loc).in(state, cstr, cstr + sz, cnext, buffer, buffer + sz, wnext);
    std::wstring result(buffer);
    delete [] buffer;
    if(res == cvt::error)
    {
        return std::wstring();
    }
    return result;
#endif
}

bool ConvertWstringToUTF8(const wstring& wstr, string& result, locale loc )
{
#ifdef MSVC
    size_t bufferLength = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, 0, 0 );
    char* buffer = (char*)malloc( bufferLength );
    memset( buffer, 0, bufferLength );
    if( !WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buffer, bufferLength, 0, 0 ) )
    {
        printf( "%d\n", GetLastError() );
        if( buffer )
        {
            free( buffer );
        }
        return false;
    }
    result = string( buffer );
    if( buffer )
    {
        free( buffer );
    }
    return true;
#else
    const size_t sz = wstr.length();
    if(sz == 0)
    {
        result = std::string();
    }

    //locale loc = std::locale("ru_RU.UTF-8");

    //typedef std::codecvt<wchar_t, char, mbstate_t> cvt;

//    mbstate_t state;
//    memset (&state,0,sizeof(state));
    mbstate_t state = std::mbstate_t();
    char *cnext;
    const wchar_t *wnext;
    const wchar_t *wcstr = wstr.c_str();


    std::codecvt<wchar_t, char, mbstate_t>::result res;
    size_t resLength = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >(loc).max_length() * sz;
    // extract current locale??
    char *buffer = new char[resLength+1];
    std::uninitialized_fill(buffer, buffer + resLength+1, 0);
    res = std::use_facet< std::codecvt<wchar_t, char, mbstate_t> >(loc).out(state, wcstr, wcstr + sz, wnext, buffer, buffer + resLength, cnext);

    result = string(buffer);
    delete[] (buffer);
    if(res == std::codecvt<wchar_t, char, mbstate_t>::error)
    {
        return false;
    }
    return true;
#endif
}

void Convert(const char* from, int size, string& to)
{
    to = "";
    stringstream ss;
    for (int j = 0; j < size; ++j)
        ss << from[j];
    for (int j = 0; j < size; ++j)
        ss >> to;
}

void Convert(const wchar_t* from, int size, wstring& to)
{
    to = L"";
    wstringstream ss;
#ifdef MSVC
    for (int j = 0; j < size*2; j += 2)
        ss << from[j];
#else
	for (int j = 0; j < size; ++j)
		ss << from[j];
#endif
    for (int j = 0; j < size; ++j)
        ss >> to;
}

bool ConvertToString( const std::wstring wstr, std::string& result )
{
    #ifdef MSVC
        // set locale
        RussianLocale locale;

        result.erase();

        size_t bufferLength = 0;
        wcstombs_s( &bufferLength, NULL, 0, wstr.c_str(), -1 );

        char* resultBuffer = (char*)malloc( bufferLength );
        memset( resultBuffer, 0, bufferLength );

        if( wcstombs_s( &bufferLength, resultBuffer, bufferLength, wstr.c_str(), -1 ) )
        {
            std::wcout << L"Error while converting: " + wstr ;
            if( resultBuffer )
            {
                free( resultBuffer );
            }
            return false;
        }
        result = string( resultBuffer );
        if( resultBuffer )
        {
            free( resultBuffer );
        }
        return true;
    #else
// number 1
        const size_t sz = wstr.length();
        if(sz == 0)
        {
            result = std::string();
            return true;
        }

        locale loc = std::locale("ru_RU.UTF-8");

        //typedef std::codecvt<wchar_t, char, mbstate_t> cvt;

        mbstate_t state;
        memset (&state,0,sizeof(state));

        char *cnext;
        const wchar_t *wnext;
        const wchar_t *wcstr = wstr.c_str();


        std::codecvt<wchar_t, char, mbstate_t>::result res;
        size_t resLength = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >(loc).max_length() * sz;
        // extract current locale??
        char *buffer = new char[resLength + 1];
        std::uninitialized_fill(buffer, buffer + resLength, 0);
        res = std::use_facet< std::codecvt<wchar_t, char, mbstate_t> >(loc).out(state, wcstr, wcstr + sz, wnext, buffer, buffer + resLength, cnext);

        result = string(buffer);
        delete[] (buffer);
        if(res == std::codecvt<wchar_t, char, mbstate_t>::error)
        {
            return false;
        }
        return true;
#endif
}

wstring ConvertToWstring( const string str )
{
    #ifdef MSVC
        // set locale
        RussianLocale locale;

        std::wstring result;

        size_t bufferLength;
        mbstowcs_s( &bufferLength, NULL, 0, str.c_str(), -1 );
        wchar_t* resultBuffer = (wchar_t*)malloc( bufferLength * sizeof( wchar_t ) );
        memset( resultBuffer, 0, bufferLength );

        if( mbstowcs_s( &bufferLength, resultBuffer, bufferLength, str.c_str(), -1 ) )
        {
            std::wcout << L"Error while converting: " + wstring( resultBuffer );
            if( resultBuffer )
            {
                free( resultBuffer );
            }
            return false;
        }

        result = wstring( resultBuffer );
        if( resultBuffer )
        {
            free( resultBuffer );
        }
        return result;
    #else

        const size_t sz = str.length();
        if(sz == 0)
        {
            return std::wstring();
        }
        mbstate_t state = std::mbstate_t();
//		mbrlen(NULL,0,&state);
        const char *cnext;
        wchar_t *wnext;
        const char *cstr = str.c_str();
        wchar_t *buffer = new wchar_t[sz + 1];
        std::uninitialized_fill(buffer, buffer + sz + 1, 0);
        locale loc = std::locale("ru_RU.UTF-8");
        typedef std::codecvt<wchar_t, char, mbstate_t> cvt;
        cvt::result res;
        res = std::use_facet<cvt>(loc).in(state, cstr, cstr + sz, cnext, buffer, buffer + sz, wnext);
        std::wstring result(buffer);
        delete [] buffer;
        if(res == cvt::error)
        {
            return std::wstring();
        }
        return result;

    #endif
}

bool IntToWstring(const int number, wstring& result)
{
//	#if defined(COMPILER_MVS2010)
//		int strlen = 2;
//		int num = number/10;
//		while ( (num >= 1) || (num <= -1) )
//		{
//			strlen++;
//			num = num/10;
//		}
//		wchar_t* buffer = (wchar_t*)malloc( (strlen + 2)*sizeof(wchar_t) );
//		if (_itow_s(number, buffer, strlen+2 , 10))
//		{
//			printf( "%d\n", GetLastError() );
//			free(buffer);
//			return false;
//		}
//		result = wstring(buffer);
//		free(buffer);
//		return true;
//	#elif defined(COMPILER_GPP47)
//		wcout << "IntToWstring: WINDOWS DEPENDENT FUNCTION";
//		throw "IntToWstring: WINDOWS DEPENDENT FUNCTION";
//	#endif
//	wcout << "IntToWstring: WRONG COMPILER HEADER";
//	throw "IntToWstring: WRONG COMPILER HEADER";
    std::wstringstream ss;
    ss<<number;
    result = wstring( ss.str() );
    return true;
}

bool ConvertToInt(const wstring& str, int& number)
{
    wchar_t* endptr;
    number = wcstol(str.c_str(),&endptr,10);
    if ( endptr == str.c_str())
    {
        return false;
    }
    if ( *endptr != '\0')
    {
        return false;
    }
    return true;
}

bool ConvertToInt(const string& str, int& number)
{
    char* endptr;
    number = strtol(str.c_str(),&endptr,10);
    if ( endptr==str.c_str() )
    {
        return false;
    }
    if ( *endptr !='\0' )
    {
        return false;
    }
    return true;
}


};

