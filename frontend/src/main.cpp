#include "wallet.h"
#include "feed.h"

const int screenWidth = 1024, screenHeight = 600;

int main(){
    InitWindow(screenWidth, screenHeight, "OOP Project");
    while(!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(BLACK);
            if(!loginSuccessfulProceed){
                renderWalletWindow(screenWidth, screenHeight);
            }
            else{
                renderFeedWindow(screenWidth, screenHeight, userIDLogin);
            }
        EndDrawing();
    }
    CloseWindow();
}