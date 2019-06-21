#include <stdio.h>

void printWelcome()
{
    printf("\nWelcome to the security program\n");
    printf("Made by Agustín Calatayud, Diego Bruno Cilla and Lóránt Mikolás\n");
    printf("Execute with option -h for help\n");
    printf("Reading options and starting...\n\n");
}

void printHelp()
{
    printf("-d: sets distribution mode.\n");
    printf("-r: sets recovery mode.\n");
    printf("-s [PATH_TO_IMAGE]: the path of the image of extension .bmp.\n");
    printf("                    In case of distribution the file must exist\n");
    printf("                    and it must be black and white (8 bits/pixel)\n");
    printf("                    Otherwise the path will be used as a output file to save the secret.\n");
    printf("-m [PATH_TO_IMAGE]: the path of the image of extension .bmp\n");
    printf("                    In case of distribution the file must exist\n");
    printf("                    it must be black and white (8 bits/pixel)\n");
    printf("                    and it will serve as a watermark verification\n");
    printf("                    Otherwise the image will contain de transformation of the watermark.\n");
    printf("-k [NUMBER]: the minimum quantity of necessary shadows to recover the secret.\n");
    printf("-n [NUMBER]: total ammount of shadows that will be distributed.\n");
    printf("-i [PATH_TO_DIRECTORY]: in case of distribution the directory where the images that\n");
    printf("                        will be use to hide the secret are.\n");
    printf("                        Otherwise is the path to directory where the images with the secret are.\n");
}

void printError(char * error)
{
    printf("%s\nFor help use option -h\n", error);
}