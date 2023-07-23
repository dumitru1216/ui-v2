#pragma once
#include "../../c_includes.hpp"

namespace sdk {
	namespace input {
		inline bool key_state[ 256 ];
		inline bool prev_key_state[ 256 ];

		inline int scroll;

		inline math::vec2_t mouse_pos;
		inline math::vec2_t mouse_delta;

		c_function update( );

		bool down( int key );
		bool released( int key );
		bool pressed( int key );
	}
}