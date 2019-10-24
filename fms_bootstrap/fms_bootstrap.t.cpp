// fms_bootstrap.t.cpp - Test bootstrap algorithm.
#include <cassert>
#include "fms_bootstrap.h"

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

template<class U, class C>
int test_bootstrap()
{
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
int test_bootstrap_int_int = test_bootstrap<int,int>();

int main()
{
	return 0;
}