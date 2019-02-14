/*
 * KeyableVector.cc
 *
 *  Created on: Feb 5, 2019
 *      Author: jordan
 */

#include "Edge.h"

namespace ns3 {


Edge::Edge(uint32_t source, uint32_t destination, uint32_t phase): m_source(source),
																   m_destination(destination),
																   m_phase(phase) {
}


uint32_t Edge::GetSource() const {
	return m_source;
}

uint32_t Edge::GetDestination() const {
	return m_destination;
}

uint32_t Edge::GetPhase() const {
	return m_phase;
}

void Edge::SetSource(uint32_t source) {
	m_source = source;
}

void Edge::SetDestination(uint32_t destination) {
	m_destination = destination;
}


void Edge::SetPhase(uint32_t phase) {
	m_phase = phase;
}


ostream& operator<< (std::ostream& stream, const Edge& edge) {
//	cout << "ostream Edge" << endl;
	stream << edge.GetSource() << "-" << edge.GetDestination() << "*" << edge.GetPhase();
	return stream;
}

}
