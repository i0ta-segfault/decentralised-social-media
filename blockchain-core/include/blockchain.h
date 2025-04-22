#pragma once
#include "block.h"

class Blockchain{
    std::vector<Block> blocks;
    void create_genesis(uint64_t system_time);

public:
    Blockchain(uint64_t system_time);
    Block get_last_added_block();
    std::vector<Block> get_all_blocks();
    bool add_block(Block block);
};