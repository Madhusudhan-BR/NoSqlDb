#pragma once

//////////////////////////////////////////////////////////////////////////////
//  QueryHeader.h - This header file consists of functions that can be used //
//					to query the database.									//
//  ver 1.0																	//
//																			//
//  Language:      C++														//
//  Platform:      2016 15" MacBook pro running Windows 10 EDU via Bootcamp //
//  Application:   NoSql in memory database Project 1 CSR687				//
//  Author:        Madhusudhan Bengaluru Ramanujam SUID:914198627			//
//////////////////////////////////////////////////////////////////////////////
/*
 Operations of the header:
==================

This defines a Query class
* -The functions defined in this class are used for,
> Returning value and children when a specific key is given
> Returning the records when key is matched.
> Returning the records when Item name, category, or data is matched.
> Returning the records created between a specific time interval.

Keys queryOne(NoSqlDb<Data> &db, Keys list, std::string str, std::string pattern);	 // This is used to return a set of keys that matches the pattern "pattern" inside the
																						keys name, category, description and keys
Keys queryOne(NoSqlDb<Data> &db, std::string str, std::string search);				 // This returns the set of keys matching the paramater "search"


Keys queryOne(NoSqlDb<Data> &db, Data data, Keys list , std::string str);			// This is used to query the data

Keys queryOne(Keys queryResultOne, Keys queryResultTwo);							//this function performs the union of two queries		

Keys queryData(NoSqlDb<Data> &db, Keys keys, std::string str, Data data);			//This function is used to query the data		

Keys queryTime(NoSqlDb<Data> &db,Keys keys,std::string startTime, std::string endTime); //this query is used to return records within a specified duration 



*/
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <fstream>
#include "../Convert/Convert.h"
#include "../CppProperties/CppProperties.h"
#include "../nosqldb/NoSqlDb.h"
#include "../strhelper.h"
#include <regex>

template<typename Data> 
class Query {

public : 
	using Key    = std::string;
	using Keys   = std::vector<std::string>; 

	Keys queryOne(NoSqlDb<Data> &db, Keys list, std::string str, std::string pattern);

	Keys queryOne(NoSqlDb<Data> &db, std::string str, std::string search);

	Keys queryOne(NoSqlDb<Data> &db, Data data, Keys list , std::string str);
	
	Keys queryOne(Keys queryResultOne, Keys queryResultTwo);

	Keys queryData(NoSqlDb<Data> &db, Keys keys, std::string str, Data data); 
	Keys queryTime(NoSqlDb<Data> &db,Keys keys,std::string startTime, std::string endTime); 
};
//This is used to return a set of keys that matches the pattern "pattern" inside the
//keys name, category, description and keys
template<typename Data> 
std::vector<std::string> Query<Data>::queryOne(NoSqlDb<Data> &db, Keys list, std::string str, std::string pattern) {
	Keys t; 
	std::regex re("(.*)("+pattern+")(.*)");
	if (str == "name")
	{
		for (Key k : list)
		{
	
			if (std::regex_match(k,re))
				t.push_back(k);
		}
	}
	else if (str == "category")
	{
		for (Key k : list)
		{
			if (std::regex_match(std::string(db.value(k).category), std::regex(".*" + pattern + ".*") ))
				t.push_back(k);
		}
	}
	else if (str == "description")
	{
		for (Key k : list)
		{
			if (std::regex_match(std::string(db.value(k).description), std::regex(".*" + pattern + ".*") ))
				t.push_back(k);
		}
	}
	else if (str == "key")
	{
		for (Key k : list)
		{
			if (std::regex_match(std::string(k), std::regex(".*" + pattern + ".*")))
			{
				t.push_back(k);
			}
		}
	}
	return t;

}
//This returns the set of keys matching the paramater "search"
template<typename Data>
std::vector<std::string> Query<Data>::queryOne(NoSqlDb<Data> &db, std::string str, std::string pattern) {

	Keys allkeys = db.keys();
	Keys temp;
	for (Key k : allkeys)
	{
		if (k == pattern)
		{
			if (str == "value")
			{
				Element<Data> tvalue = db.value(k);
				std::cout << tvalue.show();
				/*std::vector<std::string> returnVector;
				Data dat = tvalue.data;
				returnVector.push_back[dat];
				return returnVector;
				*/
				return temp;
			}
			else if (str == "children")
			
			{
				Element<Data> tvalue = db.value(k);
				std::vector<std::string> childofElem = tvalue.childKeys;
				return tvalue.childKeys;
			}
		}
	}

	return temp;

}
// This is used to query the data
template<typename Data>
std::vector<std::string> Query<Data>::queryOne(NoSqlDb<Data> &db, Data data, Keys list, std::string str) {
	
	Keys temp_keys;
	std::string search_data = Convert<Data>::toString(data);
	std::regex rexp(".*" + search_data + ".*");
	if (str == "data")
	{
		for (Key k : list)
		{
			Data d = db.value(k).data;
			std::string dataText = Convert<Data>::toString(d);
			if (std::regex_match(dataText, rexp))
				temp_keys.push_back(k);
		}
	}
	return temp_keys;
}
// This is used to query the data
template<typename Data> 
std::vector<std::string> Query<Data>::queryData(NoSqlDb<Data> &db,Keys keys, std::string str, Data data) {

	Keys temp;
	std::string dat = Convert<Data>::toString(data);
	std::regex rexp(".*" + dat + ".*");
	if (str == "data")
	{
		for (Key k : keys)
		{
			Data d = db.value(k).data;
			std::string dataText = Convert<Data>::toString(d);
			if (std::regex_match(dataText, rexp))
				temp.push_back(k);
		}
	}
	return temp;
}

//this function performs the union of two queries
template<typename Data> 
std::vector<std::string> Query<Data>::queryOne(Keys queryResultOne, Keys queryResultTwo) {
	
	Keys unionKeys; 
	for (Key k : queryResultOne)
		unionKeys.push_back(k);

	for (Key k : queryResultTwo)
		unionKeys.push_back(k); 

	return unionKeys;
}
//this query is used to return records within a specified duration 
template<typename Data>
std::vector<std::string> Query<Data>::queryTime(NoSqlDb<Data> &db, Keys keys, std::string startTime, std::string endTime) {
	std::vector<Key> timeVect;
	time_t stime = db.TimeConvertfromString(startTime);
	if (endTime == "default")
	{
		
		time_t etime = time(0);
		for (Key i : keys)
		{
			time_t temp = db.value(i).timeDate;
			
			if (stime <= temp && temp <= etime) 
			{
				timeVect.push_back(i);
			}
		}
	}
	else
	{
		//std::vector<std::string> timeVect;
		time_t etime = db.TimeConvertfromString(endTime);
		for (Key i : keys) 
		{
			time_t temp = db.value(i).timeDate;
			if ((stime <= temp) && (temp <= etime))
			{
				//db.value(i).show;
				timeVect.push_back(i);
			}
		}
	}
	return timeVect;
}
