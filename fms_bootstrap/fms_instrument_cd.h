// fms_instrument_cd.h - Cash deposit
// Cash deposits have two cash flows: (0, -1) and (tenor, 1 + rate*tenor)
#pragma once
#include "fms_instrument_sequence.h"

namespace fms::instrument {

	using fms::sequence::list;

	template<class U = double, class C = double>
	struct cash_deposit : public sequence<U, C>
	{
		cash_deposit(U tenor, C rate)
			: sequence<U, C>(list({ 0, tenor }), list({ -1, 1 + rate * tenor }))
		{ }
	};
}
