#pragma once

#include "varpool.h"
#include <map>
#include <tuple>

namespace edusat {
	namespace formula {
		enum class Operation {Not, And, Or, True, False};

		template<typename T>
		class Formula {
		private:
			VariablePool<T>* vpool;
			Variable root_var;
			std::map<Variable, std::tuple<Operation, Variable, Variable>> data;
		public:
			Formula() : vpool(nullptr) {}

			Formula(VariablePool<T>& vpool, Operation operation, Variable var1 = 0, Variable var2 = 0) : vpool(&vpool) {
				root_var = vpool.reserve();
				data[root_var] = std::make_tuple(operation, var1, var2);
			}

			Formula(VariablePool<T>& vpool) : vpool(&vpool), root_var(0) {}

			Formula(Formula const& other) : vpool(other.vpool), root_var(other.root_var), data(other.data) {}

			Operation root_operation() const {
				return std::get<0>(data.find(root_var)->second);
			}

			Formula<T> clone() const {
				Formula<T> f(*vpool);
				f.root_var = root_var;
				f.data = data;
				return f;
			}

			Formula<T> operator~() const {
				Formula<T> f(*vpool);
				f.data = data;
				if (root_operation() == Operation::True) {
					f.root_var = root_var;
					f.data[root_var] = std::make_tuple(Operation::False, 0, 0);
					return f;
				}
				if (root_operation() == Operation::False) {
					f.root_var = root_var;
					f.data[root_var] = std::make_tuple(Operation::True, 0, 0);
					return f;
				}
				f.root_var = vpool->reserve();
				f.data[f.root_var] = std::make_tuple(Operation::Not, root_var, 0);
				return f;
			}

			Formula<T> operator&(Formula<T> const& other) const {
				if (root_operation() == Operation::True)
					return other.clone();
				if (other.root_operation() == Operation::True)
					return clone();
				if (root_operation() == Operation::False)
					return clone();
				if (other.root_operation() == Operation::False)
					return other.clone();
				Formula<T> f(*vpool);
				f.data = data;
				f.data.insert(other.data.begin(), other.data.end());
				f.root_var = vpool->reserve();
				f.data[f.root_var] = std::make_tuple(Operation::And, root_var, other.root_var);
				return f;
			}

			Formula<T> operator|(Formula<T> const& other) const {
				if (root_operation() == Operation::True)
					return clone();
				if (other.root_operation() == Operation::True)
					return other.clone();
				if (root_operation() == Operation::False)
					return other.clone();
				if (other.root_operation() == Operation::False)
					return clone();
				Formula<T> f(*vpool);
				f.data = data;
				f.data.insert(other.data.begin(), other.data.end());
				f.root_var = vpool->reserve();
				f.data[f.root_var] = std::make_tuple(Operation::Or, root_var, other.root_var);
				return f;
			}

			std::vector<Clause> to_cnf() const {
				std::vector<Clause> clauses;
				clauses.push_back({ root_var });
				for (const auto& c : data) {
					std::tuple<Operation, Variable, Variable> x = c.second;
					if (std::get<0>(x) == Operation::True)
						clauses.push_back({ c.first });
					else if (std::get<0>(x) == Operation::False)
						clauses.push_back({ -c.first });
					else if (std::get<0>(x) == Operation::Not) {
						clauses.push_back({ -std::get<1>(x), -c.first });
						clauses.push_back({ std::get<1>(x), c.first });
					}
					else if (std::get<0>(x) == Operation::And) {
						clauses.push_back({ -std::get<1>(x), -std::get<2>(x), c.first });
						clauses.push_back({ std::get<1>(x), -c.first });
						clauses.push_back({ std::get<2>(x), -c.first });
					}
					else if (std::get<0>(x) == Operation::Or) {
						clauses.push_back({ std::get<1>(x), std::get<2>(x), -c.first });
						clauses.push_back({ -std::get<1>(x), c.first });
						clauses.push_back({ -std::get<2>(x), c.first });
					}
				}
				return clauses;
			}
		};

		template<typename T>
		Formula<T> ITE(Formula<T> const& s, Formula<T> const& t, Formula<T> const& f) {
			return (s & t) | (~s & f);
		}

		template<typename T>
		Formula<T> FA_sum(Formula<T> const& a, Formula<T> const& b, Formula<T> const& c) {
			return (~a & ~b & ~c) | (~a & b & c) | (a & ~b & c) | (a & b & ~c);
		}

		template<typename T>
		Formula<T> FA_carry(Formula<T> const& a, Formula<T> const& b, Formula<T> const& c) {
			return (b & c) | (a & c) | (a & b);
		}
	}
}
