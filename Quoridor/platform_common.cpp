struct Button_State {
	bool is_pressed;
};

enum {
	MOUSE_LEFT,
	MOUSE_RIGHT,
	BUTTON_COUNT, // Should be the last item
};

struct Input {
	Button_State buttons[BUTTON_COUNT];
};