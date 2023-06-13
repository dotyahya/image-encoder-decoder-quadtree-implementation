/*
    Author:         Muhammad Yahya
    Date:           24/04/2023
    Time:           4:41:53 PM
    Description:    This program reads the original image and the decoded image and calculates the
					accuracy of the decoded image using MSE formula.

    Note:           This program is written in C++ and uses OpenCV library to read the image.

    Github:         Please feel free to contribute to this project by submitting pull requests or
                    reporting bugs through the issue tracker.
*/

// headers
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

// namespace(s)
using namespace std;
//using namespace cv;

// user defined functions 
double calculateValueX(int**, int**, int, int);
double gammaValue(double, double);

// driver code
int main() {
    // create image object and load image
    cv::Mat originalImage = cv::imread("D:/TestImages/t1.bmp", cv::IMREAD_GRAYSCALE);

    if (originalImage.empty()) {
        cerr << "\nImage file corrupt or not found!";
        // return -1 shows that the program ended with an error code -1
        return -1;
    }

    cv::Mat decodedImage = cv::imread("D:/TestImages/decodedImage.bmp", cv::IMREAD_GRAYSCALE);

    if (decodedImage.empty()) {
        cerr << "\nImage file corrupt or not found!";
		// return -1 shows that the program ended with an error code -1
		return -1;
	}

    // set both images' rows and cols
    int rowO = originalImage.rows;
    int colO = originalImage.cols;

    int rowD = decodedImage.rows;
    int colD = decodedImage.cols;

    // check if both images have same dimensions
    if (rowO != rowD || colO != colD) {
        cerr << "\nBoth images must have same dimensions!\n";
		return -1;
	}

    // dynamic image arrays to store pixel values
    int** originalImageArray = new int* [rowO];
    int** decodedImageArray = new int* [rowD];

    for (int i = 0; i < rowO; i++) {
		originalImageArray[i] = new int[colO];
	}

    for (int i = 0; i < rowD; i++) {
        decodedImageArray[i] = new int[colD];
    }

    // storing pixel values in arrays
    for (int i = 0; i < rowO; i++) {
        for (int j = 0; j < colO; j++) {
			originalImageArray[i][j] = static_cast<int>(originalImage.at<uchar>(i, j));
		}
	}

    for (int i = 0; i < rowD; i++) {
        for (int j = 0; j < colD; j++) {
			decodedImageArray[i][j] = static_cast<int>(decodedImage.at<uchar>(i, j));
		}
	}
    
    // calling functions for X and gamma (Y)
    double X = calculateValueX(originalImageArray, decodedImageArray, rowO, colO);
    double Y = gammaValue(X, 255);

    cout << "Accuracy(MSE): " << Y << "%" << endl;

    // deallocate memory
    for (int i = 0; i < rowO; i++) {
		delete[] originalImageArray[i];
	}

    for (int i = 0; i < rowD; i++) {
		delete[] decodedImageArray[i];
	}

    delete[] originalImageArray;
    delete[] decodedImageArray;

    // terminate program
    return 0;
}

double calculateValueX(int** O, int** D, int row, int col) {
	double sum = 0.0;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
			sum += ((O[i][j] - D[i][j]) * (O[i][j] - D[i][j]));
		}
	}
    
    int N = row * col;
	return sum / N;
}

double gammaValue(double X, double Z = 255) {
    return 100 * (1 - (X / (Z * Z)));
}





