#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("default constructor") {
	auto g = gdwg::graph<int, std::string>();
	REQUIRE(g.empty() == true);
	REQUIRE(g.node_size() == 0);
	REQUIRE(g.edge_size() == 0);
}

TEST_CASE("constructor para = initializer_list (nodes)") {
	auto il = std::initializer_list<double>{1.1, 2.2, 3.3};
	auto il2 = std::initializer_list<double>();
	auto g1 = gdwg::graph<double, int>(il);
	auto g2 = gdwg::graph<double, int>(il2);
	REQUIRE(g1.is_node(1.1) == true);
	REQUIRE(g1.is_node(2.2) == true);
	REQUIRE(g1.is_node(3.3) == true);
	REQUIRE(g1.is_node(4.4) == false);
	REQUIRE(g2.empty() == true);
}

TEST_CASE("constructor para = 2 position (nodes)") {
	SECTION("basic case") {
		auto num = std::vector<int>{10, 20, 30, 40, 50};
		auto g = gdwg::graph<int, std::string>(num.begin(), num.begin() + 3);
		REQUIRE(g.is_node(10) == true);
		REQUIRE(g.is_node(20) == true);
		REQUIRE(g.is_node(30) == true);
		REQUIRE(g.is_node(40) == false);
		REQUIRE(g.is_node(50) == false);
	}

	SECTION("empty case") {
		auto epty = std::vector<std::string>{};
		auto g2 = gdwg::graph<std::string, int>(epty.begin(), epty.end());
		REQUIRE(g2.empty() == true);
	}

	SECTION("duplicate case") {
		auto dup = std::vector<int>{1, 1, 2, 3, 3, 3, 4};
		auto g3 = gdwg::graph<int, std::string>(dup.begin(), dup.begin() + 6);
		REQUIRE(g3.is_node(1));
		REQUIRE(g3.is_node(2));
		REQUIRE(g3.is_node(3));
		REQUIRE(g3.is_node(4) == false);
		REQUIRE(g3.node_size() == 3);
	}
}

TEST_CASE("move constructor") {
	using graph = gdwg::graph<std::string, int>;
	auto const v = std::vector<graph::value_type>{{"hello", "world", 4},
	                                              {"hello", "sydney", 5},
	                                              {"goodbye", "Australia", 8}};

	SECTION("move to a empty graph") {
		auto other = graph{};
		other.insert_node("hello");
		other.insert_node("world");
		other.insert_node("sydney");
		other.insert_node("goodbye");
		other.insert_node("Australia");
		for (const auto& [from, to, weight] : v) {
			other.insert_edge(from, to, weight);
		}
		auto const initial_g = other;
		auto moved = graph(std::move(other));
		REQUIRE(moved == initial_g);
		REQUIRE(other.empty());
	}

	SECTION("move to a not empty graph") {
		auto other_2 = graph{};
		other_2.insert_node("hello");
		other_2.insert_node("world");
		other_2.insert_node("sydney");
		other_2.insert_node("goodbye");
		other_2.insert_node("Australia");
		for (const auto& [from, to, weight] : v) {
			other_2.insert_edge(from, to, weight);
		}
		auto const initial_g_2 = other_2;
		auto moved_2 = graph{};
		moved_2.insert_node("cool");
		moved_2.insert_node("guy");
		moved_2.insert_edge("cool", "guy", 5);
		moved_2 = graph(std::move(other_2));
		REQUIRE(moved_2 == initial_g_2);
		REQUIRE(other_2.empty());
		REQUIRE(!moved_2.is_node("cool"));
		REQUIRE(!moved_2.is_node("guy"));
	}
}

TEST_CASE("move assignment") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<graph::value_type>{{8, 1, 8}, {8, 1, 6}, {1, 4, 9}, {3, 5, 7}};
	auto other = graph{};
	auto nv = std::vector<int>{1, 3, 4, 5, 8};
	for (auto num : nv) {
		other.insert_node(num);
	}

	for (const auto& [from, to, weight] : v) {
		other.insert_edge(from, to, weight);
	}
	auto g = graph{};
	g.insert_node(100);
	g.insert_node(200);
	g.insert_edge(100, 200, 1000);
	auto const init_g = other;
	g = std::move(other);
	REQUIRE(g == init_g);
	REQUIRE(other.empty());
	REQUIRE(g.nodes() == nv);
	REQUIRE(!g.is_node(100));
	REQUIRE(!g.is_node(200));
}

TEST_CASE("copy constructor") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<graph::value_type>{{8, 1, 8}, {8, 1, 6}, {1, 4, 9}, {3, 5, 7}};
	auto other = graph{};
	auto nv = std::vector<int>{1, 3, 4, 5, 8};
	for (auto num : nv) {
		other.insert_node(num);
	}

	for (const auto& [from, to, weight] : v) {
		other.insert_edge(from, to, weight);
	}
	auto const init_g = other;
	auto g = graph(other);
	REQUIRE(g == other);

	g.insert_node(100);
	g.insert_node(200);
	g.insert_edge(100, 200, 1000);

	REQUIRE(other == init_g);
}

TEST_CASE("copy assignment") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<graph::value_type>{{8, 1, 8}, {8, 1, 6}, {1, 4, 9}, {3, 5, 7}};
	auto other = graph{};
	auto nv = std::vector<int>{1, 3, 4, 5, 8};
	for (auto num : nv) {
		other.insert_node(num);
	}

	for (const auto& [from, to, weight] : v) {
		other.insert_edge(from, to, weight);
	}
	auto const init_g = other;
	auto g = graph{};
	g.insert_node(100);
	g.insert_node(200);
	g.insert_edge(100, 200, 1000);
	g = other;
	REQUIRE(g == init_g);
	REQUIRE(other == init_g);
	REQUIRE(g.nodes() == nv);
	REQUIRE(!g.is_node(100));
	REQUIRE(!g.is_node(200));
}
