// Cross Plataform Mutex Library based on https://www.codeproject.com/Articles/25569/Cross-Platform-Mutex
// and
// thread.h from https://github.com/mattiasgustavsson/libs/blob/master/docs/thread.md

#ifndef _OS_GENERIC_H_
#define _OS_GENERIC_H_

//==============================================================================
//                        HEADERS AND OS DEFINITIONS                                
//==============================================================================

#include <dirent.h>
#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#define OS_GENERIC_API static


//Headers
#ifdef __WINDOWS__
    #pragma comment( lib, "winmm.lib" )

    #define _CRT_NONSTDC_NO_DEPRECATE 
    #define _CRT_SECURE_NO_WARNINGS

    #define _WIN32_WINNT 0x0601 // Windows7
    #define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <windows.h>    


    #define _WINSOCKAPI_
    #pragma warning( push )
    #pragma warning( disable: 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
    #pragma warning( disable: 4255 )
    //#include <windows.h>
    #include <process.h>
    #include <mmsystem.h>
    #include <time.h>
	#include <io.h> 
	//#include  "stdatomic_win32.h"	

	#include "dirent.h"				//Windows version of unix dirent.h
	#define access    _access_s		//To check Files Exist
	

    #pragma warning( pop )  

#else /* !__WINDOWS__ */
    //#define __USE_MISC      //To declare Old c types...
    #include <sys/types.h>
    #include <sys/stat.h>
	#include <pthread.h>
    #include <sys/time.h>
    #include <time.h>
    #include <errno.h>
    #include <math.h>
    #include <ctype.h>
	#include <unistd.h>
	#include <limits.h>
	#include <strings.h>

#ifdef __cplusplus
	//#include <atomic>
	//using namespace std;
#else
	#include <stdatomic.h>
#endif	


#endif

    //Common to all system
	#include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
	#include <stdint.h> //for udint64_t
	#include"standartypes.h"

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
//                              MUTEX FUNCTIONS                                
//==============================================================================

#ifdef __WINDOWS__
typedef CRITICAL_SECTION os_mutex_t;
#else /* !__WINDOWS__ */
typedef  pthread_mutex_t os_mutex_t;
//#define OS_MUTEX pthread_mutex_t
#endif

OS_GENERIC_API int os_mutex_init(os_mutex_t *mutex)
{
#ifdef __WINDOWS__

	return(InitializeCriticalSectionAndSpinCount(mutex, 0x00000400) != 0);

#else /* !__WINDOWS__ */
	return pthread_mutex_init(mutex, NULL);
#endif
	return -1;
}

OS_GENERIC_API int os_mutex_lock(os_mutex_t *mutex)
{
#ifdef __WINDOWS__
	EnterCriticalSection(mutex);
	return 1;
#else /* !__WINDOWS__ */
	return pthread_mutex_lock(mutex);
#endif
	return -1;
}

OS_GENERIC_API int os_mutex_unlock(os_mutex_t *mutex)
{
#ifdef __WINDOWS__
	LeaveCriticalSection(mutex);
	return 1;
#else /* !__WINDOWS__ */
	return pthread_mutex_unlock(mutex);
#endif
	return -1;
}

OS_GENERIC_API int os_mutex_destroy(os_mutex_t *mutex)
{
#ifdef __WINDOWS__
	DeleteCriticalSection(mutex);
	return 1;
#else /* !__WINDOWS__ */
	return pthread_mutex_destroy(mutex);
#endif
	return -1;
}


// Spinlock
//-------------------------------------------------------------
// Spinlock
//-------------------------------------------------------------
//typedef volatile LONG os_spinlock_t;
#ifdef __WINDOWS__

typedef intptr_t os_spinlock_t; //atomic_bool

#define ___atomic_init(obj, value) \
 do {                            \
     *(obj) = (value);           \
 } while(0)

#define ___atomic_store(object, desired)   \
 do {                                    \
     *(object) = (desired);              \
     MemoryBarrier();                    \
 } while (0)

static int ___atomic_compare_exchange_strong(intptr_t *object, intptr_t *expected, intptr_t desired)
{
	intptr_t old = *expected;
	*expected = (intptr_t)InterlockedCompareExchangePointer(
		(PVOID *)object, (PVOID)desired, (PVOID)old);
	return *expected == old;
}


#else
typedef pthread_spinlock_t os_spinlock_t;
#endif


OS_GENERIC_API void os_spinlock_init(os_spinlock_t *lock)
{
	if (!lock)
		return;
#ifdef __WINDOWS__
	___atomic_init(lock, 0);
#else
	pthread_spin_init(lock, 1);
#endif
}

OS_GENERIC_API void os_spinlock_lock(os_spinlock_t *lock)
{
	if (!lock)
		return;

#ifdef WIN32

	//while (InterlockedCompareExchange(lock, 1, 0) == 0)	{;}

	if (!lock)
		return;

	if (!lock)
		return;

	os_spinlock_t des;
	___atomic_store(&des, 0);

	while (!___atomic_compare_exchange_strong(lock, &des, 1)) { ; };
#else

	if (!lock)
		return;
 	
	pthread_spin_lock(lock);

#endif // WIN32

	return;
}

OS_GENERIC_API void os_spinlock_unlock(os_spinlock_t *lock)
{
	if (!lock)
		return;
#ifdef WIN32
	___atomic_store(lock, 0);
#else
	pthread_spin_unlock(lock);
#endif // WIN32

return;

}


//==============================================================================
//                                 TIMERS                                      
//==============================================================================
OS_GENERIC_API void os_sleep_ns(unsigned long long ns)
{
	
#ifdef __WINDOWS__

	//got from :https://gist.github.com/Youka/4153f12cf2e17a77314c

	/* Declarations */
	HANDLE timer;	/* Timer handle */
	LARGE_INTEGER li;	/* Time defintion */
	/* Create timer */
	if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
		return;
	/* Set timer properties */
	li.QuadPart = -ns;
	if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
		CloseHandle(timer);
		return;
}
	/* Start & wait for timer */
	WaitForSingleObject(timer, INFINITE);
	/* Clean resources */
	CloseHandle(timer);
	/* Slept without problems */
	return;

#else
	struct timespec rem;
	struct timespec req;
	req.tv_sec = ns / 1000000000ULL;
	req.tv_nsec =  ns % 1000000000;
	clock_nanosleep(CLOCK_MONOTONIC, 0,&req, &rem);
		
#endif
}

OS_GENERIC_API void os_sleep_us(unsigned long long us)
{
	unsigned long long ns = us * 1000;
	os_sleep_ns(ns);
}

OS_GENERIC_API void os_sleep(unsigned int ms)
{
#ifdef __WINDOWS__
	return Sleep((DWORD)ms);
#else /* !__WINDOWS__ */
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;
	nanosleep(&ts, NULL);
#endif
}

//==============================================================================
//                                 TIME                                      
//==============================================================================
#ifdef __WINDOWS__
    typedef LARGE_INTEGER os_time_count_t;


	typedef struct _timeval {
		long tv_sec;
		long tv_usec;
	}os_timeval;

#else
    typedef struct timespec os_time_count_t;
	typedef struct timeval os_timeval;
#endif

#define OS_SEC_PER_DAY   86400
#define OS_SEC_PER_HOUR  3600
#define OS_SEC_PER_MIN   60


OS_GENERIC_API void os_time_get(os_time_count_t *now)
{
    #ifdef __WINDOWS__
        QueryPerformanceCounter(now);
    #else
        clock_gettime(CLOCK_MONOTONIC, now);
    #endif
}

OS_GENERIC_API long os_time_get_diff_us(os_time_count_t *start, os_time_count_t *end)
{
    #ifdef __WINDOWS__
        LARGE_INTEGER Frequency, elapsed;

        QueryPerformanceFrequency(&Frequency); 
        elapsed.QuadPart = end->QuadPart - start->QuadPart;

        elapsed.QuadPart *= 1000000;
        elapsed.QuadPart /= Frequency.QuadPart;

        return elapsed.QuadPart;
    #else
        
		os_time_count_t result;
  		result.tv_sec  = end->tv_sec  - start->tv_sec;
    	result.tv_nsec = end->tv_nsec - start->tv_nsec;
    	if (result.tv_nsec < 0) 
		{
        	--result.tv_sec;
        	result.tv_nsec += 1000000000L;
    	}

		return (result.tv_sec * 1000000 + (result.tv_nsec / 1000L));
    #endif
}

OS_GENERIC_API long os_time_get_diff_ms(os_time_count_t *start, os_time_count_t *end)
{
    #ifdef __WINDOWS__
        LARGE_INTEGER Frequency, elapsed;

        QueryPerformanceFrequency(&Frequency); 
        elapsed.QuadPart = end->QuadPart - start->QuadPart;

        elapsed.QuadPart *= 1000;
        elapsed.QuadPart /= Frequency.QuadPart;

        return elapsed.QuadPart;
    #else

        return ((end->tv_sec * 1000) + (end->tv_nsec / 1000000)) -
                ((start->tv_sec * 1000) + (start->tv_nsec / 1000000));
    #endif
}

OS_GENERIC_API int os_gettimeofday(os_timeval * tp)
{
#ifdef __WINDOWS__

	// Got from: https://stackoverflow.com/questions/10905892/equivalent-of-gettimeday-for-windows
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	// This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
	// until 00:00:00 January 1, 1970 

	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);

	return 0;
#else
	
	return gettimeofday(tp,NULL);
#endif

}

OS_GENERIC_API lreal_t os_gettimeofday_diff_ms(os_timeval start, os_timeval end)
{
	

	double start_us = (double)start.tv_sec * 1000000 + (double)start.tv_usec;
	double end_us = (double)end.tv_sec * 1000000 + (double)end.tv_usec;

	double diff = ((double)end_us - (double)start_us)/1000.0;

	return diff;

}

OS_GENERIC_API void os_gettimeofday_add_ms(os_timeval *time, long ms)
{
	if (time)
	{
		time->tv_sec += ms / 1000;
		time->tv_usec += (ms % 1000) * 1000;
	}
}

OS_GENERIC_API void os_gettimeofday_sub_ms(os_timeval *time, long ms)
{
	if (time)
	{
		time->tv_sec -= ms / 1000;
		time->tv_usec -= (ms % 1000) * 1000;
	}

}

OS_GENERIC_API void os_printf_timeofday(os_timeval time, char *str)
{
	if (!str)
		return;

	long hms = time.tv_sec % OS_SEC_PER_DAY;
	//hms += tz.tz_dsttime * SEC_PER_HOUR;
	//hms -= tz.tz_minuteswest * SEC_PER_MIN;

	int hour = hms / OS_SEC_PER_HOUR;
	int min = (hms % OS_SEC_PER_HOUR) / OS_SEC_PER_MIN;
	int sec = (hms % OS_SEC_PER_HOUR) % OS_SEC_PER_MIN; // or hms % SEC_PER_MIN
	int ms = time.tv_usec / 1000;

	sprintf(str,"%d:%02d:%02d.%03d", hour, min, sec, ms);
}

OS_GENERIC_API void os_set_system_date(int day, int month,int year)
{
#ifdef __WINDOWS__

	SYSTEMTIME  system_time;
	
	//Get Actual Time
	GetSystemTime(&system_time);
	
	//Set new Date
	system_time.wDay = iec_min(iec_max(1,day),31);
	system_time.wMonth = iec_min(iec_max(1, month), 12);
	system_time.wYear = iec_max(1601,year);
	SetSystemTime(&system_time);

#else

	//Get Actual Time in Seconds
	time_t t = time(NULL);

	//Convert to calendar
	struct tm system_time;
	system_time = *localtime(&t);

	system_time.tm_mday		= iec_min(iec_max(1, day), 31);
	system_time.tm_mon		= iec_min(iec_max(0, month - 1), 11);
	system_time.tm_year		= year - 1900;
	system_time.tm_isdst 	= -1;

	//Buil new time
	t = mktime(&system_time);
	//Error
	if (t == -1)
		return;

	struct timeval newTime;
	newTime.tv_sec = t;

	settimeofday(&newTime, NULL);


#endif
	   	 
	return;
}

OS_GENERIC_API void os_set_system_time(int hour, int minute, int second)
{
#ifdef __WINDOWS__

	SYSTEMTIME  system_time;

	//Get Actual Time
	GetSystemTime(&system_time);

	//Set new Date
	system_time.wHour = iec_min(iec_max(0, hour), 23);
	system_time.wMinute = iec_min(iec_max(0, minute), 59);
	system_time.wSecond = iec_min(iec_max(0, second), 59);
	SetSystemTime(&system_time);

#else

	//Get Actual Time in Seconds
	time_t t = time(NULL);

	//Convert to calendar
	struct tm system_time;
	system_time = *localtime(&t);

	system_time.tm_hour 	= iec_min(iec_max(0, hour), 23);
	system_time.tm_min 		= iec_min(iec_max(0, minute), 59);
	system_time.tm_sec 		= iec_min(iec_max(0, second), 59);
	system_time.tm_isdst 	= -1;

	//Buil new time
	t = mktime(&system_time);
	//Error
	if (t == -1)
		return;

	struct timeval newTime;
	newTime.tv_sec = t;

	settimeofday(&newTime, NULL);


#endif
	   	 
	return;
}


//=============================================================================
//                             SIGNALS / EVENTS                                      
//==============================================================================
typedef struct _os_signal_
{
    #if defined( _WIN32 )
        HANDLE event;   
    #else
        pthread_mutex_t mutex;
        pthread_cond_t condition;
        int value;
    #endif
}os_signal_t;


OS_GENERIC_API void os_signal_init( os_signal_t* signal )
{
        
    #ifdef __WINDOWS__
		signal->event = CreateEvent( NULL, FALSE, FALSE, NULL );
    #else
        pthread_mutex_init( &signal->mutex, NULL );
        pthread_cond_init( &signal->condition, NULL );
		signal->value = 0;
    #endif
}

OS_GENERIC_API void os_signal_destroy( os_signal_t* signal )
{
    #if defined( _WIN32 )
        CloseHandle(signal->event );
    #else
        pthread_mutex_destroy( &signal->mutex );
        pthread_cond_destroy( &signal->condition );
    #endif
}

OS_GENERIC_API void os_signal_raise( os_signal_t* signal )
{
    #if defined( _WIN32 )
        SetEvent(signal->event );
    #else
        //pthread_mutex_lock( &signal->mutex );
		signal->value = 1;
        //pthread_mutex_unlock( &signal->mutex );
        pthread_cond_signal( &signal->condition );
    #endif
}

OS_GENERIC_API int os_signal_wait( os_signal_t* signal, int timeout_ms )
{

    #if defined( _WIN32 )
        int failed = WAIT_OBJECT_0 != WaitForSingleObject(signal->event, timeout_ms < 0 ? INFINITE : timeout_ms );
        return !failed;  
    #else
        struct timespec ts;
        if( timeout_ms >= 0 )
            {
            struct timeval tv;
            gettimeofday( &tv, NULL );
            ts.tv_sec = time( NULL ) + timeout_ms / 1000;
            ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * ( timeout_ms % 1000 );
            ts.tv_sec += ts.tv_nsec / ( 1000 * 1000 * 1000 );
            ts.tv_nsec %= ( 1000 * 1000 * 1000 );
            }

        int timed_out = 0;
        //pthread_mutex_lock( &signal->mutex );
        while( signal->value == 0 )
            {
            if( timeout_ms < 0 ) 
                pthread_cond_wait( &signal->condition, &signal->mutex );
            else if( pthread_cond_timedwait( &signal->condition, &signal->mutex, &ts ) == ETIMEDOUT )
                {
                timed_out = 1;
                break;
                }

            }           
        if( !timed_out ) signal->value = 0;
        //pthread_mutex_unlock( &signal->mutex );
        return !timed_out;
    #endif
}

//==============================================================================
//                                 THREADS                                      
//==============================================================================
typedef void* os_thread_id_t;

OS_GENERIC_API os_thread_id_t os_thread_get_id( void )
{
    #ifdef __WINDOWS__
        return (void*) (uintptr_t)GetCurrentThreadId();
    #else
        return (void*) pthread_self();
    #endif
}

OS_GENERIC_API void os_thread_yield( void )
{
   #ifdef __WINDOWS__
        SwitchToThread();
    #else
        sched_yield();
    #endif
}

OS_GENERIC_API void os_thread_set_high_priority( void )
{
    #ifdef __WINDOWS__
        SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST );   
    #else
        struct sched_param sp;
        memset( &sp, 0, sizeof( sp ) );
        sp.sched_priority = sched_get_priority_min( SCHED_RR );
        pthread_setschedparam( pthread_self(), SCHED_RR, &sp);
    #endif
}

OS_GENERIC_API void os_thread_exit( int return_code )
{
    #ifdef __WINDOWS__
        ExitThread( (DWORD) return_code );   
    #else
        int ret = return_code;
        pthread_exit((void*)&ret);
    #endif
}


typedef void* os_thread_ptr_t;

OS_GENERIC_API os_thread_ptr_t os_thread_create( int (*thread_proc)( void* ), void* user_data, int stack_size )
{
    #ifdef __WINDOWS__

        DWORD thread_id;
        HANDLE handle = CreateThread( NULL, stack_size > 0 ? (size_t)stack_size : 0U, 
            (LPTHREAD_START_ROUTINE)(uintptr_t) thread_proc, user_data, 0, &thread_id );
        if( !handle ) return NULL;

        return (os_thread_ptr_t) handle;
    
    #else

        pthread_t thread;
        if( 0 != pthread_create( &thread, NULL, ( void* (*)( void * ) ) thread_proc, user_data ) )
            return NULL;     
                
        return (os_thread_ptr_t)thread;  
    #endif
}

OS_GENERIC_API os_thread_ptr_t os_thread_create_rt(int(*thread_proc)(void*), void* user_data)
{
#ifdef __WINDOWS__

	DWORD thread_id;
	HANDLE handle = CreateThread(NULL, 0U,
		(LPTHREAD_START_ROUTINE)(uintptr_t)thread_proc, user_data, 0, &thread_id);
	if (!handle) return NULL;

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

	return (os_thread_ptr_t)handle;

#else

	//Example got from: https://wiki.linuxfoundation.org/realtime/documentation/howto/applications/application_base?s[]=real&s[]=time&s[]=applications

	struct sched_param param;
	pthread_attr_t attr;
	pthread_t thread;

	/* Initialize pthread attributes (default values) */
	if (pthread_attr_init(&attr))
		return NULL;

	/* Set a specific stack size  */
	if (pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN))
		return NULL;

	/* Set scheduler policy and priority of pthread */
	if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO))
		return NULL;

	param.sched_priority = 99;
	if (pthread_attr_setschedparam(&attr, &param))
		return NULL;
	
	/* Use scheduling parameters of attr */
	if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED))
		return NULL;


	/* And finally start the actual thread */
	if (0 != pthread_create(&thread, &attr, (void* (*)(void *)) thread_proc, user_data))
		return NULL;

	return (os_thread_ptr_t)thread;
#endif
}

OS_GENERIC_API void os_thread_destroy( os_thread_ptr_t thread )
{
    #ifdef __WINDOWS__
        WaitForSingleObject(thread, INFINITE );
        CloseHandle(thread );
    #else
        pthread_join( (pthread_t) thread, NULL );
    #endif
}

OS_GENERIC_API int os_thread_join( os_thread_ptr_t thread )
{
    #ifdef __WINDOWS__
        WaitForSingleObject( (HANDLE) thread, INFINITE );
        DWORD retval;
        //GetExitCodeThread( (HANDLE) thread, &retval );
        return (int) retval;
    
    #else
        void* retval;
        pthread_join( (pthread_t) thread, &retval );
        //return (int)*retval;
		return 1;
    #endif
}

//Not suport on POSIX systems
OS_GENERIC_API int os_thread_is_busy(os_thread_ptr_t thread)
{

#ifdef __WINDOWS__
	DWORD h_process_command_thread_exit_code;
	int bret = GetExitCodeThread(thread, &h_process_command_thread_exit_code);
	return (h_process_command_thread_exit_code == STILL_ACTIVE);
#else
	int iret = pthread_tryjoin_np((pthread_t)thread, NULL);
	return ((iret != 0) && (iret != EBUSY));
#endif

}


//==============================================================================
//                                 ATOMICS                                      
//==============================================================================
typedef union _thread_atomic_int_t { void* align; long i;} os_thread_atomic_int_t;

OS_GENERIC_API int os_atomic_int_get( os_thread_atomic_int_t* atomic )
{
    #ifdef __WINDOWS__
        return InterlockedCompareExchange( &atomic->i, 0, 0 );  
    #else
        return (int)__sync_fetch_and_add( &atomic->i, 0 );
    #endif
}

OS_GENERIC_API void os_atomic_int_set( os_thread_atomic_int_t* atomic, int desired )
{
    #ifdef __WINDOWS__ 
        InterlockedExchange( &atomic->i, desired );
    #else
        __sync_lock_test_and_set( &atomic->i, desired );
        __sync_lock_release( &atomic->i );
    #endif
}

OS_GENERIC_API int os_atomic_int_inc( os_thread_atomic_int_t* atomic )
{
    #ifdef __WINDOWS__
        return InterlockedIncrement( &atomic->i ) - 1;  
    #else
        return (int)__sync_fetch_and_add( &atomic->i, 1 );
    #endif
}

OS_GENERIC_API int os_atomic_int_dec( os_thread_atomic_int_t* atomic )
{
    #ifdef __WINDOWS__
        return InterlockedDecrement( &atomic->i ) + 1;  
    #else
       return (int)__sync_fetch_and_sub( &atomic->i, 1 );
    #endif
}

OS_GENERIC_API int os_atomic_int_add( os_thread_atomic_int_t* atomic, int value )
{
    #ifdef __WINDOWS__
        return InterlockedExchangeAdd ( &atomic->i, value );  
    #else
        return (int)__sync_fetch_and_add( &atomic->i, value );   
    #endif
}

OS_GENERIC_API int os_atomic_int_sub( os_thread_atomic_int_t* atomic, int value )
{
    #ifdef __WINDOWS__
        return InterlockedExchangeAdd( &atomic->i, -value );  
    #else
        return (int)__sync_fetch_and_sub( &atomic->i, value );
    #endif
    }

OS_GENERIC_API int os_atomic_int_swap( os_thread_atomic_int_t* atomic, int desired )
{
	#ifdef __WINDOWS__
        return InterlockedExchange( &atomic->i, desired );
    #else
        int old = (int)__sync_lock_test_and_set( &atomic->i, desired );
        __sync_lock_release( &atomic->i );
        return old;
    #endif
}

OS_GENERIC_API int thread_atomic_int_compare_and_swap( os_thread_atomic_int_t* atomic, int expected, int desired )
{
	#ifdef __WINDOWS__ 
        return InterlockedCompareExchange( &atomic->i, desired, expected );
    #else
        return (int)__sync_val_compare_and_swap( &atomic->i, expected, desired );
    #endif
}

OS_GENERIC_API bool_t os_atomic_compare_exchange(long* ptr, long compare, long exchange)
{

#ifdef __WINDOWS__
	return InterlockedCompareExchange(ptr, exchange, compare);
#else
	return __atomic_compare_exchange_n(ptr, &compare, exchange, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#endif
}


//==============================================================================
//                                 STRINGS                                     
//==============================================================================

OS_GENERIC_API char * os_str_to_upper(char* s)
{
    int c = 0;
   
    while (s[c] != '\0') {
      if (s[c] >= 'a' && s[c] <= 'z') {
         s[c] = s[c] - 32;
      }
      c++;
   }

   return s;
}

//Compere 2 Strings Case Insensitive
//Return: True if Strigns are equal, otherwise false;
OS_GENERIC_API int os_str_icmp(const char* str1, const char *str2)
{
#ifdef  __WINDOWS__ 
	return (_stricmp(str1, str2)) == 0? true : false;
#else
	return (strcasecmp(str1, str2)) == 0 ? 1 : 0;
#endif
}

OS_GENERIC_API int os_str_insert(char *dest, size_t destsize, char *ins, size_t location)
{
	size_t origsize = 0;
	size_t resize = 0;
	size_t inssize = 0;

	if (!dest || !ins)
		return -1;  // invalid parameter

	if (strlen(ins) == 0)
		return -1; // invalid parameter

	origsize = strlen(dest);
	inssize = strlen(ins);
	resize = strlen(dest) + inssize + 1; // 1 for the null terminator

	if (location > origsize)
		return -1; // invalid location, out of original string

	// resize string to accommodate inserted string if necessary
	if (destsize < resize)
		dest = (char*)realloc(dest, resize);

	// move string to make room for insertion
	memmove(&(dest)[location + inssize], &(dest)[location], origsize - location);
	(dest)[origsize + inssize] = '\0'; // null terminate string

	// insert string
	memcpy(&(dest)[location], ins, inssize);

	return iec_max(destsize, resize); // return buffer size
}

OS_GENERIC_API void os_str_removeChars(char *s, char c)
{
	int writer = 0, reader = 0;

	while (s[reader])
	{
		if (s[reader] != c)
		{
			s[writer++] = s[reader];
		}

		reader++;
	}

	s[writer] = 0;
}

OS_GENERIC_API void os_str_replace_all(char *s, char from, char to)
{
	char *current_pos = strchr(s, from);
	while (current_pos) 
	{
		*current_pos = to;
		current_pos = strchr(current_pos, from);
	}
}

//==============================================================================
//                             FILE AND FODLERS                                   
//==============================================================================

//Save data to File in binary format
//Return: 1 if OK, or negative if error
OS_GENERIC_API int os_SaveToFile(const char* filepath, const char *data, long size)
{
	//Open file
	FILE *f = fopen(filepath, "wb");

	if (f)
	{
		fwrite(data, size, 1, f);
		fclose(f);
		return 1;
	}
	else
	{
		return 0;
	}
}

//Load data from file in binary format
//Return: Number of bytes readed
OS_GENERIC_API char* os_LoadFromFile(const char* filepath, long *datasize/*out*/)
{
	//Open file
	FILE *f = fopen(filepath, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

		char *data = (char *)malloc(fsize);
		fread(data, fsize, 1, f);
		fclose(f);

		if (datasize)
			*datasize = fsize;

		return data;
	}
	else
	{
		return NULL;
	}
	
}

//Read on line from File. Buffer size must be bigger than maximum line length
//Return: True if og a Line, false on end of file
OS_GENERIC_API int os_read_line(char *buff, int size, FILE *fp)
{
	//Code got from: http://faculty.salina.k-state.edu/tim/CMST302/study_guide/topic5/readline.html
	
	buff[0] = '\0';
	buff[size - 1] = '\0';             /* mark end of buffer */
	char *tmp;

	if (fgets(buff, size, fp) == NULL) 
	{
		*buff = '\0';                   /* EOF */
		return 0;
	}
	else {
		/* remove newline */
		if ((tmp = strrchr(buff, '\n')) != NULL) {
			*tmp = '\0';
		}
	}
	return 1;
}

//Check if a File Exist
OS_GENERIC_API int	os_FileExists(const char* filepath)
{
	if (!filepath)
		return 0;

	return access(filepath, 0) == 0;
}

OS_GENERIC_API int	os_FolderExists(const char* folderpath)
{
	if (!folderpath)
		return 0;

	DIR* dir = opendir(folderpath);
	if (dir) {
		closedir(dir);
		return 1;
	}
	else
	{
		return 0;
	}
}

//Check if File Exist and add file count to path Ex: C:\File(1).txt if c:\File.txt already exist
OS_GENERIC_API void os_CheckNewFileName(char* filepath)
{
	if (!filepath)
		return;

	int i = 1;
	char strTmp[256];
	char path[256];
	char format[256];
	char *dot_pos = NULL;
	//Get File Format
	dot_pos = strrchr(filepath, '.');

	if (dot_pos)
	{
		//Copy file Format
		strcpy(format, dot_pos);
		//Remove File format
		*dot_pos = '\0';
		//Copy path
		strcpy(path, filepath);
	}

	sprintf(strTmp, u8"%s%s", path, format);

	while (os_FileExists(strTmp))
	{
		sprintf(strTmp, u8"%s(%d)%s", path, i, format);
		i++;
	}

	strcpy(filepath, strTmp);

	return;
}

OS_GENERIC_API void os_CheckNewFolderName(char* folderpath)
{
	if (!folderpath)
		return;

	int i = 1;
	char strTmp[500];

	sprintf(strTmp, u8"%s", folderpath);

	while (os_FolderExists(strTmp))
	{
		sprintf(strTmp, u8"%s(%d)", folderpath, i);
		i++;
	}

	strcpy(folderpath, strTmp);

	return;
}

OS_GENERIC_API void os_RemoveFile(const char* path)
{
	if (access(path, 0) == 0)
	{
		remove(path);
	}
}

OS_GENERIC_API void os_RemoveFolder(const char* path)
{
	//remove using command line
	char cmd[1024];

#ifdef WIN32
	sprintf(cmd, u8"rmdir \"%s\" /s /q", path);
	system(cmd);

#else
	sprintf(cmd, "rm -r \"%s\" ", path);
	system(cmd);
#endif
	
	/*

	if (os_FolderExists(path))
	{
		DIR *dir = (NULL);
		dir = opendir(path);
		struct dirent *entry = NULL;
		DIR *dir = (NULL);
		dir = opendir(path);
		while ((entry = readdir(dir)) != NULL)
		{
			DIR *sub_dir = NULL;
			FILE *file = NULL;
			char abs_path[1024];
			abs_path[0] = '\0';

			//Try to find folder or File
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
			{
				//If not empty, push alarm
				closedir(dir);
				return -1;
			}
		}

		closedir(dir);

		if (dir)
		{
			closedir(dir);
#ifdef WIN32
			RemoveDirectoryA(path);
#else
			rmdir(path);
#endif
		}

		return 1;
	}*/
		/*

		//Got from: https://stackoverflow.com/questions/734717/how-to-delete-a-folder-in-c

		struct dirent *entry = NULL;
		DIR *dir = (NULL);
		dir = opendir(path);
		while ((entry = readdir(dir)) != NULL)
		{
			DIR *sub_dir = NULL;
			FILE *file = NULL;
			char abs_path[100];
			abs_path[0] = '\0';

			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
			{
				sprintf(abs_path, u8"%s\\%s", path, entry->d_name);
				if (sub_dir == opendir(abs_path))
				{
					closedir(sub_dir);
					os_RemoveFolder(abs_path);
				}
				else
				{
					if (file == fopen(abs_path, "r"))
					{
						fclose(file);
						remove(abs_path);
					}
				}
			}
		}
		//_wrmdir(path);
		//remove(path);
		if (dir)
		{
			closedir(dir);
#ifdef WIN32
			RemoveDirectoryA(path);
#else
			rmdir(path);
#endif
		}

		
	}
	*/
}

OS_GENERIC_API int os_RenameFile(const char* file, const char* newfile)
{
	return (rename(file, newfile) == 0);
}

OS_GENERIC_API int os_RenameFolder(const char* path, const char* newpath)
{
	return (rename(path, newpath) == 0);
}

OS_GENERIC_API char *os_GetFileName(char* path)
{
	//Got from: https://stackoverflow.com/questions/5901624/extract-file-name-from-full-path-in-c-using-msvs2005

#ifdef WIN32
	
	char *filename = strrchr(path, '\\');
	char* filename2 = strrchr(path, '/');
	if (filename == NULL && filename2 == NULL)
		return path;
	else if(filename)
		return ++filename;
	else if (filename2)
		return ++filename2;
	return path;

#else

	char *filename = strrchr(path, '\\');
	char* filename2 = strrchr(path, '/');
	if (filename == NULL && filename2 == NULL)
		return path;
	else if (filename)
		return filename++;
	else if (filename2)
		return filename2++;
	return path;

#endif


}

OS_GENERIC_API const char* os_GetFileExtension(const char* path)
{
	const char *dot = strrchr(path, '.');
	if (!dot || dot == path) return NULL;
	return dot + 1;
}

OS_GENERIC_API int os_CopyFile(const char* source, const char* dest)
{
	char ch;
	FILE *fpSource, *fpTarget;

	fpSource = fopen(source, "r");

	if (fpSource == NULL)
		return -1;

	fpTarget = fopen(dest, "w");

	if (fpTarget == NULL)
	{
		fclose(fpSource);
		return -2;
	}

	while ((ch = fgetc(fpSource)) != EOF)
		fputc(ch, fpTarget);

	fclose(fpSource);
	fclose(fpTarget);

	return 1;
	
}

OS_GENERIC_API int os_create_dir(const char* path)
{
#ifdef __WINDOWS__
	return CreateDirectory(path, NULL); //1 to sucefull, 0 on fail
#else
	
	return mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

OS_GENERIC_API void os_path_normalize(char* path)
{
	//Got from: https://stackoverflow.com/questions/32496497/standard-function-to-replace-character-or-substring-in-a-char-array/32496721

	char *current_pos = strchr(path, '\\');
	while (current_pos) 
	{
		*current_pos = '/';
		current_pos = strchr(current_pos, '\\');
	}


}


/****************************** end **************************/
#ifdef __cplusplus
}
#endif

#endif
