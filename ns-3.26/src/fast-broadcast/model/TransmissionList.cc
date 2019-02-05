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
//		TODO
	}
	transmissions.at(source).push_back(destination);
}

void TransmissionList::PrintTransmission() const {
//	for(const map<KeyableVector, KeyableVector>::iterator it = transmissions.begin(); it != transmissions.end(); ++it) {
//			cout << it->first << " " << it->second << endl;
//	}
}

}
