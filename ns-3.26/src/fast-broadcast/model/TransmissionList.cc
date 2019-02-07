/*
 * TransmissionList.cc
 *
 *  Created on: Feb 5, 2019
 *      Author: jordan
 */

#include "TransmissionList.h"

namespace ns3 {

void TransmissionList::AddEdge(KeyableVector source, KeyableVector destination) {
	if (transmissions.count(source) == 0) {
		transmissions.insert(pair<KeyableVector, vector<KeyableVector>>(source, vector<KeyableVector>()));
	}
	transmissions.at(source).push_back(destination);
}

string TransmissionList::ToString() const {
	stringstream ss;
	for(auto it = transmissions.begin(); it != transmissions.end(); ++it) {
//		copies the key
		ss << it->first<<"{";
		vector<KeyableVector> v = it->second;
		for (auto kv: v) {
			ss << kv << ";";
		}
		ss << "}";
	}
	return ss.str();
}

}
