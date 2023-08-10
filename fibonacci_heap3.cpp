#include <iostream>
#include <cassert>
#include <vector>

namespace lib
{

template <typename Tp>
class dlist
{
public:
	struct Node
	{
		Node *prev{nullptr};
		Node *next{nullptr};
		Tp    key{};
		uint64_t id{0};
	};
public:
	dlist() = default;

	void push_at(size_t idx, Tp key)
	{
		Node *new_node = node_ctor(key);
		insert_at_idx(idx, new_node);
	}

	void pop_at(size_t idx)
	{ delete_at_idx(idx); }

	void append(dlist<Tp>& dl)
	{
		root_->prev->next = dl.root_;
		dl.root_->prev = root_->prev;

		Node *n = dl.M_get_node(dl.size() - 1);
		// printf("n_d(%d)\n", n->key);
		n->next = root_;
		root_->prev = n;

		sz_ += dl.size();
	}

	void debug()
	{
		FILE *f = fopen("graph.dot", "w");
		fprintf(f, "digraph g {\n");

		print_nodes(root_, f);
		print_connections(root_, f);

		fprintf(f, "}\n");
		fclose(f);

		system("dot -Tpng graph.dot -o graph.png");
	}

	void delete_node(Tp el)
	{
		Node *search_node = root_;
		size_t idx = 0;

		for (size_t el_idx = 0; el_idx < sz_; ++el_idx) {
			if (el->id == search_node->key->id) {
				idx = el_idx;
				break;
			}

			search_node = search_node->next;
		}
		printf("idx of node(%zu)\n", idx);

		delete_at_idx(idx);
	}

			Tp& operator[](size_t idx)       { return M_get_node(idx)->key; }
	const	Tp& operator[](size_t idx) const { return M_get_node(idx)->key; }

public:
	Node *node_ctor(Tp key)
	{
		Node *n = new Node;
		n->key = key;
		n->next = n;
		n->prev = n;
		n->id = (uint64_t)n;

		return n;
	}

	void insert_at_idx(size_t idx, Node *n)
	{
		if (root_ == nullptr) {
			root_ = n;
			// tail_ = n;
			++sz_;
			return;
		} else {
			if (idx == 0) {
				    n->next = root_;
				n->prev = root_->prev;
				root_->prev->next = n;
				root_->prev = n;
				
				root_ = n;

				++sz_;
				return;
			}
		}

		Node *node_after = root_;
		for (size_t el_idx = 0; el_idx < idx - 1; ++el_idx) {
			if (el_idx == sz_ - 1) {
				break;
			}

			node_after = node_after->next;
		}

		Node *after_next = nullptr;
		if (node_after->next != nullptr) {
			after_next = node_after->next;
		}

		node_after->next = n;
		n->prev = node_after;

		if (after_next != nullptr) {
			after_next->prev = n;
			n->next = after_next;
		}
		++sz_;
	}

	void delete_at_idx(size_t idx)
	{
		if (root_ == nullptr) {
			sz_ = 0;
			return;
		} else {
			if (idx == 0) {
				Node *old_root = root_;
				root_ = root_->next;

				old_root->prev->next = root_;
				root_->prev = old_root->prev;

				old_root->~Node();

				--sz_;
				return;
			}
		}

		Node *node_after = root_;
		for (size_t el_idx = 0; el_idx < idx; ++el_idx) {
			if (el_idx == sz_ - 1) {
				break;
			}

			node_after = node_after->next;
		}


		Node *after_next = nullptr;
		if (node_after->next != nullptr) {
			node_after->next->prev = node_after->prev;
			node_after->prev->next = node_after->next;
		} else {
			node_after->prev->next = nullptr;
			node_after->~Node();
		}

		--sz_;
	}

 	void print_connections(Node *n, FILE *f)
	{
		if (n == nullptr) {
			return;
		}

		if (n->next != nullptr) {
			fprintf(f,
				"\tn%lu -> n%lu\n\n",
				n->id, n->next->id);
		}

		if (n->prev != nullptr) {
			fprintf(f,
				"\tn%lu -> n%lu\n\n",
				n->id, n->prev->id);
		}

		if (n->next != nullptr) {
			if (n->next != root_) {
				print_connections(n->next, f);
			}
		}
	}

	void print_nodes(Node *n, FILE *f)
	{
		if (n == nullptr) {
			return;
		}

		if (std::is_pointer<Tp>() && !n->key) {
			fprintf(f,
				"\tn%lu[shape=\"rectangle\" label=\"%s\"]\n\n",
				n->id, "nullptr");

		} else {
			fprintf(f,
				"\tn%lu[shape=\"rectangle\" label=\"%ld\"]\n\n",
				n->id, (int64_t)n->key);
		}

		if (n->next != nullptr) {
			if (n->next != root_) {
				print_nodes(n->next, f);
			}
		}
	}

	Node *M_get_node(uint64_t idx)
	{
		Node *cur_node = root_;
		for (uint64_t cur_i = 0; cur_i < idx; ++cur_i) {
			if (cur_node->next == nullptr) {
				return nullptr;
			}

			cur_node = cur_node->next;
		}

		return cur_node;
	}

	size_t size()
	{ return sz_; }

public:
	Node  *root_{nullptr};
	// Node  *tail_{nullptr};
	size_t sz_{0};
};
}; // end of lib

namespace lib
{
template <typename Tp>
class fheap
{
public:
	struct Node
	{

		Tp key{};
		uint64_t id{0};
		bool is_coorupted{false};
		dlist<Node*> children{};
	};

	using BinT = typename dlist<Node*>::Node *;

	void insert(Tp key)
	{
		Node *new_node = NodeCtor(key);
		trees_.push_at(trees_.size(), new_node);
		++sz_;

		if (min == nullptr) {
			min = trees_.root_;
		}else {
			// int a = GetKey(min->key);
			min = (GetKey(min->key) < key)? min : trees_.M_get_node(trees_.size() - 1);
		}
	}

	size_t size()
	{ return sz_; }

	Tp get_min()
	{ return GetKey(min->key); }

	void remove_min()
	{

		if (trees_.root_ == nullptr) {
			return;
		}

		printf("=============\n");

		typename dlist<Node*>::Node *old_min = min;
		if (trees_.root_ == min) {
			trees_.root_ = trees_.root_->next;
		}

		// Node *n = min->key;

		if (min->key->children.size() > 0) {
			trees_.append(min->key->children);

			////////////////////////////////////
			// typename dlist<Node*>::Node *save = min->key->children.root_;
			// printf("save_d(%d)\n", save->key->key);

			// // save->prev = trees_.root_->prev;
			// trees_.root_->prev->next = save;

			// printf("size of children:(%zu)\n", min->key->children.size());

			// typename dlist<Node*>::Node *last_n = min->key->children.M_get_node(min->key->children.size() - 1);
			// printf("save_d(%d)\n", last_n->key->key);

			// trees_.root_->prev = last_n;
			// printf("?\n");
			// last_n->next = trees_.root_;

			////////////////////////////////////

			// trees_.sz_ += min->key->children.size();

			// printf("next_next_d(%d)\n", save->next->next->key->key);


			// printf("size(%zu)\n", trees_.size());
			// lib::dlist<lib::fheap<int>::Node*>::Node *debug_node = trees_.root_;
			// for (size_t cur_el = 0; cur_el < trees_.size(); ++cur_el) {
			// 	printf("el:(%d)\n", debug_node->key->key);
			// 	debug_node = debug_node->next;
			// }
			// exit(0);
		}


		min->prev->next = min->next;
		min->next->prev = min->prev;
		--trees_.sz_;

		if (sz_ == 1) {
			trees_.root_ = min->next;
		}

		sz_ -= (1 - min->key->children.size());
		min = min->next;

		printf("new sz:(%zu)\n", sz_);
		if (sz_ == 3) {
			Debug();
			// exit(0);
		}


		bool is_changed = true;
		std::vector<BinT> ptrs(sz_, nullptr);

		// Current binomial tree.
		BinT c_list_cell = min;
		while (is_changed) {
			is_changed = false;

			Node *n = c_list_cell->key;
			printf("cur_c_data:(%d)\n", n->key);
			printf("sz_:(%zu)\n", sz_);

			if (ptrs[n->children.size()] == nullptr) {
				ptrs[n->children.size()] = c_list_cell;
				is_changed = true;

				min = (c_list_cell->next->key->key < min->key->key)? c_list_cell->next : min;

				c_list_cell = c_list_cell->next;

				// printf("\t?next data(%d)\n", c_list_cell->key->key);

				printf("we save node_w_d:(%d) with idx(%zu)\n\n",
					ptrs[n->children.size()]->key->key,
					n->children.size());

				continue;

			} else {
				if (c_list_cell == ptrs[n->children.size()]) {
					continue;
				}

				is_changed = true;
				if (GetKey(n) < GetKey(ptrs[n->children.size()]->key) ) {
					// printf("???\n");
					// min = (lower_node->key->key < min->key->key)? lower_node : min;

					Node *to_push = ptrs[n->children.size()]->key;

					ptrs[n->children.size() + 1] = c_list_cell;
					ptrs[n->children.size()] = nullptr;
					
					n->children.push_at(
						n->children.size(),
						to_push);


					trees_.delete_node(to_push);
					--sz_;
				} else {
					size_t n_was_children = n->children.size();
					// Lower node placed at vector previously met.
					Node *lower_node = ptrs[n->children.size()]->key;
					BinT  lower_binT = ptrs[n->children.size()];
					
					printf("we push node_w_d(%d) as child of(%d)\n",
						n->key,
						lower_node->key);

					// n->next = nullptr;
					// n->prev = nullptr;

					min = (lower_node->key < min->key->key)? ptrs[n->children.size()] : min;

					// If we have situation when 
					// previous node in this cell (in vector) has
					// lower key we push node to it's chilren list(on last place) and
					// remove node from list
					lower_node->children.push_at(
						// push at last place of children list of lower node;
						ptrs[n->children.size()]->key->children.size(),
						// New meeted node.
						n);

					ptrs[n->children.size()] = nullptr;

					// lib::dlist<lib::fheap<int>::Node*>::Node *debug_node1 = trees_.root_;
					// for (size_t cur_el = 0; cur_el < trees_.size() - 1; ++cur_el) {
					// 	printf("el:(%d)\n", debug_node1->key->key);
					// 	debug_node1 = debug_node1->next;
					// }

					// We should delete this element from trees_ list.
					// printf("\tsz:(%zu)\n", trees_.size());
					trees_.delete_node(n);
					--sz_;
					// printf("\tsz:(%zu)\n", trees_.size());

					// printf("size(%zu)\n", trees_.size());
					// lib::dlist<lib::fheap<int>::Node*>::Node *debug_node = trees_.root_;
					// for (size_t cur_el = 0; cur_el < trees_.size() - 1; ++cur_el) {
					// 	printf("el:(%d)\n", debug_node->key->key);
					// 	debug_node = debug_node->next;
					// }
					// printf("?\n");

					Debug();
					// exit(0);

					trees_.debug();
					// Why lower_binT has next n???
					// printf("\t!next data:(%d)\n", lower_binT->key->key);
					// exit(0);

					ptrs[n_was_children] = nullptr;
					min = (lower_binT->key->key < min->key->key)? lower_binT : min;
					c_list_cell = lower_binT;
					continue;
				}
			}

			min = (c_list_cell->next->key->key < min->key->key)? c_list_cell : min;
			c_list_cell = c_list_cell->next;
		}
	}

	void Debug()
	{
		FILE *f = fopen("graph.dot", "w");
		fprintf(f, "digraph g {\n");

		for (size_t el_idx = 0; el_idx < trees_.size(); ++el_idx) {
			// printf("debug el_idx(%zu)\n", el_idx);
			PrintNodes(trees_[el_idx], f);
			PrintConnections(trees_[el_idx], f);
		}

		fprintf(f, "}\n");
		fclose(f);

		system("dot -Tpng graph.dot -o graph.png");
	}

public:
	Tp GetKey(Node *n)
	{ return n->key; }

	Node *NodeCtor(Tp key)
	{
		Node *new_node = new Node;
		new(&new_node->key) Tp(std::move(key));

		new_node->id  = (uint64_t)new_node;
		new_node->is_coorupted = false;

		return new_node;
	}

	void PrintConnections(Node *n, FILE *f)
	{
		if (n == nullptr) {
			return;
		}

		if (n->children.size() > 0) {
			for (size_t child_idx = 0; child_idx < n->children.size(); ++child_idx) {
				fprintf(f,
					"\tn%lu -> n%lu\n\n",
					n->id, n->children[child_idx]->id);
			}

			for (size_t child_idx = 0; child_idx < n->children.size(); ++child_idx) {
				PrintConnections(n->children[child_idx], f);
			}
		}
	}

	void PrintNodes(Node *n, FILE *f)
	{
		if (n == nullptr) {
			return;
		}

		fprintf(f,
			"\tn%lu[shape=\"rectangle\" label=\"%d\"]\n\n",
			n->id, n->key);

		if (n->children.size() > 0) {
			for (size_t child_idx = 0; child_idx < n->children.size(); ++child_idx) {
				PrintNodes(n->children[child_idx], f);
			}
		}
	}
public:
	dlist<Node*> trees_{};
	size_t sz_{0};
	typename dlist<Node*>::Node *min{nullptr};
};

}; // end of lib

int main()
{
	// Testing of doubly-linked list:
	// lib::dlist<int> dl;
	// dl.push_at(0, 1);
	// dl.push_at(1, 2);
	// dl.push_at(2, 3);
	// dl.push_at(3, 4);

	// lib::dlist<int> dl2;
	// dl2.push_at(0, 10);
	// dl2.push_at(1, 20);
	// dl2.push_at(2, 30);
	// dl2.push_at(3, 40);
	// // std::cout << dl[0] << '\n';
	// dl.append(dl2);
	// dl.debug();
	// exit(0);

	lib::fheap<int> fh;

	fh.insert(1);
	fh.insert(3);
	fh.insert(4);
	fh.insert(5);
	fh.insert(2);

	fh.insert(9);
	
	fh.insert(6);
	fh.insert(7);
	fh.insert(8);

	fh.remove_min();
	fh.remove_min();
	// fh.remove_min();
	// fh.remove_min();
	// fh.remove_min();
	// fh.remove_min();
	std::cout << fh.get_min() << '\n';

	fh.Debug();
	exit(0);
	
	std::cout << "size:" << fh.size() << '\n';
	std::cout << fh.get_min() << '\n';

	return 0;
}