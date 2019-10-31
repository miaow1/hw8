// fms_instrument_fra.h - Forward Rate Agreement.
// Forward rate agreements have two cash flows: -1 at the effective date
// and 1 + forward*tenor at the expriation date = effective + tenor.
// The argument order for the constructor is (effective, tenor, forward)
#pragma once

//!!! Implement the class forward_rate_agreement in namespace fms::instrument.
#include "fms_instrument_sequence.h"

namespace fms::instrument {

	template<class U = double, class C = double>
	struct forward_rate_agreement : public sequence<fms::sequence::list<U>, fms::sequence::list<C>> {
		forward_rate_agreement(U effective, U tenor, C forward)
			: sequence<fms::sequence::list<U>, fms::sequence::list<C>>(
				fms::sequence::list<U>({ effective, effective+tenor }), fms::sequence::list<C>({ -1, 1 + forward * tenor })
				)
		{ }
	};
}
