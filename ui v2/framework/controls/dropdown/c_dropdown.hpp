#pragma once
#include "../../../c_includes.hpp"
#include "../../../sdk/easing/c_easing.hpp"

namespace gui {
	namespace controls {
		bool dropdown( const sdk::c_str& name, sdk::c_str_vec values, int* var_name, int max_items = 10 );
	}
}