#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
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
	while(valid && (option = getopt(argc, argv, "d:r:h:s:m:k:n:i")) != -1)
	{
		printf("a\n");
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
				valid = setK(atoi(optarg));
				break;
			// total number of shadows to be distributed
			case 'n':
				valid = setN(atoi(optarg));
				break;
			// In case of -d path to where the storing images are
			// In case of -r path to where the images that contain the secret are
			case 'i':
				valid = setDirectory(optarg);
				break;
			case 'h':
				printHelp();
				break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
	}
	if (optind >= argc || argv[optind] == NULL) 
	{
		printHelp();
		free(options);
		exit(EXIT_FAILURE);
	}
	if (!valid)
	{
		printError(options->error);
		free(options);
		exit(EXIT_FAILURE);
	}
	if (options->k > options->n)
	{
		printError("k should be less or equal to n");
		free(options);
		exit(EXIT_FAILURE);
	}
	execute(options);
	free(options);
	exit(EXIT_SUCCESS);
}

int execute(options_st * options)
{
	switch(options->mode)
	{
		case DISTRIBUTION_MODE: distributeSecret(options->image, options->k, options->n, options->dir, options->watermark); break;
		case RECOVERY_MODE: //recoverSecret(); break;
		default: exit(0);
	}
	return 0;
}

int setImage(const char* image)
{
	printf("d\n");
	if (fileExists(image))
	{
		options->image = image;
		return 1;
	}
	return 0;
}

int setWatermark(const char* watermark)
{
	printf("d\n");
	if (fileExists(watermark))
	{
		options->watermark = watermark;
		return 1;
	}
	return 0;
}

int setDirectory(const char* directory)
{
	printf("d\n");
	if (directoryExists(directory))
	{
		options->dir = directory;
		return 1;
	}
	return 0;
}


int directoryExists(const char* directory)
{
	printf("d\n");
	DIR* dir = opendir(directory);
	if (dir)
	{
    	closedir(dir);
	}
	else if (ENOENT == errno)
	{
		options->error = "Directory does not exist";
	}
	return 0;
}

int fileExists(const char * name)
{
	printf("d\n");
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
		printf("d\n");
	printf("d\n");
	if (mode != DISTRIBUTION_MODE && mode != RECOVERY_MODE)
	{
		options->error = "Error setting mode";
		return 0;
	}
	options->mode = mode;
	return 1; 
}

int setK(int k)
{
	if (k >= 2 && k < 252)
	{
		options->k = k;
		return 1;
	}
	options->error = "K must be a positive integer and less than 252";
	return 0;
}

int setN(int n)
{
	if (n > 0 && n < 252)
	{
		options->n = n;
		return 1;
	}
	options->error = "N must be a positive integer and less than 252";
	return 0;
}
