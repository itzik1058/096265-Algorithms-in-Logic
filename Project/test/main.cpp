#include "formula.h"
#include "varpool.h"
#include "cnf.h"
#include "card.h"
#include <assert.h>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;
using namespace edusat::formula;


void test_variable_pool() {
	cout << "test_variable_pool start" << endl;
	VariablePool<string> vpool;
	vpool.set_occupied(1);
	vpool.set_occupied(2);
	vector<Variable> occupied = { 4, 5, 6 };
	vpool.set_occupied(occupied);
	Variable test1 = vpool.variable("test1");
	Variable test2 = vpool.variable("test2");
	string label3 = vpool.label(3);
	string label7 = vpool.label(7);
	assert(test1 == 3);
	assert(test2 == 7);
	assert(label3 == "test1");
	assert(label7 == "test2");
	cout << "test_variable_pool end" << endl;
}

void test_cnf() {
	cout << "test_cnf start" << endl;
	CNF cnf;
	Clause clause = { 1, 2, -3 };
	vector<Clause> clauses;
	clauses.push_back({ 2, 1, 3 });
	clauses.push_back({ 3, -2, 1 });
	cnf.insert(clause);
	cnf.insert(clauses);
	cout << cnf;
	cout << "test_cnf end" << endl;
}


void test_formula() {
	cout << "test_formula start" << endl;
	VariablePool<string> vpool;
	Formula f(vpool, Operation::And, vpool.variable("1"), vpool.variable("2"));
	Formula f2 = ~f;
	Formula f3 = f & ~f;
	Formula f4(vpool, Operation::Not, vpool.variable("3"));
	Formula f5 = ~f3 | f4;
	CNF cnf;
	cnf.insert(f5.to_cnf());
	cout << cnf << endl;
	cout << "test_formula end" << endl;
}


void test_encoding() {
	cout << "test_encoding start" << endl;
	VariablePool<string> vpool;
	CNF cnf;
	cout << "2x-4y>=-2" << endl;
	vector<pair<Variable, int>> coef = { make_pair(vpool.variable("x"), 2), make_pair(vpool.variable("y"), -4) };
	cnf.insert(card::constraint(coef, vpool, -2, false, false));
	cout << cnf << endl;
	cnf.solve();
	cout << endl << endl << "adding constraint -x & y" << endl << endl << endl;
	Variable x = vpool.variable("x");
	Variable y = vpool.variable("y");
	cnf.insert({ -x });
	cnf.insert({ y });
	cnf.solve();
	cout << "test_encoding end" << endl;
}


int main(int argc, char** argv) {
	if (argc < 3) {
		cout << "run with test.exe use_bdd file_path" << endl;
		return 0;
	}
	bool use_bdd = string(argv[1]) == "true";
	string path = argv[2];
	VariablePool<string> vpool;
	CNF cnf;
	ifstream in(path);
	string line;
	while (getline(in, line)) {
		if (line[0] == '*')
			continue;
		vector<pair<Variable, int>> coef;
		int rhs = 0;
		istringstream iss(line);
		string token;
		while (iss >> token) {
			if (token == ";")
				break;
			if (token == ">=") {
				iss >> rhs;
				cnf.insert(card::constraint(coef, vpool, rhs, false, use_bdd));
			}
			else if (token == "=") {
				iss >> rhs;
				cnf.insert(card::constraint(coef, vpool, rhs, false, use_bdd));
				for (auto it = coef.begin(); it != coef.end(); it++)
					it->second *= -1;
				cnf.insert(card::constraint(coef, vpool, -rhs, false, use_bdd));
			}
			else {
				int c = stoi(token);
				string variable;
				iss >> variable;
				coef.push_back(make_pair(vpool.variable(variable), c));
			}
		}
	}
	ofstream out(path + ".cnf");
	out << cnf;
	cout << endl;
	return 0;
}
