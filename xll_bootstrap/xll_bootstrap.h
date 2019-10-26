// xll_bootstrap.h - Bootstrap piecewise constant forward curves.
#pragma once
#include <utility>
#include "../fms_sequence/fms_sequence_list.h"
#include "../xll12/xll/xll.h"

#ifndef CATEGORY
#define CATEGORY L"BOOTSTRAP"
#endif

namespace xll {

	// sequence from FP reference
	inline auto list(const _FP12& a)
	{
		return fms::sequence::list(size(a), a.array);
	}

}
