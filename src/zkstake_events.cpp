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
#include "zkstake_events.h"
#include "zkstake_globals.h"
#include "zkstake_bitcoind.h" // zkstake_verifynotarization
#include "zkstake_notary.h" // zkstake_notarized_update
#include "zkstake_utils.h" // portable_mutex_lock / unlock
#include "zkstake_kv.h"

#define ZKSTAKE_EVENT_RATIFY 'P'
#define ZKSTAKE_EVENT_NOTARIZED 'N'
#define ZKSTAKE_EVENT_KMDHEIGHT 'K'
#define ZKSTAKE_EVENT_REWIND 'B'
#define ZKSTAKE_EVENT_PRICEFEED 'V'
#define ZKSTAKE_EVENT_OPRETURN 'R'

/*****
 * Add a notarized event to the collection
 * @param sp the state to add to
 * @param symbol
 * @param height
 * @param ntz the event
 */
void zkstake_eventadd_notarized( zkstake_state *sp, const char *symbol, int32_t height, zkstake::event_notarized& ntz)
{
    if (IS_ZKSTAKE_NOTARY)   {
        int32_t ntz_verify = zkstake_verifynotarization(symbol, ntz.dest, height, ntz.notarizedheight, ntz.blockhash, ntz.desttxid);
        LogPrint("notarisation", "zkstake_verifynotarization result %d\n", ntz_verify);

        if (ntz_verify < 0)    {
            static uint32_t counter;
            if ( counter++ < 100 )
                LogPrintf("[%s] error validating notarization ht.%d notarized_height.%d, if on a pruned %s node this can be ignored\n",
                        chainName.symbol().c_str(), height, ntz.notarizedheight, ntz.dest);
            return;
        }
    }
    
    if (chainName.isSymbol(symbol) || chainName.isKMD() && std::string(symbol) == "KMD" /*special case for KMD*/)
    {
        if (sp != nullptr)
        {
            sp->add_event(symbol, height, ntz);
            zkstake_notarized_update(sp, height, ntz.notarizedheight, ntz.blockhash, ntz.desttxid, ntz.MoM, ntz.MoMdepth);
        } else {
            LogPrintf("could not update notarisation event: zkstake_state is null");
        }
    } else {
        LogPrintf("could not update notarisation event: invalid symbol %s", symbol);
    }
}

/*****
 * Add a pubkeys event to the collection
 * @param sp where to add
 * @param symbol
 * @param height
 * @param pk the event
 */
void zkstake_eventadd_pubkeys(zkstake_state *sp, const char *symbol, int32_t height, zkstake::event_pubkeys& pk)
{
    if (sp != nullptr)
    {
        sp->add_event(symbol, height, pk);
        zkstake_notarysinit(height, pk.pubkeys, pk.num);
    }
}

/********
 * Add a pricefeed event to the collection
 * @note was for PAX, deprecated
 * @param sp where to add
 * @param symbol
 * @param height
 * @param pf the event
 */
void zkstake_eventadd_pricefeed( zkstake_state *sp, const char *symbol, int32_t height, zkstake::event_pricefeed& pf)
{
    if (sp != nullptr)
    {
        sp->add_event(symbol, height, pf);
    }
}

/*****
 * Add an opreturn event to the collection
 * @param sp where to add
 * @param symbol
 * @param height
 * @param opret the event
 */
void zkstake_eventadd_opreturn( zkstake_state *sp, const char *symbol, int32_t height, zkstake::event_opreturn& opret)
{
    if ( sp != nullptr && !chainName.isKMD() )
    {
        sp->add_event(symbol, height, opret);
        //zkstake_opreturn(height, opret->value, opret->opret.data(), opret->opret.size(), opret->txid, opret->vout, symbol);
        if ( opret.opret.data()[0] == 'K' && opret.opret.size() != 40 )
        {
            zkstake_kvupdate(opret.opret.data(), opret.opret.size(), opret.value);
        }
    }
}

/*****
 * @brief Undo an event
 * @note seems to only work for KMD height events
 * @param sp the state object
 * @param ev the event to undo
 */
template<class T>
void zkstake_event_undo(zkstake_state *sp, T& ev)
{
}

template<>
void zkstake_event_undo(zkstake_state* sp, zkstake::event_kmdheight& ev)
    {
    if ( ev.height <= sp->SAVEDHEIGHT )
        sp->SAVEDHEIGHT = ev.height;
    }
 


void zkstake_event_rewind(zkstake_state *sp, const char *symbol, int32_t height)
{
    if ( sp != nullptr )
    {
        if ( chainName.isKMD() && height <= ZKSTAKE_LASTMINED && prevZKSTAKE_LASTMINED != 0 )
        {
            LogPrintf("undo ZKSTAKE_LASTMINED %d <- %d\n",ZKSTAKE_LASTMINED,prevZKSTAKE_LASTMINED);
            ZKSTAKE_LASTMINED = prevZKSTAKE_LASTMINED;
            prevZKSTAKE_LASTMINED = 0;
        }
        while ( sp->events.size() > 0)
        {
            auto ev = sp->events.back();
            if (ev-> height < height)
                    break;
            zkstake_event_undo(sp, *ev);
            sp->events.pop_back();
        }
    }
}

void zkstake_setkmdheight(struct zkstake_state *sp,int32_t kmdheight,uint32_t timestamp)
{
    if ( sp != nullptr )
    {
        if ( kmdheight > sp->SAVEDHEIGHT )
        {
            sp->SAVEDHEIGHT = kmdheight;
            sp->SAVEDTIMESTAMP = timestamp;
        }
        if ( kmdheight > sp->CURRENT_HEIGHT )
            sp->CURRENT_HEIGHT = kmdheight;
    }
}

/******
 * @brief handle a height change event (forward or rewind)
 * @param sp
 * @param symbol
 * @param height
 * @param kmdht the event
 */
void zkstake_eventadd_kmdheight(struct zkstake_state *sp, const char *symbol,int32_t height,
        zkstake::event_kmdheight& kmdht)
{
    if (sp != nullptr)
    {
        if ( kmdht.kheight > 0 ) // height is advancing
        {

            sp->add_event(symbol, height, kmdht);
            zkstake_setkmdheight(sp, kmdht.kheight, kmdht.timestamp);
        }
        else // rewinding
        {
            zkstake::event_rewind e(height);
            sp->add_event(symbol, height, e);
            zkstake_event_rewind(sp,symbol,height);
        }
    }
}
