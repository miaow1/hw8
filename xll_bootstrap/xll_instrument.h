// xll_intrument.h - Virtual interface to instruments.
#pragma once
#include <utility>

namespace xll {

	// NVI interface to instrument
	template<class T = double, class C = double>
	struct instrument {
		virtual ~instrument()
		{ }
		operator bool() const
		{
			return op_bool();
		}
		std::pair<T, C> operator*() const
		{
			return op_star();
		}
		instrument& operator++()
		{
			return op_incr();
		}
	private:
		virtual bool op_bool() const = 0;
		virtual std::pair<T, C> op_star() const = 0;
		virtual instrument& op_incr() = 0;
	};

	// This class knows the actual instrument type.
	template<class I>
	class instrument_impl : public instrument<> {
		I i;
	public:
		instrument_impl(const I& i)
			: i(i)
		{ }
		bool op_bool() const override
		{
			return i;
		}
		std::pair<double, double> op_star() const override
		{
			return *i;
		}
		instrument_impl& op_incr()
		{
			++i;

			return *this;
		}
	};

}


