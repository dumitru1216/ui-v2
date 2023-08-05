#pragma once
#include "../../../c_includes.hpp"

namespace gui {
	namespace child {
		sdk::c_function begin_child(
			const sdk::c_str& name, const sdk::math::vec2_t& size
		);
		sdk::c_function end_child( );
		sdk::c_function set_next_size(
			const sdk::math::vec2_t& size
		);
		sdk::c_function pop_last_size( );
	}
}