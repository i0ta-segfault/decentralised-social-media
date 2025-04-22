const axios = require('axios');

let blockPayload = {
  validator_id: null,
  block_id: 78912,
  block_time: "20400212052025",
  transactions: [
    {
      transaction_id: 21,
      transaction_time: "02014912052025",
      user_id: 721,
      mod_stat: 1,
      signature: "xyz987signature",
      interaction_type: 0,
      post: {
        post_type: 0,
        post_id: 9002,
        like_count: 10,
        dislike_count: 2,
        comment_count: 3,
        post_content_id: "content_1",
        post_media_url: "http://example.com/image1.jpg",
        comment_ids: ["c1", "c2"]
      },
      comment: {
        comment_type: 1,
        comment_id: 102,
        parent_post_id: 9002,
        parent_comment_id: 0,
        like_count: 5,
        dislike_count: 1,
        reply_count: 2,
        comment_content_id: "comment_1",
        comment_media_url: "http://example.com/comment1.jpg",
        reply_ids: ["r1", "r2"]
      },
      vote: {
        post_voted: true,
        comment_voted: false,
        post_id: 9002,
        comment_id: 0,
        vote_type: true
      },
      wallet: {
        wallet_type: 0,
        wallet_public_key: "pubkey123",
        wallet_address: "wallet123",
        receiver_wallet_address: "wallet456",
        wallet_balance: 500
      }
    },
    {
      transaction_id: 49,
      transaction_time: "14481012052025",
      user_id: 459,
      mod_stat: 1,
      signature: "cvs912signature",
      interaction_type: 0,
      post: {
        post_type: 0,
        post_id: 1819,
        like_count: 0,
        dislike_count: 22,
        comment_count: 3,
        post_content_id: "content_1",
        post_media_url: "http://example.com/image1.jpg",
        comment_ids: ["c1", "c2"]
      },
      comment: {
        comment_type: 1,
        comment_id: 201,
        parent_post_id: 1001,
        parent_comment_id: 0,
        like_count: 5,
        dislike_count: 1,
        reply_count: 2,
        comment_content_id: "comment_1",
        comment_media_url: "http://example.com/comment1.jpg",
        reply_ids: ["r1", "r2"]
      },
      vote: {
        post_voted: true,
        comment_voted: false,
        post_id: 1001,
        comment_id: 0,
        vote_type: true
      },
      wallet: {
        wallet_type: 0,
        wallet_public_key: "burpeykey123",
        wallet_address: "paywallz",
        receiver_wallet_address: "walletyuneed",
        wallet_balance: 500
      }
    },
    // Add more transactions if needed
  ]
};

let validators = [
    {
        user_id: 119911,
        coins_staked: 400,
        reputation_metric: 10.0
    },
    {
        user_id: 661616,
        coins_staked: 90,
        reputation_metric: 80.9
    },
    {
        user_id: 909123,
        coins_staked: 9,
        reputation_metric: 70.0
    }
]

axios.post('http://localhost:18080/api/validator', validators)
    .then(response => {
        const validator = response.data.chosen_validator;
        console.log("Chosen Validator:", validator);
        blockPayload.validator_id = validator;
        return axios.post('http://localhost:18080/api/consensus', blockPayload);
    })
    .then(response => {
        console.log("Consensus Response:");
        console.log(response.data);
    })
    .catch(error => {
        console.error("Error sending block:");
        console.error(error.response?.data || error.message);
    });