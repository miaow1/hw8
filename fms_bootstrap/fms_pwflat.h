// fms_pwflat.h - Piecewise flat forward curves.
#pragma once
#include <cmath>
#include <limits>
#include <type_traits>
#include <utility>
#include "../fms_sequence/fms_sequence_traits.h"

/*
	A piecewise flat curve is determined by points (t[i], f[i]), 0 <= i < n, and an extrapolation value _f.

	f(t) = f[i] if t[i-1] < t <= t[i];
		 = _f if t > t[n-1];
		 = undefined if t < 0.
	
	Note f is continuous from the left and f(t[i]) = f[i].

	|                                   _f
	|        f[1]             f[n-1] o--------
	| f[0] o------          o--------x
	x------x      ... ------x
	|
	0-----t[0]--- ... ---t[n-2]---t[n-1]
*/

// Type of *S
/*
template<class S>
using value_type = std::invoke_result_t<decltype(&S::operator*), S>;

template<class... S>
using common_value_type = std::common_type_t<value_type<S>...>;
*/

using fms::sequence::value_type;
using fms::sequence::common_value_type;

namespace fms::pwflat {

	// Shorthand for Not a Number.
	template<class X>
	constexpr X NaN = std::numeric_limits<X>::quiet_NaN();

	// Value of the pwflat forward curve at u given sequences for points determining the curve.
	template<class T, class F>
//	inline value_type<F> value(const value_type<T>& u, T t, F f, const value_type<F> _f = NaN<value_type<F>>)
	inline double value(const double& u, T t, F f, const double _f = NaN<double>)
	{
		if (u < 0) {
			return NaN<double>; //!!! value_type<F >> ;
		}

		while (t and *t < u) {
			++t;
			++f;
		}

		return f ? *f : _f;
	}

	// Integral of the pwflat forward from 0 to u given sequences for points determining the curve.
	template<class T, class F>
//	inline auto integral(const value_type<T>& u, T t, F f, const decltype(*f)& _f = NaN<decltype(*f)>)
	inline double integral(const double& u, T t, F f, const double& _f = NaN<double>)
	{
		//common_value_type<T,F> I = 0;
		double I = 0;
		//!!!value_type<T> t_ = 0;
		double t_ = 0;

		if (u < 0) {
			return NaN<double>; //!!! value_type<F >> ;
		}

		while (t and *t < u) {
			I += (*f) * (*t - t_);
			t_ = *t;
			++t;
			++f;
		}

		return I + (f ? *f : _f)*(u - t_);
	}
	
	template<class T, class F>
	class forward {
		//using _T = value_type<T>;
		using _T = double;
		//using _F = value_type<F>;
		using _F = double;
		T t;
		F f;
		_F _f;
	public:
		/*
		forward(const _F& _f = NaN<_F>)
			: _f(_f)
		{ }
		*/
//		forward(T t, F f, const _F& _f = NaN<_F>)
		forward(T t, F f, const double& _f = NaN<double>)
			: t(t), f(f), _f(_f)
		{ }

		// Extrapolate past end of curve; f(t) = _f if t > t[n-1].
		forward& extrapolate(const _F& f_ = NaN<_F>) // not const
		{
			_f = f_;

			return *this;
		}

		_F value(const _T& u) const
		{
			return fms::pwflat::value(u, t, f, _f);
		}
		_F operator()(const _T& u) const
		{
			return value(u);
		}

		// Integral from 0 to t of forward.
		_F integral(const _T& u) const
		{
			return fms::pwflat::integral(u, t, f, _f);
		}

		// D(t) = exp(-int_0^t f(s) ds).
		_F discount(const _T& u) const
		{
			return exp(-integral(u));
		}

		// D(t) = exp(-t r(t)). Note f(t) = r(t) on [0, t0].
		_F spot(const _T& u) const
		{
			if (!t) {
				return _f;
			}

			_T t0 = *t;

			return u <= t0 ? value(u) : - log(discount(u)) / u;
		}
	};

}
