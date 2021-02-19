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
			template<typename T>
			Formula<T> make_bdd(VariablePool<T>& vpool, std::vector<std::pair<Variable, int>> coef, int rhs, int size, int sum, int material_left, std::map<std::pair<int, int>, Formula<T>> memo) {
				if (sum >= rhs)
					return Formula<T>(vpool, Operation::True);
				if (sum + material_left < rhs)
					return Formula<T>(vpool, Operation::False);
				std::pair<int, int> key = std::make_pair(size, sum);
				if (memo.find(key) == memo.end()) {
					size--;
					material_left -= coef[size].second;
					bool sign = coef[size].first > 0;
					int hi_sum = sign ? sum + coef[size].second : sum;
					int lo_sum = sign ? sum : sum + coef[size].second;
					Formula<T> hi_result = make_bdd(vpool, coef, rhs, size, hi_sum, material_left, memo);
					Formula<T> lo_result = make_bdd(vpool, coef, rhs, size, lo_sum, material_left, memo);
					Variable v = abs(coef[size].first);
					Formula<T> selector(vpool, Operation::And, v, v);
					memo.insert(make_pair(key, formula::ITE(selector, hi_result, lo_result)));
				}
				return memo[key];
			}

			template<class _Container, typename T>
			std::vector<Clause> constraint(_Container coefficients, VariablePool<T>& vpool, int rhs,
				bool check_satisfiability = false, bool use_bdd = true) {
				if (coefficients.empty())
					throw std::exception("No coefficients are present");
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
					return {};
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
				coef_sum = 0;
				for (auto it = coef.begin(); it != coef.end(); it++)
					coef_sum += it->second;
				std::map<std::pair<int, int>, Formula<T>> memo;
				return make_bdd(vpool, coef, rhs, coef.size(), 0, coef_sum, memo).to_cnf();
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
		}
	}
}
