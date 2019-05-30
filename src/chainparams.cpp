// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin developers
// Copyright (c) 2014-2019 The Dash developers
// Copyright (c) 2015-2019 The PIVX developers
// Copyright (c) 2019 The Vulcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x000005f669ee05905157e085dc45fd43377b003126e1690584ac88f913b4df59"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1558921791, // * UNIX timestamp of last checkpoint block
    0,          // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));

static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1740710,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};
void MineGenesis(CBlock genesis, uint256 bnProofOfWorkLimit) {
    printf("Mining genesis block...\n");

	uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
	
	bnTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow);
    //for (genesis.nNonce == 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++) { } 
	if(genesis.nNonce == 0){
		while (genesis.GetHash() > bnTarget){
			genesis.nNonce++;
		}
	}

    printf("Mined genesis block:\n");
    printf("Merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
    printf("Nonce: %u\n", genesis.nNonce);
	printf("Bits: %u\n", genesis.nBits);
    printf("Hash: %s\n", genesis.GetHash().ToString().c_str());
	printf("POW: %s\n", bnTarget.ToString().c_str());
}
class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x3a;
        pchMessageStart[1] = 0x19;
        pchMessageStart[2] = 0xfe;
        pchMessageStart[3] = 0xa8;
        vAlertPubKey = ParseHex("049844e566f16fb8039ff3de11da6e9e086fd825de36ff400ea47df61f09464eff700551b656e90f835ce663cf1ff2ad12d8ca30eb1f06c0ace544e90231d1fc10");
        nDefaultPort = 22300;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        bnProofOfStakeLimit = ~uint256(0) >> 24;
        nSubsidyHalvingInterval = 10050000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 3 * 60;      // Vulcoin: 3 minutes
        nTargetSpacing = 3 * 60;       // Vulcoin: 3 minutes
        nStakeMinAge = 3 * 60 * 60;   // Vulcoin: 24 hours
        nMaturity = 10;
        nMasternodeCountDrift = 15;
        nMaxMoneyOut = 77000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 350;
        nModifierUpdateBlock = 1; // We use the version 2 for VLC

		/**

		Merkle root: 7bbcd6458f795847babf3ce2a1b133dac37bdb32c355fbcbc8e8611316207040
		Nonce: 1235998
		Bits: 504365040
		Hash: 000005f669ee05905157e085dc45fd43377b003126e1690584ac88f913b4df59
		**/
		
        const char* pszTimestamp = "The Vulcoin is the best for your money 19/02/2019 for your life";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04f3efc5b10b3b1f3b654ee120a708ecc38ae9236ab3dfe0e7b6f17d67c0b1188a6d16b5e40fcb45728cb4ee4f5aa5b7415499a52e7976acd46b8c8305e0c36526") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1558921791;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 1235998;
		
		//MineGenesis(genesis, bnProofOfWorkLimit);
		
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000005f669ee05905157e085dc45fd43377b003126e1690584ac88f913b4df59"));
        assert(genesis.hashMerkleRoot == uint256("0x7bbcd6458f795847babf3ce2a1b133dac37bdb32c355fbcbc8e8611316207040"));

        // DNS Seeding
        vSeeds.push_back(CDNSSeedData("202.182.114.6", "202.182.114.6"));
		vSeeds.push_back(CDNSSeedData("45.77.31.201", "45.77.31.201"));
		vSeeds.push_back(CDNSSeedData("202.182.115.47", "202.182.115.47"));
		vSeeds.push_back(CDNSSeedData("45.32.8.155", "45.32.8.155"));
		
        // Vulcoin addresses start with 'V'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 70);
        // Vulcoin script addresses start with '3'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 10);
        // Vulcoin private keys start with 'K'
        base58Prefixes[VLCRET_KEY] = std::vector<unsigned char>(1, 46);
        // Vulcoin BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Vulcoin BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_VLCRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // Vulcoin BIP44 coin type is '259' (0x80000103)
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x01)(0x03).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "034d70c92ac068eba8b30d4dacc3f3f08aa6a98825da7bcfed9a188c935f1d493d";
        strMasternodePoolDummyAddress = "ScvD2T53bhMGBtgN34UgT93Jh3eDHz8fFd";
        nStartMasternodePayments = genesis.nTime + 6000; // 12 hours after genesis creation

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x15;
        pchMessageStart[1] = 0xa9;
        pchMessageStart[2] = 0xeb;
        pchMessageStart[3] = 0x31;
        vAlertPubKey = ParseHex("04325307e3c5b41c8dab8afbe1aad4baea0590c51440d3edeac88b31663e9b772e3b0d5ced5f88ae718e7a412906f9c4ff973aeaf9834eb9d475451dfa2971e9ed");
        nDefaultPort = 12040;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60;        // Vulcoin: 1 day
        nTargetSpacing = 2 * 60;         // Vulcoin: 2 minutes
        nStakeMinAge = 10 * 60;          // Vulcoin: 10 minutes
        nLastPOWBlock = 200;
        nMaturity = 10;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 1;
        nMaxMoneyOut = 29300000 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1536871198;
        genesis.nNonce = 0;

        hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0x00000ffc5446ef4936227f471288c4556dcd6b935e555542a0871c1bdaa5d447"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // Testnet Vulcoin addresses start with 'z'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 142);
        // Testnet Vulcoin script addresses start with '5' or '6'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 12);
        // Testnet private keys start with 'k'
        base58Prefixes[VLCRET_KEY] = std::vector<unsigned char>(1, 108);
        // Testnet Vulcoin BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet Vulcoin BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_VLCRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Testnet vulcoin BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04b53318b4ff87bd5b6669be3526b33b67da7e1d2d9cc54ed289a70b284c7b88ff9f3704ea490777dc3753f1323e2aeeefb7e297cc3efcb8927a2a0ff9174e4567";
        strMasternodePoolDummyAddress = "gbJ4Qad4xc77PpLzMx6rUegAs6aUPWkcUq";
        nStartMasternodePayments = genesis.nTime + 86400; // 24 hours after genesis
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xb8;
        pchMessageStart[1] = 0xf5;
        pchMessageStart[2] = 0x47;
        pchMessageStart[3] = 0xc7;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Vulcoin: 1 day
        nTargetSpacing = 2 * 60;        // Vulcoin: 2 minutes
        nStakeMinAge = 10 * 60;          // Vulcoin: 10 minutes
        nLastPOWBlock = 200;
        nMaturity = 10;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        bnProofOfStakeLimit = ~uint256(0) >> 24;
        genesis.nTime = 1526573756;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 20542301;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 34925;
        //assert(hashGenesisBlock == uint256("0x32ea7da4d019071c2f819d064a1d8f5e6f82b619975d8207f695cc92bf41394e"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
