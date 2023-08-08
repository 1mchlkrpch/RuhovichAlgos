#include <iostream>

#include <cstdio>

template <typename Tp>
class dList
{
 protected:
	struct Node
	{
		Node *next{nullptr};
		Node *prev{nullptr};

		Tp key{};

		uint64_t id{0};
	};

 public:
	void Add(size_t after_idx, Tp key)
	{
		Node *cur_node = root_;

		if (after_idx >= sz_) {
			if (sz_ > 0) {
				for (size_t i = 0; i < sz_ - 1; ++i) {
					cur_node = cur_node->next;
				}

				cur_node->next = NodeCtor(key);
				cur_node->next->prev = cur_node;
			} else {
				root_ = NodeCtor(key);
			}
		} else {
			for (size_t i = 0; i < after_idx; ++i) {
				cur_node = cur_node->next;
			}

			Node *old_next = cur_node->next;

			printf("k:%d\n", cur_node->key);

			cur_node->next = NodeCtor(key);
			if (old_next != nullptr) {
				old_next->prev = cur_node->next;
			}
			cur_node->next->next = old_next;

			cur_node->next->prev = cur_node;
		}

		++sz_;
	}

	void Debug()
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
	Node *NodeCtor(Tp key)
	{
		Node *new_node = new Node;
		new_node->key = key;

		new_node->id = (uint64_t)new_node;

		return new_node;
	}

 	void PrintConnections(Node *n, FILE *f)
	{
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
			PrintConnections(n->next, f);
		}
	}

	void PrintNodes(Node *n, FILE *f)
	{
		fprintf(f,
			"\tn%lu[shape=\"rectangle\" label=\"%d\"]\n\n",
			n->id, n->key);

		if (n->next != nullptr) {
			PrintNodes(n->next, f);
		}
	}

 private:
 	Node *root_{nullptr};
 	size_t  sz_{0};
};

int main() {
	dList<int> dl;
	dl.Add(0, 5);
	dl.Add(0, 2);
	dl.Add(1, 1);
	dl.Add(0, 9);

	dl.Debug();

	return 0;
}