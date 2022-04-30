class Player {
	int line;
	int column;
	vector<int> valid_next_pos;
	int color;
	int number_walls;
	int wall_high;
public:
	Player(bool player) {
		if (player) {
			line = 0;
			column = GRID_SIZE/2;
			valid_next_pos = { column - 1, column + 1, column + GRID_SIZE };
			color = 0xECB377;
			wall_high = -wall_height;
		}
		else {
			line = GRID_SIZE-1;
			column = GRID_SIZE / 2;
			valid_next_pos = { line * GRID_SIZE + column - 1, line * GRID_SIZE + column + 1, (line-1) * GRID_SIZE + column };
			color = 0x5E0C1D;
			wall_high = GRID_SIZE * square_size + (GRID_SIZE-1) * wall_width; // grid_size
		}
		number_walls = GRID_SIZE+1;
	}

	Player(const Player& player) {
		line = player.get_line();
		column = player.get_column();
		valid_next_pos = player.get_valid_next_pos();
		color = player.color;
		wall_high = player.wall_high;
		number_walls = player.number_walls;
	}

	int get_line() const{
		return line;
	}
	int get_column() const{
		return column;
	}
	int get_number_walls() const{
		return number_walls;
	}
	void dec_wall() {
		number_walls--;
	}
	vector<int> get_valid_next_pos() const{
		return valid_next_pos;
	}

	vector<int> get_close_walls() {
		vector<int> close_walls;
		if (line < GRID_SIZE-1) {
			if (column != 0) close_walls.push_back(line * ((GRID_SIZE-1)*2) + (column - 1) * 2);
			if (column != GRID_SIZE-1) close_walls.push_back(line * ((GRID_SIZE - 1) * 2) + column * 2);
		}
		if (line > 0) {
			if (column != 0) close_walls.push_back((line - 1) * ((GRID_SIZE - 1) * 2) + (column - 1) * 2);
			if (column != GRID_SIZE - 1) close_walls.push_back((line - 1) * ((GRID_SIZE - 1) * 2) + column * 2);
		}
		if (column < GRID_SIZE - 1) {
			if (line != GRID_SIZE - 1) close_walls.push_back(line * ((GRID_SIZE - 1) * 2) + column * 2 + 1);
			if (line != 0) close_walls.push_back((line - 1) * ((GRID_SIZE - 1) * 2) + column * 2 + 1);
		}
		if (column > 0) {
			if (line != GRID_SIZE - 1) close_walls.push_back(line * ((GRID_SIZE - 1) * 2) + (column - 1) * 2 + 1);
			if (line != 0) close_walls.push_back((line - 1) * ((GRID_SIZE - 1) * 2) + (column - 1) * 2 + 1);
		}
		return close_walls;
	}

	void update_valid_next_pos(const Player & other_player, const Grid &grid) {
		valid_next_pos = {};
		update_valid_next_pos_right(other_player, grid);
		update_valid_next_pos_left(other_player, grid);
		update_valid_next_pos_up(other_player, grid);
		update_valid_next_pos_down(other_player, grid);
	}
	void update_valid_next_pos_right(const Player & other_player, const Grid &grid) {
		bool behind = false, right = false, left = false;
		int state1, state2, state3;
		if (column > 0) {
			if (line != 0) state1 = (line - 1) * ((GRID_SIZE - 1) * 2) + (column - 1) * 2 + 1;
			else state1 = -1;
			if (line != (GRID_SIZE - 1)) state2 = line * ((GRID_SIZE - 1) * 2) + (column - 1) * 2 + 1;
			else state2 = -1;
			if (!values_in_lists(state1, state2, grid.get_list_walls_P1(), grid.get_list_walls_P2())) {
				if (column - 1 == other_player.get_column() && line == other_player.get_line()) { 
					if (column > 1) state1 = x_y_to_state(line, column - 2);
					else state1 = -1;
					if (line > 0) state2 = x_y_to_state(line - 1, column - 1);
					else state2 = -1;
					if (line < (GRID_SIZE - 1)) state3 = x_y_to_state(line + 1, column - 1);
					else state3 = -1;
					for (int pos : other_player.get_valid_next_pos()) {
						if (pos == state1) {
							behind = true;
							break;
						}
						else if (pos == state2) {
							left = true;
						}
						else if (pos == state3) {
							right = true;
						}
					}
					if (behind) valid_next_pos.push_back(state1);
					else {
						if (left) valid_next_pos.push_back(state2);
						if (right) valid_next_pos.push_back(state3);
					}
				}
				else {
					valid_next_pos.push_back(x_y_to_state(line, column - 1));
				}
			}
		}
	}
	void update_valid_next_pos_left(const Player &other_player, const Grid &grid) {
		bool behind = false, right = false, left = false;
		int state1, state2, state3;
		if (column < (GRID_SIZE - 1)) {
			if (line != 0) state1 = (line - 1) * ((GRID_SIZE - 1) * 2) + column * 2 + 1;
			else state1 = -1;
			if (line != (GRID_SIZE - 1)) state2 = line * ((GRID_SIZE - 1) * 2) + column * 2 + 1;
			else state2 = -1;
			if (!values_in_lists(state1, state2, grid.get_list_walls_P1(), grid.get_list_walls_P2())) {
				if (column + 1 == other_player.get_column() && line == other_player.get_line()) { 
					if (column < (GRID_SIZE - 2)) state1 = x_y_to_state(line, column + 2);
					else state1 = -1;
					if (line < (GRID_SIZE - 1)) state2 = x_y_to_state(line + 1, column + 1);
					else state2 = -1;
					if (line > 0) state3 = x_y_to_state(line - 1, column + 1);
					else state3 = -1;
					for (int pos : other_player.get_valid_next_pos()) {
						if (pos == state1) {
							behind = true;
							break;
						}
						else if (pos == state2) {
							left = true;
						}
						else if (pos == state3) {
							right = true;
						}
					}
					if (behind) valid_next_pos.push_back(state1);
					else {
						if (left) valid_next_pos.push_back(state2);
						if (right) valid_next_pos.push_back(state3);
					}
				}
				else {
					valid_next_pos.push_back(x_y_to_state(line, column + 1));
				}
			}

		}
	}
	void update_valid_next_pos_down(const Player &other_player, const Grid &grid) {
		bool behind = false, right = false, left = false;
		int state1, state2, state3;
		if (line > 0) {
			if (column != 0) state1 = (line - 1) * ((GRID_SIZE - 1) * 2) + (column - 1) * 2;
			else state1 = -1;
			if (column != (GRID_SIZE - 1)) state2 = (line - 1) * ((GRID_SIZE - 1) * 2) + column * 2;
			else state2 = -1;
			if (!values_in_lists(state1, state2, grid.get_list_walls_P1(), grid.get_list_walls_P2())) {
				if (column == other_player.get_column() && line - 1 == other_player.get_line()) {
					if (line > 1) state1 = x_y_to_state(line - 2, column);
					else state1 = -1;
					if (column < (GRID_SIZE - 1)) state2 = x_y_to_state(line - 1, column + 1);
					else state2 = -1;
					if (column > 0) state3 = x_y_to_state(line - 1, column - 1);
					else state3 = -1;
					for (int pos : other_player.get_valid_next_pos()) {
						if (pos == state1) {
							behind = true;
							break;
						}
						else if (pos == state2) {
							left = true;
						}
						else if (pos == state3) {
							right = true;
						}
					}
					if (behind) valid_next_pos.push_back(state1);
					else {
						if (left) valid_next_pos.push_back(state2);
						if (right) valid_next_pos.push_back(state3);
					}
				}
				else {
					valid_next_pos.push_back(x_y_to_state(line - 1, column));
				}
			}
		}
	}
	void update_valid_next_pos_up(const Player &other_player, const Grid &grid) {
		bool behind = false, right = false, left = false;
		int state1, state2, state3;
		if (line < (GRID_SIZE - 1)) {
			if (column != 0) state1 = line * ((GRID_SIZE - 1) * 2) + (column - 1) * 2;
			else state1 = -1;
			if (column != (GRID_SIZE - 1)) state2 = line * ((GRID_SIZE - 1) * 2) + column * 2;
			else state2 = -1;
			if (!values_in_lists(state1, state2, grid.get_list_walls_P1(), grid.get_list_walls_P2())) {
				if (column == other_player.get_column() && line + 1 == other_player.get_line()) {
					if (line < (GRID_SIZE - 2)) state1 = x_y_to_state(line + 2, column);
					else state1 = -1;
					if (column > 0) state2 = x_y_to_state(line + 1, column - 1);
					else state2 = -1;
					if (column < (GRID_SIZE - 1)) state3 = x_y_to_state(line + 1, column + 1);
					else state3 = -1;
					for (int pos : other_player.get_valid_next_pos()) {
						if (pos == state1) {
							behind = true;
							break;
						}
						else if (pos == state2) {
							left = true;
						}
						else if (pos == state3) {
							right = true;
						}
					}
					if (behind) valid_next_pos.push_back(state1);
					else {
						if (left) valid_next_pos.push_back(state2);
						if (right) valid_next_pos.push_back(state3);
					}
				} else {
					valid_next_pos.push_back(x_y_to_state(line + 1, column));
				}
			}
		}
	}

	void moove_player(int new_state, const Player & other_player, const Grid &grid) {
		tie(this->line, this->column) = state_to_x_y(new_state);
		update_valid_next_pos(other_player, grid);
	}

	tuple<int, int> state_to_x_y(int state) {
		return make_tuple(state / GRID_SIZE, state % GRID_SIZE);
	}

	int x_y_to_state(int line, int column) {
		return line * GRID_SIZE + column;
	}

	bool is_possible(int state) {
		bool possible = false;
		for (int valid_state : valid_next_pos) {
			if (valid_state == state) {
				possible = true;
				break;
			}
		}
		return possible;
	}

	void draw(int grid_x, int grid_y) {
		draw_rect(grid_x + column * (square_size + wall_width) + square_size / 12.0, grid_y + line * (square_size + wall_width) + square_size / 12.0, square_size-square_size/6.0, square_size- square_size/6.0, color);
	}

	void draw_win(int grid_x, int grid_y) {
		draw_rect(grid_x + column * (square_size + wall_width) - square_size / 12.0, grid_y + line * (square_size + wall_width) - square_size / 12.0, square_size+ square_size / 6.0, square_size+ square_size / 6.0, 0x2FEA00);
	}

	void draw_walls(int grid_x, int grid_y) {
		for (int i = 0; i < number_walls; i++) {
			draw_rect(grid_x - wall_width + i * (square_size + wall_width), wall_high + grid_y, wall_width, wall_height, color);
		}
	}

	void draw_next_pos(int grid_x, int grid_y) {
		int x, y;
		for (int state : valid_next_pos) {
			tie(y, x) = state_to_x_y(state);
			draw_rect(grid_x + x * (square_size + wall_width) + square_size / 12.0, grid_y + y * (square_size + wall_width) + square_size / 12.0, square_size- +square_size / 6.0, square_size- +square_size / 6.0, 0x41EA00);
		}
	}

	void draw_all(int grid_x, int grid_y) {
		draw(grid_x, grid_y);
		draw_walls(grid_x, grid_y);
	}
};