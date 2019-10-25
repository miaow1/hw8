// fms_bootstrap.t.cpp - Test bootstrap algorithm.
#include <cassert>
#include "../fms_sequence/fms_sequence.h"
#include "fms_bootstrap.h"
#include "fms_instrument.h"
#include "fms_pwflat.h"

using namespace fms::bootstrap;
using namespace fms::pwflat;

using fms::sequence::list;
using fms::sequence::apply;
using fms::sequence::sum;

template<class X = double>
int scratch()
{
	list<X> t{ 1, 2, 3 };
	list<X> f{ 0.1, 0.2, 0.3 };
	forward F(t, f);
	auto D = [&F](const X& x) { return F.discount(x); };

	list<X> u{ 1, 2, 3 };
	list<X> c{ -1, .1, 1.1 };
	{
		auto Du = apply(D, u);
		assert(D(*u) == *Du);
		++Du; ++u;
		assert(D(*u) == *Du);
	}
	{
		auto Du = apply(D, u);
		auto s = sum(Du);
		assert(s > 0);
	}
	{
		auto Du = apply(D, u);
		auto cDu = c * Du;
		auto s = sum(cDu);
		assert(s > 0);
	}

    return 0;
}
int scratch_double = scratch<double>();

#if 0
int test_bootstrap_extend()
{
	auto t = list<double>{};
	auto f = list<double>{};
	forward F(t,f);
//	auto cd0 = fms::instrument::sequence(list({ 0., -1. }), list({ 0.25, 1.01 }));
	auto u0 = list<double>({ 0., 0.25 });
	auto c0 = list<double>({ -1, 1.01 });
//	auto p = extend(F, 0., 0., u0, c0);

	/*
	C r = 0.01;
	cd cd1(0.25, r);
	//...
	fra fra1(.25, 1, r);
	//...
	swap swap1(2, 0.5, r);

	forward f;
	f = bootstrap(f, cd1);
	*/
	return 0;
}
int test_bootstrap_extend_ = test_bootstrap_extend();
#endif

int main()
{
	return 0;
}