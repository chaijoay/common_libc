//
//
//  FACILITY    : FRM system-wide definition for AIS
//
//  FILE NAME   : frm_ais_glob.h
//
//  ABSTRACT    : FRM system-wide definition for AIS, which specific for version to version of FRM
//
//  CREATE DATE : 05-Feb-2021
//
//  CURRENT VERSION NO : 1.0
//
//  LAST RELEASE DATE  : Feb-2021
//
//  MODIFICATION HISTORY :
//
//

#ifndef _FRM_CDR_GLOB_H_
#define _FRM_CDR_GLOB_H_

#ifdef	__cplusplus
extern "C" {
#endif

#if	!defined(TRUE) || ((TRUE) != 1)
	#ifdef TRUE
		#undef TRUE
	#endif
	#define TRUE	1
#endif
#if	!defined(FALSE) || ((FALSE) != 0)
	#ifdef FALSE
		#undef FALSE
	#endif
	#define FALSE	0
#endif

#define SUCCESS			    0
#define FAILURE			    -1
#define FMS_MAX_STR_LEN     100

// Changes to support new fms v11 by Thanakorn on 09-Sep-2012
#ifdef __FRM_V12__
    // FRM partiton
    #define GBL_PARTITION           "0"
    #define AWN_PARTITION           "2"
    #define AIN_PARTITION           "4"

    // Event type id used by eFIT
    #define VOICE_EVENT_TYPE_IDD    5   // Voice Event Type (IDD Category just for categorize Priority Tube)
    #define VOICE_EVENT_TYPE        6   // Voice Event Type
    #define SMS_EVENT_TYPE          7   // SMS, MMS Event Type
    #define VAS_EVENT_TYPE          8   // VAS Event Type (USC, CDG, RBT, SESSION, SCG, AMF)
    #define DATA_EVENT_TYPE         9   // Data Event Type (3G, GPRS)
    #define IR_EVENT_TYPE           10  // IR Event Type
    #define FIXED_EVENT_TYPE        11  // Fixed Voice Event Type
    
    // Categories
    #define CAT_TOTAL           0x00001
    #define CAT_VOICE           0x00002
    #define CAT_SMS             0x00004
    #define CAT_MMS             0x00008
    #define CAT_DATA            0x00010
    #define CAT_VAS             0x00020
    #define CAT_PREMIUM         0x00040
    #define CAT_LOCAL           0x00080
    #define CAT_IDD             0x00100
    #define CAT_IR              0x00200
    #define CAT_ONNET           0x00400
    #define CAT_OFFNET          0x00800
    #define CAT_OUTGOING        0x01000
    #define CAT_INCOMING        0x02000
    #define CAT_PACKAGE         0x04000     // This category use outside cdr_feed/ium_frontend. that process handle directly by IUM
    #define CAT_APP_PURCHASE    0x08000
    #define CAT_MARITIME        0x10000
    #define CAT_INCOMP_CALL     0x20000     // incomplete call category for AIN - added by Thanakorn on Aug-2016

    // Skip Detection Parameters - Input Mask
    #define SKIP_ARCHIVE            0x0000001   // Skip Archiving
    #define SKIP_DETECTION          0x0000002   // Skip All Detection
    #define SKIP_PIPELINE           0x0000004   // Skip Pipeline
    #define SKIP_ACCUM_PATTERN      0x0000008   // Skip Accum Pattern Check
    #define SKIP_AUTHORIZED_FEATURE 0x0000010   // Skip AuthFeature Check
    #define SKIP_BLACKLIST          0x0000020   // Skip Blacklist Check
    #define SKIP_DEST               0x0000040   // Skip Destination Check
    #define SKIP_DEST_USAGE         0x0000080   // Skip Destination Usage
    #define SKIP_IMMEDIATE_ROAM     0x0000100   // Skip Immediate Roam Check
    #define SKIP_UNKNOWN            0x0000200   // Skip Unknown Entity Check
    #define SKIP_PREMATURE_CHANGE   0x0000400   // Skip Premature Change Check
    #define SKIP_ROAM_NOHOME        0x0000800   // Skip Roam No Home Check
    #define SKIP_INACTIVE_CHECK     0x0001000   // Skip Inactive Entity Check
    #define SKIP_CONTACT            0x0002000   // Skip Contact Check
    #define SKIP_PATTERN            0x0004000   // Skip Pattern Check
    #define SKIP_SEQ_PATTERN        0x0008000   // Skip Sequence Pattern Check
    #define SKIP_SUSP               0x0010000   // Skip Suspension Check
    #define SKIP_THRESHOLD          0x0020000   // Skip Threshold Check
    #define SKIP_USAGE_ACCUM        0x0040000   // Skip Usage Accumulation
    #define SKIP_COLLISION          0x0080000   // Skip Collision Check
    #define SKIP_VELOCITY           0x0100000   // Skip Velocity Check
    #define SKIP_POST_PATTERN       0x0200000   // Skip Post Pattern Check
    #define SKIP_RECENT_CHARGE      0x0400000   // Skip Recent Charge
    #define SKIP_DUP_EVENT          0x0800000   // Skip Duplicate Event Check
    #define SKIP_UPD_OLDEVENT       0x1000000   // Skip Update of Older Event in Collision/Velocity

#else   // ========= Current Running Version =========

    // FRM partiton
    #define GBL_PARTITION           "0"
    #define AWN_PARTITION           "2"
    #define MPAY_PARTITION          "4"

    // Event type id used by eFIT
    #define VOICE_EVENT_TYPE_IDD    0   // Voice Event Type (IDD Category just for categorize Priority Tube)
    #define VOICE_EVENT_TYPE        1   // Voice Event Type
    #define SMS_EVENT_TYPE          2   // SMS, MMS Event Type
    #define VAS_EVENT_TYPE          3   // VAS Event Type (USC, CDG, RBT, SESSION, SCG, AMF)
    #define DATA_EVENT_TYPE         4   // Data Event Type (3G, GPRS)
    #define IR_EVENT_TYPE           5   // IR Event Type
    #define FIXED_EVENT_TYPE        6   // Fixed Voice Event Type
    #define PACKAGE_EVENT_TYPE      7   
    #define MPAY_EVENT_TYPE         8   // mPay Event Type (mPay partition)
    
    // Categories
    #define CAT_TOTAL           0x00000001
    #define CAT_VOICE           0x00000002
    #define CAT_SMS             0x00000004
    #define CAT_MMS             0x00000008
    #define CAT_DATA            0x00000010
    #define CAT_VAS             0x00000020
    #define CAT_PREMIUM         0x00000040
    #define CAT_LOCAL           0x00000080
    #define CAT_IDD             0x00000100
    #define CAT_IR              0x00000200
    #define CAT_PACKAGE         0x00000400
    #define CAT_APP_PURCHASE    0x00000800
    #define CAT_MARITIME        0x00001000
    #define CAT_FIXED_LINE      0x00002000
    #define CAT_IR_ONNET        0x00004000
    #define CAT_M2M             0x00008000
    #define CAT_VAS_ONETIME     0x00010000       // CCT => 11
    #define CAT_VAS_RECUR       0x00020000       // CCT => 10
    #define CAT_VAS_SUBS        0x00040000       // CCT =>  9
    #define CAT_VOIP            0x00080000
    #define CAT_CHANGE_TYPE     0x00100000
    #define CAT_CONTRACT_PHONE  0x00200000
    #define CAT_NEW_REGISTER    0x00400000
    #define CAT_PORT            0x00800000
    #define CAT_SIM             0x01000000
    #define CAT_SUSP_ORDER      0x02000000
    #define CAT_TERMINATED      0x04000000
    #define CAT_WALLET          0x08000000
    #define CAT_OTHER           0x10000000

    // Skip Detection Parameters, is now moved from mask to string (string literately is defined in .c)
    typedef enum {
        SKIP_DETECTION=0,
        SKIP_ANCESTORS,
        SKIP_ANCESTORSIFNOSIB,
        SKIP_PARENT,
        SKIP_PARENTIFNOSIB,
        SKIP_PIPELINE,
        SKIP_ARCHIVETOORACLE,
        SKIP_ARCHIVETOVERTICA,
        SKIP_HISTORY,
        SKIP_COLLISION,
        SKIP_VELOCITY,
        SKIP_DUPEVENT,
        SKIP_UPDOLDEVENT,
        SKIP_BLACKLIST,
        SKIP_USAGEACCUM,
        SKIP_RECENTCHARGE,
        SKIP_DESTUSAGE,
        SKIP_THRESHOLD,
        SKIP_DEST,
        SKIP_SUSP,
        SKIP_AUTHORIZEDFEATURE,
        SKIP_PATTERN,
        SKIP_ACCUMPATTERN,
        SKIP_SEQPATTERN,
        SKIP_IMMEDIATEROAM,
        SKIP_UNKNOWN,
        SKIP_PREMATURECHANGE,
        SKIP_ROAMNOHOME,
        SKIP_INACTIVECHECK,
        SKIP_CONTACT,
        SKIP_REALTIMEANALYSIS,
        SKIP_UTILITYMODULE,
        SKIP_EVENTPARTITIONOVERRIDE,
        SKIP_POSTPATTERN,
        NOF_SKIP
    } IDX_SKIP_DET_TABLES;
#endif

//
// FIT CDR Call Features Flag Name
// Each byte of FIT CDR Call Features field have different meaning as the following
//
#define FEAT_POSTPAID_CALL  0x0000001
#define FEAT_PREPAID_CALL   0x0000002
#define FEAT_FORWARD        0x0000004
#define FEAT_XFER           0x0000008
#define FEAT_WAIT           0x0000010
#define FEAT_HOLD           0x0000020
#define FEAT_CONFERENCE     0x0000040
#define FEAT_3WAY           0x0000080
#define FEAT_MBOX_REC       0x0000100
#define FEAT_MBOX_RETR      0x0000200
#define FEAT_FEAT_SETUP     0x0000400
#define FEAT_BAR_IN         0x0000800
#define FEAT_BAR_OUT        0x0001000
#define FEAT_MALIC_CALLID   0x0002000
#define FEAT_COMPL_BUSY     0x0004000
#define FEAT_USER_USER_SIG  0x0008000
#define FEAT_CLOSE_USR_GRP  0x0010000
#define FEAT_NO_AIR         0x0020000
#define FEAT_SPEED_CALL     0x0040000
#define FEAT_FREE_SERV      0x0080000
#define FEAT_FAX            0x0100000
#define FEAT_DATA_XMIT      0x0200000
#define FEAT_IR             0x0400000       // Added by Kawee on 22-Oct-2007, Use this in future on FMS.V.10 Phase2
#define FEAT_IDD            0x0800000       // Added by Thanakorn on 27-Sep-2012, Use this in FMS V11
#define FEAT_THAI           0x1000000
#define FEAT_LOCAL          0x2000000

//
// FIT EVENT CDR Service Type Values
// Value of FIT Service Type should be only one of the below value.
//
// - Added by Kawee on 27-Feb-2007
//
#define ST_HOME         "0"
#define ST_HOST         "1"
#define ST_ROAM         "2"

#define INCOMING        "1"
#define OUTGOING        "2"
#define _UNKNOWN        "15"

#define DEF_CELL_AREA   "00000"     // cell area for non-cell_id cdr, eg. SMS, FDA, USC, etc.
#define UNK_CELL_AREA   "TH001"     // cell area for un-map-able cell_id
//
// Unknown Value for Network Type, Direction, Deposition and Switch Type - Added by Kawee on 08-Jul-2003
//
#define TYPE_UNKNOWN    15
//
// Coded Value
//
#define SOF_NON3D               0           // Sof is NOT 3d
#define SOF_3D                  1           // Sof is 3d

#define SOF_TXN_UNKNOWN         0
#define SOF_TXN_CC              1
#define SOF_TXN_DCB             2
#define SOF_TXN_IBANKING        3
#define SOF_TXN_INSTALLMENT     4
#define SOF_TXN_PROMPTPAY       5
#define SOF_TXN_RLP             6

#define TXN_UNKNOWN             0
#define TXN_PAYMENT             1
#define TXN_VOID                2
#define TXN_REFUND              3


#ifdef  __cplusplus
    }
#endif

#endif // _FRM_CDR_GLOB_H_ //
