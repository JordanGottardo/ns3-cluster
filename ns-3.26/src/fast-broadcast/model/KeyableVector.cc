/*
 * KeyableVector.cc
 *
 *  Created on: Feb 5, 2019
 *      Author: jordan
 */

#include "KeyableVector.h"

namespace ns3 {

KeyableVector::KeyableVector(Vector v): Vector(v) {

}

bool KeyableVector::operator <(const KeyableVector& v) const {
	if (x < v.x) {
		return true;
	}
	if (x == v.x) {
		if (y < v.y) {
			return true;
		}
		if (y == v.y) {
			return z < v.z;
		}
	}
	return false;
}

bool KeyableVector::operator ==(const KeyableVector& v) const {
	return x == v.x && y == v.y && z == v.z;
}
//
//bool KeyableVector::operator !=(const KeyableVector& v) const {
//	return !(this.operator ==(v));
//}

}
