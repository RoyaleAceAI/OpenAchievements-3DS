// example_game.c -- demonstrates usage of achieve.h
#include <3ds.h>
#include <stdio.h>
#include "achieve.h"

int main(){
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    consoleInit(GFX_BOTTOM, NULL);

    printf("Example Game (demo)\n");
    printf("Press A to unlock an achievement (this writes an event file)\n");
    printf("Run overlay.3dsx to see it become unlocked.\n");
    printf("Press START to exit.\n");

    while(aptMainLoop()){
        hidScanInput();
        u32 kDown = hidKeysDown();
        if(kDown & KEY_START) break;

        if(kDown & KEY_A){
            oa_unlock("examplegame","FIRSTSTEP","First Step","You pressed A in the example demo!");
            consoleSelect(GFX_TOP);
            printf("\nAchievement event written. Open overlay to view.\n");
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
