// xll_instrument.cpp - Excel add-in for fixed income instruments.
#include "../fms_sequence/fms_sequence_list.h"
#include "../fms_bootstrap/fms_instrument.h"
#include "../xll12/xll/shfb/entities.h"
#include "xll_bootstrap.h"
#include "xll_instrument.h"

#ifdef CATEGORY
#undef CATEGORY
#endif
#define CATEGORY L"INSTRUMENT"

using namespace fms;
using namespace xll;

AddIn xai_instrument(
	Document(CATEGORY)
	.Category(CATEGORY)
	.Documentation(
		L"Functions for fixed income instruments."
	)
);

AddIn xai_instrument_sequence(
	Function(XLL_HANDLE, L"?xll_instrument_sequence", CATEGORY L".SEQUENCE")
	.Arg(XLL_FP, L"time", L"is an array of cash flow times.")
	.Arg(XLL_FP, L"cash", L"is an array of cash flow amounts.")
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to an instrument sequence.")
	.Documentation(
		L"Specify arbitratry times in years and cash flow amounts for an instrument. "
		L"The times must be increasing and non-negative. "
	)
);
HANDLEX WINAPI xll_instrument_sequence(const _FP12* pu, const _FP12* pc)
{
#pragma XLLEXPORT
	handlex result;

	try {
		ensure(size(*pu) == size(*pc));

		auto i = fms::instrument::sequence(list(*pu), list(*pc));
		handle<xll::instrument<>> inst(new instrument_impl(i));

		result = inst.get();
	}
	catch (const std::exception & ex) {
		XLL_ERROR(ex.what());

		return 0; // #NUM!
	}

	return result;
}

AddIn xai_instrument_cash_flows(
	Function(XLL_FP, L"?xll_instrument_cash_flows", CATEGORY L".CASH_FLOWS")
	.Arg(XLL_HANDLE, L"instrument", L"is a handle to an instrument.")
	.Category(CATEGORY)
	.FunctionHelp(L"Return a two column array of cash flow time and amount.")
	.Documentation(
		L"The first column is the cash flow time and the second is the corresponding amount. "
	)
);
_FP12* WINAPI xll_instrument_cash_flows(HANDLEX inst)
{
#pragma XLLEXPORT
	static xll::FP12 result;

	try {
		handle<xll::instrument<>> inst_(inst);

		result.resize(0, 0);
		while (*inst_) {
			const auto& [u, c] = *(*inst_);
			result.push_back({ u, c });
			++(*inst_);
		}
		result.resize(result.size() / 2, 2);
	}
	catch (const std::exception & ex) {
		XLL_ERROR(ex.what());

		return 0; // #NUM!
	}

	return result.get();
}

AddIn xai_instrument_cd(
	Function(XLL_HANDLE, L"?xll_instrument_cd", CATEGORY L".CASH_DEPOSIT")
	.Arg(XLL_DOUBLE, L"tenor", L"is the time in years at which the cash deposit matures.")
	.Arg(XLL_DOUBLE, L"rate", L"is the simple compounding rate for the cash deposit.")
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a cash deposit instrument.")
	.Documentation(
		L"A cash deposit has two cash flows. The first is at time 0 and is always -1. "
		L"This corresponds to having initial price 1. "
		L"The second occurs at " C_(L"tenor") L" and is equal to  1 + r" delta_
		L" where r is the simple compounding " C_(L"rate") L" and " delta_
		L" is the " C_(L"tenor") L" in years. "
	)
);
HANDLEX WINAPI xll_instrument_cd(double tenor, double rate)
{
#pragma XLLEXPORT
	handlex result;

	try {
		auto cd = fms::instrument::cash_deposit(tenor, rate);
		handle<xll::instrument<>> cd_(new instrument_impl(cd));

		result = cd_.get();
	}
	catch (const std::exception & ex) {
		XLL_ERROR(ex.what());

		return 0; // #NUM!
	}

	return result;

}

//!!! Implement INSTRUMENT.FORWARD_RATE_AGREEMENT(effective, tenor, forward)
AddIn xai_instrument_far(
	Function(XLL_HANDLE, L"?xll_instrument_fra", CATEGORY L".FORWARD_RATE_AGREEMENT")
	.Arg(XLL_DOUBLE, L"effective", L"is the time in years at which the forward rate starts taking into account.")
	.Arg(XLL_DOUBLE, L"tenor", L"is the time in years at which the forward rate matures.")
	.Arg(XLL_DOUBLE, L"forward", L"is the simple compounding rate for the forward rate.")
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a forward rate instrument.")
	.Documentation(
		L"A forward rate agreement has two cash flows. The first is at time effective and is always -1. "
		L"This corresponds to having initial price 1. "
		L"The second occurs at " C_(L"tenor+effective") L" and is equal to  1 + r" delta_
		L" where r is the simple compounding " C_(L"rate") L" and " delta_
		L" is the " C_(L"tenor") L" in years. "
	)
);
HANDLEX WINAPI xll_instrument_fra(double effective, double tenor, double forward)
{
#pragma XLLEXPORT
	handlex result;

	try {
		auto fra = fms::instrument::forward_rate_agreement(effective,tenor, forward);
		handle<xll::instrument<>> fra_(new instrument_impl(fra));

		result = fra_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0; // #NUM!
	}

	return result;

}
//!!! Implement INSTRUMENT.INTEREST_RATE_SWAP(maturity, frequency, coupon)
AddIn xai_instrument_swap(
	Function(XLL_HANDLE, L"?xll_instrument_swap", CATEGORY L".INTEREST_RATE_SWAP")
	.Arg(XLL_DOUBLE, L"maturity", L"is the time in years at which the forward rate starts taking into account.")
	.Arg(XLL_DOUBLE, L"frequency", L"is the time in years at which the forward rate matures.")
	.Arg(XLL_DOUBLE, L"coupon", L"is the simple compounding rate for the forward rate.")
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a forward rate instrument.")
	.Documentation(
		L"A forward rate agreement has multiple cash flows. The first is at time 0 and is always -1. "
		L"The later occurs at " C_(L"i/frequency") L" and is equal to coupon" delta_
		L"The last occurs at maturity and is equal to 1+coupon"
		L" where maturity is the " C_(L"maturity") L" in years. "
	)
);
HANDLEX WINAPI xll_instrument_swap(double maturity, double frequency, double coupon)
{
#pragma XLLEXPORT
	handlex result;

	try {
		auto swa = fms::instrument::interest_rate_swap(maturity, frequency, coupon);
		handle<xll::instrument<>> swa_(new instrument_impl(swa));

		result = swa_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0; // #NUM!
	}

	return result;

} 

