///
///
/// FILE NAME   : snapfile.h
///
/// FACILITY    : Sinal handle and check of process running
///
/// AUTHOR      : Thanakorn Nitipiromchai
///
/// CREATE DATE : 21-May-2019
///
/// CURRENT VERSION NO : 1.0
///
/// LAST RELEASE DATE  : 21-May-2019
///
/// MODIFICATION HISTORY :
///     1.0     21-May-2019     First Version
///
///

#ifndef __SNAP_FILE_H__
#define __SNAP_FILE_H__

#ifdef  __cplusplus
    extern "C" {
#endif

#ifndef _XOPEN_SOURCE
    #define _XOPEN_SOURCE           700         // Required under GLIBC for nftw()
#endif
#ifndef _POSIX_C_SOURCE
    #define _POSIX_C_SOURCE         200809L
#endif
#ifndef _XOPEN_SOURCE_EXTENDED
    #define _XOPEN_SOURCE_EXTENDED  1
#endif

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <ftw.h>
#include <dirent.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>


#include "strlogutl.h"
#include "glb_str_def.h"

#define SNAP_SEPS   '|'
#define _STATE_SUFF  ".proclist"

struct FTW *ftwbuf;

typedef enum {
    E_SNP_INP_PRE = 0,
    E_SNP_INP_SUF,
    E_SNP_NOF_INP
} E_INP_IDX;

typedef enum {
    E_SNP_PATH = 0,
    E_SNP_FILE,
    E_NOF_SNP_INF
} E_SNP_FORM;

int _chkMatchFile(const char *fpath, const struct stat *info, int typeflag,  struct FTW *ftwbuf);
int listSnapFilesSimple(const char *snapfile, const char *prefix, const char *suffix, const char *inp_dir, int no_dive);

void _clearOldState();
int _catAllStateContent(const char *oFileName);
int startState(const char* state_dir, const char* appname, int keep_state_day);
int logState(const char *full_fname);
int getFreshSnap(const char *fsnap);

#ifdef  __cplusplus
    }
#endif


#endif  // __SNAP_FILE_H__
