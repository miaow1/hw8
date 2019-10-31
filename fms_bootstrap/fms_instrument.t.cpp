// fms_instrument.t.cpp - Test fms::instrument
#include <cassert>
#include "../fms_sequence/fms_sequence.h"
#include "fms_instrument.h"

using namespace fms::sequence;
using namespace fms::instrument;

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

template<class B, class U, class C>
int test_instrument_fra()
{
	forward_rate_agreement<B, U, C> fra(1, 2, 3);

	//Check times
	auto u = fra.time();
	assert(0 == *u);
	++u;
	assert(1 == *u);
	++u;
	assert(3 == *u);

	//Check cash flows
	auto c = fra.cash();
	assert(0 == *c);
	++c;
	assert(-1 == *c);
	++c;
	assert(7 == *c);

	//Check pairings
	assert(std::pair(0, 0) == *fra);
	++fra;
	assert(std::pair(1, -1) == *fra);
	++fra;
	assert(std::pair(3, 7) == *fra);
	++fra;
	assert(!fra);

	return 0;
}
int test_instrument_fra_int_int = test_instrument_fra<int, int, int>();

template<class U, class C>
int test_instrument_swap()
{
	interest_rate_swap<U, C> swap(1, 2, 3);

	//Check times, cash flows, and pairings
	auto u = swap.time();
	auto c = swap.cash();
	assert(0 == *u);
	++u;
	assert(-1 == *c);
	++c;
	assert(std::pair(0, -1) == *swap);
	++swap;

	int i = 1;
	for (i = 1; i < 2; i++) {
		assert(i / 2 == *u);
		++u;
		assert(3 / 2 == *c);
		++c;
		assert(std::pair(i / 2, 3 / 2) == *swap);
		++swap;
	}

	assert(1 == *u);
	assert(1 + 3 / 2 == *c);
	assert(std::pair(1, 1 + 3 / 2) == *swap);
	++swap;
	assert(!swap);

	return 0;
}
int test_instrument_swap_int_int = test_instrument_swap<int, int>();
