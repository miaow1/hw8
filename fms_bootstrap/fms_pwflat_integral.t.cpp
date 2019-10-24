// fms_pwflat_integral.t.cpp - Test pwflat integral.
#include <cassert>
#include "fms_pwflat.h"
#include "../fms_sequence/fms_sequence_list.h"

using namespace fms::pwflat;
#if 0
template<class T, class F>
int test_pwflat_integral()
{
	{
		T t[] = { 1, 2 };
		F f[] = { 3, 4 };
		auto t2 = take(2, t);
		auto f2 = take(2, f);

		assert(isnan(integral(-1, t2, f2)));
	}

	return 0;
}
int test_pwflat_integral_double_double = test_pwflat_integral<double, double>();
#endif
