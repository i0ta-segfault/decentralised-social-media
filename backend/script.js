const express = require('express');
const bodyParser = require('body-parser');
const walletRoutes = require('./APIs/wallet_submit');
const interactionRoutes = require('./APIs/interaction_submit');

const app = express();
const port = 3000;

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(walletRoutes);
app.use(interactionRoutes);
app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});