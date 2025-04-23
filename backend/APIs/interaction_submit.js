const express = require('express');
const router = express.Router();
const crypto = require('crypto');

// none of this follows good practices
// this is merely a PoC

router.post('/interaction/vote', async (req, res) => {
    const { user_id, post_id, comment_id, vote_type } = req.body;
    if (!user_id || !post_id || !comment_id || !vote_type) {
        return res.status(400).json({ error: 'user id, post id, comment id, and vote type are required' });
    }
})

module.exports = router;