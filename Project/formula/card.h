#pragma once

#include "formula.h"
#include "varpool.h"
#include <list>
#include <vector>
#include <exception>
#include <algorithm>
#include <numeric>

namespace edusat {
	namespace formula {
		namespace card {
			template<class _Container, typename T>
			std::vector<Clause> constraint(_Container coefficients, VariablePool<T> const& vpool, int rhs,
				bool check_satisfiability = true) {
				if (coefficients.empty())
					throw std::exception("No coefficients are present");
				std::vector<Clause> clauses;
				std::vector<std::pair<Variable, int>> coef;
				// Positive coefficients
				for (std::pair<Variable, int> const& pair : coefficients) {
					if (pair.second == 0)
						throw std::exception("Coefficients cannot be zero");
					if (pair.second < 0) {
						coef.push_back(std::make_pair(-pair.first, -pair.second));
						rhs -= pair.second;
					}
					else
						coef.push_back(std::make_pair(pair.first, pair.second));
				}
				if (rhs <= 0) // No constraint
					return clauses;
				// Trim coefficients
				int coef_sum = 0;
				for (auto it = coef.begin(); it != coef.end(); it++)
					coef_sum += it->second = std::min(it->second, rhs);
				if (check_satisfiability && coef_sum < rhs)
					throw std::exception("Unsatisfiable constraint");
				// Divide by greatest common divider
				std::size_t gcd = 0;
				for (auto const& p : coef)
					gcd = gcd ? std::gcd(gcd, p.second) : p.second;
				for (auto it = coef.begin(); it != coef.end(); it++)
					it->second /= gcd;
				rhs = (rhs - 1) / gcd + 1;
				// Sort by coefficient
				std::sort(coef.begin(), coef.end(),
					[](const std::pair<Variable, int>& x, const std::pair<Variable, int>& y)
					{ return x.second < y.second; });
				return clauses;
			}

			template<class _Container, typename T>
			std::vector<Clause> atleast(_Container const& variables, VariablePool<T> const& vpool, unsigned bound = 1) {
				std::vector<Clause> clauses;
				Clause or_clause;
				for (Variable const& v : variables)
					or_clause.push_back(v);
				clauses.push_back(or_clause);
				return clauses;
			}

			template<class _Container, typename T>
			std::vector<Clause> atmost(_Container const& variables, VariablePool<T> const& vpool, unsigned bound = 1) {

				std::vector<Clause> clauses;
				for (auto&& it = variables.begin(); it != variables.end(); it++) {
					for (auto&& it2 = std::next(it); it2 != variables.end(); it2++) {
						Clause clause;
						clause.push_back(-*it);
						clause.push_back(-*it2);
						clauses.push_back(clause);
					}
				}
				return clauses;
			}

			template<class _Container, typename T>
			std::vector<Clause> equals(_Container const& variables, VariablePool<T> const& vpool, unsigned bound = 1) {
				std::vector<Clause> clauses;
				std::vector<Clause> atleast_clauses = atleast(variables, vpool, bound);
				std::vector<Clause> atmost_clauses = atmost(variables, vpool, bound);
				clauses.insert(clauses.end(), atleast_clauses.begin(), atleast_clauses.end());
				clauses.insert(clauses.end(), atmost_clauses.begin(), atmost_clauses.end());
				return clauses;
			}

			Variable buildBDD();
		}
	}
}
