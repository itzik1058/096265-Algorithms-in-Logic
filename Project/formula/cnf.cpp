#include "cnf.h"

void edusat::formula::CNF::insert(Clause const& clause)
{
	clauses.push_back(clause);
}
