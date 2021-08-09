#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "procsig.h"


static int  nIsShutdown;
static char gszInfoStr[SIZE_ITEM_M];

int handleSignal()
{
    struct sigaction stSigActCont;
    char szErr[SIZE_ITEM_M];

    memset(gszInfoStr, 0x00, sizeof(gszInfoStr));
    memset(szErr, 0x00, sizeof(szErr));
    nIsShutdown = FALSE;

    /* Define structure for a Continue Signal Handler */
    sigemptyset(&stSigActCont.sa_mask);
    stSigActCont.sa_flags = 0;
    stSigActCont.sa_handler = catchSignal;

    /* Setup Continue Signal Handler */
    if ( ( sigaction(SIGUSR2, &stSigActCont, NULL) == -1 && strcpy(szErr, "SIGUSR2") ) ||
         ( sigaction(SIGHUP,  &stSigActCont, NULL) == -1 && strcpy(szErr, "SIGHUP" ) ) ||
         ( sigaction(SIGINT,  &stSigActCont, NULL) == -1 && strcpy(szErr, "SIGINT" ) ) ||
         ( sigaction(SIGQUIT, &stSigActCont, NULL) == -1 && strcpy(szErr, "SIGQUIT") ) ||
         ( sigaction(SIGILL,  &stSigActCont, NULL) == -1 && strcpy(szErr, "SIGILL" ) ) ||
         ( sigaction(SIGTRAP, &stSigActCont, NULL) == -1 && strcpy(szErr, "SIGTRAP") ) ||
         ( sigaction(SIGTERM, &stSigActCont, NULL) == -1 && strcpy(szErr, "SIGTERM") ) ||
         ( sigaction(SIGUSR1, &stSigActCont, NULL) == -1 && strcpy(szErr, "SIGUSR1") ) ) {
            sprintf(gszInfoStr, "handleSignal: signal %s (%s)", szErr, strerror(errno));
        return FAILED;
    }

    return SUCCESS;
}

void catchSignal(int sig)
{
    if ( sig == SIGTERM ) {
        nIsShutdown = TRUE;
        sprintf(gszInfoStr, "received signal(%d) (%s) and process terminating ...", sig, getSysDTM(DTM_DATE_TIME_FULL));
    }
    else {
        sprintf(gszInfoStr, "received signal(%d) (%s)", sig, getSysDTM(DTM_DATE_TIME_FULL));
    }
}

char *getSigInfoStr()
{
    return gszInfoStr;
}

int isTerminated()
{
    return nIsShutdown;
}

int procLock(const char *uniq_name, int oper)
{
    int result = FAILED;
    char lock_file[SIZE_ITEM_S];
    memset(lock_file, 0x00, sizeof(lock_file));
    sprintf(lock_file, "/tmp/.%s%s", uniq_name, FLOCK_SUFF);
    
    switch ( oper ) {
        case E_CHK:
            if ( access(lock_file, F_OK) != SUCCESS )   // no lock file
                result = SUCCESS;    // no other instance is running
            break;
        case E_SET:
            if ( access(lock_file, F_OK) != SUCCESS ) {
                FILE *f = fopen(lock_file, "w");
                if ( f == NULL )
                    result = FAILED;
                else {
                    result = SUCCESS;
                    fclose(f);
                }
            }
            break;
        case E_CLR:
        default:
            if ( access(lock_file, F_OK) == SUCCESS ) {
                unlink(lock_file);
                result = SUCCESS;
            }
            break;
    }
    return result;

}

long checkPoint(FILE **chk_fp, char *chk_fullname, char *type_info, const char *outpath, const char *appname, int oper)
{
    long file_pos = 0L;
    char o_chkpnt_fname[SIZE_ITEM_L];
    FILE *ofp_chkpnt = NULL;

    memset(o_chkpnt_fname, 0x00, sizeof(o_chkpnt_fname));
    sprintf(o_chkpnt_fname, "%s/%s.chk", outpath, appname);

    // check point file format is:
    // (long)file_position|(char*)full_file_name|(char*)file_type_info|(char*)process_timestamp
    switch ( oper ) {
        case E_CHK:
            // get last position of last time unfinished processed file for continue processing
            if ( (ofp_chkpnt = fopen(o_chkpnt_fname, "r")) != NULL ) {
                char rd_line[SIZE_ITEM_L];
                char *pbuf[4];
                memset(rd_line, 0x00, sizeof(rd_line));
                fgets(rd_line, sizeof(rd_line), ofp_chkpnt);
                if ( *rd_line != '\0' ) {
                    getTokenAll(pbuf, 4, rd_line, '|');
                    file_pos = atol(pbuf[0]);       // file position
                    strcpy(chk_fullname, pbuf[1]);  // full path file name
                    strcpy(type_info, pbuf[2]);     // file type
                }
                fclose(ofp_chkpnt);
            }
            break;
        case E_SET:
            // write current processing file position as a check point
            if ( *chk_fp != NULL && *chk_fullname != '\0' ) {
                if ( (ofp_chkpnt = fopen(o_chkpnt_fname, "w")) != NULL ) {
                    fprintf(ofp_chkpnt, "%ld|%s|%s|%s\n", ftell(*chk_fp), chk_fullname, type_info, getSysDTM(DTM_DATE_TIME_FULL));
                    fclose(ofp_chkpnt);
                }
                else {
                    file_pos = -1L;
                }
            }
            else {
                file_pos = -1L;
            }
            break;
        case E_CLR:
        default:
            // truncate file
            if ( (ofp_chkpnt = fopen(o_chkpnt_fname, "w")) != NULL ) {
                fclose(ofp_chkpnt);
            }
            break;
    }
    return file_pos;
    
}

