/*
	9/10/22
	Brian Hsiao
	java Project2_Main.java inFile.txt maskFile.txt 38 inputImg.txt AvgOut.txt AvgThreshold.txt MedianOut MedianThreshold GaussOut GaussThreshold
*/
//importing needed libraries
import java.io.*;
import java.util.Arrays;
import java.util.Scanner;

public class Project2_Main {
	//declaring core variables as Global since all methods will need access to it too
	static int numRows, numCols, minVal, maxVal, thrVal, maskRows, maskCols, maskMin, maskMax, newMin, newMax;
	//loadMask Method that will read from the given maskFile into a 2d array
	public static void loadMask(Scanner mFile, int[][] ary) {
		for(int i = 0 ; i < maskRows ; i++) {
			for(int j = 0 ; j < maskCols; j++) {
				ary[i][j] = mFile.nextInt();
			}
		}
	}
	//loadMask1DAry method takes the 2d array containing the mask and places into the equivalent places in a 1d array
	public static void loadMask1DAry(int[][] ary1, int[] ary2) {
		for(int i = 0 ; i < maskRows ; i++) {
			for(int j = 0 ; j < maskCols; j++) {
				ary2[i*maskRows+j] = ary1[i][j];
			}
		}
	}
	//loadImage method reads from inFile and places the image into the middle of the fitted array
	public static void loadImage(Scanner iFile, int[][] ary) {
		for(int i = 1 ; i < numRows+1 ; i++) {
			for(int j = 1 ; j < numCols+1 ; j++) {
				ary[i][j] = iFile.nextInt();
			}
		}
	}
	//mirrorFraming method to create a 1 size mirror framing
	public static void mirrorFraming(int[][] ary) {
		//setting corner for mirror frame first
		ary[0][0] = ary[1][1];
		ary[0][numCols+1] = ary[1][numCols];
		ary[numRows+1][0] = ary[numRows][1];
		ary[numRows+1][numCols+1] = ary[numRows][numCols];
		//setting the top and bottom columns of mirror frame
		for(int i = 1 ; i < numCols+1 ; i++) {
			ary[0][i] = ary[1][i];
			ary[numRows+1][i] = ary[numRows][i];
		}
		//setting the left and right rows of mirror frame
		for(int i = 1 ; i < numRows+1 ; i++) {
			ary[i][0] = ary[i][1];
			ary[i][numCols+1] = ary[i][numCols];
		}
	}
	//imgReformat method to output the given inAry into the named outFile, also applying the pretty print algorithm
	public static void imgReformat(int[][] inAry, int minV, int maxV, BufferedWriter outFile) throws IOException {
		outFile.write(numRows + " " + numCols + " " + minV + " " + maxV + "\n");
		if(maxV < 10) {
			for(int i = 1 ; i < numRows+1 ; i++) {
				for(int j = 1 ; j < numCols+1 ; j++) {
					outFile.write(inAry[i][j] + " ");
				}
				outFile.write("\n");
			}
		}
		else if(maxV > 9 && maxV < 100) {
			for(int i = 1 ; i < numRows+1 ; i++) {
				for(int j = 1 ; j < numCols+1 ; j++) {
					if(inAry[i][j] < 10) {
						outFile.write(inAry[i][j] + "  ");
					}
					else {
						outFile.write(inAry[i][j] + " ");
					}
				}
				outFile.write("\n");
			}
		}
		else if(maxV > 99) {
			for(int i = 1 ; i < numRows+1 ; i++) {
				for(int j = 1 ; j < numCols+1 ; j++) {
					if(inAry[i][j] < 100) {
						outFile.write(inAry[i][j] + "   ");
					}
					else if(inAry[i][j] < 10) {
						outFile.write(inAry[i][j] + "  ");
					}
					else {
						outFile.write(inAry[i][j] + " ");
					}
				}
				outFile.write("\n");
			}
		}
	}
	//loadNeighbor1DAry method takes an element from ary1 and 
	//places all values around that origin into the corresponding elements in a 1D array ary2
	public static void loadNeighbor1DAry(int[][] ary1, int x, int y, int[] ary2) {
		int count = 0;
		for(int i = x-1 ; i < x+2 ; i++) {
			for(int j = y-1 ; j < y+2 ; j++) {
				ary2[count++] = ary1[i][j];
			}
		}
	}
	//computeAvg method to apply the Average Filtering on the inAry and outputting to avgAry
	public static void computeAvg(int[][] inAry, int[][] avgAry, int[] neighborAry) {
		newMin = 9999;
		newMax = 0;
		int Avg;
		for(int i = 1 ; i < numRows+1 ; i++) {
			for(int j = 1 ; j < numCols+1 ; j++) {
				//resetting Average
				Avg = 0;
				//getting neighbor "pixels" and calculating the average
				loadNeighbor1DAry(inAry, i, j, neighborAry);
				for(int n = 0 ; n < neighborAry.length ; n++) {
					Avg += neighborAry[n];
				}
				Avg = Avg/neighborAry.length;
				//updating the newMax and newMin if needed
				if(Avg > newMax) {
					newMax = Avg;
				}
				if(Avg < newMin) {
					newMin = Avg;
				}
				//setting Average amount to corresponding "pixel"
				avgAry[i][j] = Avg;
			}
		}
	}
	//computeMedian method to apply the Median Filtering on the inAry and outputting to medianAry
	public static void computeMedian(int[][] inAry, int[][] medianAry, int[] neighborAry) {
		newMin = 9999;
		newMax = 0;
		int median, middle = maskMax+maskMin/2;
		for(int i = 1 ; i < numRows+1 ; i++) {
			for(int j = 1 ; j < numCols+1 ; j++) {
				//getting neighbor "pixels"
				loadNeighbor1DAry(inAry, i, j, neighborAry);
				//sorting the neighborAry
				Arrays.sort(neighborAry);
				//getting the middle element in neighborAry
				median = neighborAry[middle];
				//updating the newMax and newMin if needed
				if(median > newMax) {
					newMax = median;
				}
				if(median < newMin) {
					newMin = median;
				}
				//setting median amount to corresponding "pixel"
				medianAry[i][j] = median;
			}
		}
	}
	public static int convolution(int[] neighbor1DAry, int[] mask1DAry) {
		int result = 0;
		for(int i = 0 ; i < mask1DAry.length ; i++) {
			result += neighbor1DAry[i] * mask1DAry[i];
		}
		return result;
	}
	//computeGauss method to apply the Guass Filtering on the inAry and outputting to GaussAry
	public static void computeGauss(int[][] inAry, int[][] gaussAry, int[] neighborAry, int[] maskAry) {
		int masktotal = 0;
		for(int i = 0 ; i < maskAry.length ; i++) {
			masktotal += maskAry[i];
		}
		newMin = 9999;
		newMax = 0;
		int Gauss;
		for(int i = 1 ; i < numRows+1 ; i++) {
			for(int j = 1 ; j < numCols+1 ; j++) {
				//getting neighbor "pixels"
				loadNeighbor1DAry(inAry, i, j, neighborAry);
				//calculating convolution result on the "pixel"
				Gauss = convolution(neighborAry, maskAry);
				Gauss = Gauss/masktotal;
				//updating the newMax and newMin if needed
				if(Gauss > newMax) {
					newMax = Gauss;
				}
				if(Gauss < newMin) {
					newMin = Gauss;
				}
				//setting Average amount to corresponding "pixel"
				gaussAry[i][j] = Gauss;
			}
		}
	}
	//threshold method, applying thresholding to ary1 based on given threshold value, and outputting to ary2
	public static void threshold(int[][] ary1, int[][] ary2) {
		newMin = 0;
		newMax = 1;
		for(int i = 1 ; i < numRows+2 ; i++) {
			for(int j = 1 ; j < numCols+2 ; j++) {
				if(ary1[i][j] >= thrVal) {
					ary2[i][j] = 1;
				}
				else {
					ary2[i][j] = 0;
				}
			}
		}
		
	}
	@SuppressWarnings("resource")
	public static void main(String[] args) throws IOException{
		//opening args[0] which contains the inFile
		String inputName = args[0]; 
		FileReader fReader = new FileReader(inputName);
		BufferedReader bfReader = new BufferedReader(fReader);
		Scanner inFile = new Scanner(bfReader);
		//opening args[1] which contains the maskFile
		inputName = args[1]; 
		fReader = new FileReader(inputName);
		bfReader = new BufferedReader(fReader);
		Scanner maskFile = new Scanner(bfReader);
		//setting args[2] as the thrVal
		thrVal = Integer.parseInt(args[2]);
		//opening args[3] which contains output inputImg file name
		String outputName = args[3]; 
		FileWriter outputWriter = new FileWriter(outputName);
		BufferedWriter inputImg = new BufferedWriter(outputWriter);
		//opening args[4] which contains output AvgOut file name
		outputName = args[4]; 
		outputWriter = new FileWriter(outputName);
		BufferedWriter AvgOut = new BufferedWriter(outputWriter);
		//opening args[5] which contains output AvgThreshold file name
		outputName = args[5]; 
		outputWriter = new FileWriter(outputName);
		BufferedWriter AvgThreshold = new BufferedWriter(outputWriter);
		//opening args[6] which contains output MedianOut file name
		outputName = args[6]; 
		outputWriter = new FileWriter(outputName);
		BufferedWriter MedianOut = new BufferedWriter(outputWriter);
		//opening args[7] which contains output MedianThreshold file name
		outputName = args[7]; 
		outputWriter = new FileWriter(outputName);
		BufferedWriter MedianThreshold = new BufferedWriter(outputWriter);
		//opening args[8] which contains output GaussOut file name
		outputName = args[8]; 
		outputWriter = new FileWriter(outputName);
		BufferedWriter GaussOut = new BufferedWriter(outputWriter);
		//opening args[9] which contains output GaussThreshold file name
		outputName = args[9]; 
		outputWriter = new FileWriter(outputName);
		BufferedWriter GaussThreshold = new BufferedWriter(outputWriter);
		//setting inFile Header Variables
		numRows = inFile.nextInt();
		numCols = inFile.nextInt();
		minVal = inFile.nextInt();
		maxVal = inFile.nextInt();
		//setting maskFile Header Variables
		maskRows = maskFile.nextInt();
		maskCols = maskFile.nextInt();
		maskMin = maskFile.nextInt();
		maskMax = maskFile.nextInt();
		//dynamically allocating all needed 1D and 2D Arrays
		int mirrorFramedAry[][] = new int[numRows+2][numCols+2];
		int avgAry[][] = new int[numRows+2][numCols+2];
		int medianAry[][] = new int[numRows+2][numCols+2];
		int gaussAry[][] = new int[numRows+2][numCols+2];
		int thrAry[][] = new int[numRows+2][numCols+2];
		int mask2DAry[][] = new int[maskRows][maskCols];
		int neighbor1DAry[] = new int[9];
		int mask1DAry[] = new int[9];
		//Loading maskFile into mask2DAry and mask1DAry
		loadMask(maskFile, mask2DAry);
		loadMask1DAry(mask2DAry, mask1DAry);
		//loading inFile into mirrorFramedAry and creating the surrounding mirror frame
		loadImage(inFile, mirrorFramedAry);
		mirrorFraming(mirrorFramedAry);
		//outputting base image array into inputImg
		imgReformat(mirrorFramedAry, minVal, maxVal, inputImg);
		//Computing Average Filtering on mirrorFramedAry using the mask and outputting to AvgOut
		//also computing the Average filtered image thresholded and outputting to AvgThreshold
		computeAvg(mirrorFramedAry, avgAry, neighbor1DAry);
		imgReformat(avgAry, newMin, newMax, AvgOut);
		threshold(avgAry, thrAry);
		imgReformat(thrAry, newMin, newMax, AvgThreshold);
		//Computing Median filtering on mirrorFramedAry using the mask and outputting to MedianOut
		//also computing the Median filtered image thresholded and outputting to MedianThreshold
		computeMedian(mirrorFramedAry, medianAry, neighbor1DAry);
		imgReformat(medianAry, newMin, newMax, MedianOut);
		threshold(medianAry, thrAry);
		imgReformat(thrAry, newMin, newMax, MedianThreshold);
		//Computing Gaussian filtering on mirrorFramedAry using the mask and outputting to GaussOut
		//also computing the Gaussian filtered image thresholded and outputting to GaussThreshold
		computeGauss(mirrorFramedAry, gaussAry, neighbor1DAry, mask1DAry);
		imgReformat(gaussAry, newMin, newMax, GaussOut);
		threshold(gaussAry, thrAry);
		imgReformat(thrAry, newMin, newMax, GaussThreshold);
		//closing all files
		inFile.close();
		maskFile.close();
		inputImg.close();
		AvgOut.close();
		AvgThreshold.close();
		MedianOut.close();
		MedianThreshold.close();
		GaussOut.close();
		GaussThreshold.close();
	}
}
