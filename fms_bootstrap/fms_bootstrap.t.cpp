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

    return 0;
}
int scratch_double = scratch<double>();

int test_bootstrap_extend()
{
	auto t = list<double>{};
	auto f = list<double>{};
	forward F(t,f);
	double _t;
	double _f;
	{
	//	auto cd0 = fms::instrument::sequence(list({ 0., -1. }), list({ 0.25, 1.01 }));
		auto u0 = list<double>({ 0., 0.25 });
		auto c0 = list<double>({ -1, 1.01 });
		auto [u,r] = extend(F, 0., 0., u0, c0);
		assert(u == 0.25);
		auto p = 1.01 * exp(-r * 0.25);
		assert(p == 1);

		_t = u;
		_f = r;
	}
	{
		auto cd0 = fms::instrument::sequence(list({ 0., 0.25 }), list({ -1., 1.01 }));
		list<double> u0 = cd0.time();
		list<double> c0 = cd0.cash();
		auto [u, r] = extend(F, 0., 0., u0, c0);
		assert(u == 0.25);
		auto p = 1.01 * exp(-r * 0.25);
		assert(p == 1);
	}
	t.push_back(_t);
	f.push_back(_f);
	{
		auto cd1 = fms::instrument::sequence(list({ 0., 0.5 }), list({ -1., 1.05 }));
		auto [u, r] = extend(F, _t, 0., cd1.time(), cd1.cash());
		assert(u == 0.5);
		//auto p = 1.01 * exp(-r * 0.25);
		//assert(p == 1);
	}

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

int main()
{
	return 0;
}
