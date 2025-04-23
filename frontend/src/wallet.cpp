#include "wallet.h"
#include <curl/curl.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <iomanip>
#include <ctime>

std::string privateKey = "";
std::string userID = "";
std::string publicKey = "";
std::string privateKeyLogin = "";
std::string userIDLogin = "";
    
static bool isTypingPvt = false;
static bool isTypingID = false;
static bool isTypingPvtLogin = false;
static bool isTypingIDLogin = false;
static bool drawSuccessLogin = false;

bool loginSuccessfulProceed = false;

std::string getTimeStampW(){
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << now->tm_hour
        << std::setw(2) << std::setfill('0') << now->tm_min
        << std::setw(2) << std::setfill('0') << now->tm_sec
        << std::setw(2) << std::setfill('0') << now->tm_mday
        << std::setw(2) << std::setfill('0') << (now->tm_mon + 1)
        << (now->tm_year + 1900);
    return oss.str();
}

size_t WriteCallbackW(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void sendWalletCreationRequest(const std::string& publicKey, const std::string& userID) {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        const std::string url = "http://localhost:3000/wallet/create";
        std::string postData = "wallet_public_key=" + publicKey + "&user_id=" + userID + "&txn_time=" + getTimeStampW();
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackW);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } 
        else {
            std::cout << "Response: " << response << std::endl;
        }curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    curl_global_cleanup();
}

bool sendWalletLoginRequest(const std::string& privateKey, const std::string& userID){
    std::string message = "login_message_for";
    std::string combined = generatePublicKey(privateKey, userID) + message;
    std::string result = combined + "_sign_";

    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        const std::string url = "http://localhost:3000/wallet/login";
        std::string postData = "wallet_public_key=" + generatePublicKey(privateKey, userID) +
                               "&message_unsigned=" + message + 
                               "&message_signed=" + result;

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackW);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        bool success = false;
        if (res == CURLE_OK) {
            std::cout << "Response: " << response << std::endl;
            if (response.find("Login successful") != std::string::npos) {
                success = true;
            }
        } else {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        curl_global_cleanup();
        return success;
    }

    curl_global_cleanup();
    return false;
}

std::string generatePublicKey(const std::string& privateKey, const std::string& userID) {
    return userID + "_public_";
}

void renderSignUp(int screenWidth, int screenHeight) {
    DrawRectangleLines(12, 80, 480, 490, PINK); // this is the sign up border
    
    Rectangle privateKeyInputArea = { 100, 150, 350, 30 };
    DrawText("Private key: ", 22, 160, 10, RAYWHITE);
    DrawRectangleLines(privateKeyInputArea.x, privateKeyInputArea.y, privateKeyInputArea.width, privateKeyInputArea.height, YELLOW);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, privateKeyInputArea)) {
            // just so i don't forget, clicking on input area again will reset input to empty string
            isTypingPvt = true;
            privateKey = "";
        }
        else {
            isTypingPvt = false;
        }
    }
    if (isTypingPvt) {
        if (IsKeyPressed(KEY_BACKSPACE) && privateKey.length() > 0) {
            privateKey.pop_back();
        } 
        else {
            for (int key = KEY_SPACE; key <= KEY_Z; key++) {
                if (IsKeyPressed(key)) {
                    privateKey += (char)key;
                }
            }
        }
    }
    DrawText(privateKey.c_str(), privateKeyInputArea.x + 10, privateKeyInputArea.y + 10, 10, RAYWHITE);
    
    Rectangle userIDInputArea = { 100, 250, 350, 30 };
    DrawText("User ID: ", 22, 260, 10, RAYWHITE);
    DrawRectangleLines(userIDInputArea.x, userIDInputArea.y, userIDInputArea.width, userIDInputArea.height, YELLOW);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, userIDInputArea)) {
            // just so i don't forget, clicking on input area again will reset input to empty string
            isTypingID = true;
            userID = "";
        }
        else {
            isTypingID = false;
        }
    }
    if (isTypingID) {
        if (IsKeyPressed(KEY_BACKSPACE) && userID.length() > 0) {
            userID.pop_back();
        } 
        else {
            for (int key = KEY_SPACE; key <= KEY_Z; key++) {
                if (IsKeyPressed(key)) {
                    userID += (char)key;
                }
            }
        }
    }
    DrawText(userID.c_str(), userIDInputArea.x + 10, userIDInputArea.y + 10, 10, RAYWHITE);

    Rectangle createButtonArea = Rectangle{220, 350, 100, 50};
    DrawRectangleLines(createButtonArea.x, createButtonArea.y, createButtonArea.width, createButtonArea.height, GREEN);
    DrawText("CREATE", 250, 370, 10, RAYWHITE);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, createButtonArea)) {
            publicKey = generatePublicKey(privateKey, userID);
            if(!publicKey.empty() && !userID.empty())
                sendWalletCreationRequest(publicKey, userID);
            else
                std::cout << "user id and public key empty" << std::endl;
        }
    }
    if(!publicKey.empty())
        DrawText(publicKey.c_str(), 22, 450, 10, GREEN);
}

void renderLogin(int screenWidth, int screenHeight) {
    DrawRectangleLines(530, 80, 480, 490, BLACK); // this is the sign up border
    
    Rectangle privateKeyInputArea = { 618, 150, 350, 30 };
    DrawText("Private key: ", 540, 160, 10, BLACK);
    DrawRectangleLines(privateKeyInputArea.x, privateKeyInputArea.y, privateKeyInputArea.width, privateKeyInputArea.height, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, privateKeyInputArea)) {
            // just so i don't forget, clicking on input area again will reset input to empty string
            isTypingPvtLogin = true;
            privateKeyLogin = "";
        }
        else {
            isTypingPvtLogin = false;
        }
    }
    if (isTypingPvtLogin) {
        if (IsKeyPressed(KEY_BACKSPACE) && privateKeyLogin.length() > 0) {
            privateKeyLogin.pop_back();
        } 
        else {
            for (int key = KEY_SPACE; key <= KEY_Z; key++) {
                if (IsKeyPressed(key)) {
                    privateKeyLogin += (char)key;
                }
            }
        }
    }
    DrawText(privateKeyLogin.c_str(), privateKeyInputArea.x + 10, privateKeyInputArea.y + 10, 10, BLACK);
    
    Rectangle userIDInputArea = { 618, 250, 350, 30 };
    DrawText("User ID: ", 540, 260, 10, BLACK);
    DrawRectangleLines(userIDInputArea.x, userIDInputArea.y, userIDInputArea.width, userIDInputArea.height, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, userIDInputArea)) {
            // just so i don't forget, clicking on input area again will reset input to empty string
            isTypingIDLogin = true;
            userIDLogin = "";
        }
        else {
            isTypingIDLogin = false;
        }
    }
    if (isTypingIDLogin) {
        if (IsKeyPressed(KEY_BACKSPACE) && userIDLogin.length() > 0) {
            userIDLogin.pop_back();
        } 
        else {
            for (int key = KEY_SPACE; key <= KEY_Z; key++) {
                if (IsKeyPressed(key)) {
                    userIDLogin += (char)key;
                }
            }
        }
    }
    DrawText(userIDLogin.c_str(), userIDInputArea.x + 10, userIDInputArea.y + 10, 10, BLACK);

    Rectangle createButtonArea = Rectangle{738, 350, 100, 50};
    DrawRectangleLines(createButtonArea.x, createButtonArea.y, createButtonArea.width, createButtonArea.height, ORANGE);
    DrawText("CREATE", 768, 370, 10, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, createButtonArea)) {
            if(!privateKeyLogin.empty() && !userIDLogin.empty())
                if(!sendWalletLoginRequest(privateKeyLogin, userIDLogin)){
                    std::cout << "login failed" << std::endl;
                }
                else{
                    drawSuccessLogin = true;
                    loginSuccessfulProceed = true;
                    std::cout << "login successfult" << std::endl;
                    std::cout << userIDLogin << std::endl;
                }
            else
                std::cout << "user id and private key empty" << std::endl;
        }
    }
    if(drawSuccessLogin)
        DrawText("Successfully logged in!", 550, 450, 10, GREEN);
}

void renderWalletWindow(int screenWidth, int screenHeight) {
    DrawRectangle(0, 0, screenWidth / 2, screenHeight, BLACK);
    DrawText("CREATE A WALLET", 150, 50, 20, RAYWHITE);
    renderSignUp(screenWidth, screenHeight);

    DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, LIGHTGRAY);
    DrawText("HAVE ONE? LOGIN", 660, 50, 20, BLACK);
    renderLogin(screenWidth, screenHeight);
}
