// fms_instrument_cd.h - Cash deposit
// Cash deposits have two cash flows: (0, -1) and (tenor, 1 + rate*tenor)
#pragma once
#include "fms_instrument_sequence.h"

namespace fms::instrument {

	template<class U = double, class C = double>
	struct cash_deposit : public sequence<fms::sequence::list<U>, fms::sequence::list<C>> {
	cash_deposit(U tenor, C rate)
			: sequence<fms::sequence::list<U>, fms::sequence::list<C>>(fms::sequence::list<U>({ 0, tenor }), fms::sequence::list<C>({ -1, 1 + rate * tenor }))
		{ }
	};
}
