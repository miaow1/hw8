// fms_bootstrap.t.cpp - Test bootstrap algorithm.
#include <cassert>
#include "../fms_sequence/fms_sequence.h"
#include "fms_bootstrap.h"
#include "fms_instrument.h"
#include "fms_pwflat.h"

using namespace fms::bootstrap;
using namespace fms::pwflat;
using namespace fms::sequence;
using namespace fms::instrument;

using fms::sequence::list;
using fms::sequence::apply;
using fms::sequence::sum;

template<class X = double>
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

template<class U, class C>
int test_instrument_sequence()
{
	sequence s(iota<U>(1), constant<C>(2));
	assert(s);
	auto p = std::pair<U, C>(1, 2);
	assert(p == *s);
	++s;
	++p.first;
	assert(p == *s);

	return 0;
}
int test_instrument_sequence_int_int = test_instrument_sequence<int, int>();

template<class U, class C>
int test_instrument_cd()
{
	cash_deposit<U, C> cd(2, 3);

	auto u = cd.time();
	assert(0 == *u);
	++u;
	assert(2 == *u);

	auto c = cd.cash();
	assert(-1 == *c);
	++c;
	assert(1 + 2 * 3 == *c);

	assert(std::pair(0, -1) == *cd);
	++cd;
	assert(std::pair(2, 1 + 2 * 3) == *cd);
	++cd;
	assert(!cd);

	return 0;
}
int test_instrument_cd_int_int = test_instrument_cd<int, int>();

template<class U, class C>
int test_instrument_fra()
{
	forward_rate_agreement<U, C> fra(1,2, 3);

	auto u = fra.time();
	assert(1 == *u);
	++u;
	assert(3 == *u);

	auto c = fra.cash();
	assert(-1 == *c);
	++c;
	assert(1 + 2 * 3 == *c);

	assert(std::pair(1, -1) == *fra);
	++fra;
	assert(std::pair(3, 1 + 2 * 3) == *fra);
	++fra;
	assert(!fra);

	return 0;
}
int test_instrument_fra_int_int = test_instrument_fra<int, int>();

template<class U, class C,class T>
int test_instrument_swap()
{
	//(maturity, frequency, coupon)
	interest_rate_swap<U, C, T> swap(1.7, 2, 3);

	auto u = swap.time();
	assert(0 == *u);
	++u;	
	assert(0.5 == *u);
	++u;
	assert(1 == *u);
	++u;
	assert(1.5 == *u);
	++u;
	assert(1.7 == *u);


	auto c = swap.cash();
	assert(-1 == *c);
	++c;
	assert(1.5 == *c);
	++c;
	assert(1.5 == *c);
	++c;
	assert(1.5 == *c);
	++c;
	assert(1 == *c);


	assert(std::pair(0.0, -1.0) == *swap);
	++swap;
	assert(std::pair(0.5, 1.5) == *swap);
	++swap;
	assert(std::pair(1.0, 1.5) == *swap);
	++swap;
	assert(std::pair(1.5, 1.5) == *swap);
	++swap;
	assert(std::pair(1.7, 1.0) == *swap);
	++swap;
	assert(!swap);

	return 0;
}
int test_instrument_swap_int_int_int = test_instrument_swap<double, double, int>();
int main()
{
	return 0;
}
