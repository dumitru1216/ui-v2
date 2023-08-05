#pragma once
#include "../../../c_includes.hpp"

/* warning disable */
#pragma warning(disable : 4018)

namespace gui {
	namespace window {
		bool begin_window( sdk::c_str name );
		bool begin_tab( int icon, sdk::c_str name );

		sdk::c_function end_window( );

		/* window constructor */
		class impl {
		public:
			sdk::c_function reset( );
		};
	}

	inline const auto c_window = std::make_unique< window::impl >( );
}