// fms_pwflat.t.cpp - Test piecewise flat vector implmentation.
#include <cassert>
#include "fms_sequence_list.h"
#include "fms_pwflat.h"

using namespace fms::pwflat;
using namespace fms::sequence;

list<double> t({ 1, 2, 3 }), f({ .1, .2, .3 });

int test_pwflat_value()
{
	assert(isnan(value(-1, t, f)));
	assert(.1 == value(0, t, f));
	assert(.1 == value(0.5, t, f));
	assert(.1 == value(1, t, f));
	assert(.2 == value(1.5, t, f));
	assert(.2 == value(2, t, f));
	assert(.3 == value(3, t, f));
	assert(isnan(value(3.00001, t, f)));
	assert(.4 == value(3.00001, t, f, .4));
	assert(.4 == value(1000, t, f, .4));

	return 0;
}
int test_pwflat_value_ = test_pwflat_value();

int test_pwflat_integral()
{
	assert(isnan(integral(-1, t, f)));
	assert(0 == integral(0, t, f));
	assert(.1*0.5 == integral(0.5, t, f));
	assert(.1*1 == integral(1, t, f));
	assert(.1 * 1 + .2*0.5 == integral(1.5, t, f));
	assert(.1 * 1 + .2 * 1 == integral(2, t, f));
	assert(.1 * 1 + .2 * 1 + .3*1 == integral(3, t, f));
	assert(isnan(integral(3.00001, t, f)));
	assert(.1 * 1 + .2 * 1 + .3 * 1 + .4 * .1 == integral(3.1, t, f, .4));
	assert(.1 * 1 + .2 * 1 + .3 * 1 + .4 * 100. == integral(3 + 100., t, f, .4));

	return 0;
}
int test_pwflat_integral_ = test_pwflat_integral();

int test_pwflat_discount()
{
    auto tf = forward(t, f);
    double u_[] = { -.5, 0, .5, 1, 1.5, 2, 2.5, 3, 3.5 };
	double f_[] = { 0, 0, .05, .1, .2, .3, .45, .6, .7 };

	for (int i = 0; i < 9; i++) {
		if (i == 0 || i == 8) {
			assert(isnan(tf.discount(u_[i])));
		}
		else {
			assert(fabs(exp(-f_[i]) - tf.discount(u_[i])) < 1e-10);
		}
	}

	for (int i = 0; i < 9; i++) {
		if (i == 0) {
			tf.extrapolate(0.2);
			assert(isnan(tf.discount(u_[i])));
		}
		else {
			assert(fabs(exp(-f_[i]) - tf.discount(u_[i])) < 1e-10);
		}
	}
    
    return 0;
}
int test_pwflat_discount_ = test_pwflat_discount();

int test_pwflat_spot()
{
	double u_[] = { -.5, 0, .5, 1, 1.5, 2, 2.5, 3, 3.5 };
	double f_[] = { .1, .1, .1, .1, .2 / 1.5, .3 / 2, .45 / 2.5, .6 / 3, .7 / 3.5 };
	auto tf = forward(t, f);
	for (int i = 0; i < 9; i++) {
		if (i == 0 || i == 8) {
			assert(isnan(tf.spot(u_[i])));
		}
		else {
			assert(fabs(f_[i] - tf.spot(u_[i])) < 1e-10);
		}
	}

	for (int i = 0; i < 9; i++) {
		tf.extrapolate(0.2);
		if (i == 0) {
			assert(isnan(tf.spot(u_[i])));
		}
		else {
			assert(fabs(f_[i] - tf.spot(u_[i])) < 1e-10);
		}
	}

    return 0;
}
int test_pwflat_spot_ = test_pwflat_spot();
