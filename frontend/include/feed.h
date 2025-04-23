#ifndef FEED_H
#define FEED_H

#include <raylib.h>
#include <string>
#include <sstream>
#include <iostream>

bool sendPostRequest(const std::string& postContent, const std::string& userID, int post_type);
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
void renderFeedWindow(int screenWidth, int screenHeigh, std::string userID);
std::string getTimeStamp();

#endif