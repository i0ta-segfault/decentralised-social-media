#pragma once
#include "transaction.h"
#include <openssl/sha.h>
#include <sstream>

namespace Crypto{
    std::string sha256(const std::string& data);
    std::string serialize_transaction(const Transaction& txn);
    std::string calculate_merkle_root(const std::vector<Transaction>& transactions);
}