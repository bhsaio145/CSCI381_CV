/*
	10/13/2022
	Brian Hsiao
	Project 4: Morphologh specs and data
	g++ -std=c++11 Project4.cpp && ./a.out data1.txt Elem1.txt dilateOutFile.txt erodeOutFile.txt closingOutFile.txt openingOutFile.txt prettyPrintFile.txt 
*/
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//declaring header variables
int numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin;
int rowFrameSize, colFrameSize;

//zero2DAry function which fills the givern 2D array with zeros
void zero2DAry(int **Ary, int row, int col){
	for(int i = 0 ; i < row ; i++){
		for(int j = 0 ; j < col ; j++){
			Ary[i][j] = 0;
		}
	}
}
//loadImage function which loads the image in inFile to the 2D array, accounting for zero framing.
void loadImage(ifstream& inFile, int **Ary){
	for(int i = 0 ; i < numImgRows ; i++){
		for(int j = 0 ; j < numImgCols ; j++){
			inFile >> Ary[i+rowOrigin][j+colOrigin];
		}
	}
}
//prettyPrint function which formats the image, and outputs it. With the output size being determined by int x to if it's a image or structuring element
//Framed Borders will not be outputted to the output file
void prettyPrint(int **Ary, ofstream& outfile, int x){
	//if int x is 0, then the inputted ary is of the image
	if(x == 0){
		for(int i = 0 ; i < numImgRows ; i++){
			for(int j = 0 ; j < numImgCols ; j++){
				if(Ary[i+rowOrigin][j+colOrigin] == 0){
					outfile << ". ";
				}
				else{
					outfile << "1 ";
				}
			}
			outfile << "\n";
		}
	}
	//if x isnt 0 then the inputted array is a structuring element
	else{
		for(int i = 0 ; i < numStructRows ; i++){
			for(int j = 0 ; j < numStructCols ; j++){
				if(Ary[i][j] == 0){
					outfile << ". ";
				}
				else{
					outfile << "1 ";
				}
			}
			outfile << "\n";
		}
	}
}
//loadstruct function which loads the structuring element to the 2D array
void loadstruct(ifstream& inFile, int **Ary){
	for(int i = 0 ; i < numStructRows ; i++){
		for(int j = 0 ; j < numStructCols ; j++){
			inFile >> Ary[i][j];
		}
	}
}
//onePixelDilation method which applies dilation on the given "pixel"
void onePixelDilation(int i, int j, int **inAry, int **outAry, int **structAry){
	int iOffset = i-rowOrigin;
	int jOffset = j-colOrigin;
	for(int rIndex = 0; rIndex < numStructRows; rIndex++){
		for(int cIndex = 0 ; cIndex < numStructCols ; cIndex++){
			if(structAry[rIndex][cIndex]>0){
				outAry[iOffset + rIndex][jOffset + cIndex] = 1;
			}
		}
	}
}
//ComputeDilation method which applies dilation on the Image based on the structuring element given
void ComputeDilation(int **inAry, int **outAry, int **structAry){
	for(int i = 0 ; i < numImgRows ; i++){
		for(int j = 0 ; j < numImgCols ; j++){
			if(inAry[i+rowOrigin][j+colOrigin] > 0){
				onePixelDilation(i+rowOrigin,j+colOrigin,inAry,outAry,structAry);
			}
		}
	}
}
//AryToFile method which outputs given Ary to File excluding framed borders
void AryToFile(int **Ary, ofstream& outfile){
	outfile << numImgRows <<" "<<numImgCols<<" "<<imgMin<<" "<<imgMax<<"\n";
	for(int i = 0 ; i < numImgRows ; i++){
		for(int j = 0 ; j < numImgCols ; j++){
			if(Ary[i+rowOrigin][j+colOrigin] == 0){
				outfile << "0 ";
			}
			else{
				outfile << "1 ";
			}
		}
		outfile << "\n";
	}
}
//onePixelErosion method which applie erosion on the given pixel should it be able to
void onePixelErosion(int i, int j, int **inAry, int **outAry, int **structAry){
	int iOffset = i-rowOrigin;
	int jOffset = j-colOrigin;
	bool matchFlag = true;
	for(int rIndex = 0 ; (rIndex < numStructRows) && (matchFlag == true); rIndex++){
		for(int cIndex = 0 ; (cIndex < numStructCols) && (matchFlag == true); cIndex++){
			if((structAry[rIndex][cIndex]>0) && (inAry[iOffset+rIndex][jOffset+cIndex]<=0)){
				matchFlag = false;
			}
		}
	}
	if(matchFlag == true){
		outAry[i][j] = 1;
	}
	else{
		outAry[i][j] = 0;
	}
}
//ComputeErosion method which applies erosion on the image based on the structuring element
void ComputeErosion(int **inAry, int **outAry, int **structAry){
	for(int i = 0 ; i < numImgRows ; i++){
		for(int j = 0 ; j < numImgCols ; j++){
			if(inAry[i+rowOrigin][j+colOrigin] > 0){
				onePixelErosion(i+rowOrigin,j+colOrigin,inAry,outAry,structAry);
			}
		}
	}
}
//ComputeOpening method which applies erosion followed by dilation on the image
void ComputeOpening(int **inAry, int **outAry, int **tempAry, int **structAry){
	ComputeErosion(inAry, tempAry, structAry);
	ComputeDilation(tempAry, outAry, structAry);
}
//ComputeClosing method which applies dilation followed by erosion on the image
void ComputeClosing(int **inAry, int **outAry, int **tempAry, int **structAry){
	ComputeDilation(inAry, tempAry, structAry);
	ComputeErosion(tempAry, outAry, structAry);
}

int main(int argc, const char* argv[]) {
	//openning input files
	ifstream imgFile, structFile;
	imgFile.open(argv[1]);
	structFile.open(argv[2]);
	//openning output files
	ofstream dilateOutFile(argv[3]);
	ofstream erodeOutFile(argv[4]);
	ofstream openingOutFile(argv[5]);
	ofstream closingOutFile(argv[6]);
	ofstream prettyPrintFile(argv[7]);
	//reading header from imgFile
	imgFile >> numImgRows >> numImgCols >> imgMin >> imgMax;
	//reading header from structFile
	structFile >> numStructRows >> numStructCols >> structMin >> structMax;
	structFile >> rowOrigin >> colOrigin;
	//dynamically allocating arrays and variables
	rowFrameSize = numStructRows/2;
	colFrameSize = numStructCols/2;
	int extraRows = rowFrameSize*2;
	int extraCols = colFrameSize*2;
	int rowSize = numImgRows + extraRows;
	int colSize = numImgCols + extraCols;
	int** zeroFramedAry = new int * [rowSize];
	int** morphAry = new int * [rowSize];
	int** tempAry = new int * [rowSize];
	for(int i = 0 ; i < rowSize ; i++){
		zeroFramedAry[i] = new int[colSize];
		morphAry[i] = new int[colSize];
		tempAry[i] = new int[colSize];
	}
	int** structAry = new int * [numStructRows];
	for(int i = 0 ; i < numStructCols ; i++){
		structAry[i] = new int[numStructCols];
	}
	//filling 2D zero2DAry with zeros
	zero2DAry(zeroFramedAry, rowSize, colSize);
	//loading image from imgFile to zeroFramedAry
	loadImg(imgFile, zeroFramedAry);
	//outputting original image pretty printed
	prettyPrintFile << "Original Image | "<< numImgRows <<" "<<numImgCols<<" "<<imgMin<<" "<<imgMax<<"\n";
	prettyPrint(zeroFramedAry, prettyPrintFile, 0);
	//loading structuring element to its corresponding array
	zero2DAry(structAry, numStructRows, numStructCols);
	loadstruct(structFile, structAry);
	prettyPrintFile << "Original Structuring Element | "<< numStructRows <<" "<<numStructCols<<" "<<structMin<<" "<<structMax<<" | "<<rowOrigin<<" "<<colOrigin<<"\n";
	prettyPrint(structAry, prettyPrintFile, 1);
	//Computing Dilation and outputting the result
	zero2DAry(morphAry, rowSize, colSize);
	ComputeDilation(zeroFramedAry, morphAry, structAry);
	AryToFile(morphAry, dilateOutFile);
	prettyPrintFile << "Dilated Image\n";
	prettyPrint(morphAry, prettyPrintFile, 0);
	//Computing Erosion and outputting the result
	zero2DAry(morphAry, rowSize, colSize);
	ComputeErosion(zeroFramedAry, morphAry, structAry);
	AryToFile(morphAry, erodeOutFile);
	prettyPrintFile << "Eroded Image\n";
	prettyPrint(morphAry, prettyPrintFile, 0);
	//Computing Opening and outputting the result
	zero2DAry(morphAry, rowSize, colSize);
	ComputeOpening(zeroFramedAry, morphAry, tempAry, structAry);
	AryToFile(morphAry, openingOutFile);
	prettyPrintFile << "Opened Image\n";
	prettyPrint(morphAry, prettyPrintFile, 0);
	//Computing Closing and outputting the result
	zero2DAry(morphAry, rowSize, colSize);
	ComputeClosing(zeroFramedAry, morphAry, tempAry, structAry);
	AryToFile(morphAry, closingOutFile);
	prettyPrintFile << "Closed Image\n";
	prettyPrint(morphAry, prettyPrintFile, 0);
	//closing all files
	imgFile.close();
	structFile.close();
	dilateOutFile.close();
	erodeOutFile.close();
	openingOutFile.close();
	closingOutFile.close();
	prettyPrintFile.close();
	return 0;
}