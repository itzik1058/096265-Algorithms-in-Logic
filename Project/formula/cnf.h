#pragma once

#include "formula.h"

namespace edusat {
	namespace formula {
		class CNF {
		private:
			std::vector<Clause> clauses;
		public:
			void insert(Clause const&);
			template<class _Iter>
			void insert(_Iter _First, _Iter _Last);
		};

		template<class _Iter>
		inline void CNF::insert(_Iter _First, _Iter _Last)
		{
			for (; _First < _Last; _First++)
				insert(*_First);
		}
	}
}
