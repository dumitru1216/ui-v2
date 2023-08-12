#pragma once
#include "../../../c_includes.hpp"
#include "../../../sdk/easing/c_easing.hpp"

namespace gui {
	namespace controls { 
		bool text_box( const sdk::c_str& name, sdk::c_str* input, int max_text = 24 );
	}
}