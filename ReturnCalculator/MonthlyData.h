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
#include <list>
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
	
	unordered_map<string, double> stockReturns;
	
	unsigned long tenPercentSize;
	priority_queue<DataSample, vector<DataSample>, minHeapComparator> topTenPercent;
	priority_queue<DataSample, vector<DataSample>, maxHeapComparator> bottomTenPercent;
	
	vector<double> topTenPercentReturns;
	vector<double> bottomTenPercentReturns;
	
	double topTenPercentAverage;
	double bottomTenPercentAverage;
	double monthAverage;
	
	
	void sort();
	void getTopTenPercentReturns(const list<MonthlyData>::iterator& nextMonth);
	void getBottomTenPercentReturn(const list<MonthlyData>::iterator& nextMonth);
	
public:
	
	
	MonthlyData(const string& y, const string& m);
	MonthlyData(const MonthlyData& copy);
	
	
	string getYear() const;
	string getMonth() const;
	string getYearMonth() const;
	
	
	double getSingleReturn(const string& symbol);
	double getTopTenPercentReturn() const;
	double getBottomTenPercentReturn() const;
	double getMonthReturn() const;
	
	
	void addData(const string& symbol, const double& rate);
	double getMonthReturn(const list<MonthlyData>::iterator& nextMonth);
	
};


#endif /* MonthlyData_h */
