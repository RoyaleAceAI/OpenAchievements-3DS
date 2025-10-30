// overlay.c -- watches /3ds/achievements/events/ for JSON files and displays notifications
// Build with devkitPro + libctru.

#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define EVENTS_DIR "/3ds/achievements/events"
#define UNLOCKED_DIR "/3ds/achievements/unlocked"
#define MAXF 512

// naive JSON parse for the fields we write (no dependency)
static void parse_and_handle(const char *path){
    FILE *f = fopen(path, "r");
    if(!f) return;
    char buf[2048]; size_t n = fread(buf,1,sizeof(buf)-1,f);
    buf[n]=0; fclose(f);

    // extract fields
    char gameid[128]={0}, achid[128]={0}, title[256]={0}, desc[512]={0};
    char *p;
    if((p = strstr(buf, "\"gameid\":"))){
        sscanf(p, "\"gameid\": \"%127[^\"]\"", gameid);
    }
    if((p = strstr(buf, "\"achid\":"))){
        sscanf(p, "\"achid\": \"%127[^\"]\"", achid);
    }
    if((p = strstr(buf, "\"title\":"))){
        sscanf(p, "\"title\": \"%255[^\"]\"", title);
    }
    if((p = strstr(buf, "\"desc\":"))){
        sscanf(p, "\"desc\": \"%511[^\"]\"", desc);
    }

    // mark unlocked: write line into UNLOCKED_DIR/<gameid>.txt
    if(strlen(gameid)){
        char outpath[MAXF];
        snprintf(outpath, sizeof(outpath), "%s/%s.txt", UNLOCKED_DIR, gameid);
        FILE *out = fopen(outpath, "a");
        if(out){
            fprintf(out, "%s|%s|%s\n", achid, title, desc);
            fclose(out);
        }
    }

    // display notification on top screen
    consoleSelect(GFX_TOP);
    printf("\x1b[2J"); // clear
    printf("Achievement unlocked!\n\n");
    printf("%s\n\n%s\n", title, desc);
}

int main(){
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    consoleInit(GFX_BOTTOM, NULL);

    printf("OpenAchievements Overlay\n");
    printf("Watching %s for events...\n", EVENTS_DIR);
    printf("Create folders:\n  %s\n  %s\n", EVENTS_DIR, UNLOCKED_DIR);
    printf("Press START to exit overlay.\n");

    // ensure dirs exist (attempt)
    mkdir("/3ds", 0777);
    mkdir("/3ds/achievements", 0777);
    mkdir(EVENTS_DIR, 0777);
    mkdir(UNLOCKED_DIR, 0777);

    while(aptMainLoop()){
        hidScanInput();
        u32 kDown = hidKeysDown();
        if(kDown & KEY_START) break;

        // scan events dir
        DIR *d = opendir(EVENTS_DIR);
        if(d){
            struct dirent *de;
            while((de = readdir(d)) != NULL){
                if(strcmp(de->d_name,".")==0 || strcmp(de->d_name,"..")==0) continue;
                char full[MAXF];
                snprintf(full, sizeof(full), "%s/%s", EVENTS_DIR, de->d_name);
                // parse and handle, then remove file
                parse_and_handle(full);
                remove(full);
            }
            closedir(d);
        }

        // draw minimal bottom console info
        consoleSelect(GFX_BOTTOM);
        printf("\x1b[2J");
        printf("Events folder: %s\n", EVENTS_DIR);
        printf("Unlocked are saved in: %s\n", UNLOCKED_DIR);
        printf("\nStart to exit.\n");

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
