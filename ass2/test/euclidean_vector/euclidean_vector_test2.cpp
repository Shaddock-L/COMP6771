#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <sstream>
#include <vector>

double epsilon = 0.0001;
TEST_CASE("Member Functions") {
	SECTION("Returns the value of the magnitude in the dimension given as the function parameter ") {
		auto read = comp6771::euclidean_vector{1.6, 1.7, 1.8};
		// index < 0
		REQUIRE_THROWS_WITH(read.at(-1), "Index X is not valid for this euclidean_vector object");
		// index >= dimensions
		REQUIRE_THROWS_WITH(read.at(100), "Index X is not valid for this euclidean_vector object");
		// correct parameter
		REQUIRE(read.at(0) == 1.6);
		REQUIRE(read.at(1) == 1.7);
		REQUIRE(read.at(2) == 1.8);
	}

	SECTION("Returns the reference of the magnitude in the dimension given as the function "
	        "parameter") {
		auto ref = comp6771::euclidean_vector(2, 3.0);
		// index < 0
		REQUIRE_THROWS_WITH(ref.at(-1), "Index X is not valid for this euclidean_vector object");
		// index >= dimensions
		REQUIRE_THROWS_WITH(ref.at(100), "Index X is not valid for this euclidean_vector object");
		// general parameter
		ref.at(0) = 6.6;
		REQUIRE(ref.at(0) == 6.6);
	}

	SECTION("Return the number of dimensions in a particular euclidean_vector") {
		auto a = comp6771::euclidean_vector{1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
		auto b = comp6771::euclidean_vector(10, 3.8);
		auto v = std::vector<double>{1.1, 2.2};
		auto c = comp6771::euclidean_vector(v.begin(), v.end());
		REQUIRE(a.dimensions() == 6);
		REQUIRE(b.dimensions() == 10);
		REQUIRE(c.dimensions() == 2);
	}
}

TEST_CASE("Constructors") {
	SECTION("Default Constructor") {
		auto a = comp6771::euclidean_vector();
		CHECK(a.dimensions() == 1);
		CHECK(a.at(0) == 0);
	}

	SECTION("Single-argument Constructor") {
		auto b = comp6771::euclidean_vector(1);
		int i{3};
		auto c = comp6771::euclidean_vector(i);
		CHECK(b.dimensions() == 1);
		CHECK(b.at(0) == 0);
		CHECK(c.dimensions() == 3);
		CHECK(c.at(0) == 0);
		CHECK(c.at(1) == 0);
		CHECK(c.at(2) == 0);
	}

	SECTION("General Constructor") {
		auto d = comp6771::euclidean_vector(2, 4.0);
		CHECK(d.dimensions() == 2);
		CHECK(d.at(0) == 4.0);
		CHECK(d.at(1) == 4.0);
		auto x_1 = int{3};
		auto y_1 = double{3.24};
		auto e = comp6771::euclidean_vector(x_1, y_1);
		CHECK(e.at(0) == 3.24);
		CHECK(e.at(1) == 3.24);
		CHECK(e.at(2) == 3.24);
	}

	SECTION("You have iterators as para") {
		std::vector<double> v_1{1.1, 2.2, 3.3};
		auto f = comp6771::euclidean_vector(v_1.begin(), v_1.end());
		CHECK(f.dimensions() == 3);
		CHECK(f.at(0) == 1.1);
		CHECK(f.at(1) == 2.2);
		CHECK(f.at(2) == 3.3);
	}
	SECTION("You have list as para") {
		auto g = comp6771::euclidean_vector{1.5, 2.6, 3.7};
		CHECK(g.dimensions() == 3);
		CHECK(g.at(0) == 1.5);
		CHECK(g.at(1) == 2.6);
		CHECK(g.at(2) == 3.7);
	}

	SECTION("Copy Constructor") {
		auto h_init = comp6771::euclidean_vector{1.5, 2.6, 3.7};
		auto h = comp6771::euclidean_vector(h_init);
		CHECK(h.dimensions() == 3);
		CHECK(h.at(0) == 1.5);
		CHECK(h.at(1) == 2.6);
		CHECK(h.at(2) == 3.7);
	}

	SECTION("Move Constructor") {
		auto j = comp6771::euclidean_vector{1.0, 2.0, 3.0};
		auto jCopy = comp6771::euclidean_vector(j);
		auto jMove = comp6771::euclidean_vector(std::move(j));
		CHECK(jMove.dimensions() == 3);
		CHECK(jMove.at(0) == 1.0);
		CHECK(jMove.at(1) == 2.0);
		CHECK(jMove.at(2) == 3.0);
		CHECK(j.dimensions() == 0);
		// CHECK(j.at(0) == throw{"Index X is not valid for this euclidean_vector object"});
		REQUIRE_THROWS_WITH(j.at(0), "Index X is not valid for this euclidean_vector object");
	}
}

TEST_CASE("Operations") {
	SECTION("Copy assignment") {
		auto a = comp6771::euclidean_vector(3, 6.6);
		auto b = comp6771::euclidean_vector{};
		b = a;
		REQUIRE(b.dimensions() == 3);
		REQUIRE(b.at(0) == 6.6);
		REQUIRE(b.at(1) == 6.6);
		REQUIRE(b.at(2) == 6.6);
	}

	SECTION("Move assignment") {
		auto c = comp6771::euclidean_vector(2, 9.9);
		auto cMove = std::move(c);
		REQUIRE(c.dimensions() == 0);
		REQUIRE_THROWS_WITH(c.at(0), "Index X is not valid for this euclidean_vector object");
		REQUIRE(cMove.dimensions() == 2);
		REQUIRE(cMove.at(0) == 9.9);
		REQUIRE(cMove.at(1) == 9.9);
	}

	SECTION("Subscript") {
		auto d = comp6771::euclidean_vector{1.1, 2.2, 3.3, 4.4, 5.5};
		double e{d[3]};
		d[0] = 8.8;
		REQUIRE(e == 4.4);
		REQUIRE(d[0] == 8.8);
	}

	SECTION("Unary plus") {
		auto e = comp6771::euclidean_vector{-1.1, 2.2};
		auto f = +e;
		REQUIRE(f[0] == 1.1);
		REQUIRE(f[1] == 2.2);
	}

	SECTION("Negation") {
		auto g = comp6771::euclidean_vector{-1.1, 2.2};
		auto h = -g;
		REQUIRE(h[0] == 1.1);
		REQUIRE(h[1] == -2.2);
	}

	SECTION("Compound Addition") {
		auto i = comp6771::euclidean_vector(1, 2.8);
		auto j = comp6771::euclidean_vector(2, 3.6);
		auto k = comp6771::euclidean_vector(2, 2.4);
		// adding vectors of different dimensions
		REQUIRE_THROWS_WITH(i += j, "Dimensions of LHS(X) and RHS(Y) do not match");
		// adding vectors of same dimensions
		j += k;
		REQUIRE(j[0] == 6.0);
		REQUIRE(j[1] == 6.0);
	}

	SECTION("Compound Subtraction") {
		auto l = comp6771::euclidean_vector(1, 2.8);
		auto m = comp6771::euclidean_vector(2, 3.0);
		auto n = comp6771::euclidean_vector(2, 2.0);
		// substracting vectors of different dimensions
		REQUIRE_THROWS_WITH(l -= m, "Dimensions of LHS(X) and RHS(Y) do not match");
		// substracting vectors of same dimensions
		m -= n;
		REQUIRE(m[0] == 1.0);
		REQUIRE(m[1] == 1.0);
	}

	SECTION("Compound Multiplication") {
		auto o = comp6771::euclidean_vector(2, 3.0);
		o *= 3;
		REQUIRE(o[0] == 9);
		REQUIRE(o[1] == 9);
	}

	SECTION("Compound division") {
		auto p = comp6771::euclidean_vector{1.0, 2.0, 3.0, 4.0};
		// divided by 0
		REQUIRE_THROWS_WITH(p /= 0, "Invalid vector division by 0");
		// divided by none-zero
		p /= 3;
		REQUIRE(abs(p[0] - 0.3333) < epsilon);
		REQUIRE(abs(p[1] - 0.6666) < epsilon);
		REQUIRE(abs(p[2] - 1.0) < epsilon);
		REQUIRE(abs(p[3] - 1.3333) < epsilon);
	}

	SECTION("Vector Type Conversion") {
		auto q = comp6771::euclidean_vector{1.0, 2.0, 3.0};
		auto vf = static_cast<std::vector<double>>(q);
		REQUIRE(vf[0] == 1.0);
		REQUIRE(vf[1] == 2.0);
		REQUIRE(vf[2] == 3.0);
	}

	SECTION("List Type Conversion") {
		auto r = comp6771::euclidean_vector{0.0, 1.0, 2.0};
		auto lf = static_cast<std::list<double>>(r);
		auto answer = std::list<double>{0.0, 1.0, 2.0};
		REQUIRE(lf == answer);
	}
}

TEST_CASE("Friends") {
	SECTION("Equal and NOT Equal") {
		auto a = comp6771::euclidean_vector(1, 2.0);
		auto b = comp6771::euclidean_vector(1, 2.0);
		auto c = comp6771::euclidean_vector(2, 2.0);
		auto bol_equal_1 = (a == b);
		auto bol_equal_2 = (a == c);
		REQUIRE(bol_equal_1 == true);
		REQUIRE(bol_equal_2 == false);
		auto bol_not_equal_1 = (a != b);
		auto bol_not_equal_2 = (b != c);
		REQUIRE(bol_not_equal_1 == false);
		REQUIRE(bol_not_equal_2 == true);
	}

	SECTION("Addtion") {
		auto d = comp6771::euclidean_vector{1.1, 2.2};
		auto e = comp6771::euclidean_vector{0.9, 0.8};
		auto f = comp6771::euclidean_vector{0.9, 0.3, 0.1};
		// different dimensions
		REQUIRE_THROWS_WITH(e + f, "Dimensions of LHS(X) and RHS(Y) do not match");
		auto h = d + e;
		REQUIRE(h.dimensions() == 2);
		REQUIRE(h[0] == 2.0);
		REQUIRE(h[1] == 3.0);
	}

	SECTION("Subtraction") {
		auto i = comp6771::euclidean_vector{10.0, 3.0};
		auto j = comp6771::euclidean_vector{5.0, 0.5};
		auto k = comp6771::euclidean_vector{0.9, 0.3, 0.1};
		// different dimensions
		REQUIRE_THROWS_WITH(i - k, "Dimensions of LHS(X) and RHS(Y) do not match");
		auto m = i - j;
		REQUIRE(m.dimensions() == 2);
		REQUIRE(m[0] == 5.0);
		REQUIRE(m[1] == 2.5);
	}

	SECTION("Multiply") {
		auto n = comp6771::euclidean_vector{1.0, 2.0};
		auto o = n * 3;
		auto p = 3 * n;
		REQUIRE(o == p);
		REQUIRE(o.dimensions() == 2);
		REQUIRE(o[0] == 3.0);
		REQUIRE(o[1] == 6.0);
	}

	SECTION("Divide") {
		auto q = comp6771::euclidean_vector{6.0, 8.0};
		auto dvd = double{3.0};
		auto zero = double{0.0};
		// divided by zero
		REQUIRE_THROWS_WITH(q / zero, "Invalid vector division by 0");
		// general parameter
		auto r = q / dvd;
		REQUIRE(r.dimensions() == 2);
		REQUIRE(abs(r[0] - 2) < epsilon);
		REQUIRE(abs(r[1] - 2.6667) < epsilon);
	}

	SECTION("Print the euclidean vector") {
		auto const a1 = comp6771::euclidean_vector(3, 9.9);
		auto oss = std::ostringstream{};
		oss << a1;
		REQUIRE(oss.str() == "[9.9 9.9 9.9]");
	}
}

TEST_CASE("Utility functions") {
	SECTION("Returns the Euclidean norm of the vector") {
		auto a = comp6771::euclidean_vector{1, 2, 3};
		// initial cache test
		auto cache_state_a_1 = a.get_cache_status();
		REQUIRE(cache_state_a_1 == false);
		auto b = comp6771::euclidean_norm(a);
		REQUIRE(abs(b - 3.7417) < epsilon);

		// cache test after used
		auto cache_state_a_2 = a.get_cache_status();
		REQUIRE(cache_state_a_2 == true);
	}

	SECTION("Returns a Euclidean vector that is the unit vector of another euclidean vector") {
		auto c = comp6771::euclidean_vector(4, 1);
		auto d = comp6771::euclidean_vector(0);
		auto e = comp6771::euclidean_vector(4, 0);
		// 0 dimension case
		REQUIRE_THROWS_WITH(comp6771::unit(d),
		                    "euclidean_vector with no dimensions does not have a unit vector");

		// the value of each magnitude is 0
		REQUIRE_THROWS_WITH(comp6771::unit(e),
		                    "euclidean_vector with zero euclidean normal does not have a unit "
		                    "vector");
		// general parameter
		auto answer = comp6771::euclidean_vector(4, 0.5);
		REQUIRE(comp6771::unit(c) == answer);
	}

	SECTION("Computing the dot product") {
		auto x = comp6771::euclidean_vector{1, 2};
		auto y = comp6771::euclidean_vector{3, 4};
		auto z = comp6771::euclidean_vector{5, 6, 7};
		// different dimensions
		REQUIRE_THROWS_WITH(double{comp6771::dot(x, z)},
		                    "Dimensions of LHS(X) and RHS(Y) do not match");
		// same dimensions
		auto rst = double{comp6771::dot(x, y)};
		REQUIRE(rst == 11);
	}
}
