// xll_pwflat.cpp - Excel add-in for piecewise flat forward curves.
#include "../fms_bootstrap/fms_pwflat.h"
#include "xll_bootstrap.h"

#ifdef CATEGORY
#undef CATEGORY
#endif
#define CATEGORY L"PWFLAT"

using namespace xll;

AddIn xai_pwflat(
	Document(CATEGORY)
	.Category(CATEGORY)
	.Documentation(
		L"Functions for piecewise flat forward curves. "
	)
);

using forward = fms::pwflat::forward<fms::sequence::list<double>, fms::sequence::list<double>>;

AddIn xai_pwflat_forward(
	Function(XLL_HANDLE, L"?xll_pwflat_forward", CATEGORY L".FORWARD")
	.Arg(XLL_FP, L"time", L"is an array of times. ")
	.Arg(XLL_FP, L"cash", L"is an array of forwards. ")
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a piecewise flat forward curve. ")
	.Documentation(
		L"Specify arbitratry times in years and forward rates for a curve. "
		L"The times must be increasing and positive. "
	)
);
HANDLEX WINAPI xll_pwflat_forward(const _FP12* pt, const _FP12* pf)
{
#pragma XLLEXPORT
	handlex result;

	try {
		ensure(size(*pt) == size(*pf));

		handle<forward> forward_(new forward(list(*pt), list(*pf)));

		result = forward_.get();
	}
	catch (const std::exception & ex) {
		XLL_ERROR(ex.what());

		return 0; // #NUM!
	}

	return result;
}

AddIn xai_pwflat_forward_value(
	Function(XLL_FP, L"?xll_pwflat_forward_value", CATEGORY L".FORWARD.VALUE")
	.Arg(XLL_HANDLE, L"forward", L"is a handle to a piecewise flat forward. ")
	.Arg(XLL_FP, L"times", L"is an array of times at which to find the forward value. ")
	.Category(CATEGORY)
	.FunctionHelp(L"Return an array of forward values corresponding to times. ")
	.Documentation(
		L"Return an array of forward values corresponding to times. "
	)
);
_FP12* WINAPI xll_pwflat_forward_value(HANDLEX fwd, const _FP12* pt)
{
#pragma XLLEXPORT
	static xll::FP12 result;

	try {
		handle<forward> fwd_(fwd);

		result.resize(rows(*pt), columns(*pt));
		for (int i = 0; i < size(*pt); ++i) {
			result[i] = fwd_->value(pt->array[i]);
		}
	}
	catch (const std::exception & ex) {
		XLL_ERROR(ex.what());

		return 0; // #NUM!
	}

	return result.get();
}

//!!! Implement PWFLAT.FORWARD.SPOT
AddIn xai_pwflat_forward_spot(
	Function(XLL_FP, L"?xll_pwflat_forward_spot", CATEGORY L".FORWARD.SPOT")
	.Arg(XLL_HANDLE, L"forward", L"is a handle to a piecewise flat forward. ")
	.Arg(XLL_FP, L"times", L"is an array of times at which to find the forward spot. ")
	.Category(CATEGORY)
	.FunctionHelp(L"Return the forward spots corresponding to times. ")
	.Documentation(
		L"Return the forward spots. "
	)
);
_FP12* WINAPI xll_pwflat_forward_spot(HANDLEX fwd, const _FP12* pt)
{
#pragma XLLEXPORT
	static xll::FP12 result;

	try {
		handle<forward> fwd_(fwd);

		result.resize(rows(*pt), columns(*pt));
		for (int i = 0; i < size(*pt); ++i) {
			result[i] = fwd_->spot(pt->array[i]);
		}

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0; // #NUM!
	}

	return result.get();
}
//!!! Implement PWFLAT.FORWARD.DISCOUNT

AddIn xai_pwflat_forward_discount(
	Function(XLL_FP, L"?xll_pwflat_forward_discount", CATEGORY L".FORWARD.DISCOUNT")
	.Arg(XLL_HANDLE, L"forward", L"is a handle to a piecewise flat forward. ")
	.Arg(XLL_FP, L"times", L"is an array of times at which to find the forward discount. ")
	.Category(CATEGORY)
	.FunctionHelp(L"Return the forward discounts corresponding to times. ")
	.Documentation(
		L"Return the forward discounts. "
	)
);
_FP12* WINAPI xll_pwflat_forward_discount(HANDLEX fwd, const _FP12* pt)
{
#pragma XLLEXPORT
	static xll::FP12 result;

	try {
		handle<forward> fwd_(fwd);

		result.resize(rows(*pt), columns(*pt));
		for (int i = 0; i < size(*pt); ++i) {
			result[i] = fwd_->discount(pt->array[i]);
		}

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0; // #NUM!
	}

	return result.get();
} 

