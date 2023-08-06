#pragma once
#include "../../../c_includes.hpp"
#include "../../../sdk/easing/c_easing.hpp"

namespace gui {
	namespace controls {
		/* int */
		bool slider( const sdk::c_str& name, int* var_name, int min, int max, 
					 const sdk::c_str& display = "%d", const sdk::c_str& tool_tip = "" );

		template <typename T>
		__forceinline T lerp( T a, T b, float t ) {
			return a + ( b - a ) * t;
		}
	}
}