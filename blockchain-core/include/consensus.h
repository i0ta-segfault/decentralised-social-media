#pragma once

// so the consensus algorithm will be the heart of this blockchain
// i like this hybrid approach for the social media platform
// rather than something computationally intensive like PoW
// we do a PoS + PoR consensus algorithm
// users can stake their coins and these coins will have a weighted sum effect depending on reputation
// so say like validators are bidding to add a block to a chain it'll be like
// validator = choose(x) where x is some metric that is influenced as : x = W.Nc where W is reputation weight and N sub c is number of coins staked


// here we define what it means to "validate" a block
// stuff like recompute and check hash

#pragma once
#include "blockchain.h"

namespace Consensus{
    bool recompute_hash(Block block);
    bool check_timestamp(Block block, uint64_t system_time);
    bool validate_block(Block& block, Blockchain& chain, uint32_t validator_id, uint64_t system_time);
}