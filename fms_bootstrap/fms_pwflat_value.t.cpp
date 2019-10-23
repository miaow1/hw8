// fms_pwflat_value.t.cpp - Test piecewise flat value.
#include <cassert>
#include "fms_pwflat.h"

using namespace fms::pwflat;
#if 0
template<class T, class F>
int test_pwflat_value()
{
	{
		T t[] = { 1, 2 };
		F f[] = { 3, 4 };
		auto t2 = take(2, t);
		auto f2 = take(2, f);

		assert(isnan(value(-1, t2, f2)));
		assert(f[0] == value(0, t2, f2));
		assert(f[0] == value(0.5, t2, f2));
		assert(f[1] == value(1, t2, f2));
		assert(f[1] == value(1.5, t2, f2));
		assert(isnan(value(2, t2, f2)));
	}
	{
		T t[] = { 1, 2 };
		F f[] = { 3, 4 };
		auto t2 = take(2, t);
		auto f2 = take(2, f);
		F _f = 5;

		assert(_f == value(2, t2, f2, _f));
		assert(_f == value(100, t2, f2, _f));
	}

	return 0;
}
int test_pwflat_value_double_double = test_pwflat_value<double,double>();
#endif 