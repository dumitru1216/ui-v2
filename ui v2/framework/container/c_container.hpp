#pragma once
#include "../../c_includes.hpp"
#include "../gui/c_gui.hpp"

namespace gui {
	namespace containter {
		class impl {
		public:
			sdk::c_function draw_menu( );

			/* warp elements */
			__forceinline sdk::c_function begin_child( const sdk::c_str& name,
													   const sdk::math::vec2_t& size ) {
				gui::child::begin_child( name, size );
			}

			__forceinline sdk::c_function end_child( ) {
				gui::child::end_child( );
			}

			__forceinline bool checkbox( const sdk::c_str& name, bool* value, const sdk::c_str& tool_tip = "" ) {
				return gui::controls::checkbox( name, value, tool_tip );
			}

			__forceinline bool begin_window( sdk::c_str name ) {
				return gui::window::begin_window( name );
			}

			__forceinline bool begin_tab( sdk::c_str name ) {
				return gui::window::begin_tab( name );
			}

			__forceinline sdk::c_function end_window( ) {
				gui::window::end_window( );
			}
		};
	}

	inline const auto c_container = std::make_unique< containter::impl >( );
}