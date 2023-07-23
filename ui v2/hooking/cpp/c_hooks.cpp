#include "../c_hooks.hpp"

/*
	note @dutu: we could have used .get()-> on c_init for better looking but i dont want to get 
	undefined behavior when the object its going to delete itself
*/

sdk::c_function i::hooks::impl::initialize( ) {

}

sdk::c_function i::hooks::impl::clean_device( ) {
	if ( device ) {
		sdk::c_warper->release_device( &device );
	}

	if ( device_x ) {
		sdk::c_warper->release_device_pointer( &device_x );
	}
}

sdk::c_function i::hooks::impl::reset_device( ) {
	sdk::c_result result{};

	/* store out in result */
	result = sdk::c_warper->reset_device( device_parameter );
	if ( result != D3DERR_INVALIDCALL ) { }
}

sdk::c_function i::hooks::impl::init_device( sdk::c_dev* device, std::function<void( )> function ) {
	IDirect3DStateBlock9* state_block{ NULL };

	/* create state block */
	if ( create_block( D3DSBT_ALL, &state_block ) < 0 ) {
		return;
	}

	/* get display size */
	sdk::c_rect screen_rect{};
	sdk::c_warper->take_client_rect( i::window, &screen_rect );

	/* display size, we are going to directly initialize it */
	sdk::math::vec2_t display_size{ sdk::math::vec2_t(
		( float )( screen_rect.right - screen_rect.left ), ( float )( screen_rect.bottom - screen_rect.top )
	) };

	/* flags */
	sdk::c_warper->set_pixel_shader( device, NULL );
	sdk::c_warper->set_vertex_shader( device, NULL );

	/* render states */ {
		sdk::c_warper->set_render_state( device, D3DRS_CULLMODE, D3DCULL_NONE );
		sdk::c_warper->set_render_state( device, D3DRS_LIGHTING, false );
		sdk::c_warper->set_render_state( device, D3DRS_ZENABLE, false );
		sdk::c_warper->set_render_state( device, D3DRS_ALPHABLENDENABLE, true );
		sdk::c_warper->set_render_state( device, D3DRS_ALPHATESTENABLE, false );
		sdk::c_warper->set_render_state( device, D3DRS_BLENDOP, D3DBLENDOP_ADD );
		sdk::c_warper->set_render_state( device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		sdk::c_warper->set_render_state( device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		sdk::c_warper->set_render_state( device, D3DRS_SCISSORTESTENABLE, true );
	}

	/* texture stages */ {
		sdk::c_warper->set_texture_stage_state( device, 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		sdk::c_warper->set_texture_stage_state( device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		sdk::c_warper->set_texture_stage_state( device, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		sdk::c_warper->set_texture_stage_state( device, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
		sdk::c_warper->set_texture_stage_state( device, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		sdk::c_warper->set_texture_stage_state( device, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	}

	/* sampler state */ {
		sdk::c_warper->set_sampler_state( device, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		sdk::c_warper->set_sampler_state( device, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	}

	/* viewport */
	sdk::c_viewport viewport{};
	viewport.X = viewport.Y = 0;
	viewport.Width = ( sdk::c_dword )display_size.x;
	viewport.Height = ( sdk::c_dword )display_size.y;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;

	/* set viewport */
	sdk::c_warper->set_viewport( device, &viewport );

	/* run the functions */
	function( );

	/* restore stateblock */
	state_block->Apply( );
	state_block->Release( );
}

sdk::c_bool i::hooks::impl::hook_create_device( sdk::c_hwnd window ) {
	if ( ( i::device_x = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL ) {
		return FALSE;
	}

	clean_memory( &i::device_parameter, sizeof( i::device_parameter ) );

	/* warp dx */
	sdk::c_warper->warp_dx( &i::device_parameter );

	/* create device */
	if ( create_device( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, i::window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &i::device_parameter, &i::device ) < 0 )
		return FALSE;

	return TRUE;
}

/* predefine */ sdk::c_lresult __stdcall wnd_handle( sdk::c_hwnd window, UINT msg, WPARAM wp, LPARAM lp );
sdk::c_atom i::hooks::impl::register_window( sdk::c_instance instance, sdk::c_lstr name ) {
	WNDCLASSEX wcex{};

	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wnd_handle;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon( instance, MAKEINTRESOURCE( IDI_GUITEST ) );
	wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = MAKEINTRESOURCE( IDC_GUITEST );
	wcex.lpszClassName = name;
	wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );
}

sdk::c_lresult __stdcall wnd_handle( sdk::c_hwnd window, UINT msg, WPARAM wp, LPARAM lp ) {
	return DefWindowProcA( window, msg, wp, lp );
}

sdk::c_atom i::hooks::impl::initialize_window( sdk::c_instance instance, sdk::c_lstr class_name, sdk::c_lstr title ) {
	sdk::c_rect screen_rect{};
	sdk::c_warper->take_window_rect( GetDesktopWindow( ), &screen_rect );

	/* finnaly we initialize our window */
	i::window = create_window_ex( WS_EX_APPWINDOW, class_name, title, WS_POPUP, screen_rect.left, screen_rect.top,
																 screen_rect.right, screen_rect.bottom, NULL, NULL, instance, NULL );

	if ( !i::window ) { return FALSE; }
	return TRUE;
}

sdk::c_function i::hooks::impl::init_imgui( sdk::c_hwnd window, sdk::c_dev* device ) {
	IMGUI_CHECKVERSION( );

	ImGui::CreateContext( );
	ImGuiIO& io = ImGui::GetIO( );
	ImGui::StyleColorsDark( );

	ImGui_ImplWin32_Init( window );
	ImGui_ImplDX9_Init( device );
}