#pragma once
#include "../../../c_includes.hpp"
#include "../../../sdk/easing/c_easing.hpp"

namespace gui {
	namespace controls {
		sdk::c_function button( const sdk::c_str& name, 
								std::function< sdk::c_function( ) > callback, bool use_unique_id = false );
	}
}