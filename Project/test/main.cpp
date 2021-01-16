#include "formula.h"
#include "varpool.h"
#include "cnf.h"
#include "card.h"
#include <assert.h>
#include <string>
#include <iostream>


using namespace std;
using namespace edusat::formula;


void test_variable_pool() {
	VariablePool<string> vpool;
	vpool.set_occupied(1);
	vpool.set_occupied(2);
	vector<Variable> occupied = { 4, 5, 6 };
	vpool.set_occupied(occupied.begin(), occupied.end());
	Variable test1 = vpool.variable("test1");
	Variable test2 = vpool.variable("test2");
	string label3 = vpool.label(3);
	string label7 = vpool.label(7);
	assert(test1 == 3);
	assert(test2 == 7);
	assert(label3 == "test1");
	assert(label7 == "test2");
	cout << "VariablePool tests success" << endl;
}

void test_cnf() {
	CNF cnf;
	Clause clause = { 1, 2, -3 };
	vector<Clause> clauses;
	clauses.push_back({ 2, 1, 3 });
	clauses.push_back({ 3, -2, 1 });
	cnf.insert(clause);
	cnf.insert(clauses);
	//cout << cnf;
	cout << "CNF tests success" << endl;
}


int main() {
	test_variable_pool();
	test_cnf();
	return 0;
}
