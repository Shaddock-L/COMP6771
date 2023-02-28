#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("is node") {
	using graph = gdwg::graph<char, int>;
	auto const v = std::vector<graph::value_type>{{'A', 'B', 4},
	                                              {'A', 'C', 10},
	                                              {'A', 'D', 3},
	                                              {'B', 'B', 4},
	                                              {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	REQUIRE(g1.is_node('A'));
	REQUIRE(g1.is_node('B'));
	REQUIRE(g1.is_node('C'));
	REQUIRE(g1.is_node('D'));
	REQUIRE(g1.is_node('E'));
	REQUIRE(!g1.is_node('l'));
}

TEST_CASE("is edge") {
	using graph = gdwg::graph<char, int>;
	auto const v = std::vector<graph::value_type>{{'A', 'B', 4},
	                                              {'A', 'C', 10},
	                                              {'A', 'D', 3},
	                                              {'B', 'B', 4},
	                                              {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
	}
	REQUIRE(g1.is_edge('A', 'B', 4));
	REQUIRE(g1.is_edge('A', 'C', 10));
	REQUIRE(g1.is_edge('A', 'D', 3));
	REQUIRE(g1.is_edge('B', 'B', 4));
	REQUIRE(g1.is_edge('C', 'A', 2));
}

TEST_CASE("is empty") {
	using graph = gdwg::graph<char, int>;
	auto g = graph{};
	REQUIRE(g.empty());
	REQUIRE(g.node_size() == 0);
	REQUIRE(g.edge_size() == 0);
}

TEST_CASE("is_connected") {
	using graph = gdwg::graph<char, int>;
	auto const v = std::vector<graph::value_type>{{'A', 'B', 4},
	                                              {'A', 'C', 10},
	                                              {'A', 'D', 3},
	                                              {'B', 'B', 4},
	                                              {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
	}

	SECTION("basic case") {
		REQUIRE(g1.is_connected('A', 'B'));
		REQUIRE(g1.is_connected('A', 'C'));
		REQUIRE(g1.is_connected('A', 'D'));
		REQUIRE(g1.is_connected('B', 'B'));
		REQUIRE(g1.is_connected('C', 'A'));
	}
	SECTION("error case") {
		auto const n1 = 'x';
		auto const n2 = 'y';
		REQUIRE_THROWS_WITH(!g1.is_connected('A', n2),
		                    "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't "
		                    "exist in the graph");
		REQUIRE_THROWS_WITH(!g1.is_connected(n1, 'B'),
		                    "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't "
		                    "exist in the graph");
		REQUIRE_THROWS_WITH(!g1.is_connected(n1, n2),
		                    "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't "
		                    "exist in the graph");
	}
}

TEST_CASE("list all the nodes") {
	using graph = gdwg::graph<int, int>;
	auto g = graph{};
	g.insert_node(10);
	g.insert_node(2);
	g.insert_node(8);
	auto wanted = std::vector<int>{2, 8, 10};
	REQUIRE(g.nodes() == wanted);

	auto g_emp = graph{};
	auto ept = std::vector<int>{};
	REQUIRE(g_emp.nodes() == ept);
}

TEST_CASE("list the weights") {
	using graph = gdwg::graph<char, int>;
	auto const v = std::vector<graph::value_type>{{'A', 'B', 4},
	                                              {'A', 'C', 10},
	                                              {'A', 'B', 20},
	                                              {'A', 'B', 30},
	                                              {'A', 'D', 3},
	                                              {'B', 'B', 4},
	                                              {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
	}

	SECTION("basic case") {
		auto wanted = std::vector<int>{4, 20, 30};
		REQUIRE(g1.weights('A', 'B') == wanted);
		auto empty_v = std::vector<int>{};
		REQUIRE(g1.weights('B', 'D') == empty_v);
	}
	SECTION("error case") {
		auto n1 = 'x';
		auto n2 = 'y';
		REQUIRE_THROWS_WITH(g1.weights('A', n2),
		                    "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist "
		                    "in the graph");
		REQUIRE_THROWS_WITH(g1.weights(n1, 'B'),
		                    "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist "
		                    "in the graph");
		REQUIRE_THROWS_WITH(g1.weights(n1, n2),
		                    "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist "
		                    "in the graph");
	}
}

TEST_CASE("find") {
	using graph = gdwg::graph<char, int>;
	auto const v = std::vector<graph::value_type>{{'A', 'B', 4},
	                                              {'A', 'C', 10},
	                                              {'A', 'D', 3},
	                                              {'B', 'B', 4},
	                                              {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
	}
	REQUIRE(g1.find('A', 'B', 4) == g1.begin());
	REQUIRE(g1.find('A', 'C', 10) == ++g1.begin());
}

TEST_CASE("connections") {
	using graph = gdwg::graph<char, int>;
	auto const v =
	   std::vector<graph::value_type>{{'A', 'B', 4}, {'A', 'D', 3}, {'B', 'B', 4}, {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
	}
	SECTION("basic case") {
		auto wanted = std::vector<char>{'B', 'D'};
		auto ept = std::vector<char>{};
		REQUIRE(g1.connections('A') == wanted);
		REQUIRE(g1.connections('E') == ept);
	}
	SECTION("error case") {
		auto const n = 'x';
		REQUIRE_THROWS_WITH(g1.connections(n),
		                    "Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the "
		                    "graph");
	}
}

TEST_CASE("nodes number") {
	using graph = gdwg::graph<char, int>;
	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	REQUIRE(g1.node_size() == 5);
}

TEST_CASE("edges number") {
	using graph = gdwg::graph<char, int>;
	auto const v =
	   std::vector<graph::value_type>{{'A', 'B', 4}, {'A', 'D', 3}, {'B', 'B', 4}, {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	auto g1 = graph{};
	for (auto node : nv) {
		g1.insert_node(node);
	}
	for (auto const& [from, to, weight] : v) {
		g1.insert_edge(from, to, weight);
	}
	REQUIRE(g1.edge_size() == 4);
}