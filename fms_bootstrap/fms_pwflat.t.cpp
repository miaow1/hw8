// fms_pwflat_vector.t.cpp - Test piecewise flat vector implmentation.
#include <cassert>
#include "fms_sequence_list.h"
#include "fms_pwflat.h"

using namespace fms::pwflat;
using namespace fms::sequence;

int test_pwflat_value()
{
	list<double> t({ 1, 2, 3 }), f({ .1, .2, .3 });

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
	list<double> t({ 1, 2, 3 }), f({ .1, .2, .3 });

	assert(isnan(integral(-1, t, f)));
	assert(0 == integral(0, t, f));
	assert(.1*0.5 == integral(0.5, t, f));
	assert(.1*1 == integral(1, t, f));
	assert(.1 * 1 + .2*0.5 == integral(1.5, t, f));
	assert(.1 * 1 + .2 * 1 == integral(2, t, f));
	assert(.1 * 1 + .2 * 1 + .3*1 == integral(3, t, f));
	assert(isnan(integral(3.00001, t, f)));
	assert(.1 * 1 + .2 * 1 + .3 * 1 + .4 * .1 == integral(3.1, t, f, .4));
	assert(.1 * 1 + .2 * 1 + .3 * 1 + .4 * 100 == integral(3 + 100, t, f, .4));

	return 0;
}
int test_pwflat_integral_ = test_pwflat_integral();

#if 0
inline int test_fms_pwflat()
{
	list<double> t({ 1, 2, 3 }), f({ .1, .2, .3 });
	list<double> t2({ 1 }), f2({ .1 });

	{ // forward
		int u;
		u = 1;
		double x{ 0.2 }, x_;
		//1, 0, null, null, 0.2
		forward_vector<double, double> f0(0, nullptr, nullptr);
		x_ = f0(u);
		assert(isnan(x_));
		f0.extrapolate(x);
		x_ = f0(u);
		assert(x_ == x);

		double u_[] = { -1, 0, 0.5, 1, 1.5 };
		double a_[] = { 0, 0.1, 0.1, 0.1, 0.2 };

		auto f2 = forward_vector(t_2.size(), t_2.data(), f_2.data());
		for (int i = 0; i < 5; i++) {
			if (i == 0 || i == 4) {
				assert(isnan(f2(u_[i])));
			}
			else {
				assert(f2(u_[i]) == a_[i]);
			}
		}

		for (int i = 0; i < 5; i++) {
			f2.extrapolate(0.2);
			if (i == 0) {
				assert(isnan(f2(u_[i])));
			}
			else {
				assert(f2(u_[i]) == a_[i]);
			}
		}

		auto tf = forward_vector<double,double>(t.size(), t.data(), f.data());
		for (int i = 0; i < 3; ++i)
			assert(f[i] == tf(t[i]));
	}
	{ // integral
		double u;
		auto tf = forward_vector<double,double>(t.size(), t.data(), f.data());
		u = -1;
		assert(isnan(tf.integral(u)));
		u = 4;
		assert(isnan(tf.integral(u)));
		u = 0;
		assert(0 == tf.integral(u));
		u = 0.5;
		assert(.1 * .5 == tf.integral(u));
		u = 1;
		assert(.1 == tf.integral(u));
		u = 1.5;
		assert(.1 + .2 * .5 == tf.integral(u));
		u = 2.5;
		assert(.1 + .2 + .3 * .5 == tf.integral(u));
		u = 3;
		assert(fabs(.1 + .2 + .3 - tf.integral(u)) < 1e-10);
		//		assert (.1 + .2 + .3 != .6); 
	}
	{ // discount
		double u_[] = { -.5, 0, .5, 1, 1.5, 2, 2.5, 3, 3.5 };
		double f_[] = { 0, 0, .05, .1, .2, .3, .45, .6, .7 };
		auto tf = forward_vector<double,double>(t.size(), t.data(), f.data());
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
	}
	{ // spot
		double u_[] = { -.5, 0, .5, 1, 1.5, 2, 2.5, 3, 3.5 };
		double f_[] = { .1, .1, .1, .1, .2 / 1.5, .3 / 2, .45 / 2.5, .6 / 3, .7 / 3.5 };
		auto tf = forward_vector(t.size(), t.data(), f.data());
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
	}

	{ // present_value
		double u_[] = { 0, 1, 2, 3, 4 };
		double d_[] = { 0,
			discount(u_[1], t.size(), t.data(), f.data(), 0.2),
			discount(u_[2], t.size(), t.data(), f.data(), 0.2),
			discount(u_[3], t.size(), t.data(), f.data(), 0.2),
			discount(u_[4], t.size(), t.data(), f.data(), 0.2)
		};
		double c_[] = { 0, 1, 2, 3, 4 };

		//assert(isnan(present_value(1, u_, c_, t.size(), t.data(), f.data())));
		//assert(isnan(present_value(1, u_, c_, t.size(), t.data(), f.data(), 0.2)));

		double sum = 0;
		for (int i = 0; i < 5; i++) {
			sum += c_[i] * d_[i];
			if (i == 4) {
				double tmp = present_value<double, double>(i + 1, u_, c_, t.size(), t.data(), f.data(), 0.2);
				assert(tmp == tmp);
				assert(fabs(sum - present_value(i + 1, u_, c_, t.size(), t.data(), f.data(), 0.2)) < 1e-10);
				assert(isnan(present_value(i + 1, u_, c_, t.size(), t.data(), f.data())));
			}
			else {
				double tmp = present_value<double, double>(i + 1, u_, c_, t.size(), t.data(), f.data(), 0.2);
				assert(tmp == tmp);
				assert(fabs(sum - present_value(i + 1, u_, c_, t.size(), t.data(), f.data(), 0.2)) < 1e-10);
				assert(fabs(sum - present_value(i + 1, u_, c_, t.size(), t.data(), f.data())) < 1e-10);
			}
		}

	}

	return 0;
}
int test_fms_pwflat_ = test_fms_pwflat();
#endif
