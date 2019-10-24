// fms_instrument_sequence.h - Instruments are sequences of cash flows.
#pragma once
#include <compare>
#include "../fms_sequence/fms_sequence.h"

namespace fms::instrument {

	// Fixed cash flows at given times.
	template<class U, class C>
	class sequence {
		U u;
		C c;
	public:
		sequence(const U& u, const C& c)
			: u(u), c(c)
		{ }
		const auto operator<=>(const sequence&) const = default;
		operator bool() const
		{
			return u and c;
		}
		auto operator*() const
		{
			return std::pair(*u, *c);
		}
		sequence& operator++()
		{
			++u;
			++c;

			return *this;
		}
	};

}
