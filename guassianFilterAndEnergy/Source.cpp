/*
	Andrew Jacobson - asj0108
	Project 2: Use Gaussian and Averaging convolution to image and compare total energy.
	External Library Used: OpenCV
	Written and Compiled in Visual Studio (C++)
*/

#include "opencv2\opencv.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat ocvGaussianPasses(Mat, unsigned int*);
Mat ocvBlurAveraging(Mat, unsigned int*);

int main() {
	char imagePath[] = "astronaut-interference.png";					// Image filename from project folder.
	Mat loadedImage = imread(imagePath, IMREAD_GRAYSCALE);				// Load image into opencv Mat object as a greyscale image
	if (loadedImage.empty()) {											// Check if image is loaded; if not display image and exit program.
		cout << "Input image could not be loaded." << endl;
		return 1;
	}
	unsigned int X[11] = {};											// Declare Gaussian value array to hold total energy
	unsigned int Y[11] = {};											// Declare Averaging value array to hold total energy
	Mat paramImg;
	loadedImage.copyTo(paramImg);
	Mat gausImg = ocvGaussianPasses(paramImg, X);					// Call Main Gaussian function and return the completed image
	loadedImage.copyTo(paramImg);
	Mat avgImg = ocvBlurAveraging(paramImg, Y);						// Call Main Averaging function and return the completed image
	cout << "Gaussian - IMAGE1 Values					Averaging - IMAGE2 Values" << endl;
	for (int i = 0; i < 11; i++)
		cout << "	X" << i << ": " << X[i] << "								" << "Y" << i << ": " << Y[i] << endl;


	//----- Set windows, resize windows for better viewing, then show both resulting images.
	namedWindow("Gaussian - IMAGE1", WINDOW_NORMAL);
	namedWindow("Averaging - IMAGE2", WINDOW_NORMAL);

	resizeWindow("Gaussian - IMAGE1", 500, 412);
	resizeWindow("Averaging - IMAGE2", 500, 412);

	imshow("Gaussian - IMAGE1", gausImg);
	imshow("Averaging - IMAGE2", avgImg);
	waitKey();
	//------

	return 0;
}

Mat ocvGaussianPasses(Mat img, unsigned int* X) {
	int sum;												// sum variable to hold total energy											
	int iter = 0;											// Array indexing variable
	Mat tempImg;											// temporary Mat image for the purpose of swapping
	for (int i = 1; i < 1001; i++) {						// Main loop to apply the gaussian operation to the image 1000 times
		if (i == 1 || (i % 100 == 0)) {						// Get the total energy the first time and every 100th time
			sum = 0;										// Set sum variable to 0 every time obtaining total energy
			for (int x = 0; x < img.rows; x++)				// Nested for-loops to iterate through image matrix
				for (int y = 0; y < img.cols; y++)
					sum += img.at<unsigned char>(x, y);		// Sum is calculated with each grey-scale pixel value
			X[iter] = sum;									// Store the sum value into X array (Gaussian)
			iter++;											// Increment array indexing
		}
		GaussianBlur(img, tempImg, Size(3, 3), 0);			// Mat library function to apply GaussianBlur to image
		img.release();
		tempImg.copyTo(img);								// Swap the newly applied blur image with img.
		tempImg.release();									// Clear matrix data in the temprory Mat object.
	}
	return img;												// Return the final blurred image.
}


/* 
	Function operation is exactly similiar to Gaussian execpt 
		the averaging blur function is applied and value set is added
		to the Y array.
*/
Mat ocvBlurAveraging(Mat img, unsigned int* Y) {
	int sum;
	int iter = 0;
	Mat tempImg;
	for (int i = 1; i < 1001; i++) {
		if (i == 1 || (i % 100 == 0)) {
			sum = 0;
			for (int x = 0; x < img.rows; x++)
				for (int y = 0; y < img.cols; y++)
					sum += img.at<unsigned char>(x, y);
			Y[iter] = sum;									// Store the sum value into Y array (Averaging)
			iter++;
		}
		blur(img, tempImg, Size(3, 3));						// Mat library function to apply averaging blur to image.
		img.release();
		tempImg.copyTo(img);
		tempImg.release();
	}
	return img;
}