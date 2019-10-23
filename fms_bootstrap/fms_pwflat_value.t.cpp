// fms_pwflat_value.t.cpp - Test piecewise flat value.
#include <cassert>
#include "fms_pwflat.h"

using namespace fms::pwflat;

template<class X>
class take {
	size_t n;
	X* x;
public:
	take(size_t n, X* x)
		: n(n), x(x)
	{ }
	operator bool() const
	{
		return n != 0;
	}
	X operator*() const
	{
		return *x; 
	}
	take& operator++()
	{
		++x;
		--n;

		return *this;
	}
};

template<class T, class F>
int test_pwflat_value()
{
	{
		T t[] = { 1, 2 };
		F f[] = { 3, 4 };

		assert(isnan(value(-1, take(2, t), take(2, f))));
		assert(f[0] == value(0, take(2, t), take(2, f)));
		assert(f[0] == value(0.5, take(2, t), take(2, f)));
		assert(f[1] == value(1, take(2, t), take(2, f)));
		assert(f[1] == value(1.5, take(2, t), take(2, f)));
		assert(isnan(value(2, take(2, t), take(2, f))));
	}

	return 0;
}
int test_pwflat_value_double_double = test_pwflat_value<double,double>();