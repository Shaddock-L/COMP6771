#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("insert nodes") {
	using graph = gdwg::graph<double, int>;

	SECTION("basic insertion") {
		auto g = graph{};
		const auto n = 8.8;
		auto first_ist = g.insert_node(n);
		auto already_stored = g.insert_node(n);
		REQUIRE(g.is_node(n));
		REQUIRE(first_ist == true);
		REQUIRE(already_stored == false);
	}
	SECTION("empty insertion") {
		auto g2 = gdwg::graph<std::string, int>{};
		const auto ep = "";
		auto first_ist_2 = g2.insert_node(ep);
		auto already_stored_empty = g2.insert_node(ep);
		REQUIRE(g2.is_node(ep));
		REQUIRE(first_ist_2 == true);
		REQUIRE(already_stored_empty == false);
	}
}

TEST_CASE("insert edges") {
	using graph = gdwg::graph<std::string, int>;
	auto const v = std::vector<graph::value_type>{{"hello", "world", 4},
	                                              {"hello", "world", 10},
	                                              {"hello", "sydney", 5},
	                                              {"goodbye", "Australia", 8}};
	auto const nv = std::vector<std::string>{"hello", "world", "sydney", "goodbye", "Australia"};
	SECTION("successful insertion") {
		auto g = graph{};
		for (auto node : nv) {
			g.insert_node(node);
		}

		for (auto const& [from, to, weight] : v) {
			g.insert_edge(from, to, weight);
		}
		REQUIRE(g.is_connected("hello", "world"));
		REQUIRE(g.is_connected("hello", "sydney"));
		REQUIRE(!g.is_connected("hello", "Australia"));
		auto w1 = std::vector<int>{4, 10};
		REQUIRE(g.weights("hello", "world") == w1);

		auto dup_ist = g.insert_edge("hello", "world", 4);
		REQUIRE(dup_ist == false);
	}
	SECTION("unsuccessful insertion") {
		auto g2 = graph{};
		g2.insert_node("Kevin");
		auto const fake_src = "Alice";
		auto const fake_dst = "Bob";
		REQUIRE_THROWS_WITH(g2.insert_edge(fake_src, "Kevin", 1),
		                    "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node "
		                    "does not exist");
		REQUIRE_THROWS_WITH(g2.insert_edge("Kevin", fake_dst, 2),
		                    "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node "
		                    "does not exist");
		REQUIRE_THROWS_WITH(g2.insert_edge(fake_src, fake_dst, 3),
		                    "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node "
		                    "does not exist");
	}
}

TEST_CASE("replace node") {
	using graph = gdwg::graph<std::string, int>;
	auto const v = std::vector<graph::value_type>{{"hello", "world", 4},
	                                              {"hello", "world", 10},
	                                              {"hello", "sydney", 5},
	                                              {"goodbye", "Australia", 8}};
	auto const nv = std::vector<std::string>{"hello", "world", "sydney", "goodbye", "Australia"};

	SECTION("successful replace") {
		auto g1 = graph{};
		for (auto node : nv) {
			g1.insert_node(node);
		}

		for (auto const& [from, to, weight] : v) {
			g1.insert_edge(from, to, weight);
		}
		auto const new_data_1 = "melbourne";
		auto init_cnt_nodes = g1.connections("sydney");
		g1.replace_node("sydney", new_data_1);
		REQUIRE(!g1.is_node("sydney"));
		auto new_cnt = g1.connections(new_data_1);
		REQUIRE(init_cnt_nodes == new_cnt);
	}
	SECTION("unsuccessful replace (new_data already exists as a node)") {
		auto g2 = graph{};
		for (auto node : nv) {
			g2.insert_node(node);
		}

		for (auto const& [from, to, weight] : v) {
			g2.insert_edge(from, to, weight);
		}
		auto const new_data_2 = "hello";

		REQUIRE(g2.replace_node("world", new_data_2) == false);
	}
	SECTION("unsuccessful replace (old_data is not a node)") {
		auto g3 = graph{};
		for (auto node : nv) {
			g3.insert_node(node);
		}

		for (auto const& [from, to, weight] : v) {
			g3.insert_edge(from, to, weight);
		}
		auto const fake_old = "brisbane";
		REQUIRE_THROWS_WITH(g3.replace_node(fake_old, "kk"),
		                    "Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't "
		                    "exist");
	}
}

TEST_CASE("merge and replace node") {
	using graph = gdwg::graph<char, int>;
	auto const v = std::vector<graph::value_type>{{'A', 'B', 4},
	                                              {'A', 'C', 10},
	                                              {'A', 'D', 3},
	                                              {'B', 'B', 4},
	                                              {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};
	SECTION("basic case") {
		auto g1 = graph{};
		for (auto node : nv) {
			g1.insert_node(node);
		}
		for (auto const& [from, to, weight] : v) {
			g1.insert_edge(from, to, weight);
		}
		auto A_cnt_nodes = g1.connections('A');
		g1.merge_replace_node('A', 'E');
		REQUIRE(!g1.is_node('A'));
		REQUIRE(g1.is_connected('C', 'E'));
		auto E_cnt_nodes = g1.connections('E');
		REQUIRE(E_cnt_nodes == A_cnt_nodes);
	}
	SECTION("duplicate case") {
		auto g2 = graph{};
		for (auto node : nv) {
			g2.insert_node(node);
		}
		for (auto const& [from, to, weight] : v) {
			g2.insert_edge(from, to, weight);
		}
		auto init_size = g2.edge_size();
		g2.merge_replace_node('A', 'B');
		auto merged_size = g2.edge_size();
		REQUIRE(merged_size == (init_size - 1));
	}
	SECTION("error case") {
		auto g3 = graph{};
		for (auto node : nv) {
			g3.insert_node(node);
		}
		for (auto const& [from, to, weight] : v) {
			g3.insert_edge(from, to, weight);
		}
		auto const n1 = 'K';
		auto const n2 = 'Z';
		REQUIRE_THROWS_WITH(g3.merge_replace_node(n1, 'A'),
		                    "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if "
		                    "they don't exist in the graph");
		REQUIRE_THROWS_WITH(g3.merge_replace_node('A', n1),
		                    "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if "
		                    "they don't exist in the graph");
		REQUIRE_THROWS_WITH(g3.merge_replace_node(n1, n2),
		                    "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if "
		                    "they don't exist in the graph");
	}
	SECTION("self pointer") {
		auto g4 = graph{};
		for (auto node : nv) {
			g4.insert_node(node);
		}
		for (auto const& [from, to, weight] : v) {
			g4.insert_edge(from, to, weight);
		}
		g4.insert_node('F');
		g4.insert_edge('F', 'F', 3);
		g4.merge_replace_node('F', 'A');
		REQUIRE(g4.is_connected('A', 'A'));
	}
}

TEST_CASE("erase node") {
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

	g1.erase_node('A');
	REQUIRE(!g1.is_node('A'));
	REQUIRE(!g1.is_edge('A', 'B', 4));
	REQUIRE(!g1.is_edge('C', 'A', 2));
}

TEST_CASE("erase edge (by contents)") {
	using graph = gdwg::graph<char, int>;
	auto const v = std::vector<graph::value_type>{{'A', 'B', 4},
	                                              {'A', 'C', 10},
	                                              {'A', 'D', 3},
	                                              {'B', 'B', 4},
	                                              {'C', 'A', 2}};

	auto const nv = std::vector<char>{'A', 'B', 'C', 'D', 'E'};

	SECTION("successful erase") {
		auto g1 = graph{};
		for (auto node : nv) {
			g1.insert_node(node);
		}
		for (auto const& [from, to, weight] : v) {
			g1.insert_edge(from, to, weight);
		}

		for (auto const& [from, to, weight] : v) {
			g1.erase_edge(from, to, weight);
		}
		REQUIRE(g1.edge_size() == 0);
	}

	SECTION("error case") {
		auto g2 = graph{};
		for (auto node : nv) {
			g2.insert_node(node);
		}
		for (auto const& [from, to, weight] : v) {
			g2.insert_edge(from, to, weight);
		}
		auto const n1 = 'm';
		auto const n2 = 'p';
		REQUIRE_THROWS_WITH(g2.erase_edge(n1, 'A', 5),
		                    "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't "
		                    "exist in the graph");
		REQUIRE_THROWS_WITH(g2.erase_edge('A', n2, 5),
		                    "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't "
		                    "exist in the graph");
		REQUIRE_THROWS_WITH(g2.erase_edge(n1, n2, 5),
		                    "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't "
		                    "exist in the graph");
	}
}

TEST_CASE("erase edge (by iterator)") {
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
	g1.erase_edge(g1.begin());
	REQUIRE(!g1.is_edge('A', 'B', 4));
}

TEST_CASE("erase edge (by iterator interval)") {
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
	auto i = g1.find('A', 'B', 4);
	auto s = g1.find('A', 'D', 3);
	auto erase_rst = g1.erase_edge(i, s);
	auto wanted = g1.find('A', 'D', 3);
	REQUIRE(!g1.is_edge('A', 'B', 4));
	REQUIRE(!g1.is_edge('A', 'C', 10));
	REQUIRE(g1.is_edge('A', 'D', 3));
	REQUIRE(erase_rst == wanted);
}

TEST_CASE("clear") {
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
	g1.clear();
	REQUIRE(g1.empty());
}