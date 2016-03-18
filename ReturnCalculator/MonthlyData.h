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

/**
 *
 * Return Calculator class header code:
 *
 * This header file defines class skeleton and
 * data structure to be used by main program.
 *
 * @author Shangqi Wu
 *
 */


/**
 *
 * This struct is pair of company symbol and monthly return rate,
 * which are stored as strings, and double numbers, respectively.
 *
 */
struct DataSample {
	
	// Keep company symbols with corresponding monthly return together.
	string corpSymbol;
	double returnRate;
	
	// Default constructor.
	DataSample() :
	corpSymbol(""), returnRate(0) {}
	
	// Constructor with separate input value of symbol and return rate.
	DataSample(const string& symbol, const double& rate) :
	corpSymbol(symbol), returnRate(rate) {}
	
	// Constructor with integrate pair of symbol and return rate.
	DataSample(const pair<string, double>& sample) :
	corpSymbol(sample.first), returnRate(sample.second) {}
	
};

/**
 *
 * This class stores the return rates in the same month of all companies. 
 *
 * Year and month indicate date.
 * The hash table stores return rates of companies identified by company symbol.
 * Two priority queues are used to keep track of stocks that have a return rate of 
 *		top ten percent or bottom ten percent in this month.
 * Two vectors are used to store next month's return rates, which correspond to top
 *		or bottom ten percent in this month.
 *
 */
class MonthlyData {
	
private:
	
	// This is the function object to help maintianing the max heap which
	// keeps track of bottom ten percent return rates.
	struct maxHeapComparator {
		bool operator () (const DataSample& smaller, const DataSample& larger) const {
			return smaller.returnRate < larger.returnRate;
		}
	};
	
	// This is the function object to help maintaining the min heap which
	// keeps track of top ten percent return rates.
	struct minHeapComparator {
		bool operator () (const DataSample& smaller, const DataSample& larger) const {
			return larger.returnRate < smaller.returnRate;
		}
	};
	
	// Year and month information.
	string year;
	string month;
	
	
	// The hash table to store return rates of all stocks in the same month,
	// return rate values are identified by stock symbol, the string.
	// This variable acts as database of this month's data, which is core of functionality. 
	unordered_map<string, double> stockReturns;
	
	
	// The min and max heaps to keep track of top and bottom ten percent return rate stocks.
	// The long variable is calculated proper size of heap size.
	unsigned long tenPercentSize;
	priority_queue<DataSample, vector<DataSample>, minHeapComparator> topTenPercent;
	priority_queue<DataSample, vector<DataSample>, maxHeapComparator> bottomTenPercent;
	
	
	// Retrieved next month's return rates which correspond to top or bottom ten percent
	// return rates in this month.
	vector<double> topTenPercentReturns;
	vector<double> bottomTenPercentReturns;
	
	// Calculated return results.
	double topTenPercentAverage;
	double bottomTenPercentAverage;
	double monthAverage;
	
	// The method generates max and min heap, or the bottom and top ten percent
	// return rate stocks list.
	void sort();
	
	// This method is to retrieve next month's return values correspond to stocks of
	// top ten percent return rates in this month.
	void getTopTenPercentReturns(const list<MonthlyData>::iterator& nextMonth);
	
	// This method is similar to previous one, which returns values correspond to
	// stocks of bottom ten percent return rates in this month.
	void getBottomTenPercentReturn(const list<MonthlyData>::iterator& nextMonth);
	
public:
	
	// A default constructor specifying year and month.
	MonthlyData(const string& y, const string& m);
	// The copy constructor.
	MonthlyData(const MonthlyData& copy);
	
	// Inspectors for date information.
	string getYear() const;
	string getMonth() const;
	string getYearMonth() const;
	
	// Inspector to retrieve stock return value of input company symbol.
	double getSingleReturn(const string& symbol);
	
	// Inspectors for calculated average return rates.
	double getTopTenPercentReturn() const;
	double getBottomTenPercentReturn() const;
	double getMonthReturn() const;
	
	// The mutator to insert stock symbol and return rate into hash table.
	void addData(const string& symbol, const double& rate);
	
	// A wrapper method to generate 2 heaps first, and calulate average return values.
	double getMonthReturn(const list<MonthlyData>::iterator& nextMonth);
	
};


#endif /* MonthlyData_h */
