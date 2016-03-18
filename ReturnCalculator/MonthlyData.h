//
//  MonthlyData.h
//  ReturnCalculator
//
//  Created by Shangqi Wu on 16/3/17.
//  Copyright © 2016 Shangqi Wu. All rights reserved.
//

#ifndef MonthlyData_h
#define MonthlyData_h

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
	
	unordered_map<string, double>* stockReturns;
	
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
		
		tenPercentSize = stockReturns->size() / 10;
		for (const auto& sample : (*stockReturns)) {
			
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
		
		stockReturns = new unordered_map<string, double>();
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
		
		stockReturns = new unordered_map<string, double>(*copy.stockReturns);
		topTenPercent = new priority_queue<DataSample,
		vector<DataSample>, minHeapComparator>(*copy.topTenPercent);
		bottomTenPercent = new priority_queue<DataSample,
		vector<DataSample>, maxHeapComparator>(*copy.bottomTenPercent);
		
		topTenPercentReturns = new vector<double>(*copy.topTenPercentReturns);
		bottomTenPercentReturns = new vector<double>(*copy.bottomTenPercentReturns);
		
	}
	
	
	~MonthlyData() {
		
		delete stockReturns;
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
		
		return (*stockReturns)[symbol];
		
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
		
		(*stockReturns)[symbol] = rate;
		
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


#endif /* MonthlyData_h */
