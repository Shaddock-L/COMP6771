// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "comp6771/euclidean_vector.hpp"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <iterator>
#include <math.h>
#include <vector>

namespace comp6771 {
	// define the initial state and value of cache

	// 1.3 Constructor
	euclidean_vector::euclidean_vector(const int& d, const double& m) noexcept
	: dimension_{d}
	, magnitude_{std::make_unique<double[]>(static_cast<unsigned int>(d))} {
		for (unsigned int i = 0; i < static_cast<unsigned int>(d); i++) {
			magnitude_[i] = m;
		}
		newest_norm_ = false;
	}

	// 1.4 Constructor (iter)
	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator head,
	                                   std::vector<double>::const_iterator rear) noexcept {
		int d = 0;
		for (std::vector<double>::const_iterator iter = head; iter != rear; ++iter) {
			d++;
		}
		dimension_ = d;
		magnitude_ = std::make_unique<double[]>(static_cast<unsigned int>(dimension_));
		unsigned int i = 0;
		for (auto p = head; p != rear; ++p) {
			magnitude_[i] = *p;
			i++;
		}
		newest_norm_ = false;
	}
	// 1.5 Constructor (list)
	euclidean_vector::euclidean_vector(std::initializer_list<double> l) noexcept {
		int d = 0;
		for (auto iter = l.begin(); iter != l.end(); ++iter) {
			d++;
		}
		dimension_ = d;
		magnitude_ = std::make_unique<double[]>(static_cast<unsigned int>(dimension_));
		unsigned int i = 0;
		for (auto p = l.begin(); p != l.end(); ++p) {
			magnitude_[i] = *p;
			i++;
		}
		newest_norm_ = false;
	}
	// 1.6 Copy Constructor
	euclidean_vector::euclidean_vector(euclidean_vector const& init) noexcept {
		dimension_ = init.dimension_;
		magnitude_ = std::make_unique<double[]>(static_cast<unsigned int>(dimension_));
		for (unsigned int i = 0; i < static_cast<unsigned int>(dimension_); i++) {
			magnitude_[i] = init.magnitude_[i];
		}
	}

	// 1.7 Move Constructor
	euclidean_vector::euclidean_vector(euclidean_vector&& init) noexcept
	: dimension_{std::exchange(init.dimension_, 0)}
	, magnitude_{std::exchange(init.magnitude_, nullptr)} {
		newest_norm_ = false;
	}

	// 3.1 Copy assignment
	euclidean_vector& euclidean_vector::operator=(euclidean_vector const& init) noexcept {
		if (&init != this) // avoid assigning to self
		{
			euclidean_vector copy = init;
			std::swap(copy.dimension_, dimension_);
			std::swap(copy.magnitude_, magnitude_);
		}
		return *this;
	}
	// 3.2 Move Assignment
	euclidean_vector& euclidean_vector::operator=(euclidean_vector&& init) noexcept {
		std::swap(dimension_, init.dimension_);
		std::swap(magnitude_, init.magnitude_);
		dimension_ = 0;
		// delete[] magnitude_;
		magnitude_ = nullptr;
		newest_norm_ = false;
		return *this;
	}
	// 3.3 Subscript
	// It's a requirement you use asserts to ensure the index passed is valid.
	double euclidean_vector::operator[](int index) const noexcept {
		assert(index >= 0 and index < dimension_);
		return (*this).magnitude_[static_cast<unsigned int>(index)];
	}
	double& euclidean_vector::operator[](int index) noexcept {
		assert(index >= 0 and index < dimension_);
		return (*this).magnitude_[static_cast<unsigned int>(index)];
	}

	// 3.4 Unary plus

	euclidean_vector euclidean_vector::operator+() noexcept {
		for (unsigned int i = 0; i < static_cast<unsigned int>(dimension_); i++) {
			magnitude_[i] = abs(magnitude_[i]);
		}
		return *this;
	}

	// 3.5 Negation
	euclidean_vector euclidean_vector::operator-() noexcept {
		for (unsigned int i = 0; i < static_cast<unsigned int>(dimension_); i++) {
			magnitude_[i] = -(magnitude_[i]);
		}
		return *this;
	}

	// 3.6 Compound Addition
	euclidean_vector& euclidean_vector::operator+=(euclidean_vector const& RHS) {
		if (dimension_ == RHS.dimension_) {
			for (unsigned int i = 0; i < static_cast<unsigned int>(dimension_); i++) {
				magnitude_[i] = magnitude_[i] + RHS.magnitude_[i];
			}
			newest_norm_ = false;
		}
		else {
			throw euclidean_vector_error{"Dimensions of LHS(X) and RHS(Y) do not match"};
		}
		return *this;
	}

	// 3.7 Compound Subtruction
	euclidean_vector& euclidean_vector::operator-=(euclidean_vector const& RHS) {
		if (dimension_ == RHS.dimension_) {
			for (unsigned int i = 0; i < static_cast<unsigned int>(dimension_); i++) {
				magnitude_[i] = magnitude_[i] - RHS.magnitude_[i];
			}
			newest_norm_ = false;
		}
		else {
			throw euclidean_vector_error{"Dimensions of LHS(X) and RHS(Y) do not match"};
		}
		return *this;
	}

	// 3.8 Compound Multiplication
	euclidean_vector& euclidean_vector::operator*=(double k) noexcept {
		for (unsigned int i = 0; i < static_cast<unsigned int>(dimension_); i++) {
			magnitude_[i] = k * magnitude_[i];
		}
		newest_norm_ = false;
		return *this;
	}

	// 3.9 Compound Division
	euclidean_vector& euclidean_vector::operator/=(double k) {
		if (k != 0) {
			for (unsigned int i = 0; i < static_cast<unsigned int>(dimension_); i++) {
				magnitude_[i] = magnitude_[i] / k;
			}
			newest_norm_ = false;
		}
		else {
			throw euclidean_vector_error{"Invalid vector division by 0"};
		}
		return *this;
	}
	// 3.10 Vector type conversion
	euclidean_vector::operator std::vector<double>() const noexcept {
		std::vector<double> vf;
		for (unsigned int i = 0; i < static_cast<unsigned int>(dimension_); i++) {
			vf.emplace_back(magnitude_[i]);
		}
		return vf;
	}

	// 3.11 List type conversion
	euclidean_vector::operator std::list<double>() const noexcept {
		std::list<double> lf;
		for (unsigned int i = 0; i < static_cast<unsigned int>(dimension_); i++) {
			lf.emplace_back(magnitude_[i]);
		}
		return lf;
	}

	// Member functions
	double euclidean_vector::at(int j) const {
		if (j < 0 or j >= dimension_) {
			throw euclidean_vector_error{"Index X is not valid for this euclidean_vector object"};
		}
		else {
			return magnitude_[static_cast<unsigned int>(j)];
		}
	}
	double& euclidean_vector::at(int j) {
		if (j < 0 or j >= dimension_) {
			throw euclidean_vector_error{"Index X is not valid for this euclidean_vector object"};
		}
		else {
			return magnitude_[static_cast<unsigned int>(j)];
		}
	}

	int euclidean_vector::dimensions() const noexcept {
		int dim = dimension_;
		return dim;
	}

	//#### 6. Utility #####
	auto euclidean_norm(euclidean_vector const& v) -> double {
		euclidean_vector ev = v;
		if (ev.get_cache_status() == false) {
			double sum = 0.0;

			if (ev.dimensions() == 0) {
				return 0.0;
			}
			for (int i = 0; i < ev.dimensions(); i++) {
				sum += (ev.at(i)) * (ev.at(i));
			}
			auto result = sqrt(sum);
			ev.update_norm(result);
			return result;
		}
		else {
			auto result = ev.get_norm();
			return result;
		}
	}

	auto euclidean_norm(euclidean_vector& v) -> double {
		// euclidean_vector ev = v;
		if (v.get_cache_status() == false) {
			double sum = 0.0;

			if (v.dimensions() == 0) {
				return 0.0;
			}
			for (int i = 0; i < v.dimensions(); i++) {
				sum += (v.at(i)) * (v.at(i));
			}
			auto result = sqrt(sum);
			v.update_norm(result);
			return result;
		}
		else {
			auto result = v.get_norm();
			return result;
		}
	}

	auto unit(euclidean_vector const& v) -> euclidean_vector {
		euclidean_vector ev = v;
		if (ev.get_cache_status() == true) {
			auto norm = ev.get_norm();
			if (ev.dimensions() == 0) {
				throw euclidean_vector_error{"euclidean_vector with no dimensions does not have a unit "
				                             "vector"};
			}
			else if (norm == 0) {
				throw euclidean_vector_error{"euclidean_vector with zero euclidean normal does not "
				                             "have a "
				                             "unit vector"};
			}
			else {
				for (int i = 0; i < ev.dimensions(); i++) {
					ev.at(i) = ev.at(i) / norm;
				}
			}
			return ev;
		}
		else {
			auto norm = euclidean_norm(ev);
			if (ev.dimensions() == 0) {
				throw euclidean_vector_error{"euclidean_vector with no dimensions does not have a unit "
				                             "vector"};
			}
			else if (norm == 0) {
				throw euclidean_vector_error{"euclidean_vector with zero euclidean normal does not "
				                             "have a "
				                             "unit vector"};
			}
			else {
				for (int i = 0; i < ev.dimensions(); i++) {
					ev.at(i) = ev.at(i) / norm;
				}
			}
			return ev;
		}
	}

	auto unit(euclidean_vector& v) -> euclidean_vector {
		euclidean_vector ev = v;
		if (v.get_cache_status() == true) {
			auto norm = v.get_norm();
			if (v.dimensions() == 0) {
				throw euclidean_vector_error{"euclidean_vector with no dimensions does not have a unit "
				                             "vector"};
			}
			else if (norm == 0) {
				throw euclidean_vector_error{"euclidean_vector with zero euclidean normal does not "
				                             "have a "
				                             "unit vector"};
			}
			else {
				for (int i = 0; i < ev.dimensions(); i++) {
					ev.at(i) = ev.at(i) / norm;
				}
			}
			return ev;
		}
		else {
			auto norm = euclidean_norm(v);
			if (ev.dimensions() == 0) {
				throw euclidean_vector_error{"euclidean_vector with no dimensions does not have a unit "
				                             "vector"};
			}
			else if (norm == 0) {
				throw euclidean_vector_error{"euclidean_vector with zero euclidean normal does not "
				                             "have a "
				                             "unit vector"};
			}
			else {
				for (int i = 0; i < ev.dimensions(); i++) {
					ev.at(i) = ev.at(i) / norm;
				}
			}
			return ev;
		}
	}

	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double {
		euclidean_vector ev_1 = x;
		euclidean_vector ev_2 = y;
		double sum = 0;
		if (ev_1.dimensions() != ev_2.dimensions()) {
			throw euclidean_vector_error{"Dimensions of LHS(X) and RHS(Y) do not match"};
		}
		else {
			for (int i = 0; i < ev_1.dimensions(); i++) {
				sum += ev_1.at(i) * ev_2.at(i);
			}
			return sum;
		}
	}

	// auxiliary function for storing the norm cache
	void euclidean_vector::update_norm(double nnorm) {
		newest_norm_ = true;
		norm_cache_ = nnorm;
	}

	bool euclidean_vector::get_cache_status() {
		if (newest_norm_ == true) {
			return true;
		}
		else {
			return false;
		}
	}

	double euclidean_vector::get_norm() {
		double rst = norm_cache_;
		return rst;
	}

} // namespace comp6771
