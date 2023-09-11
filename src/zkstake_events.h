/******************************************************************************
 * Copyright © 2014-2019 The SuperNET Developers.                             *
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
#pragma once
#include "zkstake_defs.h"
#include "zkstake_structs.h"

void zkstake_eventadd_notarized(zkstake_state *sp, const char *symbol,int32_t height, zkstake::event_notarized& ntz);

void zkstake_eventadd_pubkeys(zkstake_state *sp, const char *symbol,int32_t height, zkstake::event_pubkeys& pk);

void zkstake_eventadd_pricefeed(zkstake_state *sp, const char *symbol,int32_t height, zkstake::event_pricefeed& pf);

void zkstake_eventadd_opreturn(zkstake_state *sp, const char *symbol,int32_t height, zkstake::event_opreturn& opret);

void zkstake_eventadd_kmdheight(zkstake_state *sp, const char *symbol,int32_t height, zkstake::event_kmdheight& kmd_ht);

void zkstake_event_rewind(zkstake_state *sp, const char *symbol,int32_t height);

void zkstake_setkmdheight(zkstake_state *sp,int32_t kmdheight,uint32_t timestamp);
