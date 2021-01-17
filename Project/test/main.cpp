#include "formula.h"
#include "varpool.h"
#include "cnf.h"
#include "card.h"
#include <assert.h>
#include <string>
#include <set>
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


void test_encoding() {
	cout << "test_encoding start" << endl;
	VariablePool<string> vpool;
	set<Variable> variables = { vpool.variable("v1"), vpool.variable("v2"), vpool.variable("v3") };
	cout << "at most 1" << endl;
	CNF cnf;
	cnf.insert(card::atmost(variables, vpool));
	cout << cnf << endl;
	cout << "at least 1" << endl;
	cnf.clear();
	cnf.insert(card::atleast(variables, vpool));
	cout << cnf << endl;
	cout << "equals 1" << endl;
	cnf.clear();
	cnf.insert(card::equals(variables, vpool));
	cout << cnf << endl;
	cout << "test_encoding end" << endl;
}


int main() {
	test_variable_pool();
	cout << endl;
	test_cnf();
	cout << endl;
	test_encoding();
	cout << endl;
	return 0;
}
