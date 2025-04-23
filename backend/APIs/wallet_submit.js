const express = require('express');
const router = express.Router();
const crypto = require('crypto');
const mariadb = require('mysql2/promise');
const { txns, bundleIntoBlock } = require('../bundler');

// none of this follows good practices
// this is merely a PoC

function generateWalletAddress(pub_key){
    return pub_key + "_address_";
}

function verifyNameSakeSignature(pub_key, msg_unsigned, msg_signed){
    return (msg_signed === pub_key + msg_unsigned + "_sign_");
}

async function setupDB() {
    try {
        const db = await mariadb.createConnection({
            host: 'localhost',
            user: 'root',
            password: 'root',
            database: 'oop_project'
        });
        console.log('Connected to MariaDB');
        return db;
    } catch (err) {
        console.error('Error connecting to MariaDB:', err);
        return null;
    }
}

async function addToDataBase(wallet_address, user_id, wallet_public_key) {
    const db = await setupDB();
    if (!db) return;
    const insertQuery = 'INSERT INTO wallets (user_id, wallet_address, wallet_public_key, wallet_balance) VALUES (?, ?, ?, ?)';
    try {
        await db.execute(insertQuery, [user_id, wallet_address, wallet_public_key, 0]);
        console.log(`New wallet created for ${user_id} with address: ${wallet_address}`);
        return 1;
    } catch (err) {
        console.error('Error inserting wallet into DB:', err);
        return -1;
    } finally {
        db.end();
    }
}

async function getUserID(wallet_address, wallet_public_key) {
    const db = await setupDB();
    if (!db) return;

    const fetchQuery = 'SELECT user_id FROM wallets WHERE wallet_address = ? AND wallet_public_key = ?;';
    try {
        const [result] = await db.execute(fetchQuery, [wallet_address, wallet_public_key]);
        if (result.length === 0) {
            console.error(`No user found for address: ${wallet_address}`);
            return null;
        }
        return result[0].user_id;
    } catch (err) {
        console.error(`Error fetching user ID for ${wallet_address}:`, err);
        return null;
    } finally {
        db.end();
    }
}

router.post('/wallet/create', async (req, res) => {
    const { wallet_public_key, user_id, txn_time } = req.body;
    if (!wallet_public_key || !user_id) {
        return res.status(400).json({ error: 'wallet public key and user id are required' });
    }
    const wallet_address = generateWalletAddress(wallet_public_key);
    const result = await addToDataBase(wallet_address, user_id, wallet_public_key);
    console.log(`New wallet created for ${user_id} with address: ${wallet_address}`);
    if(result === 1){
        const txn = {
            transaction_id: Math.floor(Math.random() * 100) + 1,
            transaction_time: txn_time,
            user_id: user_id,
            mod_stat: 4,
            signature: "abc123signature",
            interaction_type: 3,
            post: {
                post_type: 0,
                post_id: 0,
                like_count: 0,
                dislike_count: 0,
                comment_count: 0,
                post_content_id: "",
                post_media_url: "",
                comment_ids: [""]
            },
            comment: {
                comment_type: 0,
                comment_id: 0,
                parent_post_id: 0,
                parent_comment_id: 0,
                like_count: 0,
                dislike_count: 0,
                reply_count: 0,
                comment_content_id: "",
                comment_media_url: "",
                reply_ids: [""]
            },
            vote: {
                post_voted: false,
                comment_voted: false,
                post_id: 0,
                comment_id: 0,
                vote_type: false
            },
            wallet: {
                wallet_type: 1,
                wallet_public_key: wallet_public_key,
                wallet_address: wallet_address,
                receiver_wallet_address: "",
                wallet_balance: 0
            }
        }
        txns.push(txn);
        bundleIntoBlock(1);
    }
    return res.status(201).json({
        message: 'Wallet created successfully',
        wallet_address
    });
})

router.post('/wallet/login', async (req, res) => {
    const { wallet_public_key, message_unsigned, message_signed } = req.body;
    if(!wallet_public_key || !message_signed || !message_unsigned) {
        return res.status(400).json({ error: 'wallet public key, message unsigned and message signed are required'});
    }
    const wallet_address = generateWalletAddress(wallet_public_key);
    const user_id = await getUserID(wallet_address, wallet_public_key); // this will be returned so the frontend doesn't have to constantly query the db for user id
    if(!user_id) {
        return res.status(404).json({ error: 'User not found' });
    }
    const isValid = verifyNameSakeSignature(wallet_public_key, message_unsigned, message_signed);
    if (!isValid) {
        return res.status(401).json({ error: 'Invalid signature' });
    }
    return res.status(200).json({
        message: 'Login successful',
        user_id: user_id,
    });
})

module.exports = router;