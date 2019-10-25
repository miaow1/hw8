// xll_instrument.cpp - Excel add-in for fixed income instruments.
#include "../fms_sequence/fms_sequence_list.h"
#include "../fms_bootstrap/fms_instrument.h"
#include "xll_bootstrap.h"

#ifdef CATEGORY
#undef CATEGORY
#endif
#define CATEGORY L"INSTRUMENT"

using namespace fms;
using namespace xll;

// sequence from FP reference
auto list(const _FP12& a)
{
	return sequence::list(size(a), a.array);
}

AddIn xai_instrument_sequence(
	Function(XLL_HANDLE, L"?xll_instrument_sequence", CATEGORY L".SEQUENCE")
	.Arg(XLL_FP, L"time", L"is an array of cash flow times.")
	.Arg(XLL_FP, L"cash", L"is an array of cash flow amounts.")
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to an instrument sequence.")
	.Documentation(L"doc...")
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
	.Documentation(L"doc...")
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
	.FunctionHelp(L"Return a handle to a cash deposit instrument.")
	.Documentation(L"doc...")
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