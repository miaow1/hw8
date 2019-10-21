// fms_bootstrap.h - Bootstrap piecewise constant forward curves.
#pragma once
#include <limits>
#include <tuple>

namespace fms::bootstrap {

	template<class X>
	constexpr X NaN = std::numeric_limits<X>::quiet_NaN();

	// Extend curve having one cash flow past the end of the curve.
	template<class F, class C = double, class T = double>
	inline std::pair<C, T> extend1(const F& f, C p, C pm, C cm, T um)
	{
		auto [std::ignore, u] = f.back();

		return std::pair{ (log(f.discount(u)) / (p - pv)) / (um - u), um };
	}

	// extend2

	template<class F, class P, class C, class T>
	inline std::pair<C, T> extend(const F& f, const P& p, size_t m, const C* c, const T* u)
	{
		auto [_f, _u] = f.back(); // last forward rate and time

		auto i = 
	}

}
