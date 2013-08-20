/*
** $Id: //Department/DaVinci/BRANCHES/MT6620_WIFI_DRIVER_V2_2/include/mgmt/bow_fsm.h#1 $
*/

/*! \file   bow_fsm.h
    \brief  Declaration of functions and finite state machine for BOW Module.

    Declaration of functions and finite state machine for BOW Module.
*/

/*******************************************************************************
* Copyright (c) 2010 MediaTek Inc.
*
* All rights reserved. Copying, compilation, modification, distribution
* or any other use whatsoever of this material is strictly prohibited
* except in accordance with a Software License Agreement with
* MediaTek Inc.
********************************************************************************
*/

/*******************************************************************************
* LEGAL DISCLAIMER
*
* BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK
* SOFTWARE") RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE
* PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY
* DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE
* ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY
* WHICH MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE
* FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION OR TO
* CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
* LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL
* BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT
* ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY
* BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
* WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT
* OF LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING
* THEREOF AND RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN
* FRANCISCO, CA, UNDER THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE
* (ICC).
********************************************************************************
*/

/*
** $Log: bow_fsm.h $
 *
 * 05 22 2011 terry.wu
 * [WCXRP00000735] [MT6620 Wi-Fi][BoW][FW/Driver] Protect BoW connection establishment
 * Submit missing BoW header files.
 *
 * 03 27 2011 chinghwa.yu
 * [WCXRP00000065] Update BoW design and settings
 * Support multiple physical link.
 *
 * 03 18 2011 cp.wu
 * [WCXRP00000577] [MT6620 Wi-Fi][Driver][FW] Create V2.0 branch for firmware and driver
 * create V2.0 driver release based on label "MT6620_WIFI_DRIVER_V2_0_110318_1600" from main trunk
 *
 * 02 16 2011 chinghwa.yu
 * [WCXRP00000065] Update BoW design and settings
 * Add bowNotifyAllLinkDisconnected  interface and change channel grant procedure for bow starting..
 *
 * 02 15 2011 chinghwa.yu
 * [WCXRP00000065] Update BoW design and settings
 * Add channel previledge into _BOW_FSM_INFO_T.
 *
 * 09 16 2010 chinghwa.yu
 * NULL
 * update bowChangeMediaState.
 *
 * 08 24 2010 chinghwa.yu
 * NULL
 * Update BOW for the 1st time.
 */

#ifndef _BOW_FSM_H
#define _BOW_FSM_H

/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/

/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/

#define BOW_BG_SCAN_INTERVAL_MIN_SEC        2 //30 // exponential to 960
#define BOW_BG_SCAN_INTERVAL_MAX_SEC        2 //960 // 16min

#define BOW_DELAY_TIME_OF_DISCONNECT_SEC    10

#define BOW_BEACON_TIMEOUT_COUNT_STARTING   10
#define BOW_BEACON_TIMEOUT_GUARD_TIME_SEC   1 /* Second */

#define BOW_BEACON_MAX_TIMEOUT_TU           100
#define BOW_BEACON_MIN_TIMEOUT_TU           5
#define BOW_BEACON_MAX_TIMEOUT_VALID        TRUE
#define BOW_BEACON_MIN_TIMEOUT_VALID        TRUE

#define BOW_BMC_MAX_TIMEOUT_TU              100
#define BOW_BMC_MIN_TIMEOUT_TU              5
#define BOW_BMC_MAX_TIMEOUT_VALID           TRUE
#define BOW_BMC_MIN_TIMEOUT_VALID           TRUE

#define BOW_JOIN_CH_GRANT_THRESHOLD         10
#define BOW_JOIN_CH_REQUEST_INTERVAL        2000

/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/

typedef enum _ENUM_BOW_STATE_T {
    BOW_STATE_IDLE = 0,
    BOW_STATE_SEARCH,
    BOW_STATE_SCAN,
    BOW_STATE_ONLINE_SCAN,
    BOW_STATE_LOOKING_FOR,
    BOW_STATE_WAIT_FOR_NEXT_SCAN,
    BOW_STATE_REQ_CHANNEL_JOIN,
    BOW_STATE_REQ_CHANNEL_ALONE,
    BOW_STATE_REQ_CHANNEL_MERGE,
    BOW_STATE_JOIN,
    BOW_STATE_IBSS_ALONE,
    BOW_STATE_IBSS_MERGE,
    BOW_STATE_NORMAL_TR,
    BOW_STATE_NUM
} ENUM_BOW_STATE_T;

typedef struct _BOW_FSM_INFO_T {
    ENUM_BOW_STATE_T    ePreviousState;
    ENUM_BOW_STATE_T    eCurrentState;

    BOOLEAN             fgTryScan;

    /* Channel Privilege */

    BOOLEAN             fgIsInfraChannelFinished;
    BOOLEAN             fgIsChannelRequested;
    BOOLEAN             fgIsChannelGranted;
    BOOLEAN             fgIsScanPending;
    UINT_32                      u4ChGrantedInterval;


    UINT_8                  ucPrimaryChannel;
    ENUM_BAND_T       eBand;
    UINT_16                 u2BeaconInterval;

    ENUM_BOW_STATE_T    eReturnState;     /* Return state after current activity finished or abort. */
    ENUM_BOW_STATE_T    eForwardState;   /* Step to next state if ACTION frame is TX successfully. */

    P_BSS_DESC_T prTargetBss; /* BSS of target P2P Device. For Connection/Service Discovery */

    P_STA_RECORD_T prTargetStaRec;
    P_BSS_DESC_T        prTargetBssDesc;            /* For destination */

    UINT_8          aucPeerAddress[6];

    UINT_8                        ucRole;

    BOOLEAN      fgSupportQoS;

    BOOLEAN fgIsRsponseProbe; /* Indicate if BOW can response probe request frame. */

    /* Sequence number of requested message. */
    UINT_8 ucSeqNumOfChReq;
    UINT_8 ucSeqNumOfReqMsg;
    UINT_8 ucSeqNumOfScnMsg;
    UINT_8 ucSeqNumOfScanReq;

    UINT_8 ucSeqNumOfCancelMsg;

    UINT_8 ucDialogToken;

    /* Timer */
    TIMER_T rStartingBeaconTimer;     /* For device discovery time of each discovery request from user.*/
    TIMER_T rStartingDiscoveryTimer;
    TIMER_T rOperationListenTimer;     /* For Find phase under operational state. */
    TIMER_T rFSMTimer;                      /* A timer used for Action frame timeout usage. */
    TIMER_T rIndicationOfDisconnectTimer;
    TIMER_T rChGrantedTimer;

    UINT_8  ucAvailableAuthTypes;       /* Used for AUTH_MODE_AUTO_SWITCH */

} BOW_FSM_INFO_T, *P_BOW_FSM_INFO_T;


/*******************************************************************************
*                            P U B L I C   D A T A
********************************************************************************
*/

/*******************************************************************************
*                           P R I V A T E   D A T A
********************************************************************************
*/

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/

#define bowChangeMediaState(_prAdapter, _eNewMediaState) \
            (_prAdapter->rWifiVar.arBssInfo[NETWORK_TYPE_BOW_INDEX].eConnectionState = (_eNewMediaState));

/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/

/*******************************************************************************
*                              F U N C T I O N S
********************************************************************************
*/



#endif
