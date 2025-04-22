#include "validator.h"
#include <algorithm>

// the backend will call this choose_a_validator
// vaidator chosen will call consensus.cpp's validate_block
namespace Validator{
    bool compare_validators(const ValidatorInfo& a, const ValidatorInfo& b){
        return ((a.reputation_metric * a.coins_staked) < (b.reputation_metric * b.coins_staked));
    }

    uint32_t choose_a_validator(std::vector<ValidatorInfo> validators){
        if(validators.empty()) return 0;
        auto validator = std::max_element(validators.begin(), validators.end(), compare_validators);
        return validator->user_id;
    }
}
