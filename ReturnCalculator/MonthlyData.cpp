//
//  MonthlyData.cpp
//  ReturnCalculator
//
//  Created by Shangqi Wu on 16/3/17.
//  Copyright © 2016 Shangqi Wu. All rights reserved.
//

#include "MonthlyData.h"

using namespace std;


MonthlyData::MonthlyData(const string& y, const string& m) {
	
	year = y;
	month = m;
	
	tenPercentSize = 0;
	topTenPercentAverage = 0;
	bottomTenPercentAverage = 0;
	monthAverage = 0;

}


MonthlyData::MonthlyData(const MonthlyData& copy) {
	
	year = copy.year;
	month = copy.month;
	
	tenPercentSize = copy.tenPercentSize;
	topTenPercentAverage = copy.topTenPercentAverage;
	bottomTenPercentAverage = copy.bottomTenPercentAverage;
	monthAverage = copy.monthAverage;
	
}


void MonthlyData:: sort() {
	
	while (!topTenPercent.empty()) {
		topTenPercent.pop();
	}
	
	while (!bottomTenPercent.empty()) {
		bottomTenPercent.pop();
	}
	
	tenPercentSize = stockReturns.size() / 10;
	for (const auto& sample : stockReturns
		 ) {
		
		if (topTenPercent.size() < tenPercentSize) {
			topTenPercent.push(DataSample(sample));
		} else {
			
			if (sample.second >= topTenPercent.top().returnRate) {
				
				topTenPercent.pop();
				topTenPercent.push(DataSample(sample));
				
			}
		}
		
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


void MonthlyData:: getTopTenPercentReturns(const list<MonthlyData>::iterator& nextMonth) {
	
	topTenPercentReturns.clear();
	while (!topTenPercent.empty()) {
		
		string symbol = topTenPercent.top().corpSymbol;
		double nextMonthReturn = nextMonth->getSingleReturn(symbol);
		topTenPercentReturns.push_back(nextMonthReturn);
		topTenPercent.pop();
		
	}
	
}


void MonthlyData::getBottomTenPercentReturn(const list<MonthlyData>::iterator& nextMonth) {
	
	bottomTenPercentReturns.clear();
	while (!bottomTenPercent.empty()) {
		
		string symbol = bottomTenPercent.top().corpSymbol;
		double nextMonthReturn = nextMonth->getSingleReturn(symbol);
		bottomTenPercentReturns.push_back(nextMonthReturn);
		bottomTenPercent.pop();
		
	}
	
}


string MonthlyData::getYear() const {
	
	return year;
	
}


string MonthlyData::getMonth() const {
	
	return month;
	
}


string MonthlyData::getYearMonth() const {
	
	return this->getYear() + "-" + this->getMonth();
	
}


double MonthlyData::getSingleReturn(const string& symbol) {
	
	return stockReturns[symbol];
	
}


double MonthlyData::getTopTenPercentReturn() const {
	
	return topTenPercentAverage;
	
}


double MonthlyData::getBottomTenPercentReturn() const {
	
	return bottomTenPercentAverage;
	
}


double MonthlyData::getMonthReturn() const {
	
	return monthAverage;
	
}


void MonthlyData::addData(const string& symbol, const double& rate) {
	
	stockReturns[symbol] = rate;
	
}


double MonthlyData::getMonthReturn(const list<MonthlyData>::iterator& nextMonth) {
	
	this->sort();
	this->getTopTenPercentReturns(nextMonth);
	this->getBottomTenPercentReturn(nextMonth);
	
	for (const double& rate : topTenPercentReturns) {
		topTenPercentAverage += rate;
	}
	topTenPercentAverage /= static_cast<double>(tenPercentSize);
	
	for (const double& rate : bottomTenPercentReturns) {
		bottomTenPercentAverage += rate;
	}
	bottomTenPercentAverage /= static_cast<double>(tenPercentSize);
	
	monthAverage = bottomTenPercentAverage - topTenPercentAverage;
	
	return monthAverage;
}





