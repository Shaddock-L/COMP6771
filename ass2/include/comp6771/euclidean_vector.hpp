#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what)
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		// TODO
		// #### 1. Constructors #####
		// 1.1 Default Constructor
		euclidean_vector() noexcept
		: euclidean_vector(1, 0.0) {}
		// 1.2 Single-argument Constructor
		explicit euclidean_vector(int dim) noexcept
		: euclidean_vector(dim, 0.0) {}
		// 1.3 Constructor
		euclidean_vector(const int& d, const double& m) noexcept;
		// 1.4 Constructor
		euclidean_vector(std::vector<double>::const_iterator,
		                 std::vector<double>::const_iterator) noexcept;
		// 1.5 Constructor
		euclidean_vector(std::initializer_list<double>) noexcept;
		// 1.6 Copy Constructor
		euclidean_vector(euclidean_vector const&) noexcept;
		// 1.7 Move Constructor
		euclidean_vector(euclidean_vector&&) noexcept;

		// #### 2. Destructor ######
		~euclidean_vector() noexcept {}

		// #### 3. Operations #####
		// 3.1 Copy Assignment
		euclidean_vector& operator=(euclidean_vector const&) noexcept;
		// 3.2 Move Assignment
		euclidean_vector& operator=(euclidean_vector&&) noexcept;
		// 3.3 Subscript
		// It's a requirement you use asserts to ensure the index passed is valid.
		double operator[](int index) const noexcept;
		double& operator[](int index) noexcept;
		// 3.4 Unary plus
		euclidean_vector operator+() noexcept;
		// 3.5 Negation
		euclidean_vector operator-() noexcept;
		// 3.6 Compound Addition
		euclidean_vector& operator+=(euclidean_vector const&);
		// 3.7 Compound Subtruction
		euclidean_vector& operator-=(euclidean_vector const&);
		// 3.8 Compound Multiplication
		euclidean_vector& operator*=(double) noexcept;
		// 3.9 Compound Division
		euclidean_vector& operator/=(double);
		// 3.10 Vector type conversion
		explicit operator std::vector<double>() const noexcept;
		// 3.11 List type conversion
		explicit operator std::list<double>() const noexcept;

		//#### 4. Member functions #####
		double at(int) const;
		double& at(int);
		int dimensions() const noexcept;

		//#### 5. Friends #####
		// 5.1 equal
		friend bool operator==(euclidean_vector const& LV, euclidean_vector const& RV) noexcept {
			auto state = false;
			if (LV.dimension_ == RV.dimension_) {
				for (unsigned int i = 0; i < static_cast<unsigned int>(LV.dimension_); i++) {
					if (LV.magnitude_[i] != RV.magnitude_[i]) {
						return state;
					}
				}
				state = true;
			}
			return state;
		}

		// 5.2 not equal
		friend bool operator!=(euclidean_vector const& LV, euclidean_vector const& RV) noexcept {
			auto state = true;
			if (LV.dimension_ == RV.dimension_) {
				for (unsigned int i = 0; i < static_cast<unsigned int>(LV.dimension_); i++) {
					if (LV.magnitude_[i] != RV.magnitude_[i]) {
						return state;
					}
				}
				state = false;
			}
			return state;
		}

		// 5.3 addition
		friend euclidean_vector operator+(euclidean_vector const& a, euclidean_vector const& b) {
			if (a.dimension_ == b.dimension_) {
				euclidean_vector add;
				add.dimension_ = a.dimension_;
				add.magnitude_ = std::make_unique<double[]>(static_cast<unsigned int>(add.dimension_));
				for (unsigned int i = 0; i < static_cast<unsigned int>(add.dimension_); i++) {
					add.magnitude_[i] = a.magnitude_[i] + b.magnitude_[i];
				}
				return add;
			}
			else {
				throw euclidean_vector_error{"Dimensions of LHS(X) and RHS(Y) do not match"};
			}
		}

		// 5.4 Subtraction
		friend euclidean_vector operator-(euclidean_vector const& a, euclidean_vector const& b) {
			if (a.dimension_ == b.dimension_) {
				euclidean_vector sub;
				sub.dimension_ = a.dimension_;
				sub.magnitude_ = std::make_unique<double[]>(static_cast<unsigned int>(sub.dimension_));
				for (unsigned int i = 0; i < static_cast<unsigned int>(sub.dimension_); i++) {
					sub.magnitude_[i] = a.magnitude_[i] - b.magnitude_[i];
				}
				return sub;
			}
			else {
				throw euclidean_vector_error{"Dimensions of LHS(X) and RHS(Y) do not match"};
			}
		}

		// 5.5 Multiply
		friend euclidean_vector operator*(euclidean_vector const& ev, double k) noexcept {
			euclidean_vector mult;
			mult.dimension_ = ev.dimension_;
			mult.magnitude_ = std::make_unique<double[]>(static_cast<unsigned int>(mult.dimension_));
			for (unsigned int i = 0; i < static_cast<unsigned int>(mult.dimension_); i++) {
				mult.magnitude_[i] = ev.magnitude_[i] * k;
			}
			return mult;
		}
		friend euclidean_vector operator*(double k, euclidean_vector const& ev) noexcept {
			euclidean_vector mult;
			mult.dimension_ = ev.dimension_;
			mult.magnitude_ = std::make_unique<double[]>(static_cast<unsigned int>(mult.dimension_));
			for (unsigned int i = 0; i < static_cast<unsigned int>(mult.dimension_); i++) {
				mult.magnitude_[i] = ev.magnitude_[i] * k;
			}
			return mult;
		}

		// 5.6 Divide
		friend euclidean_vector operator/(euclidean_vector const& ev, double k) {
			if (k == 0) {
				throw euclidean_vector_error{"Invalid vector division by 0"};
			}
			euclidean_vector div;
			div.dimension_ = ev.dimension_;
			div.magnitude_ = std::make_unique<double[]>(static_cast<unsigned int>(div.dimension_));
			for (unsigned int i = 0; i < static_cast<unsigned int>(div.dimension_); i++) {
				div.magnitude_[i] = ev.magnitude_[i] / k;
			}
			return div;
		}

		// 5.7 output stream
		friend std::ostream& operator<<(std::ostream& os, euclidean_vector const& ev) noexcept {
			if (ev.dimension_ == 0) {
				os << "[]";
			}
			else {
				os << "[";
				for (unsigned int i = 0; i < static_cast<unsigned int>(ev.dimension_) - 1; i++) {
					os << ev.magnitude_[i];
					os << " ";
				}
				os << ev.magnitude_[static_cast<unsigned int>(ev.dimension_) - 1];
				os << "]";
			}
			return os;
		}

		// auxiliary function for storing the norm cache
		void update_norm(double nnorm);
		bool get_cache_status();
		double get_norm();

	private:
		// ass2 spec requires we use double[]
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		int dimension_;
		std::unique_ptr<double[]> magnitude_;
		bool newest_norm_;
		double norm_cache_ = 0;

		// TODO more if needed
	};
	// utility declare
	auto euclidean_norm(euclidean_vector const& v) -> double;
	auto euclidean_norm(euclidean_vector& v) -> double;
	auto unit(euclidean_vector const& v) -> euclidean_vector;
	auto unit(euclidean_vector& v) -> euclidean_vector;
	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;

} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP