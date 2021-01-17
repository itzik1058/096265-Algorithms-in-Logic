#pragma once

#include "formula.h"
#include <ostream>

namespace edusat {
	namespace formula {
		class CNF {
		private:
			std::vector<Clause> clauses;

			size_t num_variables() const;
		public:
			void insert(Clause const&);
			template<class _Iter>
			void insert(_Iter const&);
			void clear();

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
