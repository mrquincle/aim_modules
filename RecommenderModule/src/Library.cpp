/**
 * @file Library.cpp
 * @brief Library for documents
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

#include <Library.h>
#include <iostream>
#include <algorithm>

void Library::parseDocument(const std::string & raw, Document & document) {
	if (raw == "") {
		std::cerr << "String is empty!" << std::endl;
		return;
	}

	// transform entire text to lowercase
	std::string lower;
	lower.resize(raw.size());
	std::transform(raw.begin(), raw.end(), lower.begin(), ::tolower);

   std::string delim = " ,.\"'-;:";
	std::cout << "Document is " << raw << std::endl;
	size_t start = 0U;
	size_t end = lower.find_first_of(delim);

	// get identifier as first token, separately
	std::string identifier = raw.substr(start, end - start);
	std::cout << "Set identifier to: " << identifier << std::endl;
	document.setId(identifier);
	if (end != std::string::npos) {
		start = end + 1; //delim.length();
		end = lower.find_first_of(delim, start);
	} else {
		std::cerr << "There should be more text than only an identifier" << std::endl;
		return;
	}

	// add all subsequent tokens to the map
	std::string token;
	while ( end != std::string::npos) {
		token = lower.substr(start, end - start);
		document.add(token);
		std::cout << "Add token: " << token << std::endl;
		start = end + 1; //delim.length();
		end = lower.find_first_of(delim, start);
	}
	token = lower.substr(start, end - start);
	document.add(token);
	std::cout << "Add token: " << token << std::endl;

	// some debugging info
	std::cout << "Parsed a document with " << document.count() << " terms" << std::endl;
}

void Library::add(Document &doc) {
	documents.push_back(&doc);
}

Document* Library::get(const std::string &docId) {
	for (int i = 0; i < documents.size(); i++) {
		std::string id = documents[i]->getId();
		if (!id.compare(docId)) {
			return documents[i];
		}
	}
	return NULL;
}

int Library::count() {
	return documents.size();
}

int Library::count(const std::string &term) {
	int result = 0;
	for (int i = 0; i < documents.size(); i++) {
		if (documents[i]->count(term) > 0) result++;
	}
	return result;
}

