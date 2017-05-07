//////////////////////////////////////////////////////////////////////////////
//  Source.cpp - This is the test executive that demonstrates the entire	//
//				project														//
//  ver 1.0																	//
//																			//
//  Language:      C++														//
//  Platform:      2016 15" MacBook pro running Windows 10 EDU via Bootcamp //
//  Application:   NoSql in memory database Project 1 CSR687				//
//  Author:        Madhusudhan Bengaluru Ramanujam SUID:914198627			//
//////////////////////////////////////////////////////////////////////////////

#include "../nosqldb/NoSqlDb.h"
#include "../Query/QueryHeader.h" 
#include<iostream>
//#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>

using Key = std::string; 
using Keys = std::vector<std::string>;

int main()
{
	try {
		NoSqlDb<std::string> db("../xmlfile/projectStructure.xml");

		db.fromXML("../xmlfile/projectStructure.xml");

		std::cout << "\n ----------------------------------------------------------------                                                                  ";
		std::cout << "\n			  Test Executive  Requirement 11                       ";
		std::cout << "\n                                                                 \n";

		std::cout << "\n ----------------------------------------------------------------                                                                  ";
		std::cout << "\n                  This is my project sturture                       ";
		std::cout << "\n                                                                   \n";


		std::cout << "\n Requirement 2 : Template class structure							";
		std::cout << "\n                                                                  \n";
		std::cout << db.value("NoSqlDb").show();

		std::cout << "\n --------------------------------------------------------------------                                                                  ";
		std::cout << "\n                        Addition and Deletion of a record : Requirement 3 \n                                          ";
		std::cout << "\n                                                                   ";
		std::cout << "\n --------------------------------------------------------------------------";
		std::cout << "\n                                         Displaying records                          ";
		std::cout << "\n                                                                   \n";

		time_t current;
		time(&current);

		db.addingRecord("QueryProject", " QueryProject", "test", "Project defining all the query functions", current, { "NoSqlDb" }, "Queries.cpp;Queries.h");		//*Requirement 3/ Addition of keys
		db.DeletionOfRecord("SampletoDelete");													//*Requirement 3/ Deletion of key

		std::cout << "\n  size of db = " << db.count() << "\n";
		Keys keys3 = db.keys();
		for (Key key : keys3)
		{
			std::cout << "\n  " << key << ":";
			std::cout << db.value(key).show();
		}

		std::cout << "-----------------------------------------------------------------------------------------------------";
		std::cout << "\n              Updating metadata and children                                                     \n";
		//std::cout << "\n --- Updating Metadata and children of TestExec in Database. -------<Requirement 4>";
		std::cout << "\n---------------------------------------------------------------------------------------------------                                                                   \n";

		db.updateRecord("TestExecutive", "category", "Final Application Test executive");					//*Requirement 4/ Updating metadata
		db.updateRecord("TestExecutive", "children", "add", "QueryProject");								//*Requirement 4/ Update children

		std::cout << "\n -----------------------------------------------------------------------------   Displaying records : \n";
		std::cout << "\n \n";

		std::cout << "\n  size of db = " << db.count() << "\n";
		Keys keys2 = db.keys();
		for (Key key : keys2)
		{
			std::cout << "\n  " << key << ":";
			std::cout << db.value(key).show();
		}

		std::string xml = db.toXML();																//*Requirement 5/ Persisting into XML//
		std::cout << "\n\n  -------------------------------------------------------------------------        Requirement 5  ";
		std::cout << "\n                                                                    persisting the Database into the file database.xml inside xmlfiles directory";
		std::cout << "\n                                                                   \n";
		std::cout << xml;

		Query<std::string> query_class;

		std::cout << "\n\n Requiremnts 7, 8 and 9													";
		std::cout << "\n  Querying the database now													";		//*----- Requirement 7 -----//
		std::cout << "\n  Querying is done using regular expressions								\n";
		std::cout << "\n----------------------------------------------------------------------------- \n";

		std::cout << "\n\n Value of cppproperties : Requirement 7a    \n";
		Keys seta = query_class.queryOne(db, "value", "CppProperties");
		db.showVect(seta);
		std::cout << "\n----------------------------------------------------------------------------- \n";
		std::cout << "\n\n Displaying children of TestExecutive : Requirement 7b      \n";
		Keys setb = query_class.queryOne(db, "children", "TestExecutive");
		db.showVect(setb);
		std::cout << "\n----------------------------------------------------------------------------- \n";
		std::cout << "\n\n Displaying records that have q in their Keys  >\n";
		Keys set1a = query_class.queryOne(db, db.keys(), "key", "q");
		db.showVect(set1a);
		std::cout << "\n----------------------------------------------------------------------------- \n";
		std::cout << "\n\n Displaying records with name as Query\n";
		std::vector<std::string> testVec;
		testVec.push_back("TestExecutive");
		testVec.push_back("QueryProject");
		testVec.push_back("NoSqlDb");

		Keys set1 = query_class.queryOne(db, testVec, "name", "Query");
		db.showVect(set1);
		std::cout << "\n----------------------------------------------------------------------------- \n";
		std::cout << "\n\n Displaying records with .h in their data \n";
		Keys set2 = query_class.queryData(db, db.keys(), "data", ".h");
		db.showVect(set2);
		std::cout << "\n----------------------------------------------------------------------------- \n";
		std::cout << "\n\n Record with category as test : requirement 7 e     >\n";
		Keys set3a = query_class.queryOne(db, db.keys(), "category", "test");
		db.showVect(set3a);
		std::cout << "\n----------------------------------------------------------------------------- \n";
		std::cout << "\n\n records with 4 in their name : requirement 7d    \n";
		Keys set3b = query_class.queryOne(db, db.keys(), "description", "This");
		db.showVect(set3b);
		std::cout << "\n----------------------------------------------------------------------------- \n";
		std::cout << "\n\n > Records created between Feb 7th 2017 and Feb 15th 2017 ----< Requirement 7(g)\n";
		Keys settime = query_class.queryTime(db, db.keys(), "2017-02-07 00:00:00", "2017-02-15 00:00:00");
		db.showVect(settime);
		std::cout << "\n----------------------------------------------------------------------------- \n";
		std::cout << "\n\n union of queries : requirement 9				";
		////------- Requirement 9 -union query -------//
		std::cout << "\n\n Records with q in their name and category as test					\n";
		Keys setunion = query_class.queryOne(query_class.queryData(db, db.keys(), "data", ".h"), query_class.queryOne(db, db.keys(), "item", "4"));
		db.showVect(setunion);
		std::cout << "\n----------------------------------------------------------------------------- \n";
		std::cout << "\n\n Demonstrating compound queries : Requirement 8       \n";
		//------ Requirement 8 Compound queries ----------//
		std::cout << "\n\n > Records with Category = test AND have .cpp in the Data fields\n";
		Keys set4 = query_class.queryOne(db, settime, "category", "Helper code");
		db.showVect(set4);
		std::cout << "\n----------------------------------------------------------------------------- \n";
	}
	catch(const std::exception& e) {
		std::cout << e.what();
	}
	getchar();
	return 0;
}