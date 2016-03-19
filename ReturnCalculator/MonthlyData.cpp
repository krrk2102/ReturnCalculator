//
//  MonthlyData.cpp
//  ReturnCalculator
//
//  Created by Shangqi Wu on 16/3/17.
//  Copyright © 2016 Shangqi Wu. All rights reserved.
//

#include "MonthlyData.h"

using namespace std;

/**
 *
 * This cpp file contains full codes that implement all member functions
 * of MonthlyData class.
 *
 * @author Shangqi Wu
 *
 */

/**
 *
 * This is default constructor specifying year and month information.
 *
 * @param y: a string represents current year.
 * @param m: a string represents current month.
 *
 * Other class members are initialized to empty or zero.
 *
 */
MonthlyData::MonthlyData(const string& y, const string& m) {
	
	year = y;
	month = m;
	
	tenPercentSize = 0;
	topTenPercentAverage = 0;
	bottomTenPercentAverage = 0;
	monthAverage = 0;

}

/**
 *
 * This is the copy constructor of MonthlyData.
 *
 * @param copy: input object to be fully copied.
 *
 * All class members are initialized to have identical values
 * as the input object members.
 *
 */
MonthlyData::MonthlyData(const MonthlyData& copy) {
	
	year = copy.year;
	month = copy.month;
	
	tenPercentSize = copy.tenPercentSize;
	topTenPercentAverage = copy.topTenPercentAverage;
	bottomTenPercentAverage = copy.bottomTenPercentAverage;
	monthAverage = copy.monthAverage;
	
	stockReturns = copy.stockReturns;
	
	topTenPercent = copy.topTenPercent;
	bottomTenPercent = copy.bottomTenPercent;
	
}


/**
 *
 * This method is private. While the program iterating through the hash table containing
 * stock symbols, it maintains and generates min and max heaps, which correspond to top
 * and bottom ten percent return stocks respectively. 
 *
 * This method is not supposed to receive any input parameters nor to return any results.
 * This method is supposed to be called after all stock return rates have been recorded 
 *	in the hash table class member.
 *
 */
void MonthlyData:: sort() {
	
	// Before starting heap generation, make sure 2 heaps are empty.
	while (!topTenPercent.empty()) {
		topTenPercent.pop();
	}
	
	while (!bottomTenPercent.empty()) {
		bottomTenPercent.pop();
	}
	
	// Calculate the proper size (10% of total stocks) of heaps.
	tenPercentSize = stockReturns.size() / 10;
	
	// Maintain the max and min heap while iterating the hash table.
	// Each sample is a pair of string, or stock symbol, and double, or return rate.
	for (const auto& sample : stockReturns) {
		
		// Min heap maintenance: if it is under-sized, add new sample into the heap.
		// If it is over-sized and new sample has a larger return than the value on
		// heap top, the top will be replaced by new sample, otherwise new sample
		// will be discarded.
		// All return rates in the heap will be the larger ten percent.
		if (topTenPercent.size() < tenPercentSize) {
			topTenPercent.push(DataSample(sample));
		} else {
			
			if (sample.second >= topTenPercent.top().returnRate) {
				
				topTenPercent.pop();
				topTenPercent.push(DataSample(sample));
				
			}
		}
		
		// Similar to min heap maintenance, the only difference is that it keeps a
		// max heap, so comparator is inversed.
		if (bottomTenPercent.size() < tenPercentSize) {
			bottomTenPercent.push(DataSample(sample));
		} else {
			
			if (sample.second <= bottomTenPercent.top().returnRate) {
				
				bottomTenPercent.pop();
				bottomTenPercent.push(DataSample(sample));
				
			}
		}
	}
	
}


/**
 *
 * This method extracts top ten percent return rates from the min heap one by one,
 * for each data sample, or pair of stock symbol and return rate, it refer to next month
 * data and append next month return rate to recording vector/array.
 * This method is supposed to be called after the sort() method is executed.
 * 
 * @param nextMonth: an iterator, specifying address of next month data in the STL linked list.
 *
 * No value is supposed to be returned.
 *
 */
void MonthlyData:: getTopTenPercentReturns(const list<MonthlyData>::iterator& nextMonth) {
	
	// Make sure recording vector/array is empty.
	topTenPercentReturns.clear();
	
	// Extracting heap elements by one, and retrieve next month return of
	// current stock, then append the next month return to the record.
	// If all elements in the heap are all processed, exit this loop.
	while (!topTenPercent.empty()) {
		
		string symbol = topTenPercent.top().corpSymbol;
		double nextMonthReturn = nextMonth->getSingleReturn(symbol);
		topTenPercentReturns.push_back(nextMonthReturn);
		topTenPercent.pop();
		
	}
	
}


/**
 *
 * This method extracts bottom ten percent return rates from the max heap one by one,
 * for each data sample, or pair of stock symbol and return rate, it refer to next month
 * data and append next month return rate to recording vector/array.
 * This method is supposed to be called after the sort() method is executed.
 *
 * @param nextMonth: an iterator, specifying address of next month data in the STL linked list.
 *
 * No value is supposed to be returned.
 *
 */
void MonthlyData::getBottomTenPercentReturn(const list<MonthlyData>::iterator& nextMonth) {
	
	// Make sure recording vector/array is empty.
	bottomTenPercentReturns.clear();
	
	// Extracting heap elements by one, and retrieve next month return of
	// current stock, then append the next month return to the record.
	// If all elements in the heap are all processed, exit this loop.
	while (!bottomTenPercent.empty()) {
		
		string symbol = bottomTenPercent.top().corpSymbol;
		double nextMonthReturn = nextMonth->getSingleReturn(symbol);
		bottomTenPercentReturns.push_back(nextMonthReturn);
		bottomTenPercent.pop();
		
	}
	
}


/**
 *
 * This method is inspector to get year information.
 *
 * return a string representing year.
 *
 */
string MonthlyData::getYear() const {
	
	return year;
	
}

/**
 *
 * This method is inspector to get month information.
 *
 * return a string representing month.
 *
 */
string MonthlyData::getMonth() const {
	
	return month;
	
}


/**
 *
 * This method is inspector to get year and month information combined.
 *
 * return a string representing year and month in format of "16-Mar".
 *
 */
string MonthlyData::getYearMonth() const {
	
	return this->getYear() + "-" + this->getMonth();
	
}

/**
 *
 * This method is inspector to get the return rate of this month by specified 
 * input stock symbol.
 *
 * @param symbol: a string of company stock symbol.
 *
 * return a string representing month.
 *
 */
double MonthlyData::getSingleReturn(const string& symbol) {
	
	return stockReturns[symbol];
	
}

/**
 *
 * This method is inspector to get calculated average next month's return rate 
 * of top ten percent stocks in this month. 
 * This method is supposed to be called after sorting method and heap processing
 * method is finished.
 *
 * return a double which is the value of average return rate.
 *
 */
double MonthlyData::getTopTenPercentReturn() const {
	
	return topTenPercentAverage;
	
}

/**
 *
 * This method is inspector to get calculated average next month's return rate
 * of bottom ten percent stocks in this month.
 * This method is supposed to be called after sorting method and heap processing
 * method is finished.
 *
 * return a double which is the value of average return rate.
 *
 */
double MonthlyData::getBottomTenPercentReturn() const {
	
	return bottomTenPercentAverage;
	
}

/**
 *
 * This method is inspector to get calculated average next month's return rate
 * of both top and bottom ten percent stocks in this month.
 * This method is supposed to be called after sorting method and heap processing
 * method is finished.
 *
 * return a double which is the value of average return rate.
 *
 */
double MonthlyData::getMonthReturn() const {
	
	return monthAverage;
	
}


/**
 *
 * This method is a mutator which insert new stock symbol along with its current monthly 
 * return rate into the hash table.
 * This method is supposed to be called during the monthly data construction stage. 
 * If this method is called, i.e., stock information is modified, after a sorting 
 * and heap analysis process, please execute the analysis process again. 
 * This method is not supposed to return any value.
 *
 * @param symbol: a string of stock company symbol.
 * @param rate: a double number contains value of return rate in current month.
 *
 */
void MonthlyData::addData(const string& symbol, const double& rate) {
	
	stockReturns[symbol] = rate;
	
}


/**
 *
 * This method is designed as a wrapper process of heap generating, analysis and average 
 * values calculating. It calls private methods for heap process in proper order, so improper
 * execution sequence by users can be prevented. 
 * This function is supposed to be called when all stock return rates are inserted into hash table. 
 * 
 * @param nextMonth: an iterator specifying address of next month's data in a STL linked list
 *
 */
double MonthlyData::getMonthReturn(const list<MonthlyData>::iterator& nextMonth) {
	
	// Calls to private methods.
	this->sort();
	this->getTopTenPercentReturns(nextMonth);
	this->getBottomTenPercentReturn(nextMonth);
	
	// Calculating top ten percentage next month's average return rate.
	for (const double& rate : topTenPercentReturns) {
		topTenPercentAverage += rate;
	}
	topTenPercentAverage /= static_cast<double>(tenPercentSize);
	
	// Calculating bottom ten percentage next month's average return rate.
	for (const double& rate : bottomTenPercentReturns) {
		bottomTenPercentAverage += rate;
	}
	bottomTenPercentAverage /= static_cast<double>(tenPercentSize);
	
	// Calculate the average of top ten and bottom ten percent next month's return rate. 
	monthAverage = bottomTenPercentAverage - topTenPercentAverage;
	
	return monthAverage;
}


