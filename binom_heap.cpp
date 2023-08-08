#include <iostream>
#include <vector>

static constexpr size_t kSingleChild = 1;

template <typename Tp>
class BinomHeap
{
 protected:
 	struct Node
 	{
 		Tp key{};
 		uint64_t id{0};

 		std::vector<Node*> children{};
 	};

 public:
 	BinomHeap(std::vector<Tp> v)
 	{
 		for (size_t el_idx = 0; el_idx < v.size(); ++el_idx) {
 			Add(v[el_idx]);
 		}
 	}

 	Tp GetKey(Node *n)
 	{ return n->key; }

 	Node *NodeCtor(Tp key)
 	{
 		Node *new_node = new Node;

 		new_node->key = key;
 		new_node->id  = (uint64_t)new_node;

 		return new_node;
 	}

 	void Add(Tp key)
 	{
 		Node *node_to_add = NodeCtor(key);

 		Append(node_to_add);
 	}

 	size_t Size()
 	{ return trees_.size(); }

 	void Merge(BinomHeap& other)
 	{
 		size_t first_cursor = 0;
 		size_t secon_cursor = 0;

 		min = (other.min < min)? other.min : min;

 		std::vector<Node*> new_roots;

 		while (first_cursor < trees_.size() && secon_cursor < other.Size()) {

 			if (trees_[first_cursor]->children.size() > other.trees_[secon_cursor]->children.size()) {
 				new_roots.push_back(trees_[first_cursor]);
 				++first_cursor;
 			}

 			else if (trees_[first_cursor]->children.size() == other.trees_[secon_cursor]->children.size()) {
 				new_roots.push_back(trees_[first_cursor]);
 				++first_cursor;

 				new_roots.push_back(other.trees_[secon_cursor]);
 				++secon_cursor;

 				Shrink(new_roots);
 			}

 			else if (trees_[first_cursor]->children.size() < other.trees_[secon_cursor]->children.size()) {

 				new_roots.push_back(other.trees_[secon_cursor]);
 				++secon_cursor;
 			}
 		}

 		if (first_cursor < trees_.size()) {
 			while (first_cursor < trees_.size()) {
 				new_roots.push_back(trees_[first_cursor]);
 				++first_cursor;
 			}
 		}

 		if (secon_cursor < other.trees_.size()) {
 			while (secon_cursor < other.trees_.size()) {
 				new_roots.push_back(other.trees_[secon_cursor]);
 				++secon_cursor;
 			}
 		}

 		std::swap(trees_, new_roots);
 	}

 	void RemoveMin()
 	{
 		std::vector<Node*> new_roots;

 		for (int child_idx = (int)min->children.size() - 1; child_idx >= 0; --child_idx) {
 			new_roots.push_back(min->children[child_idx]);
 		}

 		std::vector<Node*> old_roots;

 		Node *new_min = nullptr;
 		for (size_t child_idx = 0; child_idx < trees_.size(); ++child_idx) {
 			if (trees_[child_idx] == min) {
 				continue;
 			}

 			if (new_min == nullptr) {
 				new_min = trees_[child_idx];
 			} else {
 				new_min = (GetKey(new_min) > GetKey(trees_[child_idx]))? trees_[child_idx] : new_min;
 			}

 			old_roots.push_back(trees_[child_idx]);
 		}

 		min = new_min;

 		std::swap(old_roots, trees_);
 		BinomHeap children_bh(new_roots);

 		Merge(children_bh);
 	}

 	void Debug()
 	{
 		FILE *f = fopen("graph.dot", "w");
 		fprintf(f, "digraph g {\n");

 		for (size_t el_idx = 0; el_idx < trees_.size(); ++el_idx) {
 			PrintNodes(trees_[el_idx], f);
 			PrintConnections(trees_[el_idx], f);
 		}

 		fprintf(f, "}\n");
 		fclose(f);

 		system("dot -Tpng graph.dot -o graph.png");
 	}

 private:
 	BinomHeap(std::vector<Node*> v)
 			: trees_(v)
 	{
 		min = v[0];

 		for (size_t child_idx = 1; child_idx < v.size(); ++child_idx) {
 			min = (GetKey(min) > GetKey(v[child_idx]))? v[child_idx] : min;
 		}
 	}

 	void Append(Node *n)
 	{
 		trees_.push_back(n);

 		if (trees_.size() == 1) {
 			min = trees_[0];
 		}

		Shrink(trees_);
 	}

 	void Shrink(std::vector<Node*>& roots)
 	{
 		size_t tree_idx = roots.size() - 1;

 		while (tree_idx > 0
 				&& roots[tree_idx]->children.size() == roots[tree_idx - 1]->children.size()) {

 			if (GetKey(roots[tree_idx]) > GetKey(roots[tree_idx - 1])) {
 				roots[tree_idx - 1]->children.push_back(roots[tree_idx]);
 				roots.pop_back();
 			} else {
 				Node *n_last = roots[tree_idx];
 				roots.pop_back();

 				Node *pre_last = roots[tree_idx - 1];
 				roots.pop_back();


 				n_last->children.push_back(pre_last);
 				roots.push_back(n_last);

 				min = (GetKey(n_last) < GetKey(min))? n_last : min;
 			}

 			--tree_idx;
 		}
 	}

 	void PrintConnections(Node *n, FILE *f)
 	{
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
 		fprintf(f,
 			"\tn%lu[shape=\"rectangle\" label=\"%d\"]\n\n",
 			n->id, n->key);

 		if (n->children.size() > 0) {
 			for (size_t child_idx = 0; child_idx < n->children.size(); ++child_idx) {
 				PrintNodes(n->children[child_idx], f);
 			}
 		}
 	}

 private:
 	std::vector<Node*> trees_{};
 	Node *min{nullptr};
};

int main() {
	// std::vector<int> nums = {3, 1, 5, 6, 2, 7, 9, 8, 10, 13, 12, 11, 14, 15, 17};
	std::vector<int> nums = {1, 2, 3};
	BinomHeap<int> bh(nums);

	bh.Debug();

	// bh.Add(19);

	std::vector<int> second = {17, 28, 19};
	BinomHeap<int> sbh(second);

	bh.Merge(sbh);

	bh.RemoveMin();

	bh.Debug();

	return 0;
}