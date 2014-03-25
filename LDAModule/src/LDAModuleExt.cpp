/**
 * @file LDAModuleExt.cpp
 * @brief LDAModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2014 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Mar 24, 2014
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <LDAModuleExt.h>
#include <Random.h>

using namespace rur;

/**
 * The number of to-be-expected clusters is set to K=12.
 */
LDAModuleExt::LDAModuleExt(): K(12) {
	term_count_per_topic.resize(K);
	term_count_per_doc.resize(K);

	sample_topics.resize(K);

	alpha = 0.1;

	beta = 0.1;

	W = 0;
}

//! Replace with your own code
LDAModuleExt::~LDAModuleExt() {

}

/**
 * 
 */
void LDAModuleExt::Tick() {
	long_seq *sample = readSample();
	
}

void LDAModuleExt::Count(long_seq & sample) {
	static const int number_of_elements = 2;
	if (sample[0] != AIM_PROTOCOL_VERSION) return;
	if (sample[1] != number_of_elements) return;
	if (sample[2] != AIM_TYPE_SCALAR) return;
	if (sample[3] != AIM_TYPE_SCALAR) return;
	int term = sample[4];
	int document = sample[5];

	term_doc_table.push(term, document);
	W++;
}


void LDAModuleExt::Gibbs() {
	// create two tables from term_doc_table
	for (int i = 0; i < term_doc_table.size(); ++i) {
		int topic = random_value(0, K);

		tuple<int> term_doc = term_doc_table[i];
		int term = term_doc.elem0;
		int document = term_doc.elem1;

		term_topic_table.push(term, topic);
		doc_topic_table.push(document, topic);

		term_count_per_topic[topic]++;
		term_count_per_doc[document]++;
	}

	// get the corresponding matrices
	// TODO: check if copy assignment is working properly
	nd_array<int> term_topic_matrix = term_topic_table.matrix();
	nd_array<int> doc_topic_matrix = doc_topic_table.matrix();

	// we go again through term + doc table, but we will access counts in above matrices
	for (int i = 0; i < term_doc_table.size(); ++i) {
		tuple<int> term_doc = term_doc_table[i];
		int term = term_doc.elem0;
		int doc = term_doc.elem1;

		// get also current topic assignment (most convenient from table)
		tuple<int> term_topic = term_topic_table[i];
		int topic = term_topic.elem1;

		// decrement counts in matrix
		term_topic_matrix.add(term, topic, -1);
		doc_topic_matrix.add(doc, topic, -1);

		term_count_per_topic[topic]--;
		//term_count_per_doc[document]--;  // fixed

		// N_0: number of instances of word w assigned to topic j, not including current word
		// N_1: total number of words assigned to topic j, not including current word
		// N_2: number of words in document d_i assigned to topic j, not including current word
		// N_3: total number of words in document d_i, not including current word

		int N_3 = term_count_per_doc[doc] - 1;
		
		for (int k = 0; k < K; ++k) {
			int N_0 = term_topic_matrix.get(term, k);
			int N_1 = term_count_per_topic[k];
			int N_2 = doc_topic_matrix.get(doc, k);

			sample_topics[k] = ((N_0 + beta) * (N_2 + alpha)) / ((N_1 + W*beta ) * (N_3 + K*alpha));
		}
		int index_max = *std::max_element(sample_topics.begin(), sample_topics.end());
		topic = index_max;

		// add to matrices again
		term_topic_matrix.add(term, topic, +1);
		doc_topic_matrix.add(doc, topic, +1);

		term_count_per_topic[topic]++;
		//term_count_per_doc[document]++;
	}
}


//! Replace with your own code
bool LDAModuleExt::Stop() {
	return false;
}

