#include <coreinit/thread.h>
#include <coreinit/time.h>

//Homebrew libraries
#include <whb/proc.h>
#include <whb/log.h>
#include <whb/log_console.h>

//Input Library
#include <vpad/input.h>

// C Standard Libraries
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
   WHBProcInit();
   WHBLogConsoleInit();
   WHBLogPrintf("Hello World!");
   VPADStatus status;
   VPADReadError error;
   bool vpad_fatal = false;

   while(WHBProcIsRunning()) {
      VPADRead(VPAD_CHAN_0, &status, 1, &error);
      switch (error) {
            case VPAD_READ_SUCCESS: {
             break;
            }
            case VPAD_READ_NO_SAMPLES: {
               continue;
            }
            case VPAD_READ_INVALID_CONTROLLER: {
               WHBLogPrint("Gamepad disconnected!");
               vpad_fatal = true;
            break;
            }
            default: {
            WHBLogPrintf("Unknown VPAD error! %08X", error);
            vpad_fatal = true;
            break;
            }
      }
      if (vpad_fatal) break;
      if (status.trigger & VPAD_BUTTON_A) {
      WHBLogPrint("Pressed A!");
      }
      WHBLogConsoleDraw();
   }

   WHBLogPrintf("Exiting... good bye.");
   WHBLogConsoleDraw();
   OSSleepTicks(OSMillisecondsToTicks(1000));

   WHBLogConsoleFree();
   WHBProcShutdown();
   return 0;
}
/*// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <switch.h>
// Macros for console window size
#define COLUMNS 20
#define ROWS 20
// Enemy Structure
struct enemy {
        int enemyX;
        int enemyY;
        int enemyAlive;
};
// Main program entrypoint
int main(int argc, char* argv[])
{
    //initializes console
    consoleInit(NULL);
    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    struct enemy en;

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);
    padUpdate(&pad);
    u64 kDown = padGetButtonsDown(&pad);
    //TODO: Refactor to struct
    // X at last frame and Y at last frame
    int movsafe = 1;
    int x = 1;
    int y = 1;
    int xlf, ylf;
    int i, j;
    // Health and Score (Obviously)
    int health = 3;
    int score = 0;
    // Map array
    char map[ROWS][COLUMNS] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //DO NOT WRITE TO THIS LINE IT IS OVERWRITTEN BY HEALTH/SCORE
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,2,4,4,4,7,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,3,4,4,4,6,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
    // Main Loop
    while (appletMainLoop())
    {
        xlf = x;
        ylf = y;
        en.enemyAlive = 1;
        en.enemyX = 10;
        en.enemyY = 10;
        // Checks if moving is safe
        movsafe = 1;

         if (map[y][x] >= 1) {
            movsafe = 0;
            x = xlf;
            y = ylf;
        }
        // Iterating over the array
        for (j = 0; j < COLUMNS; j++) {
            for (i = 0; i < ROWS; i++) {
                switch (map[i][j]) {
                    case 0 :
                        // Change colour to green
                        printf("\x1b[32m");
                        printf("\x1b[%d;%dH%c", i, j, 219);
                        printf("\x1b[0m");
                        break;
                    case 1 :
                        // Change colour to blue
                        printf("\x1b[37;44m");   // Set background to blue, foreground to white
                        printf("\x1b[%d;%dH^", i, j); // Print the white tilde with blue background
                        printf("\x1b[0m");           // Revert back to default colors (black and white)
                        break;
                    case 2 :
                        // bottom-right
                        printf("\x1b[37;42m");
                        printf("\x1b[%d;%dH%c", i, j, 201);
                        printf("\x1b[0m");
                        break;
                    case 3 :
                        // top-right
                        printf("\x1b[37;42m");
                        printf("\x1b[%d;%dH%c", i, j, 200);
                        printf("\x1b[0m");
                        break;
                    case 4 :
                        // left-right
                        printf("\x1b[37;42m");
                        printf("\x1b[%d;%dH%c", i, j, 205);
                        printf("\x1b[0m");
                        break;
                    case 5 :
                        // bottom-top
                        printf("\x1b[37;42m");
                        printf("\x1b[%d;%dH%c", i, j, 186);
                        printf("\x1b[0m");
                        break;
                    case 6 :
                        // left-top
                        printf("\x1b[37;42m");
                        printf("\x1b[%d;%dH%c", i, j, 188);
                        printf("\x1b[0m");
                        break;
                    case 7 : 
                        // bottom - left
                        printf("\x1b[37;42m");
                        printf("\x1b[%d;%dH%c", i, j, 187);
                        printf("\x1b[0m");
                        break;
                    
                }
            }
                
        }
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Left && x > 1 && movsafe == 1) {
            if (map[y][x - 1] == 0) {
                x = x - 1;
            }
        }
        if (kDown & HidNpadButton_Right && x < COLUMNS && movsafe == 1) {
            if (map[y][x + 1] == 0) {
                x = x + 1;
            }
        }
        if (kDown & HidNpadButton_Up && y > 2 && movsafe == 1) {
            if (map[y - 1][x] == 0) {
                y = y - 1;
            }
        }
        if (kDown & HidNpadButton_Down && y < ROWS && movsafe == 1) {
            if (map[y + 1][x] == 0) {
                y = y + 1;
            }

        }
        if (kDown & HidNpadButton_A) {
            score++;
        }
        if (kDown & HidNpadButton_B) {
            health = health - 1;
        }
        printf("\x1b[37;42m");
        printf("\x1b[%d;%dH%c", y, x, 2);
        printf("\x1b[0m");
        if (en.enemyAlive == 1) {
            printf("\x1b[31;42m");
            printf("\x1b[%d;%dH%c", en.enemyY, en.enemyX, 2);
            printf("\x1b[0m");
        }
        if (kDown & HidNpadButton_Plus) {
            break;
        }
        printf("\x1b[0;0HHEALTH %c %d ", 3, health);
        printf("SCORE  %d\n", score);
        while (health <= 0) {
            consoleClear();
            printf("Game Over!");
            consoleUpdate(NULL);
        }
        //updates and clears the console 
        consoleUpdate(NULL);
        consoleClear();
    }
    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}*/
