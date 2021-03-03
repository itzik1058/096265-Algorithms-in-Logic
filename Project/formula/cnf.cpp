#include "cnf.h"
#include <set>
#include <fstream>

using namespace std;

namespace edusat {
	namespace formula {

		size_t CNF::top_variable() const
		{
			Variable top_var = 0;
			for (Clause const& c : clauses)
				for (Variable const& v : c)
					top_var = std::max(top_var, abs(v));
			return top_var;
		}

		void CNF::insert(Clause const& clause)
		{
			clauses.push_back(clause);
		}

		void CNF::clear() {
			clauses.clear();
		}

		size_t CNF::num_clauses() const
		{
			return clauses.size();
		}

		void CNF::solve()
		{
			ofstream cnf("cnf.dimac");
			cnf << *this;
			cnf.close();
			system("edusat.exe -v 1 cnf.dimac");
		}

		ostream& operator<<(ostream& os, CNF const& cnf)
		{
			os << "p cnf " << cnf.top_variable() << " " << cnf.clauses.size() << endl;
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