/////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database                 //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////

#include "NoSqlDb.h"
#include <iostream>

using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

int main()
{
	std::cout << "\n  Demonstrating NoSql Helper Code";
	std::cout << "\n =================================\n";

	std::cout << "\n  Creating and saving NoSqlDb elements with string data";
	std::cout << "\n -------------------------------------------------------\n";

	NoSqlDb<StrData> db("../db1.xml");

	Element<StrData> elem1;
	elem1.name = "elem1";
	elem1.category = "test";
	elem1.data = "elem1's StrData";
	elem1.description = "this is element 1 ";
	time_t current = time(&current);
	elem1.timeDate = current;

	std::vector<std::string> elem1Keys;
	elem1Keys.push_back("elem2");
	elem1.childKeys = elem1Keys;
	
	db.save(elem1.name, elem1);

	Element<StrData> elem2;
	elem2.name = "elem2";
	elem2.category = "test";
	elem2.data = "elem2's StrData";
	elem2.description = "hey this is element 2"; 
//	time_t current = time(&current);
	elem2.timeDate = current;

	std::vector<std::string> elem2Keys;
	elem1Keys.push_back("elem3");
	elem2.childKeys = elem1Keys;
	db.save(elem2.name, elem2);

	Element<StrData> elem3;
	elem3.name = "elem3";
	elem3.category = "test";
	elem3.data = "elem3's StrData";
	elem3.description = "this is element 3. ";
//	time_t current = time(&current);
	elem3.timeDate = current;

	std::vector<std::string> elem3Keys;
	elem3Keys.push_back("elem2");
	elem3.childKeys = elem1Keys;
	db.save(elem3.name, elem3);

	Element<StrData> elem5;
	elem5.name = "elem5";
	elem5.category = "tester2";
	elem5.data = "Elements'sData";
	elem5.description = "this is Element 4";
	elem5.childKeys = { "elem5" };

	db.save(elem5.name, elem5);

	db.updateRecord(elem3.name, "category", "Changing Category of 3");					//*Requirement 4/ Updating metadata
	db.updateRecord(elem3.name, "children", "add", "elem5");

	db.addingRecord("elem4","elem 4 ","Category", "Sample", current, { "elem3" },"Data"); // adding record
	db.addingRecord("elem6", "elem 6 ", "Category", "Sample", current, { "elem5" }, "Data");
	db.addingRecord("elem7", "elem 7 ", "Category", "Sample", current, { "elem1" }, "Data");
	db.addingRecord("elem8", "elem 8 ", "Category", "Sample", current, { "elem6" }, "Data");

	db.DeletionOfRecord(elem2.name);							// deletion of record


	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";

	std::cout << "\n  size of db = " << db.count() << "\n";
	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n\n";

	std::cout << "\n  Creating and saving NoSqlDb elements with int data";
	std::cout << "\n ----------------------------------------------------\n";

	NoSqlDb<intData> idb("../db2.xml");

	Element<intData> ielem1;
	ielem1.name = "elem1";
	ielem1.category = "test";
	ielem1.data = 1;

	idb.save(ielem1.name, ielem1);

	Element<intData> ielem2;
	ielem2.name = "elem2";
	ielem2.category = "test";
	ielem2.data = 2;

	idb.save(ielem2.name, ielem2);

	Element<intData> ielem3;
	ielem3.name = "elem3";
	ielem3.category = "test";
	ielem3.data = 3;

	idb.save(ielem3.name, ielem3);

	Element<intData> ielem4;
	ielem3.name = "elem4";
	ielem3.category = "test";
	ielem3.data = 4;

	idb.save(ielem3.name, ielem3);

	std::cout << "\n  Retrieving elements from NoSqlDb<int>";
	std::cout << "\n ---------------------------------------\n";

	std::cout << "\n  size of idb = " << idb.count() << "\n";
	Keys ikeys = idb.keys();
	for (Key key : ikeys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << idb.value(key).show();
	}
	std::cout << "\n\n";

	Key xmlfile = db.toXML();
	std::cout << " \n In Xml Format \n" << xmlfile;

	db.fromXML(xmlfile);

	std::cout << "\n  Displaying all the records in DB";
	std::cout << "\n ------------------------------------------\n";
	Keys storeContents = db.keys();
	for (Key eachElement : storeContents)
	{
		std::cout << "\n  " << eachElement << ":";
		std::cout << db.value(eachElement).show();
	}
	std::cout << "\n\n";

	getchar();

}
