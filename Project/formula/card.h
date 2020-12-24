#pragma once

#include "formula.h"
#include "varpool.h"

namespace edusat {
	namespace formula {
		namespace card {
			template<class _Iter>
			std::vector<Clause> atleast(_Iter const& _First, _Iter const& _Last, Variable top_var = 0, unsigned bound = 1);
			template<class _Iter, typename T>
			std::vector<Clause> atleast(_Iter const& _First, _Iter const& _Last, VariablePool<T> const&, unsigned bound = 1);
			template<class _Iter>
			std::vector<Clause> atmost(_Iter const& _First, _Iter const& _Last, Variable top_var = 0, unsigned bound = 1);
			template<class _Iter, typename T>
			std::vector<Clause> atmost(_Iter const& _First, _Iter const& _Last, VariablePool<T> const&, unsigned bound = 1);
			template<class _Iter>
			std::vector<Clause> equals(_Iter const& _First, _Iter const& _Last, Variable top_var = 0, unsigned bound = 1);
			template<class _Iter, typename T>
			std::vector<Clause> equals(_Iter const& _First, _Iter const& _Last, VariablePool<T> const&, unsigned bound = 1);
		}
	}
}
