#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imageIO_TGA.h"

// Compare help function for images that are rotated 0
int compare0(ImageStruct *image1, ImageStruct *image2);
// Compare help function for images that are rotated 90
int compare90(ImageStruct *image1, ImageStruct *image2);
// Compare help function for images that are rotated 180
int compare180(ImageStruct *image, ImageStruct *image2);
// Compare help function for images that are rotated 270
int compare270(ImageStruct *image1, ImageStruct *image2);
// Checks to see if the height and width of the images are the same/swapped/different
char resolution_compare(ImageStruct *image1, ImageStruct *image2); 

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Not enough arguments\n");
		exit(1);
	}
	
	const char* image_first = argv[1];
	const char* image_second = argv[2];

	ImageStruct image1 = readTGA(image_first);
	ImageStruct image2 = readTGA(image_second);

	// Returns 0 in the situation that the images are different sizes
	if(resolution_compare(&image1, &image2) == 'd') {
		printf("Different Image\n");
		return 0;
	}
	
	// If the sizes are the same
	if(resolution_compare(&image1, &image2) == 's') {
		// Checks to see if the image is a square;
		if (image1.width == image1.height) {
			// Checks which rotation the image in
			if (compare0(&image1, &image2) == 1) {
				printf("0 Degrees\n");
				return 1;
			} else if (compare90(&image1, &image2) == 1) {
				printf("90 Degrees\n");
				return 1;
			} else if (compare180(&image1, &image2) == 1) {
				printf("180 Degrees\n");
				return 1;
			} else if (compare270(&image1, &image2) == 1) {
				printf("270 Degrees\n");
				return 1;
			} else {
				printf("Different Image\n");
				return 0;
			}
		}
		// Call rect_compare for either 0 or 180
		if (compare0(&image1, &image2) == 1) {
			printf("0 Degrees\n");
			return 1;
		} else if (compare180(&image1, &image2) == 1) {
			printf("180 Degrees\n");
			return 1;
		} else {
			printf("different Image\n");
			return 0;
		}
	}
	
	// If the sizes are swapped
	if(resolution_compare(&image1, &image2) == 'c') {
		if (compare90(&image1, &image2) == 1) {
			printf("90 Degrees\n");
			return 1;
		} else if (compare270(&image1, &image2) == 1) {
			printf("270 Degrees\n");
			return 1;
		} else {
			printf("Different Image\n");
			return 0;
		}
	}
	return 0;
}

// Compares the resolution and returns a char
// Based off of the orientation of the two resolutions
char resolution_compare(ImageStruct *image1, ImageStruct *image2) {
	int image1_h = image1->height;
	int image1_w = image1->width;
	int image2_h = image2->height;
	int image2_w = image2->width;

	// Compare the height and width;
	// Checks to see if the resolution of the image are in the same places
	if ((image1_h == image2_h) && (image1_w == image2_w)) {
		return 's';
	} 
	// Checks to see if the resolution of the image is swapped
	if ((image1_h == image2_w) && (image1_w == image2_h)) {
		return 'c';
	}
	// Returns d if the image sizes are just different
	return 'd';	

}

// Compares rectangles with the same resolutions to see which rotation they are in
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
