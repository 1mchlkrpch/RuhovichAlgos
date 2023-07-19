/// @file Simple sparse table class.
#include <iostream>

#include <vector>
#include <climits>

// Returns power: 2^power >= x
static inline __attribute__ ((always_inline))
uint32_t GeqPower2(uint32_t x)
{ return sizeof(uint32_t) * CHAR_BIT - __builtin_clz(x - 1); }

static inline __attribute__((always_inline))
size_t ShiftByDegree(size_t x, size_t y)
{ return x + (1 << y); }

class SparseTable
{
 public:
	SparseTable(std::vector<int>& v)
			: line_len_(v.size()), matrix_(GeqPower2(v.size()), std::vector<int>(v.size(), 0))
	{
		for (size_t i = 0; i < line_len_; ++i) {
			matrix_[0][i] = v[i];
		}

		A_FillMatrix();
	}

	int GetMin(size_t from_idx, size_t to_idx)
	{
		if (to_idx == from_idx) {
			return matrix_[0][from_idx];
		}

		uint32_t leq_power = GeqPower2(to_idx - from_idx + 1) - 1;

		std::cout << "GetMin: leq_power:" << leq_power << '\n';
		uint32_t adjustment = 1 << leq_power;
		std::cout << "adjustment:" << adjustment << '\n';

		std::cout
			<< "indeces: from:" << from_idx
			<< " and:" << to_idx - adjustment + 1 << '\n';

		return std::min(
			matrix_[leq_power][from_idx],
			matrix_[leq_power][to_idx - adjustment + 1]
		);
	}

	void Print()
	{
		for (size_t y = 0; y < matrix_.size(); ++y) {
			for (size_t x = 0; x < line_len_ - 1; ++x) {
				std::cout << matrix_[y][x] << ' ';
			}

			std::cout << matrix_[y][line_len_ - 1] << '\n';
		}
	}

 	/* All A_functions:
 	auxilary functions to decompose system work.*/
 private:
 	void A_FillMatrix()
 	{
 		for (size_t y = 1; y < matrix_.size(); ++y) {
 			for (size_t x = 0; x < line_len_; ++x) {
 				size_t shifted_idx = ShiftByDegree(x, y - 1);

 				if (shifted_idx < line_len_) {
	 				matrix_[y][x]
	 					= std::min(
		 						matrix_[y - 1][x],
		 						matrix_[y - 1][shifted_idx]
		 					);
 				} else {
 					matrix_[y][x]
 						= std::min(
 								matrix_[y - 1][x],
 								matrix_[y - 1][line_len_ - 1]
 							);
 				}
 			}
 		}
 	}

 private:
 	size_t line_len_{0};

	std::vector<std::vector<int>> matrix_;
};

int main() {
	std::vector<int> v;

	for (size_t i = 0; i < 10; ++i) {
		int el = 0;
		std::cin >> el;

		v.push_back(el);
	}

	SparseTable sp(v);

	std::cout << "min:" << sp.GetMin(0, 9) << '\n';

	return 0;
}