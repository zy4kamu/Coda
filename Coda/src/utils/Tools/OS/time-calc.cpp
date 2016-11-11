/**
* @file	time-calc.cpp
* @brief	source file for cross-platform handling of gettimeofday function
*/

#include "time-calc.h"

#ifdef MSVC

/*
* Both GetSystemTimeAsFileTime and GetSystemTimePreciseAsFileTime share a
* signature, so we can just store a pointer to whichever we find. This
* is the pointer's type.
*/
typedef VOID(WINAPI *PgGetSystemTimeFn)(LPFILETIME);

static void WINAPI init_gettimeofday(LPFILETIME lpSystemTimeAsFileTime);

/* Storage for the function we pick at runtime */
static PgGetSystemTimeFn pg_get_system_time = &init_gettimeofday;

/*
* One time initializer.  Determine whether GetSystemTimePreciseAsFileTime
* is available and if so, plan to use it; if not, fall back to
* GetSystemTimeAsFileTime.
*/
static void WINAPI init_gettimeofday(LPFILETIME lpSystemTimeAsFileTime)
{
	/*
	* Because it's guaranteed that kernel32.dll will be linked into our
	* address space already, we don't need to LoadLibrary it and worry about
	* closing it afterwards, so we're not using Pg's dlopen/dlsym() wrapper.
	*
	* We'll just look up the address of GetSystemTimePreciseAsFileTime if
	* present.
	*
	* While we could look up the Windows version and skip this on Windows
	* versions below Windows 8 / Windows Server 2012 there isn't much point,
	* and determining the windows version is its self somewhat Windows version
	* and development SDK specific...
	*/
	pg_get_system_time = (PgGetSystemTimeFn)GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")),
		"GetSystemTimePreciseAsFileTime");
	if (pg_get_system_time == NULL)
	{
		/*
		* The expected error from GetLastError() is ERROR_PROC_NOT_FOUND, if
		* the function isn't present. No other error should occur.
		*
		* We can't report an error here because this might be running in
		* frontend code; and even if we're in the backend, it's too early
		* to elog(...) if we get some unexpected error.  Also, it's not a
		* serious problem, so just silently fall back to
		* GetSystemTimeAsFileTime irrespective of why the failure occurred.
		*/
		pg_get_system_time = &GetSystemTimeAsFileTime;
	}

	(*pg_get_system_time)(lpSystemTimeAsFileTime);
}


/*
* timezone information is stored outside the kernel so tzp isn't used anymore.
*
* Note: this function is not for Win32 high precision timing purposes. See
* elapsed_time().
*/
int
gettimeofday(struct timeval * tp, struct timezone * tzp)
{
	FILETIME	file_time;
	ULARGE_INTEGER ularge;

	(*pg_get_system_time)(&file_time);
	ularge.LowPart = file_time.dwLowDateTime;
	ularge.HighPart = file_time.dwHighDateTime;

	tp->tv_sec = (long)((ularge.QuadPart - epoch) / FILETIME_UNITS_PER_SEC);
	tp->tv_usec = (long)(((ularge.QuadPart - epoch) % FILETIME_UNITS_PER_SEC)
		/ FILETIME_UNITS_PER_USEC);

	return 0;
}

#endif