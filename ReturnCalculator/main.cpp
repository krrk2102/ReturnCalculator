//
//  main.cpp
//  ReturnCalculator
//
//  Created by Shangqi Wu on 16/3/2.
//  Copyright © 2016 Shangqi Wu. All rights reserved.
//

#include <iostream>
#include <fstream>
//#include <thread>
#include "MonthlyData.h"

using namespace std;


vector<MonthlyData> parseInput(ifstream& inputFile) {
	
	vector<MonthlyData> allData;
	
	string line;
	// Processing month & year.
	getline(inputFile, line);
	int lineSize = static_cast<int>(line.size());
	
	for (int i = 1; i < lineSize; i += 7) {
		string year = line.substr(i, 2);
		string month = line.substr(i + 3, 3);
		
		allData.push_back(MonthlyData(year, month));
	}
	
	while (getline(inputFile, line)) {
		int left = 0, right = 0;
		lineSize = static_cast<int>(line.size());
		
		if (line.empty() || line[0] == ',') {
			continue;
		}
		
		string symbol;
		int col = 0;
		while (right <= lineSize) {
			
			if (right == lineSize || line[right] == ',') {
				
				string content = line.substr(left, right - left);
				
				if (content.back() == '\r') {
					content.pop_back();
				}
				
				if (symbol.empty()) {
					symbol = content;
				} else {
					
					double value = 0;
					if (content != "#N/A") {
						value = stod(content);
					}
					
					allData[col++].addData(symbol, value);
				}
				
				left = ++right;
				
			} else {
				right++;
			}
		}
	}
	
	return allData;
}


void writeCsv(ofstream& outputFile, const vector<MonthlyData>& allData) {
	
	int numMonth = static_cast<int>(allData.size());
	
	outputFile << "Period";
	for (int i = 1; i < numMonth; i++) {
		outputFile << "," << allData[i].getYearMonth();
	}
	outputFile << endl;
	
	outputFile << "Average return of first 10% percentile/each period";
	for (int i = 1; i < numMonth; i++) {
		outputFile << "," << allData[i].getTopTenPercentReturn();
	}
	outputFile << endl;
	
	outputFile << "Average return of last 10% percentile/each period";
	for (int i = 1; i < numMonth; i++) {
		outputFile << "," << allData[i].getBottomTenPercentReturn();
	}
	outputFile << endl;
	
	double finalAverage = 0;
	
	outputFile << "Total average return/each period";
	for (int i = 1; i < numMonth; i++) {
		
		outputFile << "," << allData[i].getMonthReturn();
		finalAverage += allData[i].getMonthReturn();
		
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


int main(int argc, const char * argv[]) {
	
	string inputFileName = "/Users/wushangqi/SP50_test.csv";
	ifstream inputFile(inputFileName);
	
	if (!inputFile.is_open()) {
		cout << "File not found." << endl;
		return 1;
	}
	
	vector<MonthlyData> allData = parseInput(inputFile);
	inputFile.close();
	
	for (int i = (int)allData.size() - 1; i > 0; i--) {
		
		allData[i].getMonthReturn(allData.begin() + i - 1);

	}
	
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
