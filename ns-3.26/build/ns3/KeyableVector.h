#ifndef KEYABLEVECTOR_H
#define KEYABLEVECTOR_H


#include "ns3/vector.h"



namespace ns3 {

class KeyableVector: public Vector {

public:
	KeyableVector(Vector v);
	bool operator <(const KeyableVector& v) const;
	bool operator ==(const KeyableVector& v) const;
//	bool operator !=(const KeyableVector& v) const;

};
}

#endif
