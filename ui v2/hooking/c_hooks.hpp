#pragma once
#include "../c_includes.hpp"
namespace i {
	static sdk::c_hwnd window;
	static sdk::c_device_x device_x = NULL;
	static sdk::c_device device = NULL;

	namespace hooks {
		class impl {
		public:
			sdk::c_funcion initialize( );

			sdk::c_funcion reset_device( );
			sdk::c_funcion clean_device( );
		};
		inline const auto c_hooks = std::make_unique< impl >( );
	}
}