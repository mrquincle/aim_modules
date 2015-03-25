/**
 * @file RecommenderModuleExt.cpp
 * @brief RecommenderModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "COMMIT P3". 
 * This software is published under the LGPLv3 license.
 *
 * Copyright © 2015 Anne van Rossum <anne@dobots.nl>
 *
 * @author                   Anne van Rossum
 * @date                     Mar 23, 2015
 * @organisation             Distributed Organisms B.V. (DoBots)
 * @project                  COMMIT P3 / Sensei
 */

#include <RecommenderModuleExt.h>
#include <math.h>
#include <sstream>
#include <unistd.h>
#include <iostream>

using namespace rur;

//! Replace with your own code
RecommenderModuleExt::RecommenderModuleExt() {

}

//! Replace with your own code
RecommenderModuleExt::~RecommenderModuleExt() {

}

//! Replace with your own code
void RecommenderModuleExt::Tick() {
	// now document to be added to the corpus
	std::string *str;
  	str = readDocument();
	if (str) {
		if (!str->compare("<EOF>")) {
			//std::cout << "Skip EOF symbols (from telnet session)" << std::endl;
			return;
		}
		Document &doc = *new Document();
		library.parseDocument(*str, doc);	
		library.add(doc);
	}

	// query for a term
	str = NULL; // free / delete(?)
	str = readTerm();
	if (str) {
		if (!str->compare("<EOF>")) {
			//std::cout << "Skip EOF symbols (from telnet session)" << std::endl;
			return;
		}
		// first word should be the document under consideration, the second word the term under consideration
		// we reuse our parser for documents
		Document tmpdoc;
		library.parseDocument(*str, tmpdoc);	
		std::string docId = tmpdoc.getId();
		std::string term;
		tmpdoc.get(0, term);
		int df = library.count(term);
		int N = library.count();
		Document &doc = *library.get(docId);
		int tf = doc.count(term);
		std::cout << "Number of documents that contain the term \"" << term << "\": " << df << std::endl;
		std::cout << "Number of documents: " << N << std::endl;
		std::cout << "Number of times term \"" << term << "\" occurs in document \"" << docId << "\": " << tf << std::endl;
		
		double ltf = 1.0 + log(tf);
		double lidf = log ( 1 + N / df);
		std::cout << "tf x idf = " << ltf << " x " << lidf << std::endl;
		
		double prob = ltf * lidf;

		Result *res = new Result();
		res->docId = docId;
		res->term = term;
		std::stringstream s;
		s << prob;
		res->probability = s.str();
		results.push_back(res);
	}

	if (results.size()) {
		Result *res = results.back();
		std::string output = res->docId + ' ' + res->term + ' ' + res->probability;
		std::cout << "Send output: " << output << std::endl;
		delete res;
		results.pop_back();
		writeRecommendation(output);
	}

	usleep(100);
}

//! Replace with your own code
bool RecommenderModuleExt::Stop() {
	return false;
}

