// fms_sequence_list.h - Sequence from an initialzier list
#pragma once
#include <initializer_list>
#include <vector>

namespace fms::sequence {

	template<class X>
	class list {
		size_t n;
		std::vector<X> x;
	public:
		list(const std::initializer_list<X>& x)
			: n(0), x(x)
		{ }
		operator bool() const
		{
			return n < x.size();
		}
		X operator*() const
		{
			return x[n];
		}
		list& operator++()
		{
			if (n < x.size()) {
				++n;
			}

			return *this;
		}
	};

}
