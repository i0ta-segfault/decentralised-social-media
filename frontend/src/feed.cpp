#include "feed.h"

void renderFeedWindow(int screenWidth, int screenHeight){
    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
    DrawText("Make posts here", 100, 100, 20, RAYWHITE);
}