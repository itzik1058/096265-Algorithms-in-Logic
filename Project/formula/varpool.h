#pragma once

#include "formuladef.h"
#include <set>
#include <map>

namespace edusat {
	namespace formula {
		template<typename T>
		class VariablePool {
		private:
			Variable top_var; //Top variable used
			std::set<Variable> occupied; //Occupied variables
			std::map<T, Variable> l2v; //Label to variable
			std::map<Variable, T> v2l; //Variable to label
		public:
			explicit VariablePool(Variable = 0);
		private:
			Variable next_var() const;
		public:
			T const& label(Variable const&) const;
			Variable variable(T);
			void set_occupied(Variable);
			template<class _Iter>
			void set_occupied(_Iter const&);
			Variable reserve();
		};

		template<typename T>
		inline VariablePool<T>::VariablePool(Variable top_var) : top_var(top_var) {}

		template<typename T>
		inline Variable VariablePool<T>::next_var() const
		{
			Variable var = top_var + 1;
			while (occupied.find(var) != occupied.end())
				var++; // Increment until not blacklisted
			return var;
		}

		template<typename T>
		inline T const& VariablePool<T>::label(Variable const& variable) const
		{
			return v2l.at(variable);
		}

		template<typename T>
		inline Variable VariablePool<T>::variable(T label)
		{
			Variable var = next_var();
			std::pair<std::map<T, Variable>::iterator, bool> result = l2v.insert(make_pair(label, var));
			if (result.second) {
				v2l[var] = label;
				top_var = var;
			}
			return result.first->second;
		}

		template<typename T>
		inline void VariablePool<T>::set_occupied(Variable variable)
		{
			occupied.insert(variable);
		}

		template<typename T>
		inline Variable VariablePool<T>::reserve()
		{
			return top_var = next_var();
		}

		template<typename T>
		template<class _Iter>
		inline void VariablePool<T>::set_occupied(_Iter const& iterable)
		{
			for (Variable const& v : iterable)
				set_occupied(v);
		}
	}
}
