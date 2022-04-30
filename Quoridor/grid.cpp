
class A_node {
public:
	int state;
	int cout;
	int heuristique;
	int first_move, second_move;
	A_node(int state, int cout, int heuristique, int first_move, int second_move) : state(state), cout(cout), heuristique(heuristique), first_move(first_move), second_move(second_move) {}

	vector<shared_ptr<A_node>> get_neighbors(const vector<int> &list_walls_P1, const vector<int> &list_walls_P2) {
		vector<shared_ptr<A_node>> neighbors;
		int line = state / GRID_SIZE;
		int column = state % GRID_SIZE;
		int state1, state2;
		int first_move_neighbors, second_move_neighbors;
		if (this->cout != 0) {
			first_move_neighbors = this->first_move;
		}
		if (this->cout != 1) {
			second_move_neighbors = this->second_move;
		}

		if (column > 0) {
			if (line != 0) state1 = (line - 1) * ((GRID_SIZE-1)*2) + (column - 1) * 2 + 1;
			else state1 = -1;
			if (line != GRID_SIZE-1) state2 = line * ((GRID_SIZE - 1) * 2) + (column - 1) * 2 + 1;
			else state2 = -1;
			if (!values_in_lists(state1, state2, list_walls_P1, list_walls_P2)) {
				if (this->cout == 0) {
					first_move_neighbors = line * GRID_SIZE + column - 1;
				}
				if (this->cout == 1) {
					second_move_neighbors = line * GRID_SIZE + column - 1;
				}
				neighbors.push_back(make_shared<A_node>(line * GRID_SIZE + column - 1, cout, 0, first_move_neighbors, second_move_neighbors));
			}
		}
		if (column < GRID_SIZE-1) {
			if (line != 0) state1 = (line - 1) * ((GRID_SIZE - 1) * 2) + column * 2 + 1;
			else state1 = -1;
			if (line != GRID_SIZE-1) state2 = line * ((GRID_SIZE - 1) * 2) + column * 2 + 1;
			else state2 = -1;
			if (!values_in_lists(state1, state2, list_walls_P1, list_walls_P2)) {
				if (this->cout == 0) {
					first_move_neighbors = line * GRID_SIZE + column + 1;
				}
				if (this->cout == 1) {
					second_move_neighbors = line * GRID_SIZE + column + 1;
				}
				neighbors.push_back(make_shared<A_node>(line * GRID_SIZE + column + 1, cout, 0, first_move_neighbors, second_move_neighbors));
			}
		}
		if (line > 0) {
			if (column != 0) state1 = (line - 1) * ((GRID_SIZE - 1) * 2) + (column - 1) * 2;
			else state1 = -1;
			if (column != GRID_SIZE-1) state2 = (line - 1) * ((GRID_SIZE - 1) * 2) + column * 2;
			else state2 = -1;
			if (!values_in_lists(state1, state2, list_walls_P1, list_walls_P2)) {
				if (this->cout == 0) {
					first_move_neighbors = (line - 1) * GRID_SIZE + column;
				}
				if (this->cout == 1) {
					second_move_neighbors = (line - 1) * GRID_SIZE + column;
				}
				neighbors.push_back(make_shared<A_node>((line - 1) * GRID_SIZE + column, cout, 0, first_move_neighbors, second_move_neighbors));
			}
		}
		if (line < GRID_SIZE-1) {
			if (column != 0) state1 = line * ((GRID_SIZE - 1) * 2) + (column - 1) * 2;
			else state1 = -1;
			if (column != GRID_SIZE-1) state2 = line * ((GRID_SIZE - 1) * 2) + column * 2;
			else state2 = -1;
			if (!values_in_lists(state1, state2, list_walls_P1, list_walls_P2)) {
				if (this->cout == 0) {
					first_move_neighbors = (line + 1) * GRID_SIZE + column;
				}
				if (this->cout == 1) {
					second_move_neighbors = (line + 1) * GRID_SIZE + column;
				}
				neighbors.push_back(make_shared<A_node>((line + 1) * GRID_SIZE + column, cout, 0, first_move_neighbors, second_move_neighbors));
			}
		}
		return neighbors;
	}
};

class Stack {
public:
	vector<shared_ptr<A_node>> stack = {};
	int size = 0;

	void add_node(shared_ptr<A_node> node) {
		bool insert = false;
		for (int i = 0; i < size; i++) {
			if (node->heuristique > stack[i]->heuristique) {
				stack.insert(stack.begin() + i, move(node));
				insert = true;
				break;
			}
		}
		size++;
		if (!insert) stack.push_back(move(node));;
	}

	A_node pop() {
		A_node node = *stack[size - 1];
		stack.erase(stack.begin() + size - 1);
		size--;
		return node;
	}
};

class Grid {
private:
	int x;
	int y;
	int grid_size = GRID_SIZE * square_size + (GRID_SIZE-1) * wall_width - 1;
	vector<int> list_walls_P1, list_walls_P2, list_states_impossible;
	int color_grid = 0x201C1B, color_P1 = 0x5E0C1D, color_P2 = 0xECB377;
	int temp_state_wall;
public:
	Grid() {
		update_pos();
	}

	Grid(const Grid * grid) : grid_size(grid->grid_size),
		list_walls_P1(grid->list_walls_P1), list_walls_P2(grid->list_walls_P2), list_states_impossible(grid->list_states_impossible) {
		update_pos();
	}

	int get_x() const{
		return this->x;
	}

	int get_y() const{
		return this->y;
	}

	int get_grid_size() const{
		return this->grid_size;
	}

	vector<int> get_list_walls_P1() const{
		return list_walls_P1;
	}
	vector<int> get_list_walls_P2() const{
		return list_walls_P2;
	}

	vector<int> get_list_states_impossible() const{
		return list_states_impossible;
	}

	void draw_grid() {
		update_pos();
		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				draw_rect(x + j * (square_size+wall_width), y + i * (square_size + wall_width), square_size, square_size, color_grid);
			}
		}
	}

	void update_pos() {
		x = static_cast<int>(((render_state.width / size_pixel) - grid_size) / 2);
		y = static_cast<int>(((render_state.height / size_pixel) - grid_size) / 2);
	}

	bool is_possible(int state, int c1, int l1, int c2, int l2, int * dist1, int * dist2) {
		for (int impossible_state : list_states_impossible) {
			if (state == impossible_state) {
				return false;
			}
		}
		bool possible = true;
		temp_state_wall = state;
		list_walls_P1.push_back(state);
		if ((*dist1 = A_star(0, l1 * GRID_SIZE + c1).cout) == -1)
			possible = false;
		if (possible && (*dist2 = A_star(1, l2 * GRID_SIZE + c2).cout) == -1)
			possible = false;
		list_walls_P1.erase(list_walls_P1.begin() + list_walls_P1.size() - 1);
		return possible;
	}
	

	void add_wall(int state, bool player) {
		if (player) list_walls_P2.push_back(state);
		else list_walls_P1.push_back(state);
		list_states_impossible.push_back(state);
		if (state % 2 == 0) { // Horizontal wall
			list_states_impossible.push_back(state + 1);
			if (state % ((GRID_SIZE-1) * 2) != 0) {
				list_states_impossible.push_back(state - 2);
			}
			if (state % ((GRID_SIZE - 1) * 2) != ((GRID_SIZE - 1) * 2)-1) {
				list_states_impossible.push_back(state + 2);
			}
		}
		else { // Vertical wall
			list_states_impossible.push_back(state - 1);
			if (state / ((GRID_SIZE - 1) * 2) != 0) {
				list_states_impossible.push_back(state - ((GRID_SIZE - 1) * 2));
			}
			if (state / ((GRID_SIZE - 1) * 2) != GRID_SIZE-1) {
				list_states_impossible.push_back(state + ((GRID_SIZE - 1) * 2));
			}
		}
	}

	void draw_walls() {
		int x0, y0;
		for (int state : list_walls_P1) {
			tie(x0, y0) = state_to_x0_y0(state);
			if (state % 2 == 0) { // Horizontal wall
				draw_rect(x0, y0, wall_height, wall_width, color_P1);
			}
			else { // vertical wall
				draw_rect(x0, y0, wall_width, wall_height, color_P1);
			}

		}
		for (int state : list_walls_P2) {
			tie(x0, y0) = state_to_x0_y0(state);
			if (state % 2 == 0) { // Horizontal wall
				draw_rect(x0, y0, wall_height, wall_width, color_P2);
			}
			else { // vertical wall
				draw_rect(x0, y0, wall_width, wall_height, color_P2);
			}

		}
	}

	void draw_wall(int state) {
		int x0, y0;
		int color = 0x2FEA00;
		bool impossible = false;
		for (int impossible_state : list_states_impossible) {
			if (state == impossible_state) {
				impossible = true;
			}
		}
		if (!impossible) {
			tie(x0, y0) = state_to_x0_y0(state);
			if (state % 2 == 0) { // Horizontal wall
				draw_rect(x0, y0, wall_height, wall_width, color);
			}
			else { // vertical wall
				draw_rect(x0, y0, wall_width, wall_height, color);
			}
		}
	}

	tuple<int, int> state_to_x0_y0(int state) {
		int x0, y0;
		if (state % 2 == 0) { // horizontal wall
			y0 = y + square_size + (state / ((GRID_SIZE - 1) * 2)) * (square_size + wall_width);
			x0 = x + (state % ((GRID_SIZE - 1) * 2)) * (square_size + wall_width) / 2;
		}
		else { // vertical wall
			y0 = y + (state / ((GRID_SIZE - 1) * 2)) * (square_size + wall_width);
			x0 = x + square_size + ((state - 1) % ((GRID_SIZE - 1) * 2)) * (square_size + wall_width) / 2;
		}
		return make_tuple(x0, y0);
	}

	const A_node & A_star(bool player, int init_state) {
		vector<A_node> closedlist;
		Stack openlist;
		vector<A_node*> neighbors;
		openlist.add_node(make_shared<A_node>(init_state, 0, 0, -1, -1));
		
		while (openlist.size != 0) {
			A_node current_node = openlist.pop();
			if ((player && current_node.state / GRID_SIZE == (GRID_SIZE-1)) || (!player && current_node.state / GRID_SIZE == 0)) 
				return current_node;
			vector<shared_ptr<A_node>> neighbors = current_node.get_neighbors(get_list_walls_P1(), get_list_walls_P2());
			int size_neignbors = neighbors.size();
			for (int i = 0; i < size_neignbors; i++) {
				if (!is_in_closedlist(*neighbors[i], closedlist) && !is_in_openlist_and_smaller(*neighbors[i], openlist)) {
					neighbors[i]->cout++;
					if (player) neighbors[i]->heuristique = neighbors[i]->cout + GRID_SIZE - neighbors[i]->state / GRID_SIZE - 1;
					else neighbors[i]->heuristique = neighbors[i]->cout + neighbors[i]->state / GRID_SIZE - 1;
					openlist.add_node(move(neighbors[i]));
				}
			}
			closedlist.push_back(current_node);
		}
		A_node node_pb(-1, -1, -1, -1, -1);
		return node_pb;
	}

	bool is_in_closedlist(const A_node & neighbour, vector<A_node> closedlist) {
		int size = closedlist.size();
		for (int i = 0; i < size; i++) {
			if (neighbour.state == closedlist[i].state) return true;
		}
		return false;
	}

	bool is_in_openlist_and_smaller(const A_node & neighbour, const Stack &openlist) {
		for (int i = 0; i < openlist.size; i++) {
			if (neighbour.state == openlist.stack[i]->state && openlist.stack[i]->cout < neighbour.cout) return true;
		}
		return false;
	}
};











