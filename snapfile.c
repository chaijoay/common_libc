#ifndef _XOPEN_SOURCE
    #define _XOPEN_SOURCE           700         // Required under GLIBC for nftw()
#endif
#ifndef _POSIX_C_SOURCE
    #define _POSIX_C_SOURCE         200809L
#endif
#ifndef _XOPEN_SOURCE_EXTENDED
    #define _XOPEN_SOURCE_EXTENDED  1
#endif

#include "snapfile.h"
#include "strlogutl.h"

#include <ftw.h>

static char _szInpFileInfo[E_SNP_NOF_INP][SIZE_ITEM_L];
static char _gszInpDir[SIZE_ITEM_L];
static int  _gnFnamePreLen;
static int  _gnFnameSufLen;
static int  _gnFileCnt;
static int  _gnNoDive;
static FILE *_gfpSnap;

static char _gszToday[SIZE_DATE_ONLY+1];
static char _gszAppName[SIZE_ITEM_S];
static char _gszStateDir[SIZE_ITEM_L];
static FILE *_gfpState;
static int  _gnKeepStateDay;

int _chkMatchFile(const char *fpath, const struct stat *info, int typeflag, struct FTW *ftwbuf)
{

    const char *fname = fpath + ftwbuf->base;
    int fname_len = strlen(fname);
    char path_only[SIZE_ITEM_L];

    if ( typeflag != FTW_F && typeflag != FTW_SL && typeflag != FTW_SLN )
        return 0;
//printf("strstr(%s, %s)\n", fname, _szInpFileInfo[E_SNP_INP_PRE]);
    if ( strstr(fname, _szInpFileInfo[E_SNP_INP_PRE]) == NULL ) {
        return 0;
    }
//printf("strcmp(%s, %s)\n", fname + (fname_len - _gnFnameSufLen), _szInpFileInfo[E_SNP_INP_SUF]);
    if ( strcmp(fname + (fname_len - _gnFnameSufLen), _szInpFileInfo[E_SNP_INP_SUF]) != 0 ) {
        return 0;
    }

    if ( !(info->st_mode & (S_IRUSR|S_IRGRP|S_IROTH)) ) {
        writeLog(LOG_WRN, "no read permission for %s skipped", fname);
        return 0;
    }

    memset(path_only, 0x00, sizeof(path_only));
    strncpy(path_only, fpath, ftwbuf->base - 1);
//printf("%d, [%s]-[%s]\n",     _gnNoDive ,_gszInpDir, path_only);

    if ( _gnNoDive && strcmp(_gszInpDir, path_only) != 0 )
    {
        // when _gszInpDir set to current path "./", the path_only will be ".", so this is to prevent skipping reading current path.
        if ( ! ( strcmp(_gszInpDir, "./") == 0 && strcmp(path_only, ".") == 0 ) ) {
            return 0;
        }
    }

//printf("path_only => %s\n", path_only);
    _gnFileCnt++;
    fprintf(_gfpSnap, "%s%c%s\n", path_only, SNAP_SEPS, fname);    // write snap output format -> <DIR>|<FILE>
    //fprintf(_gfpSnap, "%s%c%s%c%ld%c%ld\n", path_only, SNAP_SEPS, fname, SNAP_SEPS, info->st_size, SNAP_SEPS, info->st_mtime);

    return 0;

}


int listSnapFilesSimple(const char *snapfile, const char *prefix, const char *suffix, const char *inp_dir, int no_dive)
{
    char cmd[SIZE_BUFF];
    _gnFileCnt = 0;
    _gnNoDive = no_dive;

    memset(_szInpFileInfo, 0x00, sizeof(_szInpFileInfo));
    memset(_gszInpDir, 0x00, sizeof(_gszInpDir));
    strcpy(_szInpFileInfo[E_SNP_INP_PRE], prefix);
    strcpy(_szInpFileInfo[E_SNP_INP_SUF], suffix);

    _gnFnamePreLen = strlen(_szInpFileInfo[E_SNP_INP_PRE]);
    _gnFnameSufLen = strlen(_szInpFileInfo[E_SNP_INP_SUF]);

    if ( _gnFnamePreLen == 0 && _gnFnameSufLen == 0 ) {
        writeLog(LOG_ERR, "no file prefix and suffix specified");
        return FAILED;
    }

    // open snap file for writing
    if ( (_gfpSnap = fopen(snapfile, "w")) == NULL ) {
        writeLog(LOG_SYS, "unable to open %s for writing: %s\n", snapfile, strerror(errno));
        return FAILED;
    }

    // recursively walk through directories and file and check matching
    if ( *inp_dir != '\0' ) {
        strcpy(_gszInpDir, inp_dir);
        writeLog(LOG_INF, "scanning ( %s %s ) file in directory %s", prefix, suffix, inp_dir);
        if ( nftw(inp_dir, _chkMatchFile, 32, FTW_DEPTH) ) {
            writeLog(LOG_SYS, "unable to read path %s: %s\n", inp_dir, strerror(errno));
            fclose(_gfpSnap);
            _gfpSnap = NULL;
            return FAILED;
        }
    }

    fclose(_gfpSnap);
    _gfpSnap = NULL;

    // if there are files then sort the snap file
    if ( _gnFileCnt > 0 ) {
        memset(cmd, 0x00, sizeof(cmd));
        sprintf(cmd, "sort -u %s > %s.tmp 2>/dev/null", snapfile, snapfile);
        //writeLog(LOG_DB3, "buildSnapFile cmd '%s'", cmd);
        if ( system(cmd) != SUCCESS ) {
            writeLog(LOG_SYS, "cannot sort file %s (%s)", snapfile, strerror(errno));
            sprintf(cmd, "rm -f %s %s.tmp", snapfile, snapfile);
            system(cmd);
            return FAILED;
        }
        sprintf(cmd, "mv %s.tmp %s 2>/dev/null", snapfile, snapfile);
        //writeLog(LOG_DB3, "buildSnapFile cmd '%s'", cmd);
        system(cmd);
    }
    else {
        writeLog(LOG_INF, "no input file");
    }

    return _gnFileCnt;

}

int startState(const char* state_dir, const char* appname, int keep_state_day)
{
    int result = SUCCESS;
    if ( _gfpState ) {
        fclose(_gfpState);
    }
    _gfpState = NULL;

    memset(_gszToday, 0x00, sizeof(_gszToday));
    memset(_gszAppName, 0x00, sizeof(_gszAppName));
    memset(_gszStateDir, 0x00, sizeof(_gszStateDir));

    strcpy(_gszToday, getSysDTM(DTM_DATE_ONLY));
    if ( strlen(appname) <= 0 ) {
        writeLog(LOG_ERR, "appname is not specified");
        result = FAILED;
    }
    strcpy(_gszAppName, appname);

    if ( strlen(state_dir) <= 0 ) {
        writeLog(LOG_ERR, "state_dir is not specified");
        result = FAILED;
    }
    else if ( access(state_dir, W_OK|R_OK) != SUCCESS ) {
        writeLog(LOG_ERR, "state_dir is not accessible");
        result = FAILED;
    }
    strcpy(_gszStateDir, state_dir);

    _gnKeepStateDay = keep_state_day;
    if ( _gnKeepStateDay <= 0 ) {
        _gnKeepStateDay = 30;
        writeLog(LOG_ERR, "keep_state_day is used default %d day(s)", _gnKeepStateDay);
    }

    return result;
}

int logState(const char *full_fname)
{
    int  result = SUCCESS;

    if ( _gfpState == NULL ) {
        char fstate[SIZE_ITEM_L];
        memset(fstate, 0x00, sizeof(fstate));
        sprintf(fstate, "%s/%s_%s%s", _gszStateDir, _gszAppName, _gszToday, _STATE_SUFF);
        _gfpState = fopen(fstate, "a");
    }
    char tmp_fname[SIZE_BUFF];
    char fname[SIZE_ITEM_L], dir_name[SIZE_ITEM_L];

    memset(fname, 0x00, sizeof(fname));
    memset(dir_name, 0x00, sizeof(dir_name));
    memset(tmp_fname, 0x00, sizeof(tmp_fname));

    strcpy(tmp_fname, full_fname);
    strcpy(dir_name, dirname((char*)tmp_fname));
    strcpy(tmp_fname, full_fname);
    strcpy(fname, basename((char*)tmp_fname));

    result = fprintf(_gfpState, "%s|%s\n", dir_name, fname);
    fflush(_gfpState);

    if ( strcmp(getSysDTM(DTM_DATE_ONLY), _gszToday) != 0 ) {
        fclose(_gfpState);
        _gfpState = NULL;
        strcpy(_gszToday, getSysDTM(DTM_DATE_ONLY));
    }

    return result;
}

void _clearOldState()
{
    struct tm *ptm;
    time_t lTime;
    char tmp[SIZE_ITEM_L];
    char szOldestFile[SIZE_ITEM_S];
    char szOldestDate[SIZE_DATE_TIME_FULL+1];
    DIR *p_dir;
    struct dirent *p_dirent;
    int len1 = 0, len2 = 0;

    /* get oldest date to keep */
    time(&lTime);
    ptm = localtime( &lTime);
//printf("ptm->tm_mday = %d\n", ptm->tm_mday);
    ptm->tm_mday = ptm->tm_mday - _gnKeepStateDay;
//printf("ptm->tm_mday(after) = %d, keepState = %d\n", ptm->tm_mday, _gnKeepStateDay);
    lTime = mktime(ptm);
    ptm = localtime(&lTime);
    strftime(szOldestDate, sizeof(szOldestDate)-1, "%Y%m%d", ptm);
//printf("szOldestDate = %s\n", szOldestDate);

	writeLog(LOG_INF, "purge state file up to %s (keep %d days)", szOldestDate, _gnKeepStateDay);
    sprintf(szOldestFile, "%s%s", szOldestDate, _STATE_SUFF);     // YYYYMMDD.proclist
    len1 = strlen(szOldestFile);
    if ( (p_dir = opendir(_gszStateDir)) != NULL ) {
        while ( (p_dirent = readdir(p_dir)) != NULL ) {
            // state file name: <APP_NAME>_YYYYMMDD.proclist
            if ( strcmp(p_dirent->d_name, ".") == 0 || strcmp(p_dirent->d_name, "..") == 0 )
                continue;
            if ( strstr(p_dirent->d_name, _STATE_SUFF) != NULL &&
                 strstr(p_dirent->d_name, _gszAppName) != NULL ) {

                len2 = strlen(p_dirent->d_name);
                // compare only last term of YYYYMMDD.proclist
                if ( strcmp(szOldestFile, (p_dirent->d_name + (len2-len1))) > 0 ) {
                    sprintf(tmp, "rm -f %s/%s 2>/dev/null", _gszStateDir, p_dirent->d_name);
                    writeLog(LOG_INF, "remove state file: %s", p_dirent->d_name);
                    system(tmp);
                }
            }
        }
        closedir(p_dir);
    }
}

int _catAllStateContent(const char *oFileName)
{
    int result = FAILED;
    DIR *p_dir;
    struct dirent *p_dirent;
    char cmd[SIZE_BUFF];
    memset(cmd, 0x00, sizeof(cmd));
    unlink(oFileName);

    if ( (p_dir = opendir(_gszStateDir)) != NULL ) {
        while ( (p_dirent = readdir(p_dir)) != NULL ) {
            // state file name: <APP_NAME>_YYYYMMDD.proclist
            if ( strcmp(p_dirent->d_name, ".") == 0 || strcmp(p_dirent->d_name, "..") == 0 )
                continue;

            if ( strstr(p_dirent->d_name, _STATE_SUFF) != NULL &&
                 strstr(p_dirent->d_name, _gszAppName) != NULL ) {
                char state_file[SIZE_ITEM_L];
                memset(state_file, 0x00, sizeof(state_file));
                sprintf(state_file, "%s/%s", _gszStateDir, p_dirent->d_name);
                if ( access(state_file, F_OK|R_OK|W_OK) != SUCCESS ) {
                    writeLog(LOG_ERR, "unable to read/write file %s", state_file);
                    result = FAILED;
                    break;
                }
                else {
                    sprintf(cmd, "cat %s >> %s 2>/dev/null", state_file, oFileName);
                    system(cmd);
                    result = SUCCESS;
                }
            }
        }
        closedir(p_dir);
        return result;
    }
    else {
        return result;
    }
}

int getFreshSnap(const char *fsnap)
{
    char cmd[SIZE_BUFF];
    char tmp[SIZE_ITEM_L];
    FILE *fp = NULL;

    memset(tmp, 0x00, sizeof(tmp));
    memset(cmd, 0x00, sizeof(cmd));
    sprintf(tmp, "/tmp/%s_XXXXXX", _gszAppName);
    mkstemp(tmp);

	// close and flush current state file, in case it's opening
	if ( _gfpState != NULL ) {
		fclose(_gfpState);
		_gfpState = NULL;
	}

    // create state file of current day just in case there is currently no any state file.
    sprintf(cmd, "touch %s/%s_%s%s", _gszStateDir, _gszAppName, _gszToday, _STATE_SUFF);
    //writeLog(LOG_DB3, "getFreshSnap cmd '%s'", cmd);
    system(cmd);

    if ( _catAllStateContent(tmp) == SUCCESS ) {
        // sort all state files (<APP_NAME>_<YYYYMMDD>.proclist) to tmp file
        // state files format is <LEAF>|<SYNC_FILE>
        //sprintf(cmd, "sort -T /tmp %s/%s_*%s > %s.tmp 2>/dev/null", _gszStateDir, _gszAppName, _STATE_SUFF, tmp);
        sprintf(cmd, "sort -T /tmp %s > %s.tmp 2>/dev/null", tmp, tmp);
        //writeLog(LOG_DB3, "getFreshSnap cmd '%s'", cmd);
        system(cmd);
    }
    else {
        unlink(tmp);
        return FAILED;
    }
    // compare tmp file(sorted all state files) with sorted first_snap to get only unprocessed new files list
    sprintf(cmd, "comm -23 %s %s.tmp > %s.tmpout 2>/dev/null", fsnap, tmp, fsnap);
    //writeLog(LOG_DB3, "getFreshSnap cmd '%s'", cmd);
    system(cmd);

    sprintf(cmd, "rm -f %s %s.tmp", tmp, tmp);
    //writeLog(LOG_DB3, "getFreshSnap cmd '%s'", cmd);
    system(cmd);

    sprintf(cmd, "mv %s.tmpout %s", fsnap, fsnap);
    //writeLog(LOG_DB3, "getFreshSnap cmd '%s'", cmd);
    system(cmd);

    // get record count from output file (fsnap)
    sprintf(cmd, "cat %s | wc -l", fsnap);
    //writeLog(LOG_DB3, "getFreshSnap cmd '%s'", cmd);
    fp = popen(cmd, "r");
    fgets(tmp, sizeof(tmp), fp);
    pclose(fp);

    _clearOldState();

    return atoi(tmp);

}
