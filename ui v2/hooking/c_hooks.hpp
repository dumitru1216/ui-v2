#pragma once
#include "../c_includes.hpp"
static sdk::c_hwnd window;
static sdk::c_device_x device_x = NULL; /* device d3d */
static sdk::c_device device = NULL; /* device handle */
static sdk::c_device_parameter device_parameter = {};

/* backround */
extern IDirect3DTexture9* backround;

namespace i {
	namespace hooks {
		class impl {
		public:
			struct c_initialization {
				bool init_dev, init_imgui, init_cd, reg_wnd, init_wnd;
			};
			c_initialization c_log;

			sdk::c_function run_initialization( );
			sdk::c_function reset_device( );
			sdk::c_function clean_device( );
			sdk::c_function init_device( sdk::c_dev* device, std::function<void( )> function );
			sdk::c_function init_imgui( sdk::c_hwnd window, sdk::c_dev* device );

			sdk::c_bool hook_create_device( sdk::c_hwnd w );
			sdk::c_atom register_window( sdk::c_instance instance, sdk::c_lstr name );
			sdk::c_atom initialize_window( sdk::c_instance instance, sdk::c_lstr class_name, sdk::c_lstr title );
		};
	}

	inline const auto c_hooks = std::make_unique< hooks::impl >( );
}