#include "cnf.h"
#include <set>

using namespace std;

namespace edusat {
	namespace formula {

		size_t CNF::num_variables() const
		{
			set<Variable> variables;
			for (Clause const& c : clauses)
				variables.insert(c.begin(), c.end());
			return variables.size();
		}

		void CNF::insert(Clause const& clause)
		{
			clauses.push_back(clause);
		}

		ostream& operator<<(ostream& os, CNF const& cnf)
		{
			os << "p cnf " << cnf.num_variables() << " " << cnf.clauses.size() << endl;
			for (Clause const& c : cnf.clauses) {
				for (Variable const& v : c) {
					os << v << " ";
				}
				os << 0 << endl;
			}
			return os;
		}

	}
}