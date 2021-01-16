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

			friend std::ostream& operator<<(std::ostream&, CNF const&); // DIMACS
		};

		template<class _Iter>
		inline void CNF::insert(_Iter const& _Iter)
		{
			for (Clause const& clause : _Iter)
				insert(clause);
		}
	}
}
