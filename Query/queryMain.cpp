
//////////////////////////////////////////////////////////////////////////////
//  queryMain.cpp -	this file implements the class declared in its associated// 
//                 header and its associated functionality					 //
//									     									//
//  ver 1.0																	//
//																			//
//  Language:      C++														//
//  Platform:      2016 15" MacBook pro running Windows 10 EDU via Bootcamp //
//  Application:   NoSql in memory database Project 1 CSR687				//
//  Author:        Madhusudhan Bengaluru Ramanujam SUID:914198627			//
//////////////////////////////////////////////////////////////////////////////


#include "../nosqldb/NoSqlDb.h"
#include "QueryHeader.h"
using key1 =std::string;
using keys1 = std::vector<std::string>;


int main() {
	
	NoSqlDb<std::string> db("../xmlfile/teststub.xml");
	db.fromXML("../xmlfile/teststub.xml");
	time_t current = time(&current);
	db.addingRecord("elem 10", "elem 10", " test", " this is element 10", current, { "elem 1" , " elem 4" }, "data");
	db.addingRecord("elem 11", "elem 11", " category", " this is element 11", current, { "elem 1" , " elem 4" }, "data");
	
	keys1 showKeys = db.keys();
	for (key1 k : showKeys) {
		std::cout << "\n  " << k << ":";
		std::cout << db.value(k).show();
	}

	Query<std::string> queriesClass; 

	std::cout << "Value of element 3 \n"; 

	keys1 qone = queriesClass.queryOne(db, "value", "elem3"); 

	for (key1 k : qone) {
		std::cout << " \n" << k << ":";
		std::cout << db.value(k).show();
	}

	std::cout << "\n\n > Children of elem 10\n";
	keys1 qtwo = queriesClass.queryOne(db, "children", "elem 10");
	for (key1 k : qtwo) {
		std::cout << " \n" << k ;
		//std::cout << db.value(k).show();
	}

	std::cout << "\n\n > Records with elem in their Keys\n";
	keys1 qthree = queriesClass.queryOne(db, db.keys(), "key", "elem");

	for (key1 k : qthree) {
		std::cout << " \n" << k << ":";
		std::cout << db.value(k).show();
	}

	std::cout << "\n\n > Records with Category as test within 2,3 and 4 key elements\n";
	keys1 qfour = queriesClass.queryOne(db, { "elem2", "elem3", "elem4","elem 10" }, "category", "test");
	
	for (key1 k : qfour) {
		std::cout << " \n" << k << ":";
		std::cout << db.value(k).show();
	}

	getchar();

}
