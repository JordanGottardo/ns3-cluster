/*
 * prova.cc
 *
 *  Created on: Mar 1, 2019
 *      Author: jordan
 */

#include <iostream>
#include<vector>
using namespace std;



int main (int argc, char *argv[])
{

	vector<bool> v;
	cout << sizeof(v) << endl;;
	for (int i = 0; i < 800; i++) {
		v.push_back(1);
	}

}
