#include "strlogutl.h"
//
// Static Global Variable (usable only within this file)
//
const char gszDtmFormat[NOF_DTM_FORM][SIZE_DATE_TIME_FULL+1] = {
    "%Y%m%d",
    "%H%M%S",
    "%Y%m%d%H%M%S",
    "%Y/%m/%d",
    "%H:%M:%S",
    "%Y/%m/%d %H:%M:%S",
    "%Y%m%d %H:%M:%S",
    "%d/%m/%Y"
};

static char glb_Log_File[SIZE_FULL_NAME];
static char glb_Logfile_Dir[SIZE_FULL_NAME];
static char glb_StartProcessTime[SIZE_DATE_TIME_FULL+1];
static char glb_LastLogDate[SIZE_DATE_ONLY+1];
static char glb_LogPrefix[SIZE_FULL_NAME];
static FILE *glb_Logfp;
static int  gn_LogLevel = 0;
static char	aSevStr[NOF_LOG_LEVEL][5] = {"SYS", "ERR", "INF", "WRN", "DBG", "DB1", "DB2", "DB3"};
//static char aFields[SIZE_ITEM_M][SIZE_ITEM_M];

const char  gszBASE64[BASE64+1] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz#$";

//
// Name         : startLogging ()
//
// Description  : Initial Log file.
//
// Parameters   : None.
//
// Return Value : None.
//
int startLogging(const char* log_dir, const char* fname_prefix, int log_level)
{
    gn_LogLevel = log_level;
    if ( glb_Logfp ) {
        fclose(glb_Logfp);
    }
    glb_Logfp = NULL;

    memset(glb_Log_File, 0x00, sizeof(glb_Log_File));
    memset(glb_Logfile_Dir, 0x00, sizeof(glb_Logfile_Dir));
    memset(glb_StartProcessTime, 0x00, sizeof(glb_StartProcessTime));
    memset(glb_LastLogDate, 0x00, sizeof(glb_LastLogDate));
    memset(glb_LogPrefix, 0x00, sizeof(glb_LogPrefix));

    strcpy(glb_Logfile_Dir, log_dir);
    strcpy(glb_LogPrefix, fname_prefix);
    strcpy(glb_StartProcessTime, getSysDTM(DTM_DATE_TIME_FULL));
    strcpy(glb_LastLogDate, getSysDTM(DTM_DATE_ONLY));
    return openLogFile();
}

//
// Name         : manageLogFile ()
//
// Description  : Check Log Date with current date for daily log file.
//
// Parameters   : None.
//
// Return Value : SUCCESS      - on Successful File Open.
//                FAILED      - on any Error.
//
int manageLogFile()
{

    if ( !*glb_LastLogDate ) {  // Is Null mean just start process
        strcpy(glb_LastLogDate, getSysDTM(DTM_DATE_ONLY));
        if ( openLogFile() ) {  // Open Log File FAILED
            return (FAILED);
        }
    }
    else if ( strncmp(glb_LastLogDate, getSysDTM(DTM_DATE_ONLY), 8) ) { // Last Log File date is different from current date
        strcpy(glb_LastLogDate, getSysDTM(DTM_DATE_ONLY));
        writeLog(LOG_INF, "------------------ End of daily log ------------------");
        if ( glb_Logfp ) {
            fclose(glb_Logfp);
        }
        if ( openLogFile() ) {  // Open Log File FAILED
            return (FAILED);
        }
        writeLog(LOG_INF, "--- Cont daily log: started on %s ---", glb_StartProcessTime);
    }       // Else use old log file
    return (SUCCESS);

}

//
// Name         : openLogFile ()
//
// Description  : Prefixes the Log Directory path to the FileName.
//                Opens Log File.
//
// Parameters   : None.
//
// Return Value : SUCCESS - on Successful File Open.
//                FAILED  - on any Error.
//
int openLogFile()
{
    // Prepare Log File Name
    if ( access(glb_Logfile_Dir, F_OK) != 0 ) {
        fprintf(stderr, "cannot access log path %s, used ./\n\n", glb_Logfile_Dir);
        strcpy(glb_Logfile_Dir, "./");
    }
    sprintf(glb_Log_File, "%s/%s_%s.log", glb_Logfile_Dir, glb_LogPrefix, getSysDTM(DTM_DATE_ONLY));

    // Open Log file
    if ( (glb_Logfp = fopen(glb_Log_File, "a+")) == 0 ) {   // Can't open log file
        fprintf(stderr, "cannot open log file %s\n\n", glb_Log_File);
        return (FAILED);
    }
    return (SUCCESS);

}

void stopLogging()
{
    if ( glb_Logfp ) {
        fclose(glb_Logfp);
    }
}

//
// Name         : writeLog ()
//
// Description  : Write error infomation to the log file.
//
// Parameters   : char *iSev - INPUT - Error message type.
//                char *LogMsg - INPUT - Error message to display.
//
// Return Value : void
//
void writeLog(int iSev, const char *LogMsg, ...)
{
    va_list vl;

    if ( iSev <= gn_LogLevel ) {
        va_start(vl, LogMsg);     // Initialize variable arguments.
        if ( glb_Logfp ) {
            fprintf(glb_Logfp, "%s [%s] ", getSysDTM(DTM_DATE_TIME_FULL), aSevStr[iSev]);
            vfprintf(glb_Logfp, LogMsg, vl);
            fprintf(glb_Logfp, "\n");
            fflush(glb_Logfp);
        }
        else {
            fprintf(stderr, "%s [%s] ", getSysDTM(DTM_DATE_TIME_FULL), aSevStr[iSev]);
            vfprintf(stderr, LogMsg, vl);
            fprintf(stderr, "\n");
            fflush(stderr);
        }
        va_end(vl);
    }

}

void getSysDate(char *sysdate, char *dateformat)
{
    struct tm *sys_time;
    time_t t;
    char nowtm[30+1];

    memset(nowtm, 0x00, sizeof(nowtm));
    t = time('\0');
    sys_time = localtime(&t);
    strftime(nowtm, 30, dateformat, sys_time);  /* format : HH24MI */
    strcpy(sysdate, nowtm);

}

char *getDateTimeT(time_t *t, int n_format)
{
    struct tm * time_info;
    static char szDateStr[SIZE_DATE_TIME_FULL+1];
    time_info = localtime(t);
    if ( n_format >= NOF_DTM_FORM || n_format < DTM_DATE_ONLY )
        n_format = 0;
    strftime(szDateStr, sizeof(szDateStr), gszDtmFormat[n_format], time_info);
    return szDateStr;
}

char *getFDateTimeT(time_t *t, const char *format_str)
{
    struct tm * time_info;
    static char szDateStr[SIZE_DATE_TIME_FULL+1];
    memset(szDateStr, 0x00, sizeof(szDateStr));
    time_info = localtime(t);
    if ( *format_str == '\0' )
        return szDateStr;

    strftime(szDateStr, sizeof(szDateStr), format_str, time_info);
    return szDateStr;
}

char *getSysDTM(int n_format)
{
    time_t sys_sec;
    struct tm * time_info;
    static char szDateStr[SIZE_DATE_TIME_FULL+1];

    time(&sys_sec);
    time_info = localtime(&sys_sec);

    if ( n_format >= NOF_DTM_FORM || n_format < DTM_DATE_ONLY )
        n_format = 0;

    strftime(szDateStr, sizeof(szDateStr), gszDtmFormat[n_format], time_info);
    return szDateStr;
}

char *getFileTimeM(const char *inpfile, char *dateformat)
{
    struct stat stat_buf;
    struct tm   *time_info;
    static char szDateStr[SIZE_DATE_TIME_FULL+1];

    memset(&stat_buf, 0x00, sizeof(stat_buf));

    if ( !lstat(inpfile, &stat_buf) ) {
        time_info = localtime(&(stat_buf.st_mtime));
        strftime(szDateStr, sizeof(szDateStr), dateformat, time_info);
    }
    else {
        getSysDate(szDateStr, dateformat);
    }
    return szDateStr;
}

time_t dateStr2TimeT(const char *ymdhms)
{
    struct tm time_info;
    char      tmp[5];

    // ymdhms => yyyymmddhhmmss
    if ( *ymdhms == '\0' || strlen(ymdhms) < SIZE_DATE_TIME )
        return 0;

    memset(&time_info, 0x00, sizeof(struct tm));

    memset(tmp, 0x00, sizeof(tmp));
    strncpy(tmp, ymdhms, 4);
    time_info.tm_year = atoi(tmp) - 1900;

    memset(tmp, 0x00, sizeof(tmp));
    strncpy(tmp, ymdhms+4, 2);
    time_info.tm_mon = atoi(tmp) - 1;

    memset(tmp, 0x00, sizeof(tmp));
    strncpy(tmp, ymdhms+6, 2);
    time_info.tm_mday = atoi(tmp);

    memset(tmp, 0x00, sizeof(tmp));
    strncpy(tmp, ymdhms+8, 2);
    time_info.tm_hour = atoi(tmp);

    memset(tmp, 0x00, sizeof(tmp));
    strncpy(tmp, ymdhms+10, 2);
    time_info.tm_min = atoi(tmp);

    memset(tmp, 0x00, sizeof(tmp));
    strncpy(tmp, ymdhms+12, 2);
    time_info.tm_sec = atoi(tmp);

    return mktime(&time_info);

}

char *getWorldDTM(const char *ref_ymdhms, float utc_ref, float utc_local, int n_format)
{
    int tm_zone_sec_ref = 0;
    int sec_in_min_ref = 0;
    int tm_zone_sec_local = 0;
    int sec_in_min_local = 0;
    time_t time_t_ref;

    // ref_ymdhms => yyyymmddhhmmss (ref_ymdhms is datetime at utc_ref time zone and to be converted to locat datetime at utc_local time zone)
    if ( *ref_ymdhms == '\0' || strlen(ref_ymdhms) < SIZE_DATE_TIME )
        return (char *)ref_ymdhms;

    if ( n_format >= NOF_DTM_FORM || n_format < DTM_DATE_ONLY )
        n_format = 0;

    sec_in_min_local = ((utc_local - floor(utc_local)) * SEC_IN_HOUR);
    tm_zone_sec_local = (floor(utc_local) * SEC_IN_HOUR) + (utc_local < 0 ? (-1)*sec_in_min_local : sec_in_min_local);

    sec_in_min_ref = ((utc_ref - floor(utc_ref)) * SEC_IN_HOUR);
    tm_zone_sec_ref = (floor(utc_ref) * SEC_IN_HOUR) + (utc_ref < 0 ? (-1)*sec_in_min_ref : sec_in_min_ref);

    if ( tm_zone_sec_ref > tm_zone_sec_local ) {
        time_t_ref = dateStr2TimeT(ref_ymdhms) - (tm_zone_sec_local - tm_zone_sec_ref);
    }
    else {
        time_t_ref = dateStr2TimeT(ref_ymdhms) - (tm_zone_sec_ref - tm_zone_sec_local);
    }

    return getDateTimeT(&time_t_ref, n_format);

}

char *getThDTM(const char *local_ymdhms, float utc_local, int n_format)
{
    float utc_th = 7.0f;
    return getWorldDTM(local_ymdhms, utc_local, utc_th, n_format);
}

char *dateAdd(const char *input_yyyymmdd, int output_format_code, int years_to_add, int months_to_add, int days_to_add)
{
    char temp[4+1], inp_ymd[SIZE_DATE_ONLY+1];
    time_t sec_time;
    struct tm time_info;
    static char szDateStr[SIZE_DATE_TIME_FULL+1];

    if ( input_yyyymmdd == NULL || input_yyyymmdd == '\0' ) {
        memset(inp_ymd, 0x00, sizeof(inp_ymd));
        strcpy(inp_ymd, getSysDTM(DTM_DATE_ONLY));
    }
    else {
        strcpy(inp_ymd, input_yyyymmdd);
    }

    int date_len = strlen(inp_ymd);

    if (date_len < SIZE_DATE_ONLY)  /* Invalid DateTime format */
        return NULL;

    memset(szDateStr, 0x00, sizeof(szDateStr));
    memset(&time_info, 0x00, sizeof(struct tm));

    memset(temp, 0x00, sizeof(temp));
    strncpy(temp, inp_ymd, 4);       /* Year */
    time_info.tm_year = atoi(temp) - 1900 + years_to_add;   /* Add years */

    memset(temp, 0x00, sizeof(temp));
    strncpy(temp, inp_ymd+4, 2);                     /* Month */
    time_info.tm_mon  = atoi(temp) - 1 + months_to_add;     /* Add months */

    memset(temp, 0x00, sizeof(temp));
    strncpy(temp, inp_ymd+6, 2);                     /* Day of Month */
    time_info.tm_mday = atoi(temp) + days_to_add;           /* Add days */

    time_info.tm_hour = 0;
    time_info.tm_min  = 0;
    time_info.tm_sec  = 0;

    sec_time = mktime(&time_info);

    memset(&time_info, 0x00, sizeof(struct tm));
    memcpy(&time_info, localtime(&sec_time), sizeof(struct tm));

    if ( output_format_code >= NOF_DTM_FORM || output_format_code < DTM_DATE_ONLY ) {
        output_format_code = 0;
    }
    strftime(szDateStr, sizeof(szDateStr), gszDtmFormat[output_format_code], &time_info);

    return (szDateStr);
}

char *sec2HMS(long secs) {
    static char hms[20];
	int h, m, s;

    memset(hms, 0x00, sizeof(hms));
	h = (secs/3600);
	m = (secs-(3600*h))/60;
	s = (secs-(3600*h)-(m*60));

	sprintf(hms, "%d:%02d:%02d", h, m, s);

	return hms;
}

long get_millsec_delta(struct timespec *stop, struct timespec *start)
{
    // return in milli sec
    return (stop->tv_sec - start->tv_sec) * 1000 + (stop->tv_nsec - start->tv_nsec) / 1000000;
}

unsigned char *trimStr(unsigned char *buff)
{
    short i = 0;
    if ( *buff == '\0' )
        return buff;

    // trim left
    while ( *(buff + i) < 33 && *(buff + i) != 0 ) i++;
    // buff += i;                                   // this line NOT modified buff variable
    memmove(buff, buff + i, strlen((const char*)buff) + 1 - i);  // this line MODIFIES the buff variable

    // trim right
    i = strlen((const char*)buff);

    /* while (i-- && *(buff+i) == ' '); */
    while (i-- && *(buff + i) < 33)
        ;  /* ascii value 0 - 32 */
    *(buff + i + 1) = '\0';

    return buff;

}

void strToLower(char *dest, const char *src)
{
    int i=0;
    while ( *(src+i) != 0 ) {
        *(dest+i) = tolower(*(src+i));
        i++;
    }
    *(dest+i) = 0;
}

void strToUpper(char *dest, const char *src)
{
    int i=0;
    while ( *(src+i) != 0 ) {
        *(dest+i) = toupper(*(src+i));
        i++;
    }
    *(dest+i) = 0;
}

char *strReplaceAll(char *orig, const char *rep, const char *with)
{
    char *result;   // the return string
    char *ins;      // the next insert point
    char *tmp;      // varies
    int len_rep;    // length of rep (the string to remove)
    int len_with;   // length of with (the string to replace rep with)
    int len_front;  // distance between rep and end of last rep
    int count;      // number of replacements

    // sanity checks and initialization
    if ( !orig || !rep )
        return NULL;
    len_rep = strlen(rep);
    if ( len_rep == 0 )
        return NULL; // empty rep causes infinite loop during count
    if ( !with )
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for ( count = 0; (tmp = strstr(ins, rep)); ++count ) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if ( !result )
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //      tmp points to the end of the result string
    //      ins points to the next occurrence of rep in orig
    //      orig points to the remainder of orig after "end of rep"
    while ( count-- ) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep;    // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;

}

void strReplaceLast(char *orig, const char *sstr, const char *rstr)
{
    int olen = strlen(orig);
    int slen = strlen(sstr);
    int rlen = strlen(rstr);

    if ( olen == 0 ) {
        return;
    }

    if ( strncmp(orig + (olen-slen), sstr, slen) == 0 ) {
        strcpy(orig + (olen-slen), rstr);
        *(orig + (olen-slen+rlen)) = '\0';
    }

}

char *reverseStr(char *str)
{
    static char revStr[SIZE_ITEM_S+1];
    memset(revStr, 0x00, sizeof(revStr));

    int len = strlen(str);
    int i = 0;
    //char c = 0;
    while ( len >= 0 ) {
        if ( i >= SIZE_ITEM_S ) {
            break;
        }
        revStr[i++] = *(str+len-1);
        len--;
    }
    return revStr;
}
//
// compare string from rightmost to left and return as same as normal strcmp function (in a reverse order string)
// in addition to strcmp, the strrcmp also returns character matching percentage comparing to max string length
//
int strrcmp(char *s1, char *s2, int *perc_match)
{
    int match_cnt = 0;
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int maxlen = (len1 > len2 ? len1 : len2);


    while ( len1-- >= 0 && len2-- >= 0 ) {
        //match_cnt++;
        //printf("(%c) <=> (%c)\n", s1[len1],  s2[len2]);
        if ( s1[len1] != s2[len2] ) {
            //printf("\t\tmatch_cnt(while) = %d\n", match_cnt);
            //printf("%2.0f\n", (((float)match_cnt/(float)maxlen)*100));
            *perc_match = (int)(((float)match_cnt/(float)maxlen)*100);
            //printf("\t%d\n", d);
            return (s1[len1] - s2[len2]);
        }
        else {
            match_cnt++;
        }
    }
    match_cnt--;
    //printf("\t\tmatch_cnt(out while) = %d\n", match_cnt);
    //printf("%2.0f\n", (((float)match_cnt/(float)maxlen)*100));
    *perc_match = (int)(((float)match_cnt/(float)maxlen)*100);
    //printf("\t%d\n", d);
    return 0;
}

void strchrreplace(char *s1, char d, char r)
{
    while ( *s1++ != '\0' ) {
        (*s1 == d ? *s1 = r : *s1 );
    }
}

void strchrremove(char *s1, char d)
{
    int len = strlen(s1)+1;
    while ( *s1 != '\0' ) {
        len--;
        if ( *s1 == d ) {
            memmove(s1, s1+1, len);
        }
        else {
            ( *s1+1 == '\0' ? *s1 : *s1++ );
        }
    }
}

int getTokenAll(char *out_arr[], int arr_size, const char *line, char sep)
{
    char *cur_ptr;
    char *cur_item;
    int  cur_idx = 0;

    if ( !sep || *line == '\0' ) {
        return cur_idx;
    }

    cur_ptr = (char*)line;
    while ( *cur_ptr != '\0' ) {

        cur_item = cur_ptr;
        while ( (*cur_ptr != sep) && (*cur_ptr) ) {
            cur_ptr++;
        }
        if ( *cur_ptr == sep ) {    // match delimiter
            *cur_ptr = 0;           // replace delimit null
            cur_ptr++;
        }
        if ( cur_idx < arr_size ) {
            out_arr[cur_idx++] = cur_item;
            if ( ! *cur_ptr ) {
                out_arr[cur_idx++] = "";    // one more count for last seperator
            }
        }

    }
    return cur_idx;

}

char *getTokenItem(const char *str, int fno, char sep, char *out)
{
    int i = 0, cur = 1, start = 0, is_copied = 0;

    if ( fno <= 0 ) {
        return (char*)str;
    }

    while ( *(str+i) != '\0' ) {
        if ( *(str+i) == sep ) {
            if ( cur == fno ) {
                if ( i == start ) {
                    strcpy(out, "");
                }
                else {
                    strncpy(out, str+start, (i-start));
                }
                is_copied = 1;
                break;
            }
            cur++;
            start=i+1;
        }
        i++;
    }
    if ( *out == 0 || is_copied == 0 ) {
        if ( i == start ) {
            strcpy(out, "");
        }
        else {
            strncpy(out, str+start, (i-start));
        }
    }
    return out;
}

int getFlTokenAll(int row, int col, char oarr[row][col], int fcnt, int *fdef, const char *line)
{
    int i = 0, cur_pos = 0, slen = 0;

    memset(oarr, 0x00, (row*col));
    if ( *line == '\0' ) {
        return -1;
    }
    slen = strlen(line);

    strncpy(oarr[i], line, fdef[i]);
    cur_pos = fdef[i];
    for ( i=1; i<fcnt; i++ ) {
        if ( cur_pos > slen ) { break; }
        strncpy(oarr[i], line+cur_pos, fdef[i]);
        cur_pos += fdef[i];
        //oarr[i][fdef[i]] = '\0';
    }
    return i;

}

char *getFlTokenItem(char *oitem, int fno_idx, int fcnt, int *fdef, const char *line)
{
    static char item[SIZE_ITEM_M];
    int i = 0, start_pos = 0, slen = 0;
    memset(item, 0x00, SIZE_ITEM_M);

    for ( i=0; i<fno_idx; i++ ) {
        start_pos += fdef[i];
    }
    slen = strlen(line);
    if ( start_pos < slen ) {
        strncpy(item, line+start_pos, fdef[fno_idx]);
    }

    strcpy(oitem, item);
    return item;

}

long getFileSize(const char *fname)
{
    struct stat stat_buf;
    long result = FALSE;

    if ( access(fname, F_OK|R_OK) != SUCCESS ) {
        fprintf(stderr, "unable to access file %s (%s)\n", fname, strerror(errno));
        return result;
    }

    memset(&stat_buf, 0x00, sizeof(stat_buf));
    if ( !lstat(fname, &stat_buf) ) {
        result = stat_buf.st_size;
    }
    return result;

}

char *decTo64(int dec)
{
   static char b64Str[CELL_LEN+1];
   memset(b64Str, 0x00, sizeof(b64Str));

   int pos = 0;
   while ( dec >= 0 ) {
        if ( pos >= CELL_LEN ) {
            break;
        }
        b64Str[pos++] = gszBASE64[ (dec%BASE64) ];
        dec = (dec/BASE64);
        if ( dec == 0 ) {
            break;
        }
   }
   int len = CELL_LEN - strlen(b64Str);
   if ( len > 0 ) {
       int i;
       for ( i=len; i>0; i-- ) {
           strcat(b64Str, "0");
       }
   }
   return reverseStr(b64Str);
}

//#if ( defined __hpux || defined _HPUX_SOURCE )
/*
 * Get next token from string *stringp, where tokens are possibly-empty
 * strings separated by characters from delim.
 *
 * Writes NULs into the string at *stringp to end tokens.
 * delim need not remain constant from call to call.
 * On return, *stringp points past the last NUL written (if there might
 * be further tokens), or is NULL (if there are definitely no more tokens).
 *
 * If *stringp is NULL, strsep returns NULL.
 */
char *strsep(char **stringp, const char *delim)
{
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;

    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}
// --------- Example of Using strsep --------
// void dummy_test
// {

    // int idx, col = 1;
    // char *token, *string, *tofree, szSep[2];

    // sprintf(szSep, "%c", sep);
    // tofree = string = strdup(line);
    // while ( (token = strsep(&string, szSep) ) != NULL) {
        // if ( col == pat_col ) {
            // fprintf(*out, "%d|", pat_val);
        // }
        // else {
            // fprintf(*out, "%s|", token);
        // }
        // col++;
    // }
    // free(tofree);
    // fprintf(*out, "\n");

// }
//#endif

#if 0
// for nondestructive tokenizer
void getStrToken(char item[][SIZE_ITEM_L], int item_size, char *str, char *sep)
{
    char *token, *string, *tofree;
    int i=0;

    tofree = string = strdup(str); i = 0;
    while ( (token = strsep(&string, sep) ) != NULL )
        if ( i < item_size )
            strcpy(item[i++], token);

    free(tofree);
}
#endif
