#include "blockchain.h"

Blockchain::Blockchain(uint64_t system_time){ create_genesis(system_time); }

bool Blockchain::add_block(Block block){
    if(blocks.empty()){
        std::cout << "blocks vector in chain was empty" << std::endl;
        return false;  // this implies there isn't even a genesis block
    }
    if(block.get_previous_hash() != blocks.back().get_current_hash()){
        return false;
    }
    blocks.push_back(block);
    return true;
}

void Blockchain::create_genesis(uint64_t system_time){
    Transaction genesis_transaction;
    genesis_transaction.interaction_type = Platform_Interaction_Type::System;
    genesis_transaction.post = Post{
        Post_Interaction_Type::NA, 0, 0, 0, 0, "0", "0", {""}
    };
    genesis_transaction.comment = Comment{
        Comment_Interaction_Type::NA, 0, 0, 0, 0, 0, 0, "0", "0", {""}
    };
    genesis_transaction.vote = Vote{
        false, false, 0, 0, 0
    };
    genesis_transaction.wallet = Wallet{
        Wallet_Interaction_Type::NA, "0", "0", "0", 0
    };
    genesis_transaction.transaction_id = 0;
    genesis_transaction.transaction_time = system_time;
    genesis_transaction.user_id = 0;  // ik 0 is supposed to be like null all throughout this application and this is technically true here, no user actually makes the genesis block, soooooo 
    genesis_transaction.mod_stat = Moderation_Status::Not_Needed;
    genesis_transaction.signature = "0";

    Block genesis_block(0, system_time, {genesis_transaction}, "0");
    genesis_block.set_validator_id(0);
    blocks.push_back(genesis_block);
}

Block Blockchain::get_last_added_block(){ return blocks.back(); }

std::vector<Block> Blockchain::get_all_blocks(){ return blocks; }