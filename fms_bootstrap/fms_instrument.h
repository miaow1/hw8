// fms_instrument.h - Fixed income instrument
#pragma once

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
