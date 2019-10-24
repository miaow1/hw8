// fms_bootstrap.t.cpp - Test bootstrap algorithm.
#include <cassert>
#include "fms_bootstrap.h"
#include "fms_pwflat.h"

using namespace fms::bootstrap;
using namespace fms::pwflat;

using fms::sequence::list;

int scratch()
{
    int x = 1;
    auto f = [&x]() { return x; };
    assert(1 == f());
    x = 2;
    assert(2 == f());

    return 0;
}
int scratch_ = scratch();

int test_bootstrap_extend()
{
	auto t = list<double>{};
	auto f = list<double>{};
	//fms::pwflat::forward<list<double>, list<double>> f;
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