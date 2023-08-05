#pragma once
#include "../../../c_includes.hpp"
#include "../../../sdk/easing/c_easing.hpp"

namespace gui {
	namespace controls {
		bool checkbox( const sdk::c_str& name, bool* value, bool unsecure = false, const sdk::c_str& tool_tip = "" );
	}
}