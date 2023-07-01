#include <iostream>
#include <cassert>

#include <stack>

class TwoStackQueue{
 private:
	std::stack<int64_t> in_{};
	std::stack<int64_t> out_{};

 private:
 	// O(n)-operation
	void ReplaceElements(size_t n_to_replace) {
		assert(n_to_replace <= in_.size() && "Not enough elements in \"in_\" stack!");

		for (size_t el_idx = 0; el_idx < n_to_replace; ++el_idx) {
			out_.push(in_.top());
			in_.pop();
		}
	}
	
 public:
	TwoStackQueue() = default;

	// Always asymptotically O(1)
	void Push(int64_t el) {
		in_.push(el);
	}

	// At worst case O(n)
	//   but we can't allege that it will be O(n^2)
	//   operations for n-actions
	// Because all elements takes only O(1) operations with itself
	//   so Top() & Pop() operations requires O(1) in average
	void Pop() {
		if (out_.empty()) {
			ReplaceElements(in_.size());
		}

		out_.pop();
	}

	int64_t Front() {
		if (out_.empty()) {
			ReplaceElements(in_.size());
		}

		return out_.top();
	}
};

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	TwoStackQueue q;

	q.Push(1);
	std::cout << q.Front() << '\n';

	return 0;
}