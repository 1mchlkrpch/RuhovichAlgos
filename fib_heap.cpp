#include <iostream>

#include <cstdio>

#include <vector>
#include <initializer_list>

namespace lib
{
template <typename Tp>
class fheap;


template <typename Tp>
class cycled_dlist
{
	template <typename T>
	friend class fheap;

protected:
	struct cell
	{
		cell *next{nullptr};
		cell *prev{nullptr};

		Tp val{};

		uint64_t id{0};

		cell(Tp val): val(std::move(val)) {}

		void operator++() { this = this->next; }
	};

 public:
	cycled_dlist(): sz_(0) {};

	cycled_dlist(std::initializer_list<Tp> l)
	{
		size_t idx = 0;
		for (Tp el : l) {
			push_at(idx, el);
			++idx;
		}
	}

	void push_at(size_t idx, Tp val)
	{
		// Create new node using
		// cell's constructor which use
		// move-constructor for (cell->val) field
		// node is cycled on itself.
		cell *c = cell_ctor(val);

		// After that default emplacement
		// of the created node to particular index in list.
		// Indexing in array cosequent:
		// zero-index equal to place before first node in list
		// next index correspond to place between first and second node in list.
		emplace_at(idx, c);
	}

	void pop_at(int idx)
	{
		// If root_ cell has value 'nullptr'
		// so we have empty list
		if (root_ == nullptr) {
			return;
		}

		// just roll the nodes to get needed one
		// either with index 'idx' or in the tail of the list.
		cell *c = root_;
		for (int el_idx = 0; el_idx < idx && el_idx < (int)sz_ - 1; ++el_idx) {
			c = c->next;
		}

		cell *to_delete = extract_cell(c);
		to_delete->~cell();
	}

	void draw()
	{
		// Open the file and write all the node
		// in it. After that compile with graphviz engine.
		FILE *f = fopen("graph.dot", "w");

		// Print all cells of list recursively
		// in that file.
		// It is assumed that we will meet root_-cell
		// if we print it. In other words list should be either
		// finit either cycled with set root_-cel.
		fprintf(f, "digraph G {\n");
		fprintf_nodes(f, root_);
		fprintf_connections(f, root_);
		fprintf(f, "}\n");

		fclose(f);
		// Compile this graph with graphviz engline.
		system("dot -Tpng graph.dot -o graph.png");
	}

	size_t size()
	{ return sz_; }

	void print()
	{
		cell *c = root_;
		size_t el_idx = 0;
		for (; el_idx < sz_ - 1; ++el_idx) {
			printf("[%zu:%d] ",
				el_idx, c->val);
			c = c->next;
		}
		printf("[%zu:%d]\n",
			el_idx, c->val);
	}

	// Connections of two lists
	// in O(1) time complexity.
	void connect_with(cycled_dlist<Tp>& other)
	{
		cell *other_last = other.root_->prev;
		cell *last       = root_->prev;

		// From previous node to next node
		// direction connections:
		last->next = other.root_;
		other_last->next = root_;

		// Opposite side dirrection connections.
		root_->prev = other_last;
		other.root_->prev = last;

		sz_+= other.size();
	}

 private:
	cell *cell_ctor(Tp val)
	{
		cell *new_cell = new cell(val);
		new_cell->id = (uint64_t)new_cell;

		new_cell->next = new_cell;
		new_cell->prev = new_cell;

		return new_cell;
	}

	cell *emplace_at(size_t idx, cell *c)
	{
		++sz_;
		if (idx == 0) {
			if (root_ == nullptr) {				
				root_ = c;
				return root_;

			} else {
				// Here is that is assumed that
				// if (root_) was already inserted
				// so if that is the only node in list it will be
				// cycled on itself in each way.
				// It means that we can address to next, prev fields.
				return emplace_before(root_, c);;
			}
		}

		cell *after = root_;
		for (size_t el_idx = 0; el_idx < idx - 1 && el_idx < sz_ - 2; ++el_idx) {
			after = after->next;
		}

		// Emplaces after particular cell
		// and moves root_ cell if it's necessary.
		return emplace_after(after, c);
	}

	cell *emplace_after(cell *after, cell *c)
	{
		cell *cel_n = after->next;

		// From previous node to next node
		// direction connections:
		after->next = c;
		c->next = cel_n;

		// Opposite side dirrection connections.
		cel_n->prev = c;
		c->prev = after;

		return c;
	}

	cell *emplace_before(cell *before, cell *c)
	{
		if (root_ == before) {
			root_ = c;
		}

		cell *prev_c = before->prev;

		// From previous node to next node
		// direction connections:
		prev_c->next = c;
		c->next = before;

		// Opposite side dirrection connections.
		before->prev = c;
		c->prev = prev_c;

		return c;
	}

	cell *extract_cell(cell *c)
	{
		--sz_;
		if (c->next == c) {
			if (c == root_) {
				root_ = nullptr;
			}
		} else {
			if (c == root_) {
				root_ = root_->next;
			}

			c->prev->next = c->next;
			c->next->prev = c->prev;
		}

		c->next = c;
		c->prev = c;

		return c;
	}

	void fprintf_nodes(FILE *f, cell *c)
	{
		if (f == nullptr || c == nullptr) {
			return;
		}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
		
		fprintf(f, "\tn%lu[shape=\"rectangle\" label=\"%ld\"]\n\n",
			c->id, c->val);

#pragma GCC diagnostic pop

		if (c->next != root_) {
			fprintf_nodes(f, c->next);
		}
	}

	void fprintf_connections(FILE *f, cell *c)
	{
		if (f == nullptr || c == nullptr) {
			return;
		}

		fprintf(f, "\tn%lu->n%lu\n\n",
			c->id, c->next->id);

		fprintf(f, "\tn%lu->n%lu\n\n",
			c->id, c->prev->id);

		if (c->next != root_) {
			fprintf_connections(f, c->next);
		}
	}

 private:
	cell *root_{nullptr};
	size_t sz_{0};
};

}; // end of lib.

namespace lib
{

template <typename Tp>
class fheap
{
protected:
	struct node
	{
		Tp key{};
		uint64_t node_id{0};

		bool is_corupted{false};

		cycled_dlist<node*> children{};
		// node *ptr_to_root{nullptr};
		// typename cycled_dlist<lib::fheap<Tp>::node*>::cell *parent{nullptr};
	};

public:
	// It uses cycled list nodes to correspond to him while
	// recursion or in rebuilding of the heap.
	using dlist_cell_t = typename cycled_dlist<lib::fheap<Tp>::node*>::cell;
	// Use this type as list of children and main list of fibonacci heap.
	using dlist_t = cycled_dlist<node*>;

	fheap() = default;

	// Insert just push new element
	// to the last place in cycled list 'trees' so
	// Asymptotic of this operation is O(1).
	void insert(Tp el)
	{ dlist_cell_t *to_push = trees_.emplace_at(trees_.size(), trees_.cell_ctor(node_ctor(el)));
	  min = (trees_.sz_ == 1)? to_push : (to_push->val->key < min->val->key)? to_push : min;
	}

	Tp get_min()
	{ return min->val->key; }

	// Debug function.
	// Draws simple graph of fibonacci heap.
	void draw()
	{
		FILE *f = fopen("graph.dot", "w");

		fprintf(f, "digraph HeapGraph {\n");

		// Use this spointer to current cell in 'trees_'-list
		// we will move this cell forward: c = c->next and
		// draw fibonacci tree with root in that node.
		dlist_cell_t *c = trees_.root_;
		
		// Firstly draw all the nodes in
		// Particular fibonacci tree and after that
		// connects this node to other nodes in 'trees_'-lilst.
		for (size_t el_idx = 0; el_idx < trees_.size(); ++el_idx) {
			fprintf_nodes(f, c->val);
			fprintf_connections(f, c->val);
			c = c->next;
		}
		fprintf(f, "}\n");

		fclose(f);
		system("dot -Tpng graph.dot -o graph.png");
	}

	Tp extract_min()
	{
		Tp ret = get_min();
		// Erase min-cell.
		// Before that save old minimal node and
		// because dlist::erase_cell doesn't have logic
		// of deleting 'minimal' cell.
		dlist_cell_t *old_min = min;

		// Also it's maybe needed to add all children of
		// minimal-nodes to 'trees_' list to continue comprise
		// these nodes in heap.
		if (old_min->val->children.size() > 0) {
			// This is O(1) connection.
			// So we don't loose to much time on it.
			if (trees_.size() == 1) {
				trees_.root_ = get_root_cell(min->val);
			}
			trees_.connect_with(old_min->val->children);

			// Well, actually in this moment we move min
			// cell to each other node, but...
			// That action is goot if we have the only fibonacci
			// tree in our list and the only node is cycled by itself
			// So we need to let minimal 'label' to some cell from children
			min = get_root_cell(min->val);

		} else {
			// If we have have zero size of children
			// we can let 'minimal' label to some cell
			// from trees_ list.
			// Because it will be either the only root in one-cell list
			// or next next cell in bit trees_ list.
			min = min->next;
		}

		// But 'erase_cell' function moves root_ cell
		// If minimal nodes corresponts to root_.
		trees_.extract_cell(old_min);

		// Collapse all trees with same ranks.
		// This function allows extract_min logarifmic assymptotic.
		// Also this function maintains correct min cell.
		shrink(trees_);

		printf("after remove min sz:(%zu)\n", trees_.size());

		return ret;
	}

private:
	// Creates node with unique
	// index and place to it 'Tp' value.
	node *node_ctor(Tp el)
	{
		node *new_node = new node;
		new_node->node_id = (uint64_t)new_node;

		new_node->key = el;
		// if (trees_.root_ != nullptr) {
		// 	new_node->ptr_to_root = trees_.root_->val;
		// }

		return new_node;
	}

	void fprintf_nodes(FILE *f, node *n)
	{
		if (n == nullptr) {
			return;
		}

		fprintf(f, "n%lu[shape=\"rectangle\" label=\"%d\"]\n\n",
			n->node_id, n->key);

		if (n->children.size() > 0) {
			dlist_cell_t *child = get_root_cell(n);
			for (size_t child_idx = 0; child_idx < n->children.size(); ++child_idx) {
				fprintf_nodes(f, child->val);
				child = child->next;
			}
		}
	}

	void fprintf_connections(FILE *f, node *n)
	{
		if (n == nullptr) {
			return;
		}

		dlist_cell_t *child = get_root_cell(n);
		for (size_t child_idx = 0; child_idx < n->children.size(); ++child_idx) {
			fprintf(f, "n%lu -> n%lu\n\n",
				n->node_id, child->val->node_id);

			child = child->next;
		}

		child = get_root_cell(n);
		for (size_t child_idx = 0; child_idx < n->children.size(); ++child_idx) {
			fprintf_connections(f, child->val);

			child = child->next;
		}
	}

	void shrink(dlist_t& list)
	{
		if (list.size() == 0) {
			return;
		}

		bool is_changed = true;
		// We will maintain array of pointers to cells
		// to fibonacci trees with different ranks.
		// 
		// Actually the precise size of this vector
		// is log(trees.size()).
		std::vector<dlist_cell_t*> ptrs(list.size(), nullptr);

		dlist_cell_t *c = list.root_;

		while (true) {
			if (ptrs[c->val->children.size()] == nullptr) {
				// If we meet root of fibonacci tree
				// with new rank this place in vector will be free.
				// So we put pointer to this root in vector.
				ptrs[c->val->children.size()] = c;
			} else {
				// The only case we do all the job
				// is when in particular place in vector
				// we already have node itself.
				// Because it means that we pass all cells int
				// cycled dlist and don't meet another root with the same rank.
				if (ptrs[c->val->children.size()] == c) {
					break;
				}

				// If we meet root with the same rank we
				// should merge this trees.
				// But firstly we will Make free space in that cell.
				dlist_cell_t *previous_that_rank_cell = ptrs[c->val->children.size()];
				ptrs[c->val->children.size()] = nullptr;

				// That is crucial that new tree
				// will be assembled at place of first
				// argument's cell.
				c = merge_trees(c, previous_that_rank_cell);
				min = (c->val->key < min->val->key)? c : min;

				ptrs[c->val->children.size()] = c;
			}

			// Check next cell in cycled list.
			c = c->next;
			min = (c->val->key < min->val->key)? c : min;
		}
	}

	// dlist_cell_t *get_parent(node *n)
	// {
	// 	return (n->ptr_to_root == nullptr)? n->parent_ : n->ptr_to_root->parent_;
	// }

	dlist_cell_t *merge_trees(dlist_cell_t *f, dlist_cell_t *s)
	{
		if (f->val->key > s->val->key) {
			std::swap(f, s);
		}

		f->val->children.emplace_at(f->val->children.size(), trees_.extract_cell(s));

		return f;
	}

	// void set_parent_attributes(dlist_cell_t *parent, dlist_cell_t *c)
	// {
	// 	if (parent->val->children.size() == 0) {
	// 		c->val->ptr_to_root = nullptr;
	// 		c->val->parent = parent;
	// 	} else {
	// 		c->val->ptr_to_root = parent->val->children.root_->val;
	// 	}
	// }

	dlist_cell_t *get_root_cell(node *n)
	{ return n->children.root_; }

private:
	dlist_t trees_{};
	dlist_cell_t *min{nullptr}; 
};

}; // end of lib.

int main()
{
	// // Part of testing cycled doubly-linked list:
	// lib::cycled_dlist<int> dl1;
	// dl1.push_at(0, 10);
	// dl1.push_at(1, 20);
	// dl1.push_at(2, 30);
	// dl1.push_at(3, 40);
	// lib::cycled_dlist<int> dl2 = {100, 200, 300};
	// dl1.connect_with(dl2);
	// dl1.draw();

	//////////////////////////////
	// fibonacci heap usage
	lib::fheap<int> fh;
	fh.insert(1);
	fh.insert(2);



	fh.insert(3);
	fh.insert(4);
	fh.insert(5);
	fh.insert(6);
	fh.insert(7);
	fh.insert(8);
	fh.insert(9);

	fh.extract_min();
	fh.extract_min();

	std::cout << fh.get_min() << '\n';

	fh.draw();

	return 0;
}