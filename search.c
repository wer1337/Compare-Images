/** 
 * This search program recursively looks through a directory and 
 * all of it's daughter directories for any valid TGA image files.
 * It then compares the TGA image to an image source folder containing
 * TGA images. When an image matches (regardless of rotation) it will
 * then print into an output file the path of the image in the searched
 * directory.
 *
 * @author Jackson Xie
 * @version 1.0
 * @since 2018-10-13
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include "imageIO_TGA.h"

void searchDir(char *ImagePath, char *filename);
int checkTGA(char *filename);
void imageComp(ImageStruct *dirImage, char* imagePath);
char stripName(char *filename);
int compare0(ImageStruct *image1, ImageStruct *image2);
int compare90(ImageStruct *image1, ImageStruct *image2);
int compare180(ImageStruct *image1, ImageStruct *image2);
int compare270(ImageStruct *image1, ImageStruct *image2);

int main(int argc, char* argv[]) {
	if(argc != 3) {
		printf("Not enough arguments\n");
		exit(1);
	}

	// Gets the name of the imagepath and rootpath
	char* imagePath = argv[1];
	char* rootPath = argv[2];

	// Calls directories passing the root folder, and the image folder
	searchDir(imagePath, rootPath);
}

void searchDir(char *imagePath, char *filename) {
	// looks through all the files in the directory
	// if the file is a folder, goes into the folder
	// recursively
	// Must strip filename prior to passing through
	
	DIR *directory = opendir(filename);
	struct dirent *entry;
	
	// Loops through the whole directory
	while((entry = readdir(directory)) != NULL) {
		char* name = entry->d_name;
		
		// Ignores invisible files
		if(name[0] != '.') {
			// If it finds a file type directory
			if(entry->d_type == DT_DIR) {
				// recursive call
				searchDir(imagePath, name);
			}
			// Checks for a regular file and tga extension
			if(entry->d_type == DT_REG && checkTGA(name) == 1) {
				// creates an ImageStruct of the tga 
				ImageStruct dirImage = readTGA(name);
				
				// calls image comparison
				imageComp(&dirImage, imagePath);
			}
		}
	}
}

/**
 * Checks if file extension is TGA
 */

int checkTGA(char* filename) {
	char *ext = strrchr(filename, '.');
	if(strcmp(ext, ".tga") == 0) {
		return 1;
	}
	return 0;
}

/** This function gets called when a valid TGA file was located prior
 *  It compares the found TGA file to all the TGA files inside image source
 *  If the image's was rotated in 0/90/180/270 degrees it will print the
 *  Source location to an output file
 */

void imageComp(ImageStruct *dirImage, char* imagePath) {
	DIR *directory = opendir(imagePath);
	struct dirent *entry;

	while(((entry = readdir(directory)) != NULL)) {
		char* name = entry->d_name;
		ImageStruct imageImg = readTGA(name);

		if((compare0(dirImage, &imageImg) == 1) || (compare90(dirImage, &imageImg) == 1) || (compare180(dirImage, &imageImg) == 1) || (compare270(dirImage, &imageImg) == 1)) {
			// print file name/location
			
			FILE *f = fopen("output.txt", "a");

			fprintf(f, "%s\n", name);
			fclose(f);
		}
	}
	closedir(directory);
}

// Comparision helpers 

int compare0(ImageStruct *image1, ImageStruct *image2) {
	int* raster1 = (int*)(image1->raster);
	int* raster2 = (int*)(image2->raster);

	int size = image1->height * image1->width;

	for (int i = 0; i < size; i++) {
		if (raster1[i] != raster2[i]) { return 0; }
	}
	return 1;
}

int compare180(ImageStruct *image1, ImageStruct *image2) {
	int* raster1 = (int*)(image1->raster);
	int* raster2 = (int*)(image2->raster);

	int size = image1->height * image1->width;

	for (int i = 0; i < size; i++) {
		if (raster1[i] != raster2[size-i]) { return 0; }
	}
	return 1;
}

int compare90(ImageStruct *image1, ImageStruct *image2) {
	int* raster1 = (int*)(image1->raster);
	int* raster2 = (int*)(image2->raster);

	for (unsigned int i = 0; i < image1->height; i++) {
		for (unsigned int j = 0; j < image1->width; j++) {
			if (raster2[(image2->height-j-1)*image2->width + i] != raster1[i*image1->width +j]) {
				return 0;
			}
		}
	}
	return 1;
}

int compare270(ImageStruct *image1, ImageStruct *image2) {
	int* raster1 = (int*)(image1->raster);
	int* raster2 = (int*)(image2->raster);

	for (unsigned int i = 0; i < image1->height; i++) {
		for (unsigned int j = 0; j < image1->width; j++) {
			if (raster2[j*image2->width + image2->width - i - 1] != raster1[i*image1->width + j]) {
				return 0;
			}
		}
	}
	return 1;
}

char stripName(char *filename) {
	char *retName[sizeof(filename)];
	
	int count = 0;
	for (int i = 0; i < sizeof(filename); i++) {
		if(isalpha(filename[i])) {
			retName[count] = filename[i];
			count++;
		}
	}
	return retName;
}
