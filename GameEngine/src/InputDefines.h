#pragma once

namespace GameEngine {

	enum class Key {
		Unknown = -1,
		Space = 32, 
		Apostrophe = 39, 
		Comma = 44, Minus, Period, Slash, 
		D0 = 48, D1, D2, D3, D4, D5, D6, D7, D8, D9,
		Semicolon = 59,
		Equal = 61, 
		A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		Left_bracket = 91, Backslash, Right_bracket, 
		Grave_accent = 96, 
		World_1 = 161, World_2,

		Escape = 256, Enter, Tab, Backspace, Insert, Delete, Right, Left, Down, Up, Page_up, Page_down, Home, End,
		Caps_lock = 280, Scroll_lock, Num_lock, Print_screen, Pause,
		F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
		Kp_0 = 320, Kp_1, Kp_2, Kp_3, Kp_4, Kp_5, Kp_6, Kp_7, Kp_8, Kp_9, Kp_decimal, Kp_divide, Kp_multiply, Kp_subtract, Kp_add, Kp_enter, Kp_equal,
		Left_shift = 340, Left_control, Left_alt, Left_super, Right_shift, Right_control, Right_alt, Right_super, Menu               

	};
	#define KEY_LAST Key::Menu

	enum class MouseButton {
		Left = 0, Right, Middle, MB4, MB5, MB6, MB7, MB8
	};
	#define MOUSE_BUTTON_LAST MouseButton::MB8

	enum class CursorType {
		Normal, Hidden, Disabled
	};
}