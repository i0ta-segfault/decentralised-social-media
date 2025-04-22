#pragma once
// a block in the chain will have the following data:
// metadata - block id, transaction(s), previous hash, block hash
#include "crypto.h"

class Block{
    uint32_t block_id;
    uint32_t validator_id;
    std::vector <Transaction> transactions;
    std::string merkle_root;
    std::string previous_hash;
    std::string current_hash;
    uint64_t block_time;

public:
    Block(uint32_t block_id, uint64_t block_time, std::vector <Transaction> transactions, std::string previous_hash);
    // some helper fns to access the instance variables
    std::vector<Transaction> get_transactions();
    std::string get_previous_hash();
    std::string get_current_hash();
    std::string get_merkle_root();
    uint32_t get_block_id();
    uint64_t get_block_time();
    uint32_t get_validator_id();
    void set_validator_id(uint32_t validator_id);
    void set_previous_hash(std::string previous_hash);

    void calculate_current_hash();
};