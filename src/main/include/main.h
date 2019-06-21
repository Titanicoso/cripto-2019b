#ifndef MAIN_H
#define MAIN_H

#define DISTRIBUTION_MODE 1
#define RECOVERY_MODE 2

typedef struct {
    int mode;
    char* image;
    char* watermark;
    int k;
    int n;
    char* dir;
    char* error;
} options_st;

int setMode(int);
int setImage(const char*);
int setWatermark(const char*);
int setN(const char *);
int setK(const char *);
int setDirectory(const char*);
int directoryExists(const char* directory);
int fileExists(const char * file);
bool execute(options_st * options);
int isNumber(const char * number);

extern options_st * options;

#endif