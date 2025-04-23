const axios = require('axios');
let txns = []
let block_id_ctr = 1000;

function bundleIntoBlock(threshold){
    if (txns.length <= threshold){
        const payload = {
            validator_id: 1023, //hardcoding this for now
            block_id: Math.floor(Math.random() * block_id_ctr) + 1,
            block_time: "14401919042025",
            transactions: txns
        };
        axios.post('http://localhost:18080/api/consensus', payload)
        .then(response => {
            console.log("consensus blockchain api hit:");
            console.log(response.data);
            txns.length = 0;
        })
        .catch(error => {
            console.error("unable to hit consensus blockchain api:");
            console.error(error.response?.data || error.message);
        })
    }
}

module.exports = {
    txns,
    bundleIntoBlock
};