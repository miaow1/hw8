// fms_pwflat.h - Piecewise flat forward curves.
#pragma once
#include <cmath>
#include <limits>
#include <type_traits>
#include <utility>

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
template<class S>
using value_type = std::invoke_result_t<decltype(&S::operator*), S>;

template<class... S>
using common_value_type = std::common_type_t<value_type<S>...>;

namespace fms::pwflat {

	// Shorthand for Not a Number.
	template<class X>
	constexpr X NaN = std::numeric_limits<X>::quiet_NaN();

	// Value of the pwflat forward curve at u given sequences for points determining the curve.
	template<class T, class F>
	inline value_type<F> value(const value_type<T>& u, T t, F f, const value_type<F>& _f = NaN<value_type<F>>)
	{
		if (u < 0) {
			return NaN<value_type<F>>;
		}

		while (t and *t <= u) {
			++t;
			++f;
		}

		return f ? *f : _f;
	}
	/*
	// Integral of the pwflat forward from 0 to u given sequences for points determining the curve.
	template<class T, class F>
	inline auto integral(T u, T t, F f, const decltype(*f)& _f = NaN<decltype(*f)>)
	{
		common_value_type<T,F> I = 0;
		value_type<T> t_ = 0;

		while (t and *t < u) {
			I += (*f) * (*t - t_);
			++t;
			++f;
			t_ = *t;
		}

		return I + (f ? *f : _f)*(u - t_);
	}
	*/
	template<class T, class F>
	struct forward_interface {
		virtual ~forward_interface()
		{ }

		// First point on curve.
		std::pair<T, F> front() const
		{
			return front_();
		}
		// Last point on curve.
		std::pair<T, F> back() const
		{
			return back_();
		}
		// Extrapolate past end of curve; f(t) = _f if t > t_{n-1}.
		forward_interface& extrapolate(const F& _f) // not const
		{
			return extrapolate_(_f);
		}

		// Value of forward at t.
		F forward(const T& t) const
		{
			return forward_(t);
		}
		F operator()(const T& t) const
		{
			return forward(t);
		}

		// Integral from 0 to t of forward.
		F integral(const T& t) const
		{
			return integral_(t);
		}

		// D(t) = exp(-int_0^t f(s) ds).
		F discount(const T& t) const
		{
			return exp(-integral(t));
		}

		// D(t) = exp(-t r(t)). Note f(t) = r(t) on [0, t0).
		F spot(const T& t) const
		{
			const auto& [f0, t0] = front();

			return t < t0 ? forward(t) : - log(discount(t)) / t;
		}
	private:
		virtual std::pair<F, T> front_() const = 0;
		virtual std::pair<F, T> back_() const = 0;
		virtual forward_interface& extrapolate_(const F&) = 0;
		virtual F forward_(const T&) const = 0;
		virtual std::common_type_t<F,T> integral_(const T&) const = 0;
	};

}
