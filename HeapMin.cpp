/**
 * @file Implementation of n-heap
 * with minimal element in root.
 */
#include <iostream>
#include <fstream>

#include <vector>

static constexpr int kGreater =  1;
static constexpr int kEqual   =  0;
static constexpr int kLower   = -1;

int Cmp(int& a, int& b)
{ return (a > b)? 1 : (a == b)? 0 : -1; }

template <typename Tp, size_t N, int (*Cmp)(Tp&, Tp&)>
class Heap {
 public:
	Heap(std::vector<Tp> v): lower_bound{0}, storage_(v)
	{
		Heapify();
		
		std::vector<Tp> new_v;
		uint64_t first_sz = storage_.size();

		for (size_t el_idx = 0; el_idx < first_sz; ++el_idx) {
			std::swap(storage_[storage_.size() - 1], storage_[0]);
			new_v.push_back(storage_[storage_.size() - 1]);
			storage_.pop_back();
			SiftDown(0);
		}

		std::swap(new_v, storage_);
	}

	void SiftUp(uint64_t idx)
	{
		while (Lw(storage_[idx], storage_[(idx - 1) / N])) {
			std::swap(storage_[idx], storage_[(idx - 1) / N]);
			idx = (idx - 1) / N;
		}
	}

	void SiftDown(uint64_t idx)
	{
		while (true) {
			uint64_t least_idx = idx;

			for (uint64_t child_idx = idx * N + 1;
					child_idx < storage_.size() && child_idx < idx * N + N + 1; ++child_idx) {
				if (Lw(storage_[child_idx], storage_[least_idx])) {
					least_idx = child_idx;
				}
			}

			if (least_idx == idx) {
				break;
			}

			std::swap(storage_[idx], storage_[least_idx]);
			idx = least_idx;
		}
	}

	bool Lw(Tp& a, Tp&b)
	{ return Cmp(a, b) == -1; }

	void Heapify()
	{
		for (uint64_t el_idx = 0; el_idx < storage_.size(); ++el_idx) {
			SiftDown(el_idx);
		}
	}

	void Print() {
		std::cout << "bin heap sz: " << storage_.size() << '\n';
		
		for (size_t el_idx = 0; el_idx < storage_.size() - 1; ++el_idx) {
			std::cout << storage_[el_idx] << ' ';
		}

		std::cout << storage_[storage_.size() - 1] << '\n';
	}

	std::vector<Tp> Sort()
	{
		std::vector<Tp> v;
		size_t first_sz = storage_.size();
		for (size_t el_idx = 0; el_idx < first_sz - 1; ++el_idx) {
			std::swap(storage_[storage_.size() - 1], storage_[0]);
			v.push_back(storage_[storage_.size() - 1]);
			storage_.pop_back();

			Print();

			SiftDown(0);
		}

		v.push_back(storage_[storage_.size() - 1]);
		storage_.pop_back();

		return v;
	}

	void Debug()
	{
		std::ofstream f("graph.dot");

		f << "digraph G {\n";

		for (size_t cur_v = 0; cur_v < storage_.size(); ++cur_v) {
			for (size_t cur_child = 1; cur_child <= N; ++cur_child) {
				if (cur_v * N + cur_child < storage_.size()) {
					f << '\t' << storage_[cur_v] << " -> " << storage_[cur_v * N + cur_child] << '\n';
				}
			}
		}

		f << "}\n";
		f.close();

		system("dot -Tpng graph.dot > graph.png");
	}

 private:
 	size_t lower_bound{0};
 	std::vector<Tp> storage_{};
};

int main() {
	std::vector<int> nums;
	for (int num = 0; num < 18; ++num) {
		nums.push_back(num);
	}

	Heap<int, 3, Cmp> hp(nums);
	std::cout << "sorted heap:\n";
	hp.Print();
	hp.Debug();

	return 0;
}