//
//  main.cpp
//  ReturnCalculator
//
//  Created by Shangqi Wu on 16/3/2.
//  Copyright © 2016 Shangqi Wu. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <list>
#include <thread>
#include "MonthlyData.h"

using namespace std;


list<MonthlyData> parseInput(ifstream& inputFile) {
	
	list<MonthlyData> allData;
	
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
		list<MonthlyData>::iterator curMonth = allData.begin();
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


void writeCsv(ofstream& outputFile, const list<MonthlyData>& allData) {
	
	int numMonth = static_cast<int>(allData.size());
	
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


int main(int argc, const char * argv[]) {
	
	string inputFileName = "/Users/wushangqi/SP50_test.csv";
	ifstream inputFile(inputFileName);
	
	if (!inputFile.is_open()) {
		cout << "File not found." << endl;
		return 1;
	}
	
	list<MonthlyData> allData = parseInput(inputFile);
	inputFile.close();
	
	
	list<MonthlyData>::iterator curMonth = allData.end();
	--curMonth;
	
	while (curMonth != allData.begin()) {
		
		list<MonthlyData>::iterator nextMonth = curMonth;
		--nextMonth;
		
		curMonth->getMonthReturn(prevMonth);
		curMonth = nextMonth;
		
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
