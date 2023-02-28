#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("default iterator constructor") {
	using graph = gdwg::graph<char, int>;
	auto it1 = graph::iterator();
	auto it2 = graph::iterator();
	REQUIRE(it1 == it2);
}

TEST_CASE("iterator operator ++/--") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<graph::value_type>{
	   {4, 1, -4},
	   {3, 2, 2},
	   {2, 4, 2},
	   {2, 1, 1},
	   {6, 2, 5},
	   {6, 3, 10},
	   {1, 5, -1},
	   {3, 6, -8},
	   {4, 5, 3},
	   {5, 2, 7},
	};
	auto const nv = std::vector<int>{1, 2, 3, 4, 5, 6};
	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
	}
	auto it1 = g1.begin();
	REQUIRE(it1 == g1.find(1, 5, -1));
	++it1;
	REQUIRE(!(it1 == g1.find(1, 5, -1)));
	REQUIRE(it1 == g1.find(2, 1, 1));

	auto it2 = g1.end();
	--it2;
	REQUIRE(it2 == g1.find(6, 3, 10));
	--it2;
	REQUIRE(!(it2 == g1.find(6, 3, 10)));
	REQUIRE(it2 == g1.find(6, 2, 5));
}

TEST_CASE("iterator operator++(int) / operator--(int)") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<graph::value_type>{
	   {4, 1, -4},
	   {3, 2, 2},
	   {2, 4, 2},
	   {2, 1, 1},
	   {6, 2, 5},
	   {6, 3, 10},
	   {1, 5, -1},
	   {3, 6, -8},
	   {4, 5, 3},
	   {5, 2, 7},
	};
	auto const nv = std::vector<int>{1, 2, 3, 4, 5, 6};
	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
	}

	auto it1 = g1.begin();
	REQUIRE(it1 == g1.find(1, 5, -1));
	auto it2 = it1++;
	REQUIRE(it2 == g1.find(1, 5, -1));
	REQUIRE(!(it1 == g1.find(1, 5, -1)));
	REQUIRE(it1 == g1.find(2, 1, 1));

	auto it3 = it1--;
	REQUIRE(it3 == g1.find(2, 1, 1));
	REQUIRE(!(it1 == g1.find(2, 1, 1)));
	REQUIRE(it1 == g1.find(1, 5, -1));
}

TEST_CASE("iterator comparison") {
	using graph = gdwg::graph<char, int>;
	auto const v = std::vector<graph::value_type>{{'A', 'B', 4},
	                                              {'A', 'C', 10},
	                                              {'A', 'D', 3},
	                                              {'B', 'B', 4},
	                                              {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	auto g2 = graph{};
	auto g3 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
		g2.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
		g2.insert_edge(from, to, weight);
	}
	REQUIRE(g1.begin() == g1.begin());
	REQUIRE(g3.begin() == g3.end());
	REQUIRE(!(g2.begin() == g3.end()));
}