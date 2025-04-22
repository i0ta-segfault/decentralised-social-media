#include "block.h"

Block::Block(uint32_t block_id, uint64_t block_time, std::vector <Transaction> transactions, std::string previous_hash){
    this->block_id = block_id;
    this->block_time = block_time;
    this->transactions = transactions;
    this->previous_hash = previous_hash;
}

void Block::calculate_current_hash() {
    merkle_root = Crypto::calculate_merkle_root(transactions);
    if(merkle_root == "empty_root_lalalala") merkle_root = "";

    // so real blockchains again serialize block metadata and merkle root to hash again so that's what we'll do too
    std::stringstream header_stream;
    header_stream << merkle_root << previous_hash << block_id << validator_id << block_time;
    std::string block_header = header_stream.str();
    current_hash = Crypto::sha256((block_header));
}

std::string Block::get_previous_hash(){ return previous_hash; }

std::string Block::get_current_hash(){ return current_hash; }

std::string Block::get_merkle_root(){ return merkle_root; }

uint32_t Block::get_block_id(){ return block_id; }

uint64_t Block::get_block_time(){ return block_time; }

uint32_t Block::get_validator_id(){ return validator_id; }

void Block::set_validator_id(uint32_t validator_id){ this->validator_id = validator_id; calculate_current_hash(); }

void Block::set_previous_hash(std::string previous_hash){ this->previous_hash = previous_hash; }

std::vector<Transaction> Block::get_transactions(){ return transactions; }