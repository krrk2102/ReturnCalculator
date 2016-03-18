//
//  main.cpp
//  ReturnCalculator
//
//  Created by Shangqi Wu on 16/3/2.
//  Copyright © 2016 Shangqi Wu. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "MonthlyData.h"

using namespace std;

/**
 *
 * This file contains main function which drives the whole process forward. 
 * This program reads in a well formated *.csv file, which matches the provided file
 * "SP50_test.csv" format exactly. It generates output file under same directory of 
 * executable file. 
 *
 * This program keep stock return rates provided in csv file by months. Then it generates
 * top 10% and bottom 10% return rates respectively. The next step is to examine each
 * stock information in the 2 heaps, and retrieve their next month's return correspondingly.
 * At last, 2 average return rates to top 10% and bottom 10% stocks are calculated, and 
 * the program generates average monthly return by subtractin top 10% average to bottom
 * 10% average.
 *
 * @author Shangqi Wu
 *
 */

// Signatures for input and output sub programs.
list<MonthlyData> parseInput(ifstream& inputFile);
void writeCsv(ofstream& outputFile, const list<MonthlyData>& allData);


int main(int argc, const char * argv[]) {
	
	// Open csv file and parse input data into MonthlyData class.
	string inputFileName = "/Users/wushangqi/SP50_test.csv";
	//cin >> inputFileName;
	ifstream inputFile;
	
	while (!inputFile.is_open()) {
		
		if (inputFileName == "q") {
			return 0;
		}
		
		inputFile.open(inputFileName.c_str());
		
		if (!inputFile.is_open()) {
			cout << "File not found. Please enter a valid file name,";
			cout << " or type \"q\" to quit." << endl;
		}

	}
	
	// Call to input parsing function.
	list<MonthlyData> allData = parseInput(inputFile);
	inputFile.close();
	
	// Process helps to generate next month's return.
	list<MonthlyData>::iterator curMonth = allData.end();
	--curMonth;
	
	while (curMonth != allData.begin()) {
		
		list<MonthlyData>::iterator nextMonth = curMonth;
		--nextMonth;
		
		curMonth->getMonthReturn(nextMonth);
		curMonth = nextMonth;
		
	}
	
	// Process to generate output csv file.
	string outputFileName = "/Users/wushangqi/result.csv";
	ofstream outputFile(outputFileName);
	
	if (!outputFile.is_open()) {
		cout << "Can't write output file." << endl;
		return 1;
	}
	
	writeCsv(outputFile, allData);
	
	outputFile.close();
	
	return 0;
}


/**
 *
 * This function accepts input file stream and parse the text data, converts 
 * data into a list of MonthlyData class. 
 *
 * @param inputFile: an opened ifstream object. This function does not check if 
 *					 it is open or not, please check it in main function.
 *
 * @return generated a STL linked list of MonthlyData objects.
 *
 */
list<MonthlyData> parseInput(ifstream& inputFile) {
	
	list<MonthlyData> allData;
	
	// Template string stores each line in csv file.
	string line;
	
	// Processing month & year: assuming they are in format of
	// "16-Mar".
	getline(inputFile, line);
	int lineSize = static_cast<int>(line.size());
	
	for (int i = 1; i < lineSize; i += 7) {
		string year = line.substr(i, 2);
		string month = line.substr(i + 3, 3);
		
		allData.push_back(MonthlyData(year, month));
	}
	
	// Process of real data till to the end of input file.
	while (getline(inputFile, line)) {
		int left = 0, right = 0;
		lineSize = static_cast<int>(line.size());
		
		// Skip empty lines.
		if (line.empty() || line[0] == ',') {
			continue;
		}
		
		string symbol;
		list<MonthlyData>::iterator curMonth = allData.begin();
		while (right <= lineSize) {
			
			if (right == lineSize || line[right] == ',') {
				
				string content = line.substr(left, right - left);
				
				// Handle different new line character in Windows and Linux/Unix.
				if (content.back() == '\r') {
					content.pop_back();
				}
				
				// Set stock symbol or return rate.
				if (symbol.empty()) {
					symbol = content;
				} else {
					
					double value = 0;
					// Some data marked as "#N/A" will be kept as 0.
					if (content != "#N/A") {
						// Return rate conversion.
						value = stod(content);
					}
					
					// Insert parsed data into MonthlyData database.
					curMonth->addData(symbol, value);
					++curMonth;
				}
				
				left = ++right;
				
			} else {
				right++;
			}
		}
	}
	
	return allData;
}


/**
 *
 * This funciton accepts processed MonthlyData objects and output file stream to write csv file.
 * 
 * @param outputFile: the output destination file stream for the csv file. 
 *					  This function does not check if it is opened, please check in main function.
 * @param allData: processed month data information object with calculated average return rates. 
 *
 * This function does not return any value.
 *
 */
void writeCsv(ofstream& outputFile, const list<MonthlyData>& allData) {
	
	// All data are generated by prevously appointed csv format. 
	int numMonth = static_cast<int>(allData.size());
	
	// Please note latest month is omitted since average cannot be
	// generated without a later month's data.
	outputFile << "Period";
	for (auto curMonth = allData.begin(); curMonth != allData.end(); ++curMonth) {
		
		if (curMonth != allData.begin()) {
			outputFile << "," << curMonth->getYearMonth();
		}
		
	}
	outputFile << endl;
	
	outputFile << "Average return of first 10% percentile/each period";
	for (auto curMonth = allData.begin(); curMonth != allData.end(); ++curMonth) {
		
		if (curMonth != allData.begin()) {
			outputFile << "," << curMonth->getTopTenPercentReturn();
		}
		
	}
	outputFile << endl;
	
	outputFile << "Average return of last 10% percentile/each period";
	for (auto curMonth = allData.begin(); curMonth != allData.end(); ++curMonth) {
		
		if (curMonth != allData.begin()) {
			outputFile << "," << curMonth->getBottomTenPercentReturn();
		}
		
	}
	outputFile << endl;
	
	double finalAverage = 0;
	
	outputFile << "Total average return/each period";
	for (auto curMonth = allData.begin(); curMonth != allData.end(); ++curMonth) {
		
		if (curMonth != allData.begin()) {
			outputFile << "," << curMonth->getMonthReturn();
			finalAverage += curMonth->getMonthReturn();
		}
		
		
	}
	outputFile << endl;
	
	for (int i = 1; i < numMonth; i++) {
		outputFile << ",";
	}
	outputFile << endl;
	
	finalAverage /= numMonth - 1;
	outputFile << "Total average return of all period";
	outputFile << "," << finalAverage;
	
	for (int i = 2; i < numMonth; i++) {
		outputFile << ",";
	}
	outputFile << endl;
	
}


