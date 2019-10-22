// fms_pwflat_vector.h - Implementation of fms::pwflat::forward using vector.
#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include "fms_pwflat.h"

namespace fms::pwflat {

	template<class T, class F>
	class forward_vector : public forward_interface<T, F>
	{
		std::vector<std::pair<T, F>> tf; // points on the curve
		F _f;
	public:
		forward_vector()
			: _f(std::numeric_limits<F>::quiet_NaN())
		{ }
		forward_vector(size_t n, const T* t, const F* f)
			: tf(n), _f(std::numeric_limits<F>::quiet_NaN())
		{
			T t_{ 0 };
			for (size_t i = 0; i < n; ++i) {
				if (t[i] <= t_) {
					throw std::runtime_error("fms::pwflat::forward_vector: times must be increasing");
				}
				tf[i] = std::pair<T,F>({ t[i], f[i]});
				t_ = t[i];
			}
		}
		forward_vector& push_back(const std::pair<T, F>& p)
		{
			if (p.first <= tf.back().first) {
				throw std::runtime_error("fms::pwflat::forward_vector::push_back: times must be increasing");
			}
			this->tf.push_back(p);

			return *this;
		}
		forward_vector& push_back(const T& t, const F& f)
		{
			return push_back(std::pair{ t, f });
		}
		std::pair<T, F> front_() const override
		{
			return tf.front();
		}
		std::pair<T, F> back_() const override
		{
			return tf.back();
		}
		forward_vector& extrapolate_(const F& f) override
		{
			_f = f;

			return *this;
		}
		F forward_(const T& t) const override
		{
			if (t < 0) {
				return std::numeric_limits<F>::quiet_NaN();
			}

			auto tfi = std::lower_bound(tf.begin(), tf.end(), std::pair{ t, F(0)});

			return tfi == tf.end() ? _f : tfi->second;
		}
		std::common_type_t<T, F> integral_(const T& t) const override
		{
			if (t < 0) {
				return std::numeric_limits<F>::quiet_NaN();
			}

			T t_{ 0 };
			F I{ 0 };
			for (const auto& [ti, fi] : tf) {
				if (ti == tf.back().first) {
					I += _f * (t - ti);

					break;
				}
				else if (ti > t) {
					I += fi * (t - t_);

					break;
				}
				I += fi * (ti - t_);
				t_ = ti;
			}

			return I;
		}

	};


}
