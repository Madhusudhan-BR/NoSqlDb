#pragma once
//////////////////////////////////////////////////////////////////////////////
//  NoSqlDb.h - Key-value pair in-memory database.							//
//  ver 1.0																	//
//																			//
//  Language:      C++														//
//  Platform:      2016 15" MacBook pro running Windows 10 EDU via Bootcamp //
//  Application:   NoSql in memory database Project 1 CSR687				//
//  Author:        Madhusudhan Bengaluru Ramanujam SUID:914198627			//
//////////////////////////////////////////////////////////////////////////////
/*
 Operations performed :
=====================

This header file defines a No SQL database class and an Element class.
* NoSqlDb instance creates an underoreded map in the Heap memory
  and it consists of a constructor, a destructor that stores the database if it closes abruptly.
  It also consists of functions that are used to create a new record, edit/ update an existing record
  and delete a record from the database too. It also has methods to read a database from XML format and to write to XML format.
* The Element instance defines the value part of the map and
  holds the meta data and the data for the record stored.

Both classes that is the NoSqlDb class and the Element class are template classes.


NoSqlDb Class :
=============

NoSqlDb()								//Creates NoSqlDb instance with path defined
~NoSqlDb()								//Destructor of NoSqlDb instance with XML persistance
keys()									//returns all the keys stored in the database
showVect(keys k)						//displays all the keys
save()									//saves the key-value pair in the database
value()									//returns the value of the specific key in the database
count()									//returns the number of elements in the database
addingRecord()							//creates a record in the database
DeletionOfRecord()						//Deletes a specific record in the database
DeleteInChildren()						//Deletes the reference of the record in children
time_t TimeConvertfromString(const std::string timeanddate); //converts time from string format to time_t and returns it
Key TimeConvert(time_t time);			//converst time from time_t to a string and returns it 
updateRecord()							//this is an overloaded function that is used to update values stored in records.
std::string toXML();					//stores the in memory database in an xml file
std::string readfromXml(std::string filename); //streams from xml file
void fromXML(const std::string xmlString);	//reads from the xml file into memory

string path							//path will hold the relative path of the XML file
shared_ptr<AbstractXmlElement> sptr		//Shared pointer used to create nodes in XML Document

Element Class > ---- >

show()							// displays the values stored in an element
name, category, description		//Meta data of an element
childrenKeys					//Vector holding children dependencies
timeDate						//of type time_t, holds Date-time of record creation
data 							//Storing the actual data of type Data<template>

Build Process:
==============
Required files
- Cppproperties.h, Convert.h,
- XmlDocument.h, XmlElement.h,
- StrHelper.h
- NoSqlDb.cpp

Build commands
- devenv oodproject_1.sln

Maintenance History:
====================
ver 1.0 : 11 Feb 17
- first release
*/



#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include<ctime>
#include<algorithm>
#include "../CppProperties/CppProperties.h"
#include "../XmlDoc/XmlDocument/XmlDocument.h"
#include "../XmlDoc/XmlElement/XmlElement.h"
#include "../Convert/Convert.h"
#include "../strhelper.h"
#include <fstream>

/////////////////////////////////////////////////////////////////////
// Element class represents a data record in our NoSql database
// - in our NoSql database that is just the value in a key/value pair
// - it needs to store child data, something for you to implement
//
template<typename Data>
class Element
{
public:
	using Name = std::string;
	using Category = std::string;
	using TimeDate = std::time_t;
	using Description = std::string;
	using key = std::string;
	using childrenKeys = std::vector<key>;
	//using allTheKeys = std::vector<std::string>;


	Property<Name> name;            // metadata
	Property<Category> category;    // metadata
	Property<TimeDate> timeDate;    // metadata
	Property<Data> data;			// data
	Property<Description> description;
	Property<childrenKeys> childKeys;


	std::string show();
};

// Displays all keys
template<typename Data>
std::string Element<Data>::show()
{
	// show children when you've implemented them

	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "name" << " : " << name;
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "Description" << " : " << description;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << std::setw(8) << "data" << " : " << data;
	out << "\n    " << std::setw(8) << "children keys " << " :";

	std::vector<std::string> childVect = childKeys;

	for (key temp : childVect) {

		out << " \n  " << std::setw(8) << temp;

	}



	out << "\n";
	return out.str();
}

/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements
// - you will need to add query capability
//   That should probably be another class that uses NoSqlDb
//

using namespace XmlProcessing;
template<typename Data>
class NoSqlDb
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;
	using sPtr = std::shared_ptr<AbstractXmlElement>;
	using xmlString = std::string;
	using xml = std::string;
	using allTheKeys = std::vector<std::string>;
	std::string path;
	
	
	Keys keys();
	bool save(Key key, Element<Data> elem);
	Element<Data> value(Key key);
	size_t count();

	void addingRecord(std::string key, std::string name, std::string category, std::string description, std::time_t time_Date, Keys children, Data data);
	void DeletionOfRecord(Key k);
	void DeletionInChildren(Key k);

	time_t TimeConvertfromString(const std::string timeanddate);
	Key TimeConvert(time_t time);

	void updateRecord(Key k, std::string meta, std::string newData);
	bool updateRecord(Key k, std::string meta, std::string addOrRemove, Key newK);
	void updateRecordChild(Key k, bool value, Key newK);
	void updateRecord(Key k, Data updateData);
	void updateRecord(Key k, std::time_t);

	std::string toXML();
	std::string readfromXml(std::string filename);
	void fromXML(const std::string xmlString);

	NoSqlDb<Data>(const std::string p);

//	std::string path; 
	void showVect(Keys k);
	~NoSqlDb();

private:
	using Item = std::pair<Key, Element<Data>>;

	std::unordered_map<Key, Element<Data>> store;
};

//streams from the xml file
template<typename Data> 
std::string NoSqlDb<Data>::readfromXml(std::string filename) {
	std::string xmldata; 
	std::string lines; 

	std::ifstream inSt(filename);
	if (inSt.is_open()) {
		while (getline(inSt, lines)) {
			xmldata = xmldata + lines; 
		}
		inSt.close();
	}
	else {
		std::cout << "Can't access file \n"; 

	}

	return xmldata; 
}
template<typename Data>
void NoSqlDb<Data>::updateRecord(Key k, std::string meta, std::string newData) {

	Element<Data> record = value(k);

	if (meta == "name") {
		record.name = newData;
		//return;
	}
	if (meta == "category") {
		record.category = newData;
		//return;
	}
	if (meta == "description") {
		record.description = newData;
		//return;
	}

	store[k] = record;

	return;


}
//Overloaded updaate functions, these functions are used to update the records in the database 
template<typename Data>
bool NoSqlDb<Data>::updateRecord(Key k, std::string meta, std::string addOrRemove, Key newK) {

	//Element<Data> temp; 
	if (store.find(k) != store.end()) {

		bool value;
		if (addOrRemove == "add") 
			value = true;
		else if (addOrRemove == "remove") 
			value = false;

		updateRecordChild(k, value, newK);
		return true;
	}
	return true; 
}

template<typename Data>
void NoSqlDb<Data>::updateRecordChild(Key k, bool values, Key newK) {
	Element<Data> temp = value(k);
	Keys child = temp.childKeys;
	bool found = false;
	if (values == true)	//Adding
	{
		//if (std::find(child.begin(), child.end(), newK) == c.end())
		for (unsigned int i = 0; i < child.size(); i++) {
			if (child[i] == newK)
				found = true;
			else
				found = false;
		}
		if (found == false) {

			child.push_back(newK);
		}
	}
	else
	{
		for (unsigned i = 0; i < child.size(); i++)
		{
			if (child[i] == newK)
			{
				child.erase(child.begin() + i);
				break;
			}
		}
	}
	temp.childKeys = child;
	store[newK] = temp;
}

template<typename Data>
void NoSqlDb<Data>::updateRecord(Key k, Data data) {
	Element<Data> temp_record;
	if (store.find(k) != store.end()) {
		temp_record = value(k);
		temp_record.data = data;
		store[k] = temp_record;
	}
	else
		out << "\n " << "Key not found";
}

template<typename Data>
void NoSqlDb<Data>::updateRecord(Key k, std::time_t time) {
	Element<Data> temp_record;
	if (store.find(k) != store.end()) {
		temp_record = value(k);
		temp_record.timeDate = time;
		store[k] = temp_record;
	}
	else
		out << " \n key not found ";
}
//return all keys in the database
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
	Keys keys;
	for (Item item : store)
	{
		keys.push_back(item.first);
	}
	return keys;
}
//adds a record to the database
template<typename Data>
void NoSqlDb<Data>::addingRecord(std::string key, std::string name, std::string category, std::string description, std::time_t time_Date, Keys children, Data data) {
	if (store.find(key) != store.end())
		return;
	else
	{
		Element<Data> newElement;
		Keys tempKeys;
		
		newElement.name = name;
		newElement.timeDate =time_Date ;
		newElement.category = category;

		newElement.data = data;
		newElement.description = description;

		for (Key k : children)
		{
			tempKeys.push_back(k);

		}

		newElement.childKeys = tempKeys;
		save(newElement.name, newElement);
		return;
	}
}
//Delets a record
template<typename Data>
void NoSqlDb<Data>::DeletionOfRecord(Key k) {
	if (store.find(k) != store.end()) {

		DeletionInChildren(k);
		store.erase(k);
		return;
	}

	else
		return;

}
//Delets dependencies and updates children
template<typename Data>
void NoSqlDb<Data> ::DeletionInChildren(Key k) {
	Keys allTheKeys = keys();
	for (Key k_temp : allTheKeys) {
		Element<Data> temp = value(k_temp);
		Keys child = temp.childKeys;

		for (unsigned int i = 0; i < child.size(); i++) {
			if (child[i] == k) {
				child.erase(child.begin() + i);
			}
		}
		temp.childKeys = child;
		store[k_temp] = temp;
	}
}

//saves the key-value pair in the database
template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
	if (store.find(key) != store.end())
		return false;
	store[key] = elem;
	return true;
}
//returns value of the specific key
template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();
}
//returns no of records in the database
template<typename Data>
size_t NoSqlDb<Data>::count()
{
	return store.size();
}

//persisting the inmemory database into xml
template<typename Data>
std::string NoSqlDb<Data>::toXML() {
	
	
		std::string document;
		XmlDocument doc;
		allTheKeys allkeys = keys();

		sPtr pRoot = makeTaggedElement("Database");
		doc.docElement() = pRoot;

		for (xml element : allkeys) {

			Element<Data> ele = value(element);
			sPtr theRecord = makeTaggedElement("Record");
			pRoot->addChild(theRecord);

			sPtr keyElement = makeTaggedElement("Key");
			sPtr keyText = makeTextElement(ele.name);
			theRecord->addChild(keyElement);
			keyElement->addChild(keyText);


			sPtr RecordName = makeTaggedElement("Name");
			sPtr RecordText = makeTextElement(ele.name);
			theRecord->addChild(RecordName);
			RecordName->addChild(RecordText);

			sPtr RecordCategory = makeTaggedElement("Category");
			sPtr RecordTextCategory = makeTextElement(ele.category);
			theRecord->addChild(RecordCategory);
			RecordCategory->addChild(RecordTextCategory);

			sPtr RecordTimeDate = makeTaggedElement("TimeDate");
			std::string timeinString = TimeConvert(ele.timeDate);
			sPtr RecordTextTimeDate = makeTextElement(timeinString);//include time currently given  DEFult string
			theRecord->addChild(RecordTimeDate);
			RecordTimeDate->addChild(RecordTextTimeDate);

			sPtr Recorddescription = makeTaggedElement("Description");
			sPtr RecordTextDescription = makeTextElement(ele.description);
			theRecord->addChild(Recorddescription);
			Recorddescription->addChild(RecordTextDescription);

			sPtr RecordData = makeTaggedElement("Data");
			std::string string_data = Convert<Data>::toString(ele.data);
			sPtr RecordTextData = makeTextElement(string_data);
			theRecord->addChild(RecordData);
			RecordData->addChild(RecordTextData);

			std::vector<std::string> dependantKeys = ele.childKeys;

			int counter = dependantKeys.size(), i = 0;

			sPtr RecordChild = makeTaggedElement("Children");
			theRecord->addChild(RecordChild);

			for (int i = 0; i < counter; i++) {
				xml child = dependantKeys[i];
				sPtr pchildelement = makeTaggedElement("child");
				RecordChild->addChild(pchildelement);
				sPtr pchildText = makeTextElement(child);
				pchildelement->addChild(pchildText);
			}
		}
		document = doc.toString();

		std::ofstream inputfile("../xmlfile/database.xml");
		inputfile << document;
		inputfile.close();


	
	


	return document;

}
//persisting contents from the xml file and saving it into the database
template<typename Data>
void NoSqlDb<Data>::fromXML(const std::string xmlString) {

	try {
		Element<Data> temp;
		std::string xml = readfromXml(xmlString);
		XmlDocument doc(xml, XmlDocument::str);			// changed doc to document here and  in the next line 
		std::vector<sPtr> desc = doc.descendents("Record").select();
		std::string valueofKey;
		//int i = 0;
		unsigned count = desc.size();
		for (unsigned int i = 0; i < count; i++) {



			valueofKey = desc[i]->children()[0]->children()[0]->value();
			valueofKey = trim(valueofKey);


			std::string valueofName = desc[i]->children()[1]->children()[0]->value();
			valueofName = trim(valueofName);
			temp.name = valueofName;

			std::string valueofcategory = desc[i]->children()[2]->children()[0]->value();
			valueofcategory = trim(valueofcategory);
			temp.category = valueofcategory;

			std::time_t valueofTime = TimeConvertfromString(desc[i]->children()[3]->children()[0]->value().c_str());
			//valueofTime = trim(valueofTime);
			temp.timeDate = valueofTime;
//			valueofCategory = trim(valueofCategory);
	//		temp.category = valueofCategory;


			std::string valueofdescription = desc[i]->children()[4]->children()[0]->value();
			valueofdescription = trim(valueofdescription);
			temp.description = valueofdescription;
			//std::time_t valueofTime = valueofTime;
			//temp.timeDate = valueofTime;


			std::string data = desc[i]->children()[5]->children()[0]->value();
			data = trim(data);
			temp.data = Convert<Data>::fromString(data);

			std::vector<std::string> children;
			std::vector<sPtr> spclPointerForChildren = desc[i]->children()[6]->children();

			for (sPtr specialPointer : spclPointerForChildren) {
				children.push_back(specialPointer->children()[0]->value());
			}
			temp.childKeys = children;
			save(valueofKey, temp);
			//store[valueofKey] = temp;

		}
	}
	catch (std::exception &e) {
		std::cout << e.what();
	}
}
//converts time from string format to time_t
template<typename Data> 
time_t NoSqlDb<Data>:: TimeConvertfromString(const std::string timeanddate) {

	struct tm temp = { 0 };
	int yy = 0;
	int mm = 0;
	int dd = 0;
	int hh = 0;
	int mins = 0;
	int ss = 0;
	const	char *ch= timeanddate.c_str();
	sscanf_s(ch, "%d-%d-%d.%d:%d:%d", &yy, &mm, &dd, &hh, &mins, &ss);
	temp.tm_year = yy - 1900;
	temp.tm_mon = mm - 1;
	temp.tm_mday = dd;
	temp.tm_hour = hh;
	temp.tm_min = mins;
	temp.tm_sec = ss;
	time_t ttime = mktime(&temp);
	return ttime;

}
//returns time in string format
template<typename Data> 
typename NoSqlDb<Data> :: Key NoSqlDb<Data>::TimeConvert(time_t time) {

	struct tm timerStruct = { } ;
	char timeVar[80]; 
	localtime_s(&timerStruct, &time);
	strftime(timeVar, sizeof(timeVar), "%Y-%m-%d.%X", &timerStruct);
	std::string timeString(timeVar);
	return timeString;
}

//constructor 
template<typename Data> 
NoSqlDb<Data> ::NoSqlDb(std::string p) {
	path = p;
}

//displays the keys
template<typename Data> 
void NoSqlDb<Data>::showVect(Keys k) {
	//keys1 showKeys = db.keys();
	for (Key ks : k) {
		std::cout << "\n  " << ks << ":";
		//std::cout << db.value(k).show();
	}
}

template<typename Data>
NoSqlDb<Data>::~NoSqlDb()
{
	 toXML();
}

/*template<typename Data> 
NoSqlDb<Data>::~NoSqlDb() {
	std::cout << " \n                                 Desctructor called";
	std::cout << "										persisting to xml \n	";
	toXML();
}
*/ 