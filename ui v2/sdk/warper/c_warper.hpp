#pragma once
#include "../../c_includes.hpp"

namespace sdk {
	namespace warper { /* i know the right form is wraper */
		class impl {
		public:
			/* directx_sdk warpers */
			c_function set_vertex_shader( c_dev* device, IDirect3DVertexShader9* shader );
			c_function set_pixel_shader( c_dev* device, IDirect3DPixelShader9* shader );
			c_function set_render_state( c_dev* device, D3DRENDERSTATETYPE state, c_dword value );
			c_function set_texture_stage_state( c_dev* device, c_dword stage, D3DTEXTURESTAGESTATETYPE type, c_dword value );
			c_function set_sampler_state( c_dev* device, c_dword sampler, D3DSAMPLERSTATETYPE type, c_dword value );
			c_function set_transform( c_dev* device, D3DTRANSFORMSTATETYPE state, const D3DMATRIX* matrix );
			c_function set_viewport( c_dev* device, const c_viewport* viewport );
			c_function warp_dx( c_device_parameter* destination );
			c_function release_device( c_device* device );
			c_function release_device_pointer( c_device_x* device_pointer );
			c_function warp_frame( );
			c_function begin_frame( );
			c_function end_frame( );

			c_result begin_scene( c_device device );
			c_result end_scene( c_device device );
			c_result reset_device( c_device_parameter reset_parrameter );
			c_result clear( c_device device, c_dword count, const D3DRECT* p_rects, c_dword flags, D3DCOLOR color, float z, c_dword stencil );
			c_result create_device_window( c_device_x* p_d3d, UINT adapter, D3DDEVTYPE device_type, c_hwnd h_focus_window,
											c_dword behavior_flags, c_device_parameter* presentation_parameters, c_device pp_device );
			
			c_device_x create_device_warper( c_uint32 sdk_version );

			/* windows_sdk warpers */
			c_function alloc_console();

			c_bool take_client_rect( c_hwnd window_handle, RECT* screen_rect );
			c_bool take_window_rect( c_hwnd window_handle, RECT* screen_rect );
			c_bool unregister_class( LPCSTR lp_class_name, HINSTANCE instance );
			c_bool destroy_window( c_hwnd hwnd );
			c_bool show_window( c_hwnd hwnd, int n_cmd_show );
			c_bool update_window( c_hwnd hwnd );
			c_bool peek_message( LPMSG lp_msg, c_hwnd hwnd, UINT w_msfilter_min, UINT w_msfilter_max,
								 UINT w_remove_msg );
			c_bool translate_message( const MSG* lp_msg );

			c_lresult dispatch_message( const MSG* lp_msg );
			c_lresult def_window_proc( c_hwnd hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param );

			c_atom register_class( const WNDCLASSEX& wcex );
		};
	}

	inline const auto c_warper = std::make_unique< warper::impl >( );
}