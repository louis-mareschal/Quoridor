#include "renderer.cpp"
#include "platform_common.cpp"
#include "board.cpp"
#include "game.cpp"
#include "minmax.cpp"

static bool running = true;

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		running = false;
	} break;

	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		render_state.width = rect.right - rect.left;
		render_state.height = rect.bottom - rect.top;
		size_pixel = min(render_state.height, render_state.width) * render_scale;
		int size = render_state.width * render_state.height * sizeof(unsigned int);
		if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
		render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
		render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
		render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
		render_state.bitmap_info.bmiHeader.biPlanes = 1;
		render_state.bitmap_info.bmiHeader.biBitCount = 32;
		render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
	} break;

	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 300;
		lpMMI->ptMinTrackSize.y = 300;
	}

	default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
	return result;
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	// Create Window Class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Game Window Class";
	window_class.lpfnWndProc = window_callback;
	size_pixel = min(render_state.height, render_state.width) * render_scale;


	// Register Class
	RegisterClass(&window_class);

	// Create Window
	HWND window = CreateWindow(window_class.lpszClassName, L"Quoridor", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);
	Input input = {};

	//Mouse pos
	CURSORINFO pci;
	pci.cbSize = sizeof(CURSORINFO);

	// Init
	bool gameover = false, invalid_play;
	char action;
	vector<int> valid_position;
	Board board;
	int state = -1, dist1, dist2, action_wall;

	/////// YOU CAN CHANGE THE PARAMETERS HERE ///////

	bool playervsplayer = false; // IA or not 
	bool player = 1; // Who starts (1 for the player and 0 for the IA)
	int depth = 1; // Difficulty of the AI (1 to 4). But maximum 3 is advised otherwise it's too long.

	/////////////////////////////////////////////////

	while (running && !gameover) {
		invalid_play = true;
		if (player == 0) {
			if (playervsplayer) { // Player 1
				while (running && invalid_play) {
					updtate_input(window, &input);
					tie(action, state) = get_player_action(window, pci, board.grid);
					if (action == 'w' && board.player1.get_number_walls() > 0 && board.is_possible_wall(state, &dist1, &dist2)) action_wall = state;
					else action_wall = -1;
					if (input.buttons[MOUSE_LEFT].is_pressed) {
						if (action != 'n') {
							if (action == 'p') {
								if (board.player1.is_possible(state)) {
									board.moove_player(0, state);
									invalid_play = false;
								}
							}
							else { // action == 'w'
								if (board.player1.get_number_walls() > 0 && board.is_possible_wall(state, &dist1, &dist2)) {
									board.grid.add_wall(state, 0);
									board.update_valid_next_pos(0);
									board.update_valid_next_pos(1);
									board.player1.dec_wall();
									invalid_play = false;
								}
							}
						}
					}
					board.grid.update_pos();
					display_game(board, 0, action_wall);
					StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
				}
				player = !player;
			}
			else { // IA minmax
				display_game(board, -1, -1);
				StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
				Node node(depth, 0, board, -1);
				int bestChoice = -1;
				int bestValue = -MAX - 1;
				node.create_children();
				int val;
				for (Node child : node.children) {
					val = minmax(child, 1, -MAX, MAX);
					if (val > bestValue) {
						bestValue = val;
						bestChoice = child.state;
					}
				}
				if (bestChoice < numberGridSquares)
					board.moove_player(0, bestChoice);
				else {
					board.grid.add_wall(bestChoice - numberGridSquares + 1, 0);
					board.update_valid_next_pos(0);
					board.update_valid_next_pos(1);
					board.player1.dec_wall();
				}
				player = !player;
			}
		} else { // Player 2
			while (running && invalid_play) {
				updtate_input(window, &input);
				tie(action, state) = get_player_action(window, pci, board.grid);
				if (action == 'w' && board.player2.get_number_walls() > 0 && board.is_possible_wall(state, &dist1, &dist2)) action_wall = state;
				else action_wall = -1;
				if (input.buttons[MOUSE_LEFT].is_pressed) {
					if (action != 'n') {
						if (action == 'p') {
							if (board.player2.is_possible(state)) {
								board.moove_player(1, state);
								invalid_play = false;
							}
						}
						else { // action == 'w'
							if (board.player2.get_number_walls() > 0 && board.is_possible_wall(state, &dist1, &dist2)) {
								board.grid.add_wall(state, 1);
								board.update_valid_next_pos(1);
								board.update_valid_next_pos(0);
								board.player2.dec_wall();
								invalid_play = false;
							}
						}
					}
				}
				board.grid.update_pos();
				display_game(board, 1, action_wall);
				StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
			}
			player = !player;
		}
		gameover = board.player1.get_line() == 0 || board.player2.get_line() == GRID_SIZE-1;
	}
	
	if (gameover) running = true;
	while (running) {
		updtate_input(window, &input);
		display_game(board, 2, -1);
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
}