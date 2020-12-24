#include "formula.h"
#include "varpool.h"
#include "cnf.h"
#include "card.h"
#include <iostream>
#include <string>


using namespace std;
using namespace edusat::formula;


int main() {
	VariablePool<string> vpool;
	vpool.blacklist(1);
	vpool.blacklist(2);
	vector<Variable> blacklist = { 4, 5, 6 };
	vpool.blacklist(blacklist.begin(), blacklist.end());
	cout << vpool.variable("test1") << endl;
	cout << vpool.variable("test2") << endl;
	cout << vpool.label(3) << endl;
	cout << vpool.label(7) << endl;
	return 0;
}
