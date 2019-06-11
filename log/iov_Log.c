// ------------------------------------------------------------------
// New Log - Based on shared memory
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2015. All rights reserved
// ------------------------------------------------------------------
//-这里的log实现了应用程序输出log，而不是为了程序调试的目的。正常的log
//-打印到标准输出设备，而这里打印到内存中，那么就可以通过一个程序，在
//-运行时显示出整体系统的运行状态。
//-应用程序中,在任何地方增加一个打印语句就能实现打印功能即被写入到了共
//-享内存中,这里每次加都有判断存不存在,不存在就创建.
/** \file
 * \brief New Log - Based on shared memory
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
// #include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
// #include <fcntl.h>
#include <time.h>

#include "iotSemaphore.h"
// #include "fileCreate.h"
#include "iov_Log.h"

#define NEWLOG_SHMKEY   99613
#define NEWLOG_SEMKEY   99621

// #define LOG_DEBUG

#ifdef LOG_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif /* LOG_DEBUG */

#define LL_LOG( f, t )
// #define LL_LOG( f, t ) filelog( f, t )
#define UNIT_TESTS

#define NEWLOG_MAX_LOGS        120


typedef struct newlog {

    int cnt;
    int index;
    int lastupdate;
    
    int reserve[7];

    onelog_t log[NEWLOG_MAX_LOGS];
    
} newlog_t;

// ------------------------------------------------------------------
// Globals
// ------------------------------------------------------------------

char * logNames[ NEWLOG_FROM_LAST ] = { "none", 
    "Database",      // 1
    "DBP",           // 2
    "Test",          // 3
    "Iov-ed",        // 4
    "Iov-sci",       // 5
    
};             

static char * newLogSharedMemory = NULL;	//-记录共享内存地址,就是一个全局变量

char logbuffer[MAX_LOG_BUFFER];

//#if defined(WIN32) || defined(WIN64)
//mutex_type log_mutex;
//#else
//static pthread_mutex_t log_mutex_store = PTHREAD_MUTEX_INITIALIZER;
//static mutex_type log_mutex = &log_mutex_store;
//#endif

// ------------------------------------------------------------------
// Open / Close
// ------------------------------------------------------------------

/**
 * \brief Open the Log module
 * \returns 1 on success, 0 on error
 */
static int newLogOpen( void ) {	//-有就获得地址,没有就创建一个

    int shmid = -1, created = 0, ret = 0;
    
    LL_LOG( "/tmp/loggy", "In newLogOpen" );

    semPautounlock( NEWLOG_SEMKEY, 10 );	//-共享内存是公共资源,这个是为了控制访问

    // Locate the segment
    if ((shmid = shmget(NEWLOG_SHMKEY, sizeof(newlog_t), 0666)) < 0) {//-判断内核中是否存在键值与key相等的共享内存
        
        LL_LOG( "/tmp/loggy", "SHM not found" );
        
        // SHM not found: try to create
        if ((shmid = shmget(NEWLOG_SHMKEY, sizeof(newlog_t), IPC_CREAT | 0666)) < 0) {//-创建一个共享内存
            // Create error
            perror("shmget-create");
            printf( "Error creating SHM for Logs\n" );
            
            LL_LOG( "/tmp/loggy", "SHM create error" );
        } else {
            // Creation Successful: init structure
            created = 1;
            DEBUG_PRINTF( "Created new SHM for Logs\n" );
            LL_LOG( "/tmp/loggy", "SHM created" );
        }

    } else {//-出错
        DEBUG_PRINTF( "Linked to existing SHM for Logs (%d)\n", shmid );
        LL_LOG( "/tmp/loggy", "Linked to existing SHM" );
    }

    if ( shmid >= 0 ) {//-共享内存的标识符大于0说明创建成功了
        // Attach the segment to our data space
        if ((newLogSharedMemory = (char*)shmat(shmid, NULL, 0)) == (char *) -1) {//-连接共享内存标识符为shmid的共享内存，连接成功后把共享内存区对象映射到调用进程的地址空间，随后可像本地空间一样访问
            perror("shmat");
            printf( "Error attaching SHM for Logs\n" );
            newLogSharedMemory = NULL;
            LL_LOG( "/tmp/loggy", "SHM attach error" );
            
        } else {//-上面返回了附加好的共享内存地址
            DEBUG_PRINTF( "Successfully attached SHM for Logs\n" );
            LL_LOG( "/tmp/loggy", "Attached to SHM" );
            if ( created ) {
                LL_LOG( "/tmp/loggy", "Created, thus initialize" );
                // Wipe memory
                memset( newLogSharedMemory, 0, sizeof( newlog_t ) );
                
                // Newly created:  init structure
                newlog_t * pnewlog = (newlog_t *)newLogSharedMemory;
                    
                pnewlog->index = 0;
                
                int i;
                
                for ( i=0; i<NEWLOG_MAX_LOGS; i++ ) {
                    pnewlog->log[i].from    = 0;
                    pnewlog->log[i].text[0] = '\0';
                }
                
                DEBUG_PRINTF( "Initialized new SHM for DB\n" );
            }
            ret = 1;
        }
    } else {
        LL_LOG( "/tmp/loggy", "SHM error" );
    }

    semV( NEWLOG_SEMKEY );
    
    if ( ret ) {
        DEBUG_PRINTF( "Opening Log module succeeded\n" );
        LL_LOG( "/tmp/loggy", "Open succeeded" );
    } else {
        printf( "Error opening Log module\n" );
        LL_LOG( "/tmp/loggy", "Open failed" );
    }

    return( ret );
}

// -------------------------------------------------------------
// Add
// -------------------------------------------------------------

/**
 * \brief Adds a log line to the log module
 * \param from Where the log comes from
 * \param text Log-text
 * \returns 1 when ok, 0 on error
 */
int newLogAdd( iLogFrom from, iLOG_LEVELS log_level, char * text ) {
    if ( newLogSharedMemory || newLogOpen() ) {
        newlog_t * pnewlog = (newlog_t *)newLogSharedMemory;
        int now = (int)time( NULL );      
        int len = strlen( text );
        
        semP( NEWLOG_SEMKEY );
        
        while ( len > 0 ) {//-一个信息太长就记录在多个信息体里面
            pnewlog->log[pnewlog->index].from = from;
            pnewlog->log[pnewlog->index].level = log_level;
            pnewlog->log[pnewlog->index].ts = now;	//-记录目前的时间
            
            strncpy( pnewlog->log[pnewlog->index].text, text, NEWLOG_MAX_TEXT );
            pnewlog->log[pnewlog->index].text[NEWLOG_MAX_TEXT] = '\0';	//-这样做的目的是防错,保证是字符串
            
            text += NEWLOG_MAX_TEXT;
            len -= NEWLOG_MAX_TEXT;
            
            if ( ++pnewlog->index >= NEWLOG_MAX_LOGS ) pnewlog->index = 0;	//-循环记录
            pnewlog->cnt++;
        }            
        
        pnewlog->lastupdate = now;
        semV( NEWLOG_SEMKEY );
        
        return 1;
    } else {
        printf( "Error adding log\n" );
    }
    return 0;
}


/**
 * \brief Empty the Log module
 * \returns 1 on success, 0 on error
 */
int newLogEmpty( void ) {
    if ( newLogSharedMemory || newLogOpen() ) {
        newlog_t * pnewlog = (newlog_t *)newLogSharedMemory;
        int now = (int)time( NULL );
        semP( NEWLOG_SEMKEY );
        pnewlog->cnt        = 0;
        pnewlog->index      = 0;
        pnewlog->lastupdate = now;
        semV( NEWLOG_SEMKEY );
        return 1;
    }
    return 0;
}

/**********************************************************************
* 函数名称： 
* 功能描述： 
* 输入参数： from 来自哪个进程，log_level 同一个进程里面的打印等级
* 输出参数： 无
* 返 回 值： 
* 其它说明： 
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2019/06/11	     V1.0	    zhao dajun    创建
***********************************************************************/
void Log(iLogFrom from, iLOG_LEVELS log_level, const char* format, ...)
{
	static char msg_buf[512];
	va_list args;	//-定义一个va_list型的变量,这个变量是指向参数的指针.
	
//	/* we're using a static character buffer, so we need to make sure only one thread uses it at a time */
//	Thread_lock_mutex(log_mutex);	//-这里调用了一个互斥锁,其实就是对库函数的又一封装 
//	//-在C中，当我们无法列出传递函数的所有实参的类型和数目时,可以用省略号指定参数表
//	va_start(args, format);	//-用va_start宏初始化变量,这个宏的第二个参数是第一个可变参数的前一个参数,是一个固定的参数
//	vsnprintf(msg_buf, sizeof(msg_buf), format, args);	//-将可变参数格式化输出到一个字符数组。
//		
//	newLogAdd( from, log_level, msg_buf );
//	
//	va_end(args);	//-用va_end宏结束可变参数的获取
//	Thread_unlock_mutex(log_mutex); 
}

/**
 * \brief Gets the current Log module index
 * \returns index on success, -1 when empty
 */
int newLogGetIndex( void ) {
    int index = -1;
    if ( newLogSharedMemory || newLogOpen() ) {
        newlog_t * pnewlog = (newlog_t *)newLogSharedMemory;
        semP( NEWLOG_SEMKEY );
        if ( pnewlog->cnt > 0 ) index = pnewlog->index;
        semV( NEWLOG_SEMKEY );
    }
    return index;
}

// ------------------------------------------------------------------
// Loopers
// ------------------------------------------------------------------

/**
 * \brief Loops the current Log module from <fromindex> to current index, or all when <fromindex> < 0. 
 * Note that the call-back may not alter the logs as it peeks right into the shared memory
 * \param cb Call-back function
 * \returns Last index on success, -1 on error
 */
int newLogLoop( int from, int startIndex, newlogCb_t cb ) {	//-可以打印出特定序号from的内容
    if ( cb && ( newLogSharedMemory || newLogOpen() ) ) {
        newlog_t * pnewlog = (newlog_t *)newLogSharedMemory;
        int i, cnt;
        
        semP( NEWLOG_SEMKEY );
        if ( startIndex < 0 ) {
            // Loop all
            cnt = pnewlog->cnt % NEWLOG_MAX_LOGS;
        } else {
            // Loop since
            cnt = pnewlog->index - startIndex;
            if ( cnt < 0 ) cnt += NEWLOG_MAX_LOGS;
        }
        int ok = 1, start = pnewlog->index - cnt;
        if ( start < 0 ) start += NEWLOG_MAX_LOGS;
        for ( i=0; i<cnt && ok; i++ ) {
            if ( from == NEWLOG_FROM_NONE || pnewlog->log[start].from == from ) {
                ok = cb( start, &pnewlog->log[start] );
            }
            if ( ++start >= NEWLOG_MAX_LOGS ) start = 0;
        }
        semV( NEWLOG_SEMKEY );
    
        return( start );
    }
    return -1;
}

// ------------------------------------------------------------------
// Low level logging
// ------------------------------------------------------------------

/**
 * \brief Log simply to file
 * \returns 1 on success, 0 on error
 */
int filelog( char * filename, char * text ) {    
    char timestr[40];
    time_t clk;
    clk = time( NULL );
    sprintf( timestr, "%s", ctime( &clk ) );
    timestr[ strlen( timestr ) - 1 ] = '\0';
    
    printf( "Logging %s to %s\n", text, filename );
    
    FILE * f;
    if ( ( f = fopen( filename, "a" ) ) != NULL ) {
        fprintf( f, "%s - %d : %s\n", timestr, getpid(), text );
        fclose( f );
        return 1;
    }
    printf( "Error logging %s to %s\n", text, filename );
    return 0;
}

#if defined(UNIT_TESTS)
#include "atoi.h"
/**
 * \brief Looper callback that prints a log line
 * \returns 1 to continue looping
 */
static int printCb( int i, onelog_t * l ) {
    time_t ts = l->ts;
    char timestr[40];
    sprintf( timestr, "%s", ctime( &ts ) );
    timestr[ strlen( timestr ) - 1 ] = '\0';
    printf( "Log %d: %d, %s : %s\n", i, l->from, timestr, l->text );
    return 1;
}

// -------------------------------------------------------------
// Main
// -------------------------------------------------------------
//-选择打印信息,实例如下:
//-./iot_lt -f 5
//-Log tail
//-From = 5
//-Log 60: 5, Tue Nov 14 19:22:59 2017 : pvReaderThread is running
//-Log 61: 5, Tue Nov 14 19:23:09 2017 : pvReaderThread is running

/**
 * \brief Log Tail's main entry point: Prints new logs and updates
 * every second. Can show all logs or only for a certain (from) program
 * \param argc Number of command-line parameters
 * \param argv Parameter list (-h = help, -f <from> = specific program to follow (all when omitted), -z = startIndex is zero)
 */
int main( int argc, char * argv[] ) {
    signed char opt;
    int i, from = NEWLOG_FROM_NONE, zero = 0;

    printf( "\n\nLog tail\n");

    while ( ( opt = getopt( argc, argv, "hf:z" ) ) != -1 ) {
        switch ( opt ) {
        case 'h':
            printf( "Usage: lt [-f <from>]\n\tWhere <from> is one of:\n" );
            for ( i=1; i<11; i++ ) {
                printf( "\t\t%2d %s\n", i, logNames[i] );
            }
            exit( 0 );
        case 'f':
            printf( "From = %s\n", optarg );
            from = Atoi0( optarg );
            break;
        case 'z':
            zero = 1;
            break;
        }
    }
     
    // Where to start
    int startIndex = ( zero ) ? 0 : newLogGetIndex();	//-获取当前记录的序号

    while ( 1 ) {
        
        startIndex = newLogLoop( from, startIndex, printCb );
        sleep( 1 );
    }
    
    return( 0 );
}

#endif

