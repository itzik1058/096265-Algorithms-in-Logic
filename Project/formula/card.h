#pragma once

#include "formula.h"
#include "varpool.h"

namespace edusat {
	namespace formula {
		namespace card {
			template<class _Iter>
			std::vector<Clause> atleast(_Iter const& _First, _Iter const& _Last, Variable top_var = 0, unsigned bound = 1);
			template<class _Iter, typename T>
			std::vector<Clause> atleast(_Iter const& _First, _Iter const& _Last, VariablePool<T> const& vpool, unsigned bound = 1);
			template<class _Iter>
			std::vector<Clause> atmost(_Iter const& _First, _Iter const& _Last, Variable top_var = 0, unsigned bound = 1);
			template<class _Iter, typename T>
			std::vector<Clause> atmost(_Iter const& _First, _Iter const& _Last, VariablePool<T> const& vpool, unsigned bound = 1);

			template<class _Iter>
			std::vector<Clause> equals(_Iter const& _First, _Iter const& _Last, Variable top_var, unsigned bound) {
				std::vector<Clause> clauses;
				std::vector<Clause> atleast_clauses = atleast(_First, _Last, top_var, bound);
				std::vector<Clause> atmost_clauses = atleast(_First, _Last, top_var, bound);
				clauses.insert(clauses.end(), atleast_clauses.begin(), atleast_clauses.end());
				clauses.insert(clauses.end(), atmost_clauses.begin(), atmost_clauses.end());
			}

			template<class _Iter, typename T>
			std::vector<Clause> equals(_Iter const& _First, _Iter const& _Last, VariablePool<T> const& vpool, unsigned bound) {
				std::vector<Clause> clauses;
				std::vector<Clause> atleast_clauses = atleast(_First, _Last, vpool, bound);
				std::vector<Clause> atmost_clauses = atleast(_First, _Last, vpool, bound);
				clauses.insert(clauses.end(), atleast_clauses.begin(), atleast_clauses.end());
				clauses.insert(clauses.end(), atmost_clauses.begin(), atmost_clauses.end());
			}
			template<class _Iter, typename T>
			std::vector<Clause> atleast(_Iter const& _First, _Iter const& _Last, VariablePool<T> const& vpool, unsigned bound)
			{
				std::vector<Clause> clauses;
				Clause or_clause;
				for (_Iter it = _First; it != _Last; it++)
					or_clause.push_back(*it);
				clauses.push_back(or_clause);
				return clauses;
			}

			template<class _Iter, typename T>
			std::vector<Clause> atmost(_Iter const& _First, _Iter const& _Last, VariablePool<T> const& vpool, unsigned bound)
			{
				std::vector<Clause> clauses;
				for (_Iter it = _First; it != _Last; it++) {
					for (_Iter it2 = it + 1; it2 != _Last; it2++) {
						Clause clause;
						clause.push_back(-*it);
						clause.push_back(-*it2);
						clauses.push_back(clause);
					}
				}
				return clauses;
			}
		}
	}
}
