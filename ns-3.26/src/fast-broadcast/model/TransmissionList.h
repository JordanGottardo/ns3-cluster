#ifndef TRANSMISSIONLIST_H
#define TRANSMISSIONLIST_H

#include <iostream>
#include <vector>
#include <map>
#include "KeyableVector.h"

using namespace std;

namespace ns3 {

class TransmissionList {

public:
	TransmissionList() {
	}

	void AddEdge(KeyableVector source, KeyableVector destination);

	string ToString() const;


private:
	std::map<KeyableVector, vector<KeyableVector>> 							transmissions; //Map to identify outgoing transmission from node (ley) to all other nodes

};
}

#endif
