#define is_pressed(b) input->buttons[b].is_pressed
using namespace std;
void update_mouse_pos(HWND window, CURSORINFO * ppci) {
	GetCursorInfo(ppci);
	ScreenToClient(window, &(ppci->ptScreenPos));
	int min_height_width = min(render_state.height, render_state.width);
	ppci->ptScreenPos.x = static_cast<LONG>(ppci->ptScreenPos.x / (min_height_width * render_scale));
	ppci->ptScreenPos.y = static_cast<LONG>((render_state.height - ppci->ptScreenPos.y) / (min_height_width * render_scale));
}

bool is_in(int x, int y, int x0, int y0, int x1, int y1) {
	return (x >= x0 && x <= x1 && y >= y0 && y <= y1);
}



tuple<char, int> bottom_wall(int column, int screenPosY) {
	int state;
	if (screenPosY / (square_size + wall_width) == 0) { // Up wall anyway (because bottom of the grid)
		state = column * 2 + 1;
		return make_tuple('w', state);
	}
	else {
		state = column * 2 + 1 + ((screenPosY / (square_size + wall_width)) - 1) * ((GRID_SIZE - 1) * 2);
		return make_tuple('w', state);
	}
}

tuple<char, int> up_wall(int column, int screenPosY) {
	int state;
	if (screenPosY / (square_size + wall_width) == (GRID_SIZE - 1)) { // Down wall anyway (because top of the grid)
		state = column * 2 + 1 + (GRID_SIZE - 2) * ((GRID_SIZE - 1) * 2);
		return make_tuple('w', state);
	}
	else {
		state = column * 2 + 1 + (screenPosY / (square_size + wall_width)) * ((GRID_SIZE - 1) * 2);
		return make_tuple('w', state);
	}
}

tuple<char, int> left_wall(int line, int screenPosX) {
	int state;
	if (screenPosX / (square_size + wall_width) == 0) { // right wall anyway (because left part of the grid)
		state = line * ((GRID_SIZE - 1) * 2);
		return make_tuple('w', state);
	}
	else {
		state = ((screenPosX / (square_size + wall_width)) - 1) * 2 + line * ((GRID_SIZE - 1) * 2);
		return make_tuple('w', state);
	}
}

tuple<char, int> right_wall(int line, int screenPosX) {
	int state;
	if (screenPosX / (square_size + wall_width) == (GRID_SIZE - 1)) { // Left wall anyway (because right part of the grid)
		state = (GRID_SIZE - 2) * 2 + line * ((GRID_SIZE - 1) * 2);
		return make_tuple('w', state);
	}
	else {
		state = (screenPosX / (square_size + wall_width)) * 2 + line * ((GRID_SIZE - 1) * 2);
		return make_tuple('w', state);
	}
}

tuple<char, int> vertical_walls(int screenPosX, int screenPosY) {
	int column = screenPosX / (square_size + wall_width);
	if (screenPosY % (square_size + wall_width) < 3)
		return bottom_wall(column, screenPosY);
	else
		return up_wall(column, screenPosY);
}

tuple<char, int> horizontal_walls(int screenPosX, int screenPosY) {
	int line = screenPosY / (square_size + wall_width);
	if (screenPosX % (square_size + wall_width) < 3)
		return left_wall(line, screenPosX);
	else
		return right_wall(line, screenPosX);
}

tuple<char, int> get_player_action(HWND window, CURSORINFO pci, const Grid &grid) {
	update_mouse_pos(window, &pci);
	int state;
	pci.ptScreenPos.x -= grid.get_x();
	pci.ptScreenPos.y -= grid.get_y();
	if (is_in(pci.ptScreenPos.x, pci.ptScreenPos.y, 0, 0, grid.get_grid_size(), grid.get_grid_size())) {
		if (pci.ptScreenPos.x % (square_size + wall_width) < square_size && pci.ptScreenPos.y % (square_size + wall_width) < square_size) { // click on a player spot
			state = (pci.ptScreenPos.y / (square_size + wall_width)) * GRID_SIZE + (pci.ptScreenPos.x / (square_size + wall_width));
			return make_tuple('p', state);
		} else { // click on a wall spot
			if (pci.ptScreenPos.x % (square_size + wall_width) >= square_size && pci.ptScreenPos.y % (square_size + wall_width) < square_size)
				return vertical_walls(pci.ptScreenPos.x, pci.ptScreenPos.y);
			else
				return horizontal_walls(pci.ptScreenPos.x, pci.ptScreenPos.y);
		}
	}
	return make_tuple('n', -1);

}


void updtate_input(HWND window, Input * input) {
	MSG message;

	input->buttons[MOUSE_LEFT].is_pressed = false;

	while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
		switch (message.message) {
		case WM_LBUTTONDOWN: {
			input->buttons[MOUSE_LEFT].is_pressed = true;
		} break;

		default: {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		}
	}
}

static void display_game(Board board, int player, int action_wall) {
	clear_screen(0x47221A);
	board.display_all(player, action_wall);
}



