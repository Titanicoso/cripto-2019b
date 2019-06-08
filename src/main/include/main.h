#ifndef MAIN_H
#define MAIN_H

#define DISTRIBUTION_MODE 1
#define RECOVERY_MODE 2

typedef struct {
    int mode;
    const char* image;
    const char* watermark;
    int k;
    int n;
    const char* dir;
    char* error;
} options_st;

int setMode(int);
int setImage(const char*);
int setWatermark(const char*);
int setN(int);
int setK(int);
int setDirectory(const char*);
int directoryExists(const char* directory);
int fileExists(const char * file);
int execute(options_st * options);

extern options_st * options;

#endif