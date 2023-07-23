#pragma once
#include "../c_includes.hpp"

namespace i {
	static sdk::c_hwnd window;
	static sdk::c_device_x device_x = NULL; /* device d3d */
	static sdk::c_device device = NULL; /* device handle */
	static sdk::c_device_parameter device_parameter = {};

	namespace hooks {
		class impl {
		public:
			sdk::c_function initialize( );

			sdk::c_function reset_device( );
			sdk::c_function clean_device( );
		};
		inline const auto c_hooks = std::make_unique< impl >( );
	}
}