#include "../c_input.hpp"

/* thanks alpha */
sdk::c_function sdk::input::update( ) {
	if ( GetForegroundWindow( ) == window ) {
		for ( sdk::c_size i = 0; i < 256; i++ ) {
			prev_key_state[ i ] = key_state[ i ];
			key_state[ i ] = GetAsyncKeyState( i );
		}
	}

	static sdk::c_hwnd f_window;
	if ( !f_window )
		f_window = FindWindowA( "Valve001", 0 ); // ui-v2

	sdk::c_point mouse;
	GetCursorPos( &mouse );
	ScreenToClient( f_window, &mouse );

	mouse_delta = mouse_pos - sdk::math::vec2_t( mouse.x, mouse.y );
	mouse_pos = sdk::math::vec2_t( mouse.x, mouse.y );
}

bool sdk::input::down( int key ) {
	return key_state[ key ] && prev_key_state[ key ];
}

bool sdk::input::pressed( int key ) {
	return key_state[ key ] && !prev_key_state[ key ];
}

bool sdk::input::released( int key ) {
	return !key_state[ key ] && prev_key_state[ key ];
}