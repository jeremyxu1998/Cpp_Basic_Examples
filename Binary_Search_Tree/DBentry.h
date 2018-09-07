#ifndef _DBENTRY_H
#define _DBENTRY_H

#include <string>
using namespace std;

class DBentry {
private:
    string name;
    unsigned int IPaddress;
    bool active;

public:
    // The default constructor
    DBentry();
    DBentry (string _name, unsigned int _IPaddress, bool _active);

    // The destructor
     ~DBentry();	

    // Sets the domain name, which we will use as a key.
    void setName(string _name);

    // Sets the IPaddress data.
    void setIPaddress(unsigned int _IPaddress);
    
    // Sets whether or not this entry is active.
    void setActive (bool _active);

    // Returns the name.
    string getName() const;

    // Returns the IPaddress data.
    unsigned int getIPaddress() const;

    // Returns whether or not this entry is active.
    bool getActive() const;

    // Prints the entry in the format 
    // name : IPaddress : active  followed by newline
    // active is printed as a string (active or inactive)
    friend ostream& operator<< (ostream& out, const DBentry& rhs);
};

#endif
