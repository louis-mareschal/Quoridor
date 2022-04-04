#include "grid.cpp"
#include "player.cpp"

class Board {
public:
	Grid grid;
	Player player1, player2;

	Board() : player1(*(new Player(0))), player2(*(new Player(1))) {}

	Board(const Board * board) : player1(board->player1), player2(board->player2), grid(board->grid) {}

	void display_all(int player, int action_wall) {
		grid.draw_grid();
		grid.draw_walls();
		if (player == 0) player1.draw_next_pos(grid.get_x(), grid.get_y());
		else if(player == 1) player2.draw_next_pos(grid.get_x(), grid.get_y());
		else if (player == 2) { // win case
			if (player1.get_line() == 0) player1.draw_win(grid.get_x(), grid.get_y());
			else player2.draw_win(grid.get_x(), grid.get_y());
		}
		player1.draw_all(grid.get_x(), grid.get_y());
		player2.draw_all(grid.get_x(), grid.get_y());
		if (action_wall != -1) grid.draw_wall(action_wall);
	}

	bool is_possible_wall(int state, int * dist1, int * dist2) {
		return grid.is_possible(state, player1.get_column(), player1.get_line(), player2.get_column(), player2.get_line(), dist1, dist2);
	}
	void update_valid_next_pos(bool player) {
		if (player) player2.update_valid_next_pos(player1, grid);
		else player1.update_valid_next_pos(player2, grid);
	}
	void moove_player(bool player, int state) {
		if (player) player2.moove_player(state, &player1, grid);
		else player1.moove_player(state, &player2, grid);
	}
	int shortest_path(bool player) {
		if (player) return grid.A_star(1, player2.get_line() * GRID_SIZE + player2.get_column());
		else return grid.A_star(0, player1.get_line() * GRID_SIZE + player1.get_column());
	}
};