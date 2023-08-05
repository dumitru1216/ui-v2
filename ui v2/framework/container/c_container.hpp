#pragma once
#include "../../c_includes.hpp"

namespace gui {
	namespace containter {
		class impl {
		public:
			sdk::c_function draw_menu( );
		};
	}

	inline const auto c_container = std::make_unique< containter::impl >( );
}