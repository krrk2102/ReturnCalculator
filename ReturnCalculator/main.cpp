//
//  main.cpp
//  ReturnCalculator
//
//  Created by Shangqi Wu on 16/3/2.
//  Copyright © 2016 Shangqi Wu. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

struct DataSample {
	
	string corpSymbol;
	double returnRate;
	
	DataSample() :
		corpSymbol(""), returnRate(0) {}
	
	DataSample(const string& s, const double& rate) :
		corpSymbol(s), returnRate(rate) {}
	
	DataSample(const pair<string, double>& sample) :
		corpSymbol(sample.first), returnRate(sample.second) {}
	
};

class MonthlyData {
private:
	
	struct maxHeapComparator {
		bool operator () (const DataSample& smaller, const DataSample& larger) const {
			return smaller.returnRate < larger.returnRate;
		}
	};
	
	struct minHeapComparator {
		bool operator () (const DataSample& smaller, const DataSample& larger) const {
			return larger.returnRate < smaller.returnRate;
		}
	};
	
	string year;
	string month;
	
	unordered_map<string, double>* data;
	
	unsigned long tenPercentSize;
	priority_queue<DataSample, vector<DataSample>, minHeapComparator>* topTenPercent;
	priority_queue<DataSample, vector<DataSample>, maxHeapComparator>* bottomTenPercent;
	
	vector<double>* topTenPercentReturns;
	vector<double>* bottomTenPercentReturns;
	
	double topTenPercentAverage;
	double bottomTenPercentAverage;
	double monthAverage;
	
	
	void sort() {
		
		while (!topTenPercent->empty()) {
			topTenPercent->pop();
		}
		
		while (!bottomTenPercent->empty()) {
			bottomTenPercent->pop();
		}
		
		tenPercentSize = data->size() / 10;
		for (const auto& sample : (*data)) {
			
			if (topTenPercent->size() < tenPercentSize) {
				topTenPercent->push(DataSample(sample));
			} else {
				
				if (sample.second >= topTenPercent->top().returnRate) {
					
					topTenPercent->pop();
					topTenPercent->push(DataSample(sample));
					
				}
			}
			
			if (bottomTenPercent->size() < tenPercentSize) {
				bottomTenPercent->push(DataSample(sample));
			} else {
				
				if (sample.second <= bottomTenPercent->top().returnRate) {
					
					bottomTenPercent->pop();
					bottomTenPercent->push(DataSample(sample));
					
				}
			}
		}
		
	}
	
	
	void getTopTenPercentReturns(const vector<MonthlyData>::iterator& nextMonth) {
		
		topTenPercentReturns->clear();
		while (!topTenPercent->empty()) {
			
			string symbol = topTenPercent->top().corpSymbol;
			double nextMonthReturn = nextMonth->getSingleReturn(symbol);
			topTenPercentReturns->push_back(nextMonthReturn);
			topTenPercent->pop();
			
		}

	}
	
	
	void getBottomTenPercentReturn(const vector<MonthlyData>::iterator& nextMonth) {
		
		bottomTenPercentReturns->clear();
		while (!bottomTenPercent->empty()) {
			
			string symbol = bottomTenPercent->top().corpSymbol;
			double nextMonthReturn = nextMonth->getSingleReturn(symbol);
			bottomTenPercentReturns->push_back(nextMonthReturn);
			bottomTenPercent->pop();
			
		}

	}
	
public:
	
	
	MonthlyData(const string& y, const string& m) :
		year(y), month(m), tenPercentSize(0),
		topTenPercentAverage(0), bottomTenPercentAverage(0),
		monthAverage(0) {
		
		data = new unordered_map<string, double>();
		topTenPercent = new priority_queue<DataSample, vector<DataSample>, minHeapComparator>();
		bottomTenPercent = new priority_queue<DataSample, vector<DataSample>, maxHeapComparator>();
		
		topTenPercentReturns = new vector<double>();
		bottomTenPercentReturns = new vector<double>();
		
	}
	
	
	MonthlyData(const MonthlyData& copy) :
		year(copy.year), month(copy.month), tenPercentSize(copy.tenPercentSize),
		topTenPercentAverage(copy.topTenPercentAverage),
		bottomTenPercentAverage(copy.bottomTenPercentAverage),
		monthAverage(copy.monthAverage) {
		
		data = new unordered_map<string, double>(*copy.data);
		topTenPercent = new priority_queue<DataSample,
					vector<DataSample>, minHeapComparator>(*copy.topTenPercent);
		bottomTenPercent = new priority_queue<DataSample,
					vector<DataSample>, maxHeapComparator>(*copy.bottomTenPercent);
		
		topTenPercentReturns = new vector<double>(*copy.topTenPercentReturns);
		bottomTenPercentReturns = new vector<double>(*copy.bottomTenPercentReturns);
		
	}
	
	
	~MonthlyData() {
		
		delete data;
		delete topTenPercent;
		delete bottomTenPercent;
		delete topTenPercentReturns;
		delete bottomTenPercentReturns;
		
	}
	
	
	string getYear() const {
		
		return year;
		
	}
	
	
	string getMonth() const {
		
		return month;
		
	}
	
	
	string getYearMonth() const {
		
		return this->getYear() + "-" + this->getMonth();
		
	}
	
	
	double getSingleReturn(const string& symbol) const {
		
		return (*data)[symbol];
		
	}
	
	
	double getTopTenPercentReturn() const {
		
		return topTenPercentAverage;
		
	}
	
	
	double getBottomTenPercentReturn() const {
		
		return bottomTenPercentAverage;
		
	}
	
	
	double getMonthReturn() const {
		
		return monthAverage;
		
	}
	
	
	void addData(const string& symbol, const double& rate) {
		
		(*data)[symbol] = rate;
		
	}
	
	
	double getMonthReturn(const vector<MonthlyData>::iterator& nextMonth) {
		
		this->sort();
		this->getTopTenPercentReturns(nextMonth);
		this->getBottomTenPercentReturn(nextMonth);
		
		for (const double& rate : *topTenPercentReturns) {
			topTenPercentAverage += rate;
		}
		topTenPercentAverage /= static_cast<double>(tenPercentSize);
		
		for (const double& rate : *bottomTenPercentReturns) {
			bottomTenPercentAverage += rate;
		}
		bottomTenPercentAverage /= static_cast<double>(tenPercentSize);
		
		monthAverage = bottomTenPercentAverage - topTenPercentAverage;
		
		return monthAverage;
	}
	
};




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
