///
///
/// FILE NAME   : procsig.h
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

#ifndef __PROC_N_SIG_H__
#define __PROC_N_SIG_H__

#ifdef  __cplusplus
    extern "C" {
#endif

#include "strlogutl.h"
#include "glb_str_def.h"

#define FLOCK_SUFF      ".lock"

typedef enum {
    E_CHK = 0,  // check if the same process uniq name is running
    E_SET,      // set lock to allow only one instance allow to run at a time
    E_CLR       // clear lock state
} E_FLOCK;

int     handleSignal();
void    catchSignal(int sig);
char    *getSigInfoStr();
int     isTerminated();

// check for mulitple instance is running or not by the given uniq_name (process name)
int     procLock(const char *uniq_name, int oper);

// check(get) or set check point for the file being processing
long    checkPoint(FILE **chk_fp, char *chk_fullname, char *type_info, const char *outpath, const char *appname, int oper);

#ifdef  __cplusplus
    }
#endif


#endif  // __PROC_N_SIG_H__
