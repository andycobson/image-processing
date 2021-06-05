/*
* Andrew S. Jacobson
* asj0108
* Project 1
*/

#include "opencv2\opencv.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h> 
#include <cmath>

using namespace cv;
using namespace std;

void displayImage(Mat);													// Display the loaded image.
void cvImageMatrix(Mat, unsigned char**);								// Convert image using opencv opbject to matrix.
void drawCenterElipse(Mat, unsigned char**);							// Iterate through matrix to draw a circle.
void matrixToImage(Mat, unsigned char**);								// Create output matrix into a PGM file.
int displayPGM();														// Display newly created image.

int main()
{
	char imagePath[] = "noisy-elliptical-object.png";					// Image filename from project folder.
	Mat loadedImage = imread(imagePath, IMREAD_GRAYSCALE);				// Load image into opencv Mat object as a greyscale image
	if (loadedImage.empty()) {											// Check if image is loaded; if not display image and exit program.
		cout << "Image was not loaded" << endl;
		return 1;
	}
	unsigned char** imageArray = new unsigned char* [loadedImage.rows]; // Initialize the array
	cvImageMatrix(loadedImage, imageArray);
	drawCenterElipse(loadedImage, imageArray);
	matrixToImage(loadedImage, imageArray);
	displayPGM();
	return 0;
}

void displayImage(Mat img) {
	imshow("Image Window", img);										// Use opencv to display loaded image
	waitKey();
}

void cvImageMatrix(Mat img, unsigned char** imageArray)
{
	for (int i = 0; i < img.rows; ++i) {								// Iterate through the image rows which represents the image height.
		imageArray[i] = new unsigned char[img.cols*img.channels()];		// Initialize an array for each row based on the image columns/width.
	}
	for (int i = 0; i < img.rows; ++i) {								// Iterate through the image rows which represents the image height.
		imageArray[i] = img.ptr<unsigned char>(i);						// Set the image data row by row.
	}
}

void drawCenterElipse(Mat img, unsigned char** imageArray) {
	int centerX = img.cols / 2;											// The middle value of the width.
	int centerY = img.rows / 2;											// The middle value of the height.
	int x, y;
	int radius = 50;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			x = i - centerY;
			y = j - centerX;
			if (x*x + y*y <= radius*radius) {							// Apply circle equation on x and y values
				imageArray[i][j] = 0;									// Set greyscale value to 0 for black in the circle equation.
			}
		}
	}
}

void matrixToImage(Mat img, unsigned char** imageData) {
	FILE* pgmimg;														// Create file object for exported pgm
	pgmimg = fopen("pgmimg.pgm", "wb");									// Name the file "pgmimg.pgm" and write it
	fprintf(pgmimg, "P2\n");											// Write the line "P2"; the magic number
	fprintf(pgmimg, "%d %d\n", img.cols, img.rows); 					// Write the line of the width and height
	fprintf(pgmimg, "255\n");											// Write the for the maximum grey value
	int temp = 0;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			temp = imageData[i][j];										// Iterate through matrix, assign greyvalue
			fprintf(pgmimg, "%d ", temp);								// Write greyvalue to file
		}
		fprintf(pgmimg, "\n");
	}
	fclose(pgmimg);
}

int displayPGM() {
	Mat loadedImage = imread("pgmimg.pgm");
	if (loadedImage.empty()) {
		cout << "Image was not loaded" << endl;
		return 1;
	}
	imshow("Created PGM", loadedImage);
	waitKey();
	return 0;
}