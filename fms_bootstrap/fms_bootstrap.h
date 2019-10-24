// fms_bootstrap.h - Bootstrap piecewise constant forward curves.
// https://github.com/keithalewis/papers/blob/master/bootstrap.pdf
#pragma once
#include <limits>
#include <utility>
#include "../fms_sequence/fms_sequence.h"

namespace fms::bootstrap {

	template<class X>
	constexpr X NaN = std::numeric_limits<X>::quiet_NaN();

	// Extend curve having one cash flow past the end of the curve.
	// p = sum_{u_j < u} c_j D_j + cu Du exp(-f du) = pv + cu Du exp(-f du); 
	template<class P, class C, class T>
	inline C extend1(P p, P pv, C cu, P Du, T du)
	{
		return  - log((p - pv) / (cu*Du)) / du;
	}

	// Extend curve having two cash flows past the end of the curve
	// 0 = c0 D0 + c1 D1, Dj = D(u) exp(-f (uj - u))
	template<class C, class T>
	inline C extend2(C c0, T u0, C c1, T u1)
	{
		return  log(-c1/c0)/(u1 - u0);
	}

	// Extend curve for any price and instrument.
	// p = sum_{u_j <= t} c_j D_j + sum_{u_k > t} c_k D(t) exp(-f (u_k - t)) = pv + _pv
	template<class F, class P, class C, class T>
	inline std::pair<T, C> extend(const F& D, const T& t, const P& p, T u, C c)
	{
		auto u_ = filter([t](auto _u) { return _u <= t; }, u);
		auto _u = filter([t](auto _u) { return _u > t; }, u);
		
		auto pv = sum(c * apply(D, u_));

		// Number of cash flows before t.
		auto n_ = length(u_);
		// Number of cash flows past t.
		auto _n = length(_u);
		// Cash flows past t.
		auto _c = skip(n_, c);
		// Discount to end of curve.
		auto Dt = D(t);

		if (_n == 0) {
			return std::pair(NaN<decltype(*u)>, NaN<decltype(*c)>);
		}
		if (_n == 1) {
			return std::pair(*_u, extend1(p, pv, *_c, Dt, *_u - t));
		}
		if (_n == 2 and (p - pv) + 1 == 1) {
			auto u0 = *_u;
			auto u1 = *++_u;
			auto c0 = *_c;
			auto c1 = *++_c;

			return std::pair(u1, extend2(c0, u0, c1, u1));
		}
		/*
		C f = 0.01;
		// f will change on each call
		auto Du = [Dt,t,&f](auto u) { return Dt * exp(-f*(u - t)); };
		auto _pv = [](C f) { return sum(_c, apply(_D, _u)); };

		// Find root using secant method.
		while (fabs(-p + pv + _pv(f)) >= .00001) {
			m = a / b;
			f_ = f + m * (f - f_);
			std::swap(f, f_);
		}
		*/
		return std::pair<decltype(*u),decltype(*c)>(0, 0);

	}

}
