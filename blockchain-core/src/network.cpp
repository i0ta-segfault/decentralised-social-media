#include "network.h"

void start_rest_api(Blockchain& chain) {
    crow::SimpleApp app; 
    CROW_ROUTE(app, "/api/validator").methods(crow::HTTPMethod::Post)(
        [](const crow::request& req) {
            auto body = crow::json::load(req.body);
            if (!body) {
                return crow::response(400, "Invalid JSON");
            }
            std::vector<ValidatorInfo> validators;
            for (const auto& item : body) {
                if (!item.has("user_id") || !item.has("coins_staked") || !item.has("reputation_metric")) {
                    return crow::response(400, "Missing fields in validator object");
                }
                validators.emplace_back(
                    item["user_id"].u(),
                    item["coins_staked"].i(),
                    item["reputation_metric"].d()
                );
            }
            uint32_t chosen_validator = Validator::choose_a_validator(validators);
            crow::json::wvalue result;
            result["chosen_validator"] = chosen_validator;
            return crow::response(result);
        }
    );

    CROW_ROUTE(app, "/api/consensus").methods(crow::HTTPMethod::Post)(
    [&chain](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        std::vector<Transaction> transactions;
        for (const auto& tx_json : body["transactions"]) {
            Post post{
                static_cast<Post_Interaction_Type>(tx_json["post"]["post_type"].i()),
                static_cast<uint32_t>(tx_json["post"]["post_id"].u()),
                static_cast<uint32_t>(tx_json["post"]["like_count"].u()),
                static_cast<uint32_t>(tx_json["post"]["dislike_count"].u()),
                static_cast<uint32_t>(tx_json["post"]["comment_count"].u()),
                tx_json["post"]["post_content_id"].s(),
                tx_json["post"]["post_media_url"].s(),
                std::vector<std::string>{}
            };
            for (auto& cid : tx_json["post"]["comment_ids"]) {
                post.comment_ids.push_back(cid.s());
            }

            Comment comment{
                static_cast<Comment_Interaction_Type>(tx_json["comment"]["comment_type"].u()),
                static_cast<uint16_t>(tx_json["comment"]["comment_id"].u()),
                static_cast<uint32_t>(tx_json["comment"]["parent_post_id"].u()),
                static_cast<uint16_t>(tx_json["comment"]["parent_comment_id"].u()),
                static_cast<uint32_t>(tx_json["comment"]["like_count"].u()),
                static_cast<uint32_t>(tx_json["comment"]["dislike_count"].u()),
                static_cast<uint32_t>(tx_json["comment"]["reply_count"].u()),
                tx_json["comment"]["comment_content_id"].s(),
                tx_json["comment"]["comment_media_url"].s(),
                std::vector<std::string>{}
            };
            for (auto& rid : tx_json["comment"]["reply_ids"]) {
                comment.reply_ids.push_back(rid.s());
            }

            Vote vote{
                tx_json["vote"]["post_voted"].b(),
                tx_json["vote"]["comment_voted"].b(),
                static_cast<uint32_t>(tx_json["vote"]["post_id"].u()),
                static_cast<uint16_t>(tx_json["vote"]["comment_id"].u()),
                tx_json["vote"]["vote_type"].b()
            };

            Wallet wallet{
                static_cast<Wallet_Interaction_Type>(tx_json["wallet"]["wallet_type"].i()),
                tx_json["wallet"]["wallet_public_key"].s(),
                tx_json["wallet"]["wallet_address"].s(),
                tx_json["wallet"]["receiver_wallet_address"].s(),
                static_cast<uint64_t>(tx_json["wallet"]["wallet_balance"].u())
            };

            Transaction txn{
                static_cast<Platform_Interaction_Type>(tx_json["interaction_type"].i()),
                post,
                comment,
                vote,
                wallet,
                static_cast<uint32_t>(tx_json["transaction_id"].u()),
                std::stoll(tx_json["transaction_time"].s()),
                static_cast<uint32_t>(tx_json["user_id"].u()),
                static_cast<Moderation_Status>(tx_json["mod_stat"].i()),
                tx_json["signature"].s()
            };

            transactions.push_back(txn);
        }

        Block block(
            static_cast<uint32_t>(body["block_id"].u()),
            std::stoll(body["block_time"].s()),
            transactions,
            ""
        );

        CROW_LOG_INFO << "Consensus JSON: " << req.body;

        bool result = Consensus::validate_block(block, chain, body["validator_id"].u(), body["block_time"].u());
        if (result) {
            std::cout << "\nBlock validated and added to the chain!\n";
            std::cout << "Blockchain state:\n";
            std::vector<Block> blocks = chain.get_all_blocks();
            for (Block& blk : blocks) {
                std::cout << "------------------------------------------\n";
                std::cout << "Block ID: " << blk.get_block_id() << std::endl;
                std::cout << "Block Hash: " << blk.get_current_hash() << std::endl;
                std::cout << "Previous Hash: " << blk.get_previous_hash() << std::endl;
                std::cout << "Merkle Root: " << blk.get_merkle_root() << std::endl;
                std::cout << "Validator ID: " << blk.get_validator_id() << std::endl;
                std::cout << "Transactions in Block: \n";
                for (const auto& tx : blk.get_transactions()) {
                    std::cout << "  - Transaction ID: " << tx.transaction_id << std::endl;
                    std::cout << "    User ID: " << tx.user_id << std::endl;
                    std::cout << "    Serialized: " << Crypto::serialize_transaction(tx) << std::endl;
                    std::cout << "    Hash: " << Crypto::sha256(Crypto::serialize_transaction(tx)) << std::endl;
                }
            }
            std::cout << "------------------------------------------\n";
        }

        crow::json::wvalue response;
        response["validation_result"] = result;
        return crow::response(response);
    });
    
    app.port(18080).multithreaded().run();
}