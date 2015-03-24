/**
 * @file Library.h
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

#include <vector>
#include <string>
#include <Document.h>

class Library {
		std::vector<Document*> documents;

	public: 
		/* Parse a document
		 *
		 * The parser assumes that the first word is the document id. And is separated by spaces. All the next words
		 * are considered terms of a dictionary. Currently we do not support other delimiters, only spaces.
		 */
		void parseDocument(const std::string & raw, Document & document);

		/** Add document to library
		 */
		void add(Document &doc);

		/** Get document by ID
		 */
		Document *get(const std::string & docId);

		/** Number of documents
		 */
		int count();

		/** Number of documents with given term
		 */
		int count(const std::string &term);
};
