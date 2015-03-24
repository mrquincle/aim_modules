/**
 * @file Document.h
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

#include <map>
#include <string>

class Document {
	private:
		std::map<std::string, int> content;
		std::string identifier;
		// total number of words
		int N;
	public:
		Document();

		void setId(std::string & identifier);

		std::string getId();

		void add(std::string & term);

		/* Counts the number of times a word is encountered in the document
		 */
		int count(const std::string & term);

		/* Counts all words (N).
		 */
		int count();

		/* Counts all terms
		 */
		int unique();

		/* Get a specific term at a position
		 */
		bool get(int index, std::string &term);
};

