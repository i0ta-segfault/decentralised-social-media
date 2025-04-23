#ifndef WALLET_H
#define WALLET_H

#include <raylib.h>
#include <string>
#include <sstream>
#include <iostream>

extern bool loginSuccessfulProceed;
extern std::string userIDLogin;

std::string generatePublicKey(const std::string& privateKey, const std::string& userID);
void sendWalletCreationRequest(const std::string& publicKey, const std::string& userID);
bool sendWalletLoginRequest(const std::string& privateKey, const std::string& userID);
size_t WriteCallbackW(void *contents, size_t size, size_t nmemb, void *userp);
void renderSignUp(int screenWidth, int screenHeight);
void renderLogin(int screenWidth, int screenHeight);
void renderWalletWindow(int screenWidth, int screenHeight);

#endif