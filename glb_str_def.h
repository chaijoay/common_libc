#ifndef __GLB_STR_DEF_H__
#define __GLB_STR_DEF_H__

#ifdef  __cplusplus
    extern "C" {
#endif

#ifndef SUCCESS
    #define SUCCESS             0
#endif
#ifndef FAILED
    #define FAILED              -1
#endif

#ifndef TRUE
    #define TRUE                1
#endif
#ifndef FALSE
    #define FALSE               0
#endif

#define SIZE_ITEM_T             24      // general tiny item/string size
#define SIZE_ITEM_S             64      // general samall item/string size
#define SIZE_ITEM_M             128     // general medium item/string size
#define SIZE_ITEM_L             256     // general large item/string size
#define SIZE_ITEM_X             512     // general large item/string size
#define SIZE_BUFF               1024    // general read line record/buffer size from file
#define SIZE_BUFF_2X            2048    // 2X read line record/buffer size from file
#define SIZE_BUFF_5X            5120    // 5X read line record/buffer size from file
#define SIZE_BUFF_10X           10240   // 10X read line record/buffer size from file
#define SIZE_BUFF_20X           20480   // 20X read line record/buffer size from file
#define SIZE_FILE_NAME          128     // max file name length
#define SIZE_PATH_NAME          2048    // max path length
#define SIZE_FULL_NAME          1024    // string length of full path and file name
#define SIZE_FULL_NAME_LONG     4096    // large string length of full path and file name

#define SIZE_TIME_ONLY          6       // fit for HHMISS
#define SIZE_TIME_FORM          8       // fit for HH:MI:SS
#define SIZE_DATE_ONLY          8       // fit for YYYYMMDD
#define SIZE_DATE_FORM          10      // fit for YYYY/MM/DD, DD/MM/YYYY, MM/DD/YYYY, etc.
#define SIZE_DATE_TIME          14      // fit for YYYYMMDDHHMISS
#define SIZE_DATEF_TIME         17      // fit for YYYY/MM/DD HHMISS
#define SIZE_DATE_TIMEF         17      // fit for YYYYMMDD HH:MI:SS
#define SIZE_DATE_TIME_FULL     19      // fit for YYYY/MM/DD HH:MI:SS, DD/MM/YYYY HH:MI:SS, MM/DD/YYYY HH:MI:SS, etc.

#define KBYTE                   1024
#define MBYTE                   1024 * KBYTE
#define GBYTE                   1024 * MBYTE

#define SEC_IN_HOUR             3600
#define SEC_IN_DAY              86400

#define SIZE_FNAME              SIZE_ITEM_L

#ifdef  __cplusplus
    }
#endif


#endif  // __GLB_STR_DEF_H__
