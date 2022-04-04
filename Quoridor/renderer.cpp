#include "utils.cpp"



struct Render_State {
	int height, width;
	void* memory;

	BITMAPINFO bitmap_info;
};

static Render_State render_state;

static void clear_screen(int color) {
	int* pixel = (int*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}

static void draw_rect_in_pixels(int x0, int y0, int x1, int y1, int color) {

	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++) {
		int* pixel = (int*)render_state.memory + x0 + y * render_state.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}


static void draw_rect(double x, double y, double size_x, double size_y, int color) {
	x *= size_pixel;
	y *= size_pixel;
	size_x *= size_pixel;
	size_y *= size_pixel;


	// Change to pixels
	int x0 = static_cast<int>(x);
	int x1 = static_cast<int>(x + size_x);
	int y0 = static_cast<int>(y);
	int y1 = static_cast<int>(y + size_y);

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}






