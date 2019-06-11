// ------------------------------------------------------------------
// atoi
// ------------------------------------------------------------------
// Save version to avoid segmentation faults
// ------------------------------------------------------------------
// Author:    nlv10677
// Copyright: NXP B.V. 2014. All rights reserved
// ------------------------------------------------------------------

/** \file
 * \brief Save atoi functions to avoid segmentation faults
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "atoi.h"

/**
 * \brief Converts an integer string into an integer
 * \returns The converted text or -1 in case of an error
 */
int Atoi( char * string ) {
    if ( string != NULL && *string != '\0' ) {
        return( atoi( string ) );
    }
    return( -1 );
}

/**
 * \brief Converts an integer string into an integer
 * \returns The converted text or 0 in case of an error
 */
int Atoi0( char * string ) {
    if ( string != NULL && *string != '\0' ) {
        return( atoi( string ) );
    }
    return( 0 );
}

/**
 * \brief Converts an integer string into an integer
 * \returns The converted text or INT_MIN in case of an error
 */
int AtoiMI( char * string ) {
    if ( string != NULL && *string != '\0' ) {
        return( atoi( string ) );
    }
    return( INT_MIN );
}

/**
 * \brief Converts a hexadecimal integer string into an integer
 * \returns The converted text or -1 in case of an error
 */
int AtoiHex( char * string ) {
    if ( string != NULL && *string != '\0' ) {
        int val = (int)strtol( string, NULL, 16 );	//-将参数nptr字符串根据参数base来转换成长整型数。
        return( val );
    }
    return( -1 );
}

/*
把字符串转化为可能的整数
使用场景是屏幕以字符的形式输入一串16进制数据,这里转化为整数数组形式
*/
int LineAtoiHex(char *line,unsigned char *tint,int tintlen)
{
	int i, j, len, dstlen;
	len = strlen(line);	//-长度不包含'\0'
	dstlen = 0;
	//-printf( "tint len = %d\n",  sizeof(*tint));
	for(i = 0; i < len; i++)
	{
		for ( ; (line[i] == ' ' || line[i] == '\t') && (i < len); i++ );	//-去除空格
		for ( j=i; line[j] != ' ' && line[j] != '\t'&& j < len; j++ );	//-找打结束符
		line[j] = '\0';
		*tint = (unsigned char)strtol( line + i, NULL, 16 );
		//-printf( "convert = %d\n",  *tint);
		tint++;
		i = j;
		dstlen++;
		if(dstlen >= tintlen)
			return ( -1 );
	}
	return dstlen;
}
