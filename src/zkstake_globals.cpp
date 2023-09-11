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
#include "zkstake_defs.h"
#include "zkstake_globals.h"
#include "zkstake_extern_globals.h"
#include "zkstake_notary.h"

// global zkstake vars definitions
std::mutex zkstake_mutex;
//pthread_mutex_t staked_mutex; // todo remove

struct knotaries_entry *Pubkeys;

struct zkstake_state ZKSTAKE_STATES[ZKSTAKE_STATES_NUMBER]; // 0 == asset chain, 1 == KMD

unsigned int WITNESS_CACHE_SIZE = _COINBASE_MATURITY+10;
uint256 ZKSTAKE_EARLYTXID;

bool ZKSTAKE_LOADINGBLOCKS; // defined in pow.cpp, boolean, 1 if currently loading the block index, 0 if not
bool IS_ZKSTAKE_NOTARY;
bool IS_MODE_EXCHANGEWALLET = false;
bool IS_ZKSTAKE_DEALERNODE;
int32_t ZKSTAKE_MININGTHREADS = -1,STAKED_NOTARY_ID,USE_EXTERNAL_PUBKEY,ASSETCHAINS_SEED,ZKSTAKE_ON_DEMAND,ZKSTAKE_EXTERNAL_NOTARIES,ZKSTAKE_REWIND,STAKED_ERA,ZKSTAKE_CONNECTING = -1,ZKSTAKE_EXTRASATOSHI,ASSETCHAINS_FOUNDERS,ASSETCHAINS_CBMATURITY,ZKSTAKE_NSPV;
int32_t ZKSTAKE_INSYNC,ZKSTAKE_LASTMINED,prevZKSTAKE_LASTMINED,ZKSTAKE_CCACTIVATE,JUMBLR_PAUSE = 1;
std::string NOTARY_PUBKEY,ASSETCHAINS_NOTARIES,ASSETCHAINS_OVERRIDE_PUBKEY,DONATION_PUBKEY,ASSETCHAINS_SCRIPTPUB,NOTARY_ADDRESS,ASSETCHAINS_SELFIMPORT,ASSETCHAINS_CCLIB;
uint8_t NOTARY_PUBKEY33[33],ASSETCHAINS_OVERRIDE_PUBKEY33[33],ASSETCHAINS_OVERRIDE_PUBKEYHASH[20],ASSETCHAINS_PUBLIC,ASSETCHAINS_PRIVATE,ASSETCHAINS_TXPOW;
int8_t ASSETCHAINS_ADAPTIVEPOW;
std::vector<std::string> vWhiteListAddress;
char NOTARYADDRS[64][64];
char NOTARY_ADDRESSES[NUM_KMD_SEASONS][64][64];

char ASSETCHAINS_SYMBOL[ZKSTAKE_ASSETCHAIN_MAXLEN],ASSETCHAINS_USERPASS[4096];
uint16_t ASSETCHAINS_P2PPORT,ASSETCHAINS_RPCPORT,ASSETCHAINS_BEAMPORT,ASSETCHAINS_CODAPORT;
uint32_t ASSETCHAIN_INIT,ASSETCHAINS_CC,ZKSTAKE_STOPAT,ZKSTAKE_DPOWCONFS = 1,STAKING_MIN_DIFF;
uint32_t ASSETCHAINS_MAGIC = 2387029918;
int64_t ASSETCHAINS_GENESISTXVAL = 5000000000;

int64_t MAX_MONEY = 200000000 * 100000000LL;

// consensus variables for coinbase timelock control and timelock transaction support
// time locks are specified enough to enable their use initially to lock specific coinbase transactions for emission control
// to be verifiable, timelocks require additional data that enables them to be validated and their ownership and
// release time determined from the blockchain. to do this, every time locked output according to this
// spec will use an op_return with CLTV at front and anything after |OP_RETURN|PUSH of rest|OPRETTYPE_TIMELOCK|script|
uint64_t ASSETCHAINS_TIMELOCKGTE = _ASSETCHAINS_TIMELOCKOFF;
uint64_t ASSETCHAINS_TIMEUNLOCKFROM = 0, ASSETCHAINS_TIMEUNLOCKTO = 0;

uint64_t ASSETCHAINS_LASTERA = 1;
uint64_t ASSETCHAINS_ENDSUBSIDY[ASSETCHAINS_MAX_ERAS+1],ASSETCHAINS_REWARD[ASSETCHAINS_MAX_ERAS+1],ASSETCHAINS_HALVING[ASSETCHAINS_MAX_ERAS+1],ASSETCHAINS_DECAY[ASSETCHAINS_MAX_ERAS+1],ASSETCHAINS_NOTARY_PAY[ASSETCHAINS_MAX_ERAS+1];
uint8_t ASSETCHAINS_CCDISABLES[256];
std::vector<std::string> ASSETCHAINS_PRICES,ASSETCHAINS_STOCKS;

uint32_t ASSETCHAINS_NUMALGOS = 1;
uint32_t ASSETCHAINS_EQUIHASH = _ASSETCHAINS_EQUIHASH;

const char *ASSETCHAINS_ALGORITHMS[]    = { "equihash" };
uint64_t ASSETCHAINS_NONCEMASK[]        = { 0xffff };
uint32_t ASSETCHAINS_NONCESHIFT[]       = { 32 };
uint32_t ASSETCHAINS_HASHESPERROUND[]   = { 1 };
// min diff returned from GetNextWorkRequired needs to be added here for each algo, so they can work with ac_staked.
uint32_t ASSETCHAINS_MINDIFF[]          = { 537857807 }; // ZKSTAKE_MINDIFF_NBITS = 0x200f0f0f

uint32_t ASSETCHAINS_ALGO = _ASSETCHAINS_EQUIHASH;

int32_t ASSETCHAINS_SAPLING = -1;
int32_t ASSETCHAINS_OVERWINTER = -1;

uint64_t ZKSTAKE_INTERESTSUM,ZKSTAKE_WALLETBALANCE;
int32_t ASSETCHAINS_STAKED;
uint64_t ASSETCHAINS_COMMISSION,ASSETCHAINS_SUPPLY = 10,ASSETCHAINS_FOUNDERS_REWARD;

uint32_t ZKSTAKE_INITDONE;
char ZKSBUSERPASS[8192+512+1],BTCUSERPASS[8192]; uint16_t ZKSB_PORT = 17838,BITCOIND_RPCPORT = 17838, DEST_PORT;
uint64_t PENDING_ZKSTAKE_TX;
unsigned int MAX_BLOCK_SIGOPS = 20000;

bool IS_ZKSTAKE_TESTNODE;
int32_t ZKSTAKE_SNAPSHOT_INTERVAL; 
CScript ZKSTAKE_EARLYTXID_SCRIPTPUB;
int32_t ASSETCHAINS_EARLYTXIDCONTRACT;
int32_t ASSETCHAINS_STAKED_SPLIT_PERCENTAGE;

std::map <std::int8_t, int32_t> mapHeightEvalActivate;


/**
 * @brief Given a currency name, return the index in the ZKSTAKE_STATES array
 * 
 * @param origbase the currency name to look for
 * @return 0 for an asset chain, 1 for KMD, or -1
 */
int32_t zkstake_baseid(const char *origbase)
{
    // convert to upper case
    std::string base(origbase);
    std::transform(base.begin(),base.end(),base.begin(),[](char s){return toupper(s & 0xff);});
    for(size_t i = 0; i < ZKSTAKE_STATES_NUMBER; ++i)
    {
        if (ZKSTAKE_STATES[i].symbol == base)
            return i;
    }
    return -1;
}

// todo remove
//#ifndef SATOSHIDEN
//#define SATOSHIDEN ((uint64_t)100000000L)
//#endif
