/*
	8/30/2022
	Brian Hsiao
	Project 1: Histogram & Threshold
	g++ -std=c++11 Project1.cpp && ./a.out inFile1.txt outFile1.txt outFile2.txt
*/
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, const char* argv[]) {
	//declaring variables
	int numRows, numCols, minVal, maxVal, thrValue;
	int currPixel;
	//opening input file
	ifstream inFile;
	inFile.open(argv[1]);
	//if statement to check if the input file is able to be open
	if(!inFile.is_open()) {
		cout << "Unable to open file" << endl;
		exit(1);
	}
	//reading the header of input file, storing the relevant integers
	inFile >> numRows >> numCols >> minVal >> maxVal;
	//ask user for the threshold Value
	cout << "Input Threshold Value: ";
	cin >> thrValue;
	//opening output files
	//outFile1 will be for binary thresholding and outFile2 will be for Non-binary thresholding
	ofstream outFile1(argv[2]);
	ofstream outFile2(argv[3]);
	//creating headers for each output files
	outFile1 << numRows << " " << numCols << " " << 0 << " " << 1 << endl;
	outFile2 << numRows << " " << numCols << " " << 0 << " " << maxVal << endl;
	//nested for loop to go through each "pixel" of the input file using information from the input file header
	for(int i = 0 ; i < numRows ; i++){
		for(int j = 0 ; j < numCols ; j++){
			//getting the greyscale value of the current "pixel" or position
			inFile >> currPixel;
			//checking the grayscale value with the set threshold value and setting the values in the output files accordingly
			if(currPixel >= thrValue){
				outFile1 << "1 ";
				outFile2 << currPixel << " ";
			}
			else{
				outFile1 << "0 ";
				outFile2 << "0  ";
			}
		}
		outFile1 << endl;
		outFile2 << endl;
	}
	//closing all files
	inFile.close();
	outFile1.close();
	outFile2.close();
	return 0;
}

