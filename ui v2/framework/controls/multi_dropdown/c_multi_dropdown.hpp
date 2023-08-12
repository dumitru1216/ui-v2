#pragma once
#include "../../../c_includes.hpp"
#include "../../../sdk/easing/c_easing.hpp"
#include "../../gui/c_gui.hpp"

namespace gui {
	namespace controls {
		bool multi_dropdown( const sdk::c_str& name, std::vector< c_multi_item_t > values, int max_items = 10 );
	}
}