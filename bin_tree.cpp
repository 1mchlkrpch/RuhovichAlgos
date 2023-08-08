#include <iostream>
#include <vector>

#include <cstdio>

template <typename Tp>
class BST
{
 protected:
	struct Node {
		Tp key{0};

		uint64_t id{0};

		struct Node *Node{nullptr};

		struct Node *l_child{nullptr};
		struct Node *r_child{nullptr};
	};

 public:
 	BST(std::vector<Tp>& v)
 			: root_(NodeCtor(v[0])){
 		for (size_t el_idx = 1; el_idx < v.size(); ++el_idx) {
 			if (Search(v[el_idx]) == nullptr) {
 				Insert(v[el_idx]);
 			}
 		}
 	}

 	void Insert(Tp key) {
 		std::cout << "insert new key:" << key << '\n';
 		Node *cur = root_;

 		while (true) {
 			if (cur->key == key) {
 				return;
 			}

 			if (cur->key > key) {
 				if (cur->l_child == nullptr) {
 					cur->l_child = NodeCtor(key);
 					break;
 				}

 				cur = cur->l_child;
 			} else {
 				if (cur->r_child == nullptr) {
 					cur->r_child = NodeCtor(key);
 					break;
 				}

 				cur = cur->r_child;
 			}
 		}
 	}

 	Node *Search(Tp key)
 	{ return Search(root_, key); }

 	void DebugTree()
 	{
 		FILE *f = fopen("graph.dot", "w");
 		fprintf(f, "digraph g {\n");

 		PrintNodes(root_, f);
 		PrintConnections(root_, f);
 		fprintf(f, "}\n");

 		fclose(f);

 		system("dot -Tpng graph.dot -o graph.png");
 	}

 private:
 	void PrintConnections(Node *n, FILE *f)
 	{
 		if (n->l_child != nullptr) {
 			fprintf(f,
 				"\tn%lu -> n%lu\n\n",
 				n->id, n->l_child->id);

 			PrintConnections(n->l_child, f);
 		}

 		if (n->r_child != nullptr) {
 			fprintf(f,
 				"\tn%lu -> n%lu\n\n",
 				n->id, n->r_child->id);

 			PrintConnections(n->r_child, f);
 		}
 	}

 	void PrintNodes(Node *n, FILE *f) {
 		fprintf(f,
 			"\tn%lu[shape=\"rectangle\" label=\"%d\"]\n\n",
 			n->id, n->key);

 		if (n->l_child != nullptr) {
 			PrintNodes(n->l_child, f);
 		}

 		if (n->r_child != nullptr) {
 			PrintNodes(n->r_child, f);
 		}
 	}

 	Node *NodeCtor(Tp key)
 	{
 		Node *new_node = new Node;
 		new_node->key = key;

 		new_node->id = (uint64_t)new_node;

 		return new_node;
 	}

 	Node *Search(Node *n, Tp key)
 	{
 		while (n->key != key) {
 			if (n->key > key) {
 				if (n->l_child != nullptr) {
 					n = n->l_child;
 				} else {
 					return nullptr;
 				}
 			} else {
 				if (n->r_child != nullptr) {
 					n = n->r_child;
 				} else {
 					return nullptr;
 				}
 			}
 		}

 		return n;
 	}

 private:
 	Node *root_{nullptr};
};


int main() {
	std::vector<int> nums = {3, 1, 2, 5};
	BST<int> bin_tree(nums);

	bin_tree.DebugTree();

	return 0;
}