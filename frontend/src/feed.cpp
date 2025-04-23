#include "feed.h"
#include <curl/curl.h>
#include <ctime>
#include <iomanip>

static bool isTypingPost = false;
static bool showPlaceholder = true;
std::string postContent = "";

std::string getTimeStamp(){
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

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool sendPostRequest(const std::string& postContent, const std::string& userID, int post_type) {
    std::srand(time(0));
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    bool success = false;
    if (curl) {
        const std::string url = "http://localhost:3000/interaction/post";
        // taking a little liberty here, rather than post_content i am storing it in post_content_id of transaction.h
        std::string postData = "post_content_id=" + postContent + "&user_id=" + userID + 
                               "&txn_time=" + getTimeStamp() + "&post_type=" + std::to_string(post_type) +
                               "&post_id=" + std::to_string((std::rand() % 1000) + 1);
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } 
        else {
            success = true;
            std::cout << "Response: " << response << std::endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    curl_global_cleanup();
    return success;
}

void renderFeedWindow(int screenWidth, int screenHeight, std::string userID){
    DrawRectangleLines(10, 20, screenWidth - 20, screenHeight - 60, RAYWHITE);
    if(userID == ""){
        userID = "user id not propagated";
    }
    DrawText(userID.c_str(), (2 * screenWidth) / 4, 7, 10, PINK);

    Rectangle createPostArea = {30, 50, 650, 120};
    Rectangle createButtonArea = {700, 50, 100, 50};
    DrawRectangleLines(createPostArea.x, createPostArea.y, createPostArea.width, createPostArea.height, YELLOW);
    if(showPlaceholder)
        DrawText("Create post", 70, 70, 12, Color{245, 245, 245, 127});
    DrawRectangleLines(createButtonArea.x, createButtonArea.y, createButtonArea.width, createButtonArea.height, GREEN);
    DrawText("Create", 730, 70, 10, RAYWHITE);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, createPostArea)) {
            // just so i don't forget, clicking on input area again will reset input to empty string
            isTypingPost = true;
            postContent = "";
            showPlaceholder = false;
        }
        else {
            isTypingPost = false;
            showPlaceholder = true;
        }
    }
    if (isTypingPost) {
        if (IsKeyPressed(KEY_BACKSPACE) && postContent.length() > 0) {
            postContent.pop_back();
        } 
        else {
            for (int key = KEY_SPACE; key <= KEY_Z; key++) {
                if (IsKeyPressed(key)) {
                    postContent += (char)key;
                }
            }
        }
    }
    DrawText(postContent.c_str(), createPostArea.x + 10, createPostArea.y + 10, 10, RAYWHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, createButtonArea)) {
            if(!postContent.empty())
                if(sendPostRequest(postContent, userID, 1)){
                    std::cout << "post added to chain" << std::endl;
                    showPlaceholder = true;
                    postContent = "";
                }
            else
                std::cout << "user id and public key empty" << std::endl;
        }
    }

    DrawLine(10, 200, screenWidth - 10, 200, RAYWHITE);


}