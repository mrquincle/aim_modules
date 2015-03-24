/**
 * @file Document.cpp
 * @brief Document for documents
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

#include <Document.h>
#include <iostream>

Document::Document() {
	N=0;
	identifier="";
}

int Document::count() {
	return N;
}

int Document::count(const std::string & term) {
	//std::cout << "For document \"" << identifier << "\" term \"" << term << "\" the count is " << content.count(term) << std::endl;
	if (content.count(term)) {
		return content[term];
	}
	return 0;
}
		
void Document::setId(std::string & identifier) { 
	this->identifier = identifier; 
}

std::string Document::getId() { 
	return identifier; 
}
		
void Document::add(std::string & term) {
	content[term]++;
	N++;
}

bool Document::get(int index, std::string &term) {
	std::map<std::string, int>::iterator iter;
	if (index >= unique()) return false;

	iter = content.begin();
	std::advance(iter, index);
	term = iter->first;
	return true;
}

int Document::unique() {
	return content.size();
}
