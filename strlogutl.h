///
///
/// FACILITY    : String Utilities and Log Managements
///
/// FILE NAME   : strlogutil.h
///
/// ABSTRACT    : Constant & Header of String Utilities and Log Managements
///
/// AUTHOR      : Thanakorn Nitipiromchai
///
/// CREATE DATE : 30-Apr-2019
///
/// CURRENT VERSION NO : 1.0
///
/// LAST RELEASE DATE  : 30-Apr-2019
///
/// MODIFICATION HISTORY :
///     1.0     30-Apr-2019     First Version
///
///

#ifndef __STR_LOG_UTL_H__
#define __STR_LOG_UTL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <math.h>
#include <errno.h>
#include "glb_str_def.h"

#ifdef  __cplusplus
    extern "C" {
#endif

#define     BASE64              64
#define     CELL_LEN            5

//
// Severity Constants used for Error Log File
//
typedef enum {
    LOG_SYS  = 0,       // --- 0
    LOG_ERR,            // --- 1
    LOG_INF,            // --- 2
    LOG_WRN,            // --- 3
    LOG_DBG,            // --- 4
    LOG_DB1,            // --- 5
    LOG_DB2,            // --- 6
    LOG_DB3,            // --- 7
    NOF_LOG_LEVEL
} LOG_LEVEL;

typedef enum {
    DTM_DATE_ONLY = 0,          // YYYYMMDD
    DTM_TIME_ONLY,              // H24MiSS
    DTM_DATE_TIME,              // YYYYMMDDH24MiSS
    DTM_DATE_FORM,              // YYYY/MM/DD
    DTM_TIME_FORM,              // H24:Mi:SS
    DTM_DATE_TIME_FULL,         // YYYY/MM/DD H24:Mi:SS
    DTM_DATE_TIMEF,             // YYYYMMDD H24:Mi:SS
    DTM_DMY_FORM,               // DD/MM/YYYY
    NOF_DTM_FORM
} DTM_FORMAT;

//
// Function prototypes for Logging Purpose
//
int     startLogging(const char* log_dir, const char* fname_prefix, int log_level);
int     manageLogFile();
int     openLogFile();
void    writeLog(int iSev, const char *LogMsg, ...);
void    stopLogging();

//
// Function prototypes for DateTime Purpose
//
void    getSysDate(char *sysdate, char *dateformat);
char    *getDateTimeT(time_t *t, int n_format);
char    *getFDateTimeT(time_t *t, const char *format_str);
char    *getSysDTM(int n_format);
char    *getFileTimeM(const char *inpfile, char *dateformat);
time_t  dateStr2TimeT(const char *ymdhms);
char    *getWorldDTM(const char *ref_ymdhms, float utc_ref, float utc_local, int n_format);
char    *getThDTM(const char *local_ymdhms, float utc_local, int n_format);
char    *dateAdd(const char *input_yyyymmdd, int out_format_code, int years_to_add, int months_to_add, int days_to_add);
long    get_millsec_delta(struct timespec *stop, struct timespec *start);
char    *sec2HMS(long secs);

//
// Function prototypes for String Manipulation
//

unsigned char *trimStr(unsigned char *buff);
void          strToLower(char *dest, const char *src);
void          strToUpper(char *dest, const char *src);
char          *strReplaceAll(char *orig, const char *rep, const char *with);     // the returned result must be freed after use
void          strReplaceLast(char *orig, const char *sstr, const char *rstr);
char          *reverseStr(char *str);
int           strrcmp(char *s1, char *s2, int *perc_match);

void          strchrreplace(char *s1, char d, char r);
void          strchrremove(char *s1, char d);

// Variable length record (with character delimiter)
int           getTokenAll(char *out_arr[], int arr_size, const char *line, char sep);
char          *getTokenItem(const char *str, int fno, char sep, char *out);
// Fixed length record
int           getFlTokenAll(int row, int col, char oarr[row][col], int fcnt, int *fdef, const char *line);
char          *getFlTokenItem(char *oitem, int fno, int fcnt, int *fdef, const char *line);

long          getFileSize(const char *fname);

char          *decTo64(int dec);

//#if ( defined __hpux || defined _HPUX_SOURCE )
/*-
 * Copyright (c) 1990, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
char    *strsep(char **stringp, const char *delim);
//#endif

#if 0
void    getStrToken(char item[][SIZE_ITEM_L], int item_size, char *str, char *sep);
#endif

#ifdef  __cplusplus
    }
#endif


#endif  // __STR_LOG_UTL_H__
