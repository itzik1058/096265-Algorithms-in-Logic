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
					bool sign = coef[size].first < 0;
					int hi_sum = sign ? sum : sum + coef[size].second;
					int lo_sum = sign ? sum + coef[size].second : sum;
					Formula<T> hi_result = make_bdd(vpool, coef, rhs, size, hi_sum, material_left, memo);
					Formula<T> lo_result = make_bdd(vpool, coef, rhs, size, lo_sum, material_left, memo);
					Variable v = abs(coef[size].first);
					Formula<T> selector(vpool, Operation::And, v, v);
					memo.insert(make_pair(key, formula::ITE(selector, hi_result, lo_result)));
				}
				return memo[key];
			}

			template<typename T>
			Formula<T> adder_tree(VariablePool<T>& vpool, std::vector<std::pair<Variable, int>> coef, int rhs) {
				std::vector<std::vector<Formula<T>>> buckets;
				int max_coef = 0;
				for (auto it = coef.begin(); it != coef.end(); it++)
					max_coef = std::max(max_coef, it->second);
				for (; max_coef != 0; max_coef >>= 1)
					buckets.push_back({});
				buckets.push_back({});
				for (auto it = coef.begin(); it != coef.end(); it++)
					for (size_t i = 0; i < buckets.size(); i++) {
						if (it->second & (1 << i))
							buckets[i].push_back(Formula<T>(vpool, Operation::And, it->first, it->first));
					}
				std::vector<Formula<T>> result;
				for (size_t i = 0; i < buckets.size(); i++) {
					while (buckets[i].size() >= 3) {
						Formula<T> sum = FA_sum(buckets[i][0], buckets[i][1], buckets[i][2]);
						Formula<T> carry = FA_carry(buckets[i][0], buckets[i][1], buckets[i][2]);
						buckets[i].erase(buckets[i].begin(), buckets[i].begin() + 3);
						buckets[i].push_back(sum);
						if (buckets.size() <= i + 1)
							buckets.push_back({});
						buckets[i + 1].push_back(carry);
					}
					if (buckets[i].size() == 2) {
						Formula<T> sum = HA_sum(buckets[i][0], buckets[i][1]);
						Formula<T> carry = HA_carry(buckets[i][0], buckets[i][1]);
						buckets[i].erase(buckets[i].begin(), buckets[i].begin() + 2);
						buckets[i].push_back(sum);
						if (buckets.size() <= i + 1)
							buckets.push_back({});
						buckets[i + 1].push_back(carry);
					}
					if (buckets[i].size() == 1) {
						result.push_back(buckets[i].back());
						buckets[i].pop_back();
					}
				}
				std::vector<Formula<T>> rhs_bits;
				for (; rhs != 0; rhs >>= 1)
					rhs_bits.push_back(Formula<T>(vpool, rhs & 1 ? Operation::True : Operation::False));
				while (result.size() < rhs_bits.size())
					result.push_back(Formula<T>(vpool, Operation::False));
				while (rhs_bits.size() < result.size())
					rhs_bits.push_back(Formula<T>(vpool, Operation::False));
				Formula<T> f(vpool, Operation::True);
				for (size_t i = 0; i < rhs_bits.size(); i++) {
					Formula<T> c(vpool, Operation::False);
					for (size_t j = i + 1; j < rhs_bits.size(); j++)
						c = c | HA_sum(rhs_bits[j], result[j]);
					c = c | ~rhs_bits[i] | result[i];
					f = f & c;
				}
				return f;
			}

			template<class _Container, typename T>
			std::vector<Clause> constraint(_Container coefficients, VariablePool<T>& vpool, int rhs,
				bool check_satisfiability = false, bool use_bdd = false) {
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
				if (!use_bdd)
					return adder_tree(vpool, coef, rhs).to_cnf();
				std::map<std::pair<int, int>, Formula<T>> memo;
				return make_bdd(vpool, coef, rhs, coef.size(), 0, coef_sum, memo).to_cnf();
			}
		}
	}
}
