#include <iostream>
#include <string>
using namespace std;

#include "DBentry.h"

//Member functions that assist TreeDB to get or change data

DBentry::DBentry() {

}

DBentry::DBentry(string _name, unsigned int _IPaddress, bool _active) {
	name = _name;
	IPaddress = _IPaddress;
	active = _active;
}

DBentry::~DBentry() {

}

void DBentry::setName(string _name) {
	name = _name;
}

void DBentry::setIPaddress(unsigned int _IPaddress) {
	IPaddress = _IPaddress;
}

void DBentry::setActive(bool _active) {
	active = _active;
}

string DBentry::getName() const {
	return name;
}

unsigned int DBentry::getIPaddress() const {
	return IPaddress;
}

bool DBentry::getActive() const {
	return active;
}

ostream& operator<< (ostream& out, const DBentry& rhs) {
	string statusString;
	if (rhs.active) statusString = "active";
	else statusString = "inactive";
	out << rhs.name << " : " << rhs.IPaddress << " : " << statusString << endl;
	return (out);
}