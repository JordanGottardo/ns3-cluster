#ifndef KEYABLEVECTOR_H
#define KEYABLEVECTOR_H


#include "ns3/vector.h"
#include "ns3/object.h"
#include <iostream>
using namespace std;


namespace ns3 {

class Edge: public Object {

public:

	Edge(uint32_t source, uint32_t destination, uint32_t phase);

	uint32_t GetSource() const;

	uint32_t GetDestination() const;

	uint32_t GetPhase() const;

	void SetSource(uint32_t source);

	void SetDestination(uint32_t source);

	void SetPhase(uint32_t source);

private:
	uint32_t								m_source; //id of source
	uint32_t								m_destination; //id of destination
	uint32_t								m_phase; //number of hops of transmission
};

ostream& operator<< (std::ostream& stream, const Edge& matrix);
}

#endif
