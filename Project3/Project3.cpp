/*
	9/27/2022
	Brian Hsiao
	Project 3: BiGaussAutoThrSelection
	g++ -std=c++11 Project3.cpp && ./a.out inFile1.txt outFile1.txt outFile2.txt
*/
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdio.h>
using namespace std;

//declaring header variables
int numRows, numCols, minVal, maxVal, maxHeight, offset;

//loadHist function which reads the histogram from the inFile and writes it into the 1D histAry. Returning the max height found.
int loadHist(int histAry[], ifstream& inFile){
	int maxHeight=0, pixVal= minVal, pixAmt;
	while(pixVal < maxVal){
		inFile >> pixVal >> pixAmt;
		histAry[pixVal] = pixAmt;
		if(maxHeight < pixAmt){
			maxHeight = pixAmt;
		}
	}
	return maxHeight;
}
//plotGraph function which plots the 1D Ary onto the 2D graph
void plotGraph(int Ary[], char **graph, char s){
	for(int i = 0; i < maxVal+1 ; i++){
		if(Ary[i] > 0){
			graph[i][Ary[i]] = s;
		}
	}
}
//setZero function which sets a given 1D array to 0
void setZero(int ary[]){
	for(int i = 0; i < maxVal+1 ; i++){
		ary[i] = 0;
	}
}
//computeMean function which computes the mean in the histogram between the given indicies
double computeMean(int leftIndex, int rightIndex, int hist[], int mHeight){
	mHeight = 0;
	double sum = 0, numPixels = 0;
	for(int i = leftIndex; i < rightIndex; i++){
		sum += (hist[i] * i);
		numPixels += hist[i];
		if(hist[i] > maxHeight){
			maxHeight = hist[i];
		}
	}
	return (sum/numPixels);
}
//computeVar function which computes the variance in the histogram between the given indicies
double computeVar(int leftIndex, int rightIndex, double mean, int hist[]){
	double sum = 0, numPixels = 0;
	for(int i = leftIndex; i < rightIndex; i++){
		sum += (double)hist[i] * pow((i-mean),2);
		numPixels += hist[i];
	}
	return sum/numPixels;
}
//modifiedGauss function which computes the Gaussian function
double modifiedGauss(int x, double mean, double var){
	return ((double)(maxHeight*exp(-(pow((x-mean),2)/(2*var)))));
}
//fitGauss function which computes the Gaussian curve fitted to the histogram and returns the difference
double fitGauss(int leftIndex, int rightIndex, int GaussAry[], int histAry[]){
	double mean, var, sum = 0.0, GVal, maxGval;
	mean = computeMean(leftIndex, rightIndex, histAry, maxHeight);
	var = computeVar(leftIndex, rightIndex, mean, histAry);
	for(int i = leftIndex ; i <= rightIndex; i++){
		GVal = modifiedGauss(i, mean, var);
		sum += abs(GVal - (double)histAry[i]);
		GaussAry[i] = (int)GVal;
	}
	return sum;
}
//biMeanGauss function which determines the best threshold selection
int biMeanGauss(int dividePt, int histAry[], int GaussAry[] ,ofstream& outFile){
	double sum1, sum2, total, minSumDiff = 999999;
	int bestThr = dividePt;
	while(dividePt < (maxVal-offset)){
		setZero(GaussAry);
		sum1 = fitGauss(0, dividePt, GaussAry, histAry);
		sum2 = fitGauss(dividePt, maxVal, GaussAry, histAry);
		total = sum1+sum2;
		if(total < minSumDiff){
			minSumDiff = total;
			bestThr = dividePt;
		}
		outFile << dividePt << " " << sum1 << " " << sum2 << " " << total << " " << minSumDiff << " " << bestThr << "\n";
		dividePt++;
	}
	return bestThr;
}
//bestFitGauss function which gets the GaussAry of the given best threshold value
void bestFitGauss(int bestThrVal, int GaussAry[], int histAry[]){
	double sum1, sum2;
	setZero(GaussAry);
	sum1 = fitGauss(0, bestThrVal, GaussAry, histAry);
	sum2 = fitGauss(bestThrVal, maxVal, GaussAry, histAry);
}
//addVertical function which adds a vertical line to the graph given the threshold value
void addVertical(int ThrVal, char **graph, char s){
	for(int i = 0 ; i < maxHeight+1 ; i++){
		graph[ThrVal][i] = s;
	}
}
//plotAll function which outputs histGraph and GaussGraph together to outfile
void plotAll(char **histGraph, char **GaussGraph, ofstream& outfile){
	//caption output
	outfile << "Histogram curve + Gaussian curves + Vertical line: \n";
	//output from histGraph, unless it is an empty space, then output from GaussGraph
	for(int i = maxHeight+1; i > 0; i--){
		for(int j = 0; j < maxVal+1; j++){
			if(histGraph[j][i] == ' '){
				outfile << " " << GaussGraph[j][i];
			}
			else{
				outfile << " " << histGraph[j][i];
			}
		}
		outfile << "\n";
	}
}

int main(int argc, const char* argv[]) {
	//opening input file
	ifstream inFile;
	inFile.open(argv[1]);
	//if statement to check if the input file is able to be open
	if(!inFile.is_open()) {
		cout << "Unable to open file" << endl;
		exit(1);
	}
	//opening output files
	ofstream outFile1(argv[2]);
	ofstream outFile2(argv[3]);
	//reading the header of input file, storing the relevant integers
	inFile >> numRows >> numCols >> minVal >> maxVal;
	//dynamically declaring rest of variables and arrays based off gained header variable values
	int *histAry = new int[maxVal+1];
	maxHeight = loadHist(histAry, inFile);
	int *GaussAry = new int[maxVal+1];
	//dynamically allocating for 2D Graph Arrays
	char** histGraph = new char * [maxVal+1];
	for(int i = 0; i < maxVal+1 ; i++){
		histGraph[i] = new char[maxHeight+1];
	}
	char** GaussGraph = new char * [maxVal+1];
	for(int i = 0; i < maxVal+1 ; i++){
		GaussGraph[i] = new char[maxHeight+1];
	}
	//Filling Graph Arrays with blank spaces
	for(int i = 0; i < maxVal+1; i++){
		for(int j = 0; j < maxHeight+1; j++){
			histGraph[i][j] = ' ';
			GaussGraph[i][j] = ' ';
		}
	}
	//plotGraph Function
	plotGraph(histAry, histGraph, '*');
	//Histogram curve to outFile1
	outFile1 << "input bimodal histogram : \n";
	for(int i = maxHeight+1; i > 0; i--){
		for(int j = 0; j < maxVal+1; j++){
			outFile1 << " " << histGraph[j][i];
		}
		outFile1 << "\n";
	}
	//declaring offset variables
	offset = (maxVal-minVal)/(10);
	int dividePt = offset;
	//calling biMeanGauss function to find the best threshold value
	int bestThrVal = biMeanGauss(dividePt, histAry, GaussAry, outFile2);
	//placing gaussian curve of found bestThrVal, back into GaussAry
	bestFitGauss(bestThrVal, GaussAry, histAry);
	//plotting the 1D GaussAry onto 2D GaussGraph and outputting it to outFile1
	plotGraph(GaussAry, GaussGraph, '+');
	outFile1 << "Gaussian Curves : \n";
	for(int i = maxHeight+1; i > 0; i--){
		for(int j = 0; j < maxVal+1; j++){
			outFile1 << " " << GaussGraph[j][i];
		}
		outFile1 << "\n";
	}
	//outputting found best threshold value to outFile1
	outFile1 << "Best Threshold Value: " << bestThrVal << "\n";
	//adding vertical line to histGraph on found bestThrVal and outputting to outFile1
	addVertical(bestThrVal, histGraph, '|');
	outFile1 << "input bimodal histogram with vertical line: \n";
	for(int i = maxHeight+1; i > 0; i--){
		for(int j = 0; j < maxVal+1; j++){
			outFile1 << " " << histGraph[j][i];
		}
		outFile1 << "\n";
	}
	//outputs histogram and gaussian graphs together to outFile1
	plotAll(histGraph, GaussGraph, outFile1);
	//closing all files
	inFile.close();
	outFile1.close();
	outFile2.close();
	return 0;
}

