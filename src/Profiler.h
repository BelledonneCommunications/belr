//
//  profiler.h
//  Quick and dirty profiler
//
//  Created by Niels Gabel on 9/1/08.
//
//	Copyright 2008-2011 Niels Gabel
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
//
//	http://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.

// To use:
// At function start, call ProfilerEnter() and pass your function name.
// Call ProfilerEnd() and pass your function name before your function returns
// ProfilerInit() installs an atexit() handler and will dump the profiler results to file
// when the program exits...

#if defined( PROFILING ) && PROFILING
#	warning Profiling ON!

#import <pthread.h>

struct ProfilerDataCounter
{
	struct ProfilerDataCounter * parent ;
	clock_t func_time ;
	unsigned long count ;
} ;

struct ProfilerData
{
	char * name ;
	
	struct ProfilerDataCounter counters[10] ;
} ;


extern pthread_key_t gProfilerDataPThreadKey ;
extern struct ProfilerData gProfilerTable[100] ;
extern struct ProfilerData * gProfilerTableLastUsedEntry ;
extern
#ifdef __cplusplus
"C"
#endif

void _ProfilerInit() ;

extern
#ifdef __cplusplus
"C"
#endif
void ProfilerOutputTree( FILE  * file ) ;

//extern clock_t gProfilerOverhead ;
//extern unsigned long gProfilerCallCount ;

#define ProfilerGetCurrent()	(struct ProfilerDataCounter*)pthread_getspecific( gProfilerDataPThreadKey )
#define ProfilerSetCurrent( __profiler_data )	pthread_setspecific( gProfilerDataPThreadKey, (const void*)__profiler_data )

#define ProfilerStackPush(  )	\
	struct ProfilerDataCounter * __profiler_parent = ProfilerGetCurrent() ;	\
	struct ProfilerDataCounter * __profiler_counter = & __profiler_data->counters[ 0 ] ;	\
	if ( __profiler_parent ) {	\
		while( __profiler_counter < & __profiler_data->counters[ 10 ] ) {	\
			++__profiler_counter ;	\
			if ( __profiler_counter->parent == __profiler_parent ) { break ; }	\
			else if ( !__profiler_counter->parent ) {	\
				__profiler_counter->parent = __profiler_parent ;	\
				__profiler_counter->func_time = 0 ;	\
				__profiler_counter->count = 0 ;	\
				break ;	\
			}	\
		}	\
	}	\
	ProfilerSetCurrent( __profiler_counter ) ;

#define ProfilerStackPop()	ProfilerSetCurrent( __profiler_parent ) ;

#define ProfilerEnter( __function_name )	\
	static struct ProfilerData * __profiler_data = NULL ;	\
	clock_t __profiler_start_time = clock() ;	\
	if ( !__profiler_data )	{ __profiler_data = gProfilerTableLastUsedEntry++ ; __profiler_data->name = (char*)__function_name ; }	\
	ProfilerStackPush() ;

#define ProfilerExit( __function_name )\
	__profiler_counter->func_time += clock() - __profiler_start_time ;\
	++__profiler_counter->count ;\
	ProfilerStackPop() ;

#define PSTART ProfilerEnter(__PRETTY_FUNCTION__) @try {{
#define PEND }} @finally { ProfilerExit( __PRETTY_FUNCTION__ ) ; }

#else
#define ProfilerEnter( __function_name ) {}
#define ProfilerExit( __function_name ) {}
#define PSTART
#define PEND

#endif
