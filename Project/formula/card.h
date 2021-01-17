#pragma once

#include "formula.h"
#include "varpool.h"
#include <vector>

namespace edusat {
	namespace formula {
		namespace card {
			template<class _Iter>
			std::vector<Clause> atleast(_Iter const& iterable, Variable top_var = 0, unsigned bound = 1);
			template<class _Iter, typename T>
			std::vector<Clause> atleast(_Iter const& iterable, VariablePool<T> const& vpool, unsigned bound = 1);
			template<class _Iter>
			std::vector<Clause> atmost(_Iter const& iterable, Variable top_var = 0, unsigned bound = 1);
			template<class _Iter, typename T>
			std::vector<Clause> atmost(_Iter const& iterable, VariablePool<T> const& vpool, unsigned bound = 1);

			template<class _Iter>
			std::vector<Clause> equals(_Iter const& iterable, Variable top_var, unsigned bound = 1) {
				std::vector<Clause> clauses;
				std::vector<Clause> atleast_clauses = atleast(iterable, top_var, bound);
				std::vector<Clause> atmost_clauses = atleast(iterable, top_var, bound);
				clauses.insert(clauses.end(), atleast_clauses.begin(), atleast_clauses.end());
				clauses.insert(clauses.end(), atmost_clauses.begin(), atmost_clauses.end());
				return clauses;
			}

			template<class _Iter, typename T>
			std::vector<Clause> equals(_Iter const& iterable, VariablePool<T> const& vpool, unsigned bound = 1) {
				std::vector<Clause> clauses;
				std::vector<Clause> atleast_clauses = atleast(iterable, vpool, bound);
				std::vector<Clause> atmost_clauses = atmost(iterable, vpool, bound);
				clauses.insert(clauses.end(), atleast_clauses.begin(), atleast_clauses.end());
				clauses.insert(clauses.end(), atmost_clauses.begin(), atmost_clauses.end());
				return clauses;
			}

			template<class _Iter, typename T>
			std::vector<Clause> atleast(_Iter const& iterable, VariablePool<T> const& vpool, unsigned bound)
			{
				std::vector<Clause> clauses;
				Clause or_clause;
				for (Variable const& v : iterable)
					or_clause.push_back(v);
				clauses.push_back(or_clause);
				return clauses;
			}

			template<class _Iter, typename T>
			std::vector<Clause> atmost(_Iter const& iterable, VariablePool<T> const& vpool, unsigned bound)
			{
				std::vector<Clause> clauses;
				for (auto&& it = iterable.begin(); it != iterable.end(); it++) {
					for (auto&& it2 = std::next(it); it2 != iterable.end(); it2++) {
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
