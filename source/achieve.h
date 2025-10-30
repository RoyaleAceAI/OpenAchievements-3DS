// achieve.h -- tiny file-based achievements API for 3DS homebrew
// Usage: include and call oa_unlock(gameid, id, title, desc);

#ifndef ACHIEVE_H
#define ACHIEVE_H

#include <stdio.h>
#include <time.h>
#include <3ds.h>

static inline void oa_unlock(const char *gameid, const char *achid, const char *title, const char *desc){
    // create events dir path (SD card root /3ds/achievements/events)
    const char *dir = "/3ds/achievements/events";
    // make filename with timestamp and pid to avoid collisions
    u64 tick = svcGetSystemTick();
    char fname[512];
    snprintf(fname, sizeof(fname), "%s/%s-%s-%llu.json", dir, gameid, achid, (unsigned long long)tick);
    // create file
    FILE *f = fopen(fname, "w");
    if(!f) return;
    // write simple JSON (small, safe)
    fprintf(f, "{\n  \"gameid\": \"%s\",\n  \"achid\": \"%s\",\n  \"title\": \"%s\",\n  \"desc\": \"%s\",\n  \"ts\": %llu\n}\n",
            gameid, achid, title, desc, (unsigned long long)tick);
    fclose(f);
}

#endif // ACHIEVE_H
