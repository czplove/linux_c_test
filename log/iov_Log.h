// ------------------------------------------------------------------
// New Log - Based on shared memory - include file
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2015. All rights reserved
// ------------------------------------------------------------------

/** \file
 * \brief New Log - Based on shared memory
 */

#define NEWLOG_MAX_TEXT       98
#define MAX_LOG_BUFFER        200

typedef enum {
    NEWLOG_FROM_NONE = 0,
    NEWLOG_FROM_DATABASE,   // 1
    NEWLOG_FROM_DBP  ,      // 2
    NEWLOG_FROM_TEST,       // 3          See logNames in .c file and in logs.js
    
    NEWLOG_FROM_IOVED,     // 4   SPI进程
    NEWLOG_FROM_IOVSCI,    // 5   上行协议进程
    
    NEWLOG_FROM_LAST,      // 始终放在最后，用于统计个数
} iLogFrom;

typedef enum {
	INVALID_LEVEL = -1,
	TRACE_MAXIMUM = 1,
	TRACE_MEDIUM,
	TRACE_MINIMUM,
	TRACE_PROTOCOL,
	LOG_ERROR,
	LOG_SEVERE,
	LOG_FATAL,
} iLOG_LEVELS;

typedef struct onelog {
    int from;
    int level;
    int ts;
    char text[NEWLOG_MAX_TEXT+2];    // Extra  bytes e.g. for \0
} onelog_t;

typedef int (*newlogCb_t)( int i, onelog_t * l );
    

extern char * logNames[ NEWLOG_FROM_LAST ];
extern char logbuffer[MAX_LOG_BUFFER];

int newLogAdd( iLogFrom from, iLOG_LEVELS log_level, char * text );
int newLogEmpty( void );
int newLogGetIndex( void );
int newLogLoop( int from, int level, int startIndex, newlogCb_t cb );

int filelog( char * filename, char * text );


