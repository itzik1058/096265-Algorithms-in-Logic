#pragma once

#include "formuladef.h"
#include <ostream>

namespace edusat {
	namespace formula {
		class CNF {
		private:
			std::vector<Clause> clauses;

			size_t top_variable() const;
		public:
			void insert(Clause const&);
			template<class _Iter>
			void insert(_Iter const&);
			void clear();

			size_t num_clauses() const;

			void solve();

			friend std::ostream& operator<<(std::ostream&, CNF const&); // DIMACS
		};

		template<class _Iter>
		inline void CNF::insert(_Iter const& iterable)
		{
			for (Clause const& clause : iterable)
				insert(clause);
		}
	}
}
