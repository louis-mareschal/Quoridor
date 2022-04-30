#include <vector>
#include <tuple>
#include <windows.h>
#include <string>
#include <iostream>

static double size_pixel;

const int MAX = 100000;
const int MIN = -100000;

/////// YOU CAN CHANGE THE PARAMETERS HERE ///////

const int GRID_SIZE = 7; // Grid contains GRID_SIZE * GRID_SIZE squares
const int wall_width = 3; // Must be 2*square_size - wall_height
const int wall_height = 19; // Must be square_size * 2 + wall_width
const int square_size = 8; // Must be (wall_height - wall_width) / 2

/////////////////////////////////////////////////

const int numberGridSquares = GRID_SIZE * GRID_SIZE;
static double render_scale = 0.01f - (GRID_SIZE * square_size + wall_width * (GRID_SIZE - 1) + wall_height) * 0.00003;

using namespace std;

inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}


bool values_in_lists(int value1, int value2, const vector<int> &list1, const vector<int> &list2) {
	for (int val : list1) {
		if (val == value1 || val == value2) {
			return true;
		}
	}
	for (int val : list2) {
		if (val == value1 || val == value2) {
			return true;
		}
	}
	return false;
}

bool is_value_in_list(int value, const vector<int> &vec) {
	for (int val : vec) {
		if (val == value)
			return true;
	}
	return false;
}