#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include "matrix.h"
#include "bmp.h"
#include "main.h"
#include "random.h"
#include "messages.h"
#include "recovery.h"
#include "distribution.h"

options_st * options;

int main(int argc, char* const argv[])
{
	printWelcome();
	options = malloc(sizeof(*options));
    int option, valid = 1;
	while(valid && (option = getopt(argc, argv, "drs:m:k:n:i:h")) != -1)
	{
		switch(option)
		{
			// secret image distribution
			case 'd':
				valid = setMode(DISTRIBUTION_MODE);
				break;
			// secret image recovery
			case 'r':
				valid = setMode(RECOVERY_MODE);
				break;
			// In case of -d it must be a B&W and it will be the image to hide
			// In case of -r it will the output image file.
			case 's':
				valid = setImage(optarg);
				break;
			// In case of -d it must be a B&W and same size as secret image (watermark).
			// In case of -r it will contain the transformation of the de watermark image.
			case 'm':
				valid = setWatermark(optarg);
				break;
			// mininum number of necessary shadows
			case 'k':
				valid = setK(optarg);
				break;
			// total number of shadows to be distributed
			case 'n':
				valid = setN(optarg);
				break;
			// In case of -d path to where the storing images are
			// In case of -r path to where the images that contain the secret are
			case 'i':
				valid = setDirectory(optarg);
				break;
			case 'h':
				printHelp();
				exit(EXIT_SUCCESS);
				break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
	}
	if (optind < argc) 
	{
		printError("Error in getopt (maybe wrong agument count).");
		free(options);
		exit(EXIT_FAILURE);
	}
	if (!valid)
	{
		printError(options->error);
		free(options);
		exit(EXIT_FAILURE);
	}
	if (options != NULL && options->k > options->n)
	{
		printError("k should be less or equal to n");
		free(options);
		exit(EXIT_FAILURE);
	}
	if(options->mode == DISTRIBUTION_MODE && !fileExists(options->image)) 
	{
		printError("No secret image provided");
		free(options);
		exit(EXIT_FAILURE);
	}
	if (!execute(options))
	{
		printError(options->error);
		free(options);
		exit(EXIT_FAILURE);
	}
	free(options);
	printf("Program ending...");
	exit(EXIT_SUCCESS);
}

bool execute(options_st * options)
{
	bool ret;
	switch(options->mode)
	{
		case DISTRIBUTION_MODE: if (!fileExists(options->image))
								{
									options->error = "image does not exist";
									ret = false;
									break;
								}
								printf("\nStarting distribution...\n");
								ret = distributeSecret(options->image, options->k, options->n, options->dir, options->watermark); 
								printf("Secret distributed!\n");
								break;
								
		case RECOVERY_MODE: 	printf("\nStarting recovery...\n");
								ret = recoverSecret(options->image, options->k, options->n, options->dir, options->watermark);
								printf("Secret recovered!\n");
								break;
		default: ret = false; options->error = "invalid mode set"; break;
	}
	return ret;
}

int setImage(const char* image)
{
	options->image = (char *) image;
	return 1;
}

int setWatermark(const char* watermark)
{
	if (fileExists(watermark))
	{
		options->watermark = (char*) watermark;
		printf("Watermark path set to: %s...\n", watermark);
		return 1;
	}
	options->error = "watermark does not exist";
	return 0;
}

int setDirectory(const char* directory)
{
	if (directoryExists(directory))
	{
		options->dir = (char*) directory;
		printf("Directory set to: %s\n", directory);
		return 1;
	}
	return 0;
}


int directoryExists(const char* directory)
{
	DIR* dir = opendir(directory);
	if (dir)
	{
    	closedir(dir);
		return 1;
	}
	else if (ENOENT == errno)
	{
		options->error = "Directory does not exist";
	}
	return 0;
}

int fileExists(const char * name)
{
    FILE *file;
    if ((file = fopen(name, "r")))
	{
        fclose(file);
        return 1;
    }
    return 0;
}

int setMode(int mode)
{
	if (mode != DISTRIBUTION_MODE && mode != RECOVERY_MODE)
	{
		options->error = "Error setting mode";
		return 0;
	}
	options->mode = mode;
	if (mode == DISTRIBUTION_MODE)
		printf("Distribution mode set...\n");
	else
		printf("Recovery mode set...\n");
	return 1; 
}

int setK(const char * k)
{	
	int knum;

	if (!isNumber(k))
		return 0;
	knum = atoi(k);
	if (knum >= 2 && knum < 252)
	{
		printf("Set K to: %d...\n", knum);
		options->k = knum;
		return 1;
	}
	options->error = "K must be a positive integer and less than 252";
	return 0;
}

int setN(const char * n)
{
	int nnum;

	if (!isNumber(n))
		return 0;
	nnum = atoi(n);
	if (nnum > 0 && nnum < 252)
	{
		printf("Set N to: %d...\n", nnum);
		options->n = nnum;
		return 1;
	}
	options->error = "N must be a positive integer and less than 252";
	return 0;
}

int isNumber(const char * number)
{
	for (int i = 0; number[i] != '\0'; i++)
	{
		if (!isdigit(number[i]))
		{
			options->error = "the argument is not a number";
			return 0;
		}
	}
	return 1;
}
