#pragma once
/******************************************************************************
 * Copyright © 2021 ZKstake Core Developers                                    *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * SuperNET software, including this file may be copied, modified, propagated *
 * or distributed except according to the terms contained in the LICENSE file *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/
/****
 * This file provides extern access to variables in zkstake_globals.h
 * Please think twice before adding to this list. Can it be done with a better scope?
 */
#include "zkstake_defs.h"
#include <mutex>
#include <cstdint>

extern bool IS_ZKSTAKE_NOTARY;
extern bool IS_ZKSTAKE_DEALERNODE;
extern char ZKSBUSERPASS[8192+512+1];
extern char BTCUSERPASS[8192]; 
extern char ASSETCHAINS_USERPASS[4096];
extern uint8_t NOTARY_PUBKEY33[33];
extern uint8_t ASSETCHAINS_OVERRIDE_PUBKEY33[33];
extern uint8_t ASSETCHAINS_OVERRIDE_PUBKEYHASH[20];
extern uint8_t ASSETCHAINS_PUBLIC;
extern uint8_t ASSETCHAINS_PRIVATE;
extern uint8_t ASSETCHAINS_TXPOW;
extern uint16_t ZKSB_PORT;
extern uint16_t BITCOIND_RPCPORT;
extern uint16_t DEST_PORT;
extern uint16_t ASSETCHAINS_P2PPORT;
extern uint16_t ASSETCHAINS_RPCPORT;
extern uint16_t ASSETCHAINS_BEAMPORT;
extern uint16_t ASSETCHAINS_CODAPORT;
extern int32_t ZKSTAKE_INSYNC;
extern int32_t ZKSTAKE_LASTMINED;
extern int32_t prevZKSTAKE_LASTMINED;
extern int32_t ZKSTAKE_CCACTIVATE;
extern int32_t JUMBLR_PAUSE;
extern int32_t ZKSTAKE_MININGTHREADS;
extern int32_t STAKED_NOTARY_ID;
extern int32_t USE_EXTERNAL_PUBKEY;
//extern int32_t ASSETCHAINS_SEED;
extern int32_t ZKSTAKE_ON_DEMAND;
extern int32_t ZKSTAKE_EXTERNAL_NOTARIES;
extern int32_t ZKSTAKE_REWIND;
extern int32_t STAKED_ERA;
extern int32_t ZKSTAKE_CONNECTING;
extern int32_t ZKSTAKE_EXTRASATOSHI;
extern int32_t ASSETCHAINS_FOUNDERS;
extern int32_t ASSETCHAINS_CBMATURITY;
extern int32_t ZKSTAKE_NSPV;
extern bool ZKSTAKE_LOADINGBLOCKS;
extern uint32_t ASSETCHAINS_CC;
extern uint32_t ZKSTAKE_STOPAT;
extern uint32_t ZKSTAKE_DPOWCONFS;
extern uint32_t STAKING_MIN_DIFF;
extern uint32_t ASSETCHAIN_INIT;
extern uint32_t ASSETCHAINS_NUMALGOS;
extern uint32_t ASSETCHAINS_MINDIFF[];
extern uint64_t PENDING_ZKSTAKE_TX;
extern uint64_t ASSETCHAINS_TIMELOCKGTE;
extern uint64_t ASSETCHAINS_ENDSUBSIDY[ASSETCHAINS_MAX_ERAS+1];
extern uint64_t ASSETCHAINS_REWARD[ASSETCHAINS_MAX_ERAS+1];
extern uint64_t ASSETCHAINS_HALVING[ASSETCHAINS_MAX_ERAS+1];
extern uint64_t ASSETCHAINS_DECAY[ASSETCHAINS_MAX_ERAS+1];
extern uint64_t ASSETCHAINS_NOTARY_PAY[ASSETCHAINS_MAX_ERAS+1];
extern uint64_t ASSETCHAINS_TIMEUNLOCKFROM;
extern uint64_t ASSETCHAINS_TIMEUNLOCKTO;

extern std::mutex zkstake_mutex;
//extern pthread_mutex_t zkstake_mutex;
extern pthread_mutex_t ZKSTAKE_CC_mutex;

/**
 * @brief Given a currency name, return the index in the ZKSTAKE_STATES array
 * 
 * @param origbase the currency name to look for
 * @return the index in the array, or -1
 */
int32_t zkstake_baseid(const char *origbase);

uint64_t zkstake_current_supply(uint32_t nHeight);
