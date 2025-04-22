#include "consensus.h"

namespace Consensus{
    bool validate_block(Block& block, Blockchain& chain, uint32_t validator_id, uint64_t system_time){
        std::cout << "validate block called" << std::endl;
        std::cout << "validator id " << validator_id << std::endl;
        block.set_previous_hash(chain.get_last_added_block().get_current_hash());
        block.set_validator_id(validator_id);
        std::cout << "previous hash " << block.get_previous_hash() << std::endl;
        std::cout << "current hash " << block.get_current_hash() << std::endl;
        if(recompute_hash(block) && check_timestamp(block, system_time)){
            std::cout << "entered the recompute hash if clause" << std::endl;
            return(chain.add_block(block));
        }
        return false;
    }

    bool recompute_hash(Block block){
        Block temp = block;
        temp.calculate_current_hash();
        return(block.get_current_hash() == temp.get_current_hash());
    }

    bool check_timestamp(Block block, uint64_t system_time){
        // hr_min_sec_day_month_year
        uint64_t time_stamp = block.get_block_time();
        std::cout << "block time " << time_stamp << std::endl;
        std::cout << "system time " << system_time << std::endl;
        if(time_stamp > system_time){
            return false; // checks if block was manipulated to hold timestamp of some future date 
        }
        if(time_stamp % 10000 != system_time % 10000){
            std::cout << "failing at year" << std::endl;
            return false; // check if year sent by backend and transaction year is same
        }
        // 18014914032025
        // 23370215032025
        int year = time_stamp % 10000;
        int month = (int)(time_stamp / 10000) % 100;
        int day = (int)(time_stamp / 1000000) % 100;
        int sec = (int)(time_stamp / 100000000) % 100;
        int min = (int)(time_stamp / 10000000000) % 100;
        int hour = (int)(time_stamp / 1000000000000) % 100;
        if (month < 1 || month > 12){
            std::cout << "failing at month" << month << std::endl;
            return false;
        }
        if (hour > 23 || min > 59 || sec > 59){
            std::cout << "failing at hr sec min" << hour << min << sec << std::endl;
            return false;
        }
        if (day < 1 || day > 31){
            std::cout << "failing at day" << std::endl;
            return false;
        }
        return true;
    }
}