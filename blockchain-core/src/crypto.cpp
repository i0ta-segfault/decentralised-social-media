#include "crypto.h"
#include <iomanip>

namespace Crypto{
    std::string sha256(const std::string& data) {   
        // i literally got this off a stackoverflow answer - this works fine though
        // https://terminalroot.com/how-to-generate-sha256-hash-with-cpp-and-openssl
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, data.c_str(), data.size());
        SHA256_Final(hash, &sha256);

        std::stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }
        ss << std::dec;
        return ss.str();
    }

    std::string serialize_transaction(const Transaction& txn){ 
        // convert the Transaction struct to a string - that's serialization - basically converting a complex data structure to a simple one
        // this will make it easier to hash it using sha256
        std::stringstream ss;   
        ss << static_cast<int>(txn.interaction_type)
           << txn.transaction_id
           << txn.transaction_time
           << txn.user_id
           << static_cast<int>(txn.mod_stat)
           << txn.signature;

        if(txn.post.post_type != Post_Interaction_Type::NA){
            ss << static_cast<int>(txn.post.post_type)
               << txn.post.post_id 
               << txn.post.like_count << txn.post.dislike_count << txn.post.comment_count
               << txn.post.post_content_id << txn.post.post_media_url;
            for (const auto& comment_id : txn.post.comment_ids) {
                ss << comment_id;
            }
        }

        if(txn.comment.comment_type != Comment_Interaction_Type::NA){
            ss << static_cast<int>(txn.comment.comment_type)
               << txn.comment.comment_id
               << txn.comment.parent_post_id << txn.comment.parent_comment_id
               << txn.comment.like_count << txn.comment.dislike_count << txn.comment.reply_count
               << txn.comment.comment_content_id << txn.comment.comment_media_url;
            for (const auto& reply_id : txn.comment.reply_ids) {
                ss << reply_id;
            }
        }

        if(txn.vote.post_voted || txn.vote.comment_voted){
            ss << txn.vote.post_id
               << txn.vote.comment_id
               << txn.vote.vote_type;
        }

        if(txn.wallet.wallet_type != Wallet_Interaction_Type::NA){
            ss << static_cast<int>(txn.wallet.wallet_type)
               << txn.wallet.wallet_public_key
               << txn.wallet.wallet_address
               << txn.wallet.receiver_wallet_address
               << txn.wallet.wallet_balance;
        }

        return ss.str();
    }

    std::string calculate_merkle_root(const std::vector<Transaction>& transactions){
        if(transactions.empty()) return "empty_root_lalalala";

        std::vector<std::string> hashes;
        for(const auto& txn: transactions){
            std::string serializedTxn = serialize_transaction(txn);
            hashes.push_back(sha256(serializedTxn));
        }

        while(hashes.size() > 1){
            if(hashes.size() % 2 == 1){
                hashes.push_back(hashes.back()); // if there are odd number of hashes, typically the last hash is duplicated to facilitate proper pairing
            }
            std::vector<std::string> newHashes;
            for(size_t i = 0; i < hashes.size(); i+=2){
                std::string combinedHash = hashes[i] + hashes[i + 1];
                newHashes.push_back(sha256(combinedHash));
            }
            hashes = newHashes;
        }
        return hashes[0];
    }
}