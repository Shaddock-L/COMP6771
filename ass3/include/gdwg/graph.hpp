#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP
#include <iostream>
#include <iterator>
#include <set>
#include <vector>
// TODO: Make this graph generic
//       ... this won't just compile
//       straight away
namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		class iterator;
		struct value_type {
			N from;
			N to;
			E weight;
		};
		struct edge {
			std::shared_ptr<N> from;
			std::shared_ptr<N> to;
			std::shared_ptr<E> weight;
		};
		// 2.2 Constructors
		graph() = default;

		graph(std::initializer_list<N> il)
		: graph(il.begin(), il.end()) {}

		template<typename InputIt>
		graph(InputIt first, InputIt last) {
			for (auto it = first; it != last; ++it) {
				insert_node(*it);
			}
		}

		graph(graph&& other) noexcept {
			// reset the value
			node_.clear();
			edge_.clear();
			std::swap(node_, other.node_);
			std::swap(edge_, other.edge_);
		}

		auto operator=(graph&& other) noexcept -> graph& {
			// reset the value
			node_.clear();
			edge_.clear();
			std::swap(node_, other.node_);
			std::swap(edge_, other.edge_);
			return *this;
		}

		graph(graph const& other) {
			for (auto it_n : other.node_) {
				insert_node(*it_n);
			}

			for (auto eg : other.edge_) {
				auto src = *eg->from;
				auto dst = *eg->to;
				auto w = *eg->weight;
				insert_edge(src, dst, w);
			}
		}

		auto operator=(graph const& other) -> graph& {
			if (*this == other) {
				return *this;
			}

			node_.clear();
			edge_.clear();

			for (auto it_n : other.node_) {
				insert_node(*it_n);
			}

			for (auto it_e : other.edge_) {
				auto src = *(node_.find(*(it_e->from)));
				auto dst = *(node_.find(*(it_e->to)));
				auto w = *(it_e->weight);
				insert_edge(*src, *dst, w);
			}

			return *this;
		}

		// 2.3 Modifiers
		auto insert_node(N const& value) -> bool {
			auto ist_or_not = node_.emplace(std::make_shared<N>(value));
			return ist_or_not.second;
		}

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (is_node(src) and is_node(dst)) {
				auto ist_edge = edge{*(node_.find(src)), *(node_.find(dst)), std::make_shared<E>(weight)};
				auto ist_or_not = edge_.emplace(std::make_shared<edge>(ist_edge));
				return ist_or_not.second;
			}
			else {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or dst node does not exist");
			}
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (!is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
			}
			else {
				if (is_node(new_data)) {
					return false;
				}
				else {
					auto old_n = node_.find(old_data);
					// auto new_n = std::make_shared<N>(new_data);
					insert_node(new_data);
					auto temp_edge_from = std::set<std::shared_ptr<edge>, CompareEdge>();
					auto temp_edge_to = std::set<std::shared_ptr<edge>, CompareEdge>();

					// replace outgoing first
					for (auto it_1 : edge_) {
						if (*(it_1->from) == old_data) {
							temp_edge_from.emplace(
							   std::make_shared<edge>(edge{it_1->from, it_1->to, it_1->weight}));
						}
					}
					for (auto it_2 : temp_edge_from) {
						insert_edge(new_data, *(it_2->to), *(it_2->weight));
						edge_.erase(it_2);
					}

					// replace incoming
					for (auto it_3 : edge_) {
						if (*(it_3->to) == old_data) {
							temp_edge_to.emplace(
							   std::make_shared<edge>(edge{it_3->from, it_3->to, it_3->weight}));
						}
					}
					for (auto it_4 : temp_edge_to) {
						insert_edge(*(it_4->from), new_data, *(it_4->weight));
						edge_.erase(it_4);
					}

					node_.erase(old_n);

					return true;
				}
			}
		}

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (is_node(old_data) and is_node(new_data)) {
				auto old_n = node_.find(old_data);
				auto temp_edge_from = std::set<std::shared_ptr<edge>, CompareEdge>();
				auto temp_edge_to = std::set<std::shared_ptr<edge>, CompareEdge>();

				for (auto it_1 : edge_) {
					if (*(it_1->from) == old_data) {
						temp_edge_from.emplace(
						   std::make_shared<edge>(edge{it_1->from, it_1->to, it_1->weight}));
					}
				}
				for (auto it_2 : temp_edge_from) {
					insert_edge(new_data, *(it_2->to), *(it_2->weight));
					edge_.erase(it_2);
				}

				// replace incoming
				for (auto it_3 : edge_) {
					if (*(it_3->to) == old_data) {
						temp_edge_to.emplace(
						   std::make_shared<edge>(edge{it_3->from, it_3->to, it_3->weight}));
					}
				}
				for (auto it_4 : temp_edge_to) {
					insert_edge(*(it_4->from), new_data, *(it_4->weight));
					edge_.erase(it_4);
				}

				node_.erase(old_n);
			}
			else {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}
		}

		auto erase_node(N const& value) -> bool {
			if (is_node(value)) {
				auto to_erase = node_.find(value);

				auto es_edge = std::vector<std::shared_ptr<edge>>{};

				for (auto x : edge_) {
					if (*((x)->from) == value or *((x)->to) == value) {
						es_edge.emplace_back(x);
					}
				}

				for (auto y : es_edge) {
					edge_.erase(y);
				}

				node_.erase(to_erase);
				return true;
			}
			else {
				return false;
			}
		}

		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (is_node(src) and is_node(dst)) {
				auto es_edge = std::vector<std::shared_ptr<edge>>{};
				for (auto x : edge_) {
					if (*((x)->from) == src and *((x)->to) == dst and *((x)->weight) == weight) {
						es_edge.emplace_back(x);
					}
				}
				for (auto y : es_edge) {
					edge_.erase(y);
				}
				return true;
			}
			else {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they don't exist in the graph");
			}
		}

		auto erase_edge(iterator i) -> iterator {
			auto copy = i.edge_iter_;
			++i.edge_iter_;
			edge_.erase(copy);
			return i;
		}

		auto erase_edge(iterator i, iterator s) -> iterator {
			edge_.erase(i.edge_iter_, s.edge_iter_);
			return s;
		}

		auto clear() noexcept -> void {
			node_.clear();
			edge_.clear();
		}

		// 2.4 Accessors
		[[nodiscard]] auto is_node(N const& value) const -> bool {
			auto find_or_not = node_.find(value);
			if (find_or_not == node_.end()) {
				return false;
			}
			else {
				return true;
			}
		}

		[[nodiscard]] auto is_edge(N const& src, N const& dst, E const& weight) const -> bool {
			if (is_node(src) and is_node(dst)) {
				for (auto it : edge_) {
					if (*(it->from) == src and *(it->to) == dst and *(it->weight) == weight) {
						return true;
					}
				}
				return false;
			}
			else {
				return false;
			}
		}

		[[nodiscard]] auto empty() const -> bool {
			return node_.empty();
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
			if (is_node(src) and is_node(dst)) {
				for (auto it : edge_) {
					if (*(it->from) == src and *(it->to) == dst) {
						return true;
					}
				}
				return false;
			}
			else {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
				                         "node don't exist in the graph");
			}
		}

		[[nodiscard]] auto nodes() const -> std::vector<N> {
			auto node_vct = std::vector<N>();
			for (auto it : node_) {
				node_vct.emplace_back(*(it));
			}
			return node_vct;
		}

		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
			if (is_node(src) and is_node(dst)) {
				auto weight_vct = std::vector<E>();
				for (auto it : edge_) {
					if (*(it->from) == src and *(it->to) == dst) {
						weight_vct.emplace_back(*(it->weight));
					}
				}
				return weight_vct;
			}
			else {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't exist in the graph");
			}
		}

		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const -> iterator {
			// auto find_it = iterator();
			for (auto it = edge_.begin(); it != edge_.end(); ++it) {
				if (*((*it)->from) == src and *((*it)->to) == dst and *((*it)->weight) == weight) {
					return iterator(it);
				}
			}

			return iterator(edge_.end());
		}

		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			if (is_node(src)) {
				auto cnt_nodes = std::vector<N>();

				for (auto egs : edge_) {
					if (*(egs->from) == src) {
						if (is_connected(src, *(egs->to)))
							cnt_nodes.emplace_back(*(egs->to));
					}
				}
				return cnt_nodes;
			}
			else {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't "
				                         "exist in the graph");
			}
		}

		[[nodiscard]] auto node_size() const -> int {
			return static_cast<int>(node_.size());
		}

		[[nodiscard]] auto edge_size() const -> int {
			return static_cast<int>(edge_.size());
		}

		// Iterator access
		[[nodiscard]] auto begin() const -> iterator {
			return iterator(edge_.begin());
		}

		[[nodiscard]] auto end() const -> iterator {
			return iterator(edge_.end());
		}

		[[nodiscard]] auto operator==(graph const& other) const -> bool {
			if (node_.size() != other.node_.size() or edge_.size() != other.edge_.size()) {
				return false;
			}
			else {
				auto it_n = node_.begin();
				auto it_n_oth = other.node_.begin();

				while (it_n != node_.end()) {
					if (**it_n != **it_n_oth) {
						return false;
					}
					else {
						++it_n;
						++it_n_oth;
					}
				}
				auto it_e = edge_.begin();
				auto it_e_oth = other.edge_.begin();
				while (it_e != edge_.end()) {
					if (*((*it_e)->from) != *((*it_e_oth)->from) or *((*it_e)->to) != *((*it_e_oth)->to)
					    or *((*it_e)->weight) != *((*it_e_oth)->weight))
					{
						return false;
					}
					else {
						++it_e;
						++it_e_oth;
					}
				}

				return true;
			}
		}

		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			for (auto it_n : g.node_) {
				os << *it_n << " (\n";
				for (auto it_e : g.edge_) {
					if (*(it_e->from) == *it_n) {
						os << "  " << *(it_e->to) << " | " << *(it_e->weight) << "\n";
					}
				}
				os << ")\n";
			}

			return os;
		}

	private:
		/* sorting by nodes value */
		struct CompareNode {
			using is_transparent = void;
			auto operator()(std::shared_ptr<N> const& n1, std::shared_ptr<N> const& n2) const -> bool {
				return *n1 < *n2;
			}
			auto operator()(N const& n1, std::shared_ptr<N> const& n2) const -> bool {
				return n1 < *n2;
			}
			auto operator()(std::shared_ptr<N> const& n1, N const& n2) const -> bool {
				return *n1 < n2;
			}
		};
		/* sorting edges by the order of from/ to/ weight. */
		struct CompareEdge {
			using is_transparent = void;
			auto operator()(std::shared_ptr<edge> const& e1, std::shared_ptr<edge> const& e2) const
			   -> bool {
				if (*(e1->from) != *(e2->from)) {
					return *(e1->from) < *(e2->from);
				}
				else if (*(e1->to) != *(e2->to)) {
					return *(e1->to) < *(e2->to);
				}
				else {
					return *(e1->weight) < *(e2->weight);
				}
			}
		};
		std::set<std::shared_ptr<N>, CompareNode> node_;
		std::set<std::shared_ptr<edge>, CompareEdge> edge_;

		// iter

	public:
		class iterator {
		public:
			using value_type = graph<N, E>::value_type;
			using reference = value_type;
			using pointer = void;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;
			// Iterator constructor
			iterator() = default;
			// Iterator source
			auto operator*() const -> reference {
				return value_type{*((*edge_iter_)->from), *((*edge_iter_)->to), *((*edge_iter_)->weight)};
			}
			// Iterator traversal
			auto operator++() -> iterator& {
				++edge_iter_;
				return *this;
			}
			auto operator++(int) -> iterator {
				auto copy{*this};
				++(*this);
				return copy;
			}
			auto operator--() -> iterator& {
				--edge_iter_;
				return *this;
			}
			auto operator--(int) -> iterator {
				auto copy{*this};
				--(*this);
				return copy;
			}
			// Iterator comparison
			auto operator==(iterator const& other) const -> bool

			{
				return (this->edge_iter_ == other.edge_iter_);
			}

		private:
			typename std::set<std::shared_ptr<edge>, CompareEdge>::iterator edge_iter_;
			explicit iterator(typename std::set<std::shared_ptr<edge>, CompareEdge>::iterator it)
			: edge_iter_{it} {}
			friend class graph<N, E>;
		};
	};
} // namespace gdwg

// namespace gdwg

#endif // GDWG_GRAPH_HPP