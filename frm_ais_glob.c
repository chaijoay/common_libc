#include "fms_ais_glob.h"

const char gaszFrmSkipDet[NOF_SKIP][40] = {
    " FmsDet_SkipDetection=1",                      // Skip All Detection
    " DetParm_SkipAncestors=1",                     // Skip Ancestor Detection
    " DetParm_SkipAncestorsIfNoSib=1",              // Skip Ancestor Detection If No Siblings
    " DetParm_SkipParent=1",                        // Skip Parent Detection
    " DetParm_SkipParentIfNoSib=1",                 // Skip Parent Detection If No Siblings
    " FmsDet_SkipPipeLine=1",                       // Skip Pipeline
    " DetParm_SkipArchiveToOracle=1",               // Skip Archiving to Oracle
    " DetParm_SkipArchiveToVertica=1",              // Skip Archiving to Vertica
    " EntLoad_SkipHistory=1",                       // Skip Entity History Fields
    " FmsDet_SkipCollision=1",                      // Skip Collision Check
    " FmsDet_SkipVelocity=1",                       // Skip Velocity Check
    " FmsDet_SkipDupEvent=1",                       // Skip Duplicate Event Check
    " FmsDet_SkipUpdOldEvent=1",                    // Skip Update of Older Event in Collision/Velocity
    " FmsDet_SkipBlacklist=1",                      // Skip Blacklist Check
    " FmsDet_SkipUsageAccum=1",                     // Skip Usage Accumulation
    " ErmDet_SkipRecentCharge=1",                   // Skip Recent Charge
    " FmsDet_SkipDestUsage=1",                      // Skip Destination Usage
    " FmsDet_SkipThreshold=1",                      // Skip Threshold Check
    " FmsDet_SkipDest=1",                           // Skip Destination Check
    " FmsDet_SkipSusp=1",                           // Skip Suspension Check
    " FmsDet_SkipAuthorizedFeature=1",              // Skip Authorized Feature Check
    " FmsDet_SkipPattern=1",                        // Skip Pattern Check
    " FmsDet_SkipAccumPattern=1",                   // Skip Accumulation Pattern Check
    " FmsDet_SkipSeqPattern=1",                     // Skip Sequence Pattern Check
    " FmsDet_SkipImmediateRoam=1",                  // Skip Immediate Roam Check
    " FmsDet_SkipUnknown=1",                        // Skip Unknown Entity Check
    " FmsDet_SkipPrematureChange=1",                // Skip Premature Change Check
    " FmsDet_SkipRoamNoHome=1",                     // Skip Roam No Home Check
    " FmsDet_SkipInactiveCheck=1",                  // Skip Inactive Entity Check
    " FmsDet_SkipContact=1",                        // Skip Contact Check
    " ErmParm_SkipRealtimeAnalysis=1",              // Skip Realtime Event Analysis
    " ErmDet_SkipUtilityModule=1",                  // Skip ERM Utility Module
    " ErmDet_SkipEventPartitionOverride=1",         // Skip Event partition override
    " ErmDet_SkipPostPattern=1"                     // Skip Post Pattern Check
};