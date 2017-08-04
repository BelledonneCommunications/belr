//
//  profiler.m
//  Quick and dirty profiler
//
//  Created by Niels Gabel on 9/1/08.
//
//	Copyright 208-2011 Niels Gabel
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

#import "profiler.h"

#if defined( PROFILING ) && PROFILING

#ifndef __has_feature
// not LLVM Compiler
#define __has_feature(x) 0
#endif

#if __has_feature(objc_arc)
#	ifndef ARC
#		define ARC 1
#	endif
#else
#endif

/*
 To use:
 
 At function start, call ProfilerEnter() and pass your function name.
 Call ProfilerExit() and pass your function name before your function returns
 ProfilerInit() installs an atexit() handler and will dump the profiler results to file
 when the program exits...
 
 Define PROFILER_OUTPUT_TO_FILE if you want to append profiler output to "profiler_output.txt"
 in your app's Documents folder. You can retrieve the file using the Organizer in Xcode.
 (Click on your device, find it in the Applications list, turn down the disclosure triangle,
 and click the download arrow on the right.)
 If PROFILER_OUTPUT_TO_FILE is not defined, output written to stdout.
 */

//#import "utils.h"
//#import "debug.h"
#import <signal.h>

pthread_key_t gProfilerDataPThreadKey = 0 ;
struct ProfilerData	gProfilerTable[100] ;
struct ProfilerData * gProfilerTableLastUsedEntry = & gProfilerTable[0] ;

static NSString * GetDocumentsFolderPath()
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
    return basePath;
}

//clock_t gProfilerOverhead = 0 ;
//unsigned long gProfilerCallCount = 0 ;

static void ProfilerOutputTreeNode( FILE * file, struct ProfilerData * data, struct ProfilerDataCounter * counter, int indent )
{
	{
		float time = (float)counter->func_time / (float)CLOCKS_PER_SEC ;
		fprintf( file, "%*s%-*s %15.5f %15lu %10.5f\n", indent * 4, "", 80 - (indent * 4 ), data->name, time, counter->count, time / (float)counter->count ) ;
	}
	
	for( struct ProfilerData * testData = & gProfilerTable[0]; testData < gProfilerTableLastUsedEntry; ++testData )
	{
		if ( !testData->name )
		{
			break ;
		}
		
		if ( testData != data )
		{
			for( struct ProfilerDataCounter * testCounter = & testData->counters[1] ; testCounter < &testData->counters[10]; ++testCounter )
			{
				if ( !testCounter->parent )
				{
					break ;
				}
				
				if ( testCounter->parent == counter )
				{
					ProfilerOutputTreeNode( file, testData, testCounter, indent + 1 ) ;
				}
			}
		}
	}
}

void
ProfilerOutputTree( FILE  * file )
{
	fprintf( file, "%-80s %15s %15s %10s\n", "FUNCTION", "TOTAL TIME", "COUNT", "AVG. TIME") ;
	
	for( struct ProfilerData * data = & gProfilerTable[0]; data < gProfilerTableLastUsedEntry; ++data )
	{
		if ( data->counters[0].count > 0 )
		{
			ProfilerOutputTreeNode( file, data, & data->counters[0], 0 ) ;
		}
	}
}

static void
ProfilerExitFunction()
{
	DebugLog(@"%s\n", __PRETTY_FUNCTION__ ) ;

	@autoreleasepool {
	#ifdef PROFILER_OUTPUT_TO_FILE
		FILE * file = fopen( [ [ GetDocumentsFolderPath() stringByAppendingPathComponent:@"profiler_output.txt" ] UTF8String ], "a" ) ;
		if ( !file )
		{
			file = stdout ;
		}
	#else
		FILE * file = stdout ;
	#endif
		
		fprintf( file, "%s %s\n", __DATE__, __TIME__ ) ;
		//	{
		//		float time = (float)gProfilerOverhead / (float)CLOCKS_PER_SEC ;
		//
		//		fprintf( file, "profiler time used: %.5f\n", time ) ;
		//		fprintf( file, "profiler avg. time: %.2f\n", time / (float)gProfilerCallCount ) ;
		//	}
		ProfilerOutputTree( file ) ;
	}
}

static void ProfilerInterruptSignalHandler( int signal )
{
	DebugLog(@"%s\n", __PRETTY_FUNCTION__ ) ;
	ProfilerExitFunction() ;
}

struct ProfilerInit
{
	ProfilerInit()
	{
		@autoreleasepool {
			DebugLog( @"Using profiling...\n" ) ; 
			DebugAssert( 0 == atexit( ProfilerExitFunction ) ) ; 
			DebugAssert( 0 == pthread_key_create( & gProfilerDataPThreadKey, NULL ) ) ;
			
			{
				const struct sigaction act = 
				{
					{ ProfilerInterruptSignalHandler }, 0, 0 
				} ;
				
				sigaction( SIGKILL, & act, NULL ) ;
				sigaction( SIGINT, & act, NULL ) ;
			}
			
			bzero( gProfilerTable, sizeof( gProfilerTable ) ) ;
			gProfilerTableLastUsedEntry = & gProfilerTable[ 0 ] ;
		}
	}
} ;
static struct ProfilerInit gProfilerInit = ProfilerInit() ;

#endif