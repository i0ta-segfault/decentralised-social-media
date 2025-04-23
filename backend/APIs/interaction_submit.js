const express = require('express');
const router = express.Router();
const { txns, bundleIntoBlock } = require('../bundler');
const mariadb = require('mysql2/promise');

// none of this follows good practices
// this is merely a PoC

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

async function addToDataBase(user_id, post_id, post_content_id, like_count, dislike_count, txn_time) {
    const db = await setupDB();
    if (!db) return;
    const insertQuery = 'INSERT INTO posts (user_id, post_id, post_content_id, like_count, dislike_count, txn_time, comment_ids) VALUES (?, ?, ?, ?, ?, ?, ?)';
    try {
        await db.execute(insertQuery, [user_id, post_id, post_content_id, like_count, dislike_count, txn_time, JSON.stringify([])]);
        console.log(`New post created for ${user_id}`);
        return 1;
    } catch (err) {
        console.error('Error inserting post into DB:', err);
        return -1;
    } finally {
        db.end();
    }
}

async function getPostFromDatabase(user_id, post_id) {
    const db = await setupDB();
    if (!db) return;
    const deleteQuery = 'DELETE FROM posts WHERE user_id = ? AND post_id = ?;';
    try {
        await db.execute(deleteQuery, [user_id, post_id]);
        console.log(`Post with ${post_id} and ${user_id} deleted`);
        return 1;
    } catch (err) {
        console.error('Error deleting post from DB:', err);
        return -1;
    } finally {
        db.end();
    }
}

router.post('/interaction/post', async (req, res) => {
    const { post_content_id, user_id, txn_time, post_type, post_id } = req.body;
    if (!user_id || !post_id || !post_content_id || !post_type) {
        return res.status(400).json({ error: 'user id, post id, post content id, and post type are required' });
    }
    if(post_type === "1"){ // create
        const result = await addToDataBase(user_id, post_id, post_content_id, 0, 0, txn_time);
        console.log(`New post created for ${user_id}`);
        if(result === -1){
            console.error("interaction post route failed to add new post");
            return;
        }
        const txn = {
            transaction_id: Math.floor(Math.random() * 100) + 1,
            transaction_time: txn_time,
            user_id: user_id,
            mod_stat: 4,
            signature: "abc123signature",
            interaction_type: 0,
            post: {
                post_type: parseInt(post_type),
                post_id: post_id,
                like_count: 0,
                dislike_count: 0,
                comment_count: 0,
                post_content_id: post_content_id,  // for timesake this will hold the actual post
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
                wallet_type: 0,
                wallet_public_key: "",
                wallet_address: "",
                receiver_wallet_address: "",
                wallet_balance: 0
            }
        }
        txns.push(txn);
        bundleIntoBlock(1);
    }
    else if(post_type === "2"){ // delete
        const result = await getPostFromDatabase(user_id, post_id);
        console.log(`Post associated with ${user_id} and ${post_id} deleted`);
        if(result === -1){
            console.error("interaction post route failed to delete post");
            return;
        }
        const txn = {
            transaction_id: Math.floor(Math.random() * 100) + 1,
            transaction_time: txn_time,
            user_id: user_id,
            mod_stat: 4,
            signature: "abc123signature",
            interaction_type: 0,
            post: {
                post_type: parseInt(post_type),
                post_id: post_id,
                like_count: 0,
                dislike_count: 0,
                comment_count: 0,
                post_content_id: post_content_id,  // for timesake this will hold the actual post
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
                wallet_type: 0,
                wallet_public_key: "",
                wallet_address: "",
                receiver_wallet_address: "",
                wallet_balance: 0
            }
        }
        txns.push(txn);
        bundleIntoBlock(1);

    }    
})

module.exports = router;