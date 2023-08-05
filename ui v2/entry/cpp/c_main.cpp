#include "../../c_includes.hpp"
#include "../../sdk/notify/c_notify.hpp"
#include "../../sdk/input/c_input.hpp"
#include "../../framework/container/c_container.hpp"
#include "../../framework/gui/c_gui.hpp"

int main( sdk::c_instance instance, sdk::c_instance prev_instance,
		  LPWSTR cmd_line, int cmd_show ) {
	sdk::c_warper->alloc_console( );

	/* init */
	sdk::interfaces::c_globals->c_frame_time = 0.02;

	/* name */
	sdk::c_lstr lpz_class{ "ui-v2" };
	sdk::c_lstr name{ "ui-v2" };

	if ( !i::c_hooks->register_window( instance, lpz_class ) ||
		 !i::c_hooks->initialize_window( instance, lpz_class, name ) ) {

		/* stop executing */
		return 0;
	}

	/* initialize window and device -> gheto */ {
		if ( ( device_x = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL ) {
			return FALSE;
		}

		clean_memory( &device_parameter, sizeof( device_parameter ) );

		/* warp dx */
		sdk::c_warper->warp_dx( &device_parameter );

		if ( !window ) {
			sdk::c_rect screen_rect{};
			GetWindowRect( GetDesktopWindow( ), &screen_rect );

			/* on the whole screen */
			window = CreateWindowEx( WS_EX_APPWINDOW, lpz_class, name, WS_POPUP,
									 screen_rect.left, screen_rect.top, screen_rect.right /* width */,
									 screen_rect.bottom /* height */, NULL, NULL, instance, NULL );
		}

		/* create device */
		if ( device_x->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &device_parameter, &device ) < 0 )
			return 0;
	}

	/* initialize input system */
	{
		D3DDEVICE_CREATION_PARAMETERS params;
		device->GetCreationParameters( &params );
		window = params.hFocusWindow;

		if ( !sdk::input::input_sys::get( )->initialize( device ) ) {
			return false;
		}
	}

	/* window updating */
	sdk::c_warper->show_window( window, SW_SHOWDEFAULT );
	sdk::c_warper->update_window( window );

	/* warp imgui */
	i::c_hooks->init_imgui( window, device );

	/* mem handler */
	sdk::c_msg msg{ };
	clean_memory( &msg, sizeof( msg ) );
	
	/* initialize */
	i::c_hooks->run_initialization( );

	/* loop */
	while ( msg.message != WM_QUIT ) {
		if ( sdk::c_warper->peek_message( &msg, NULL, 0U, 0U, PM_REMOVE ) ) {
			sdk::c_warper->translate_message( &msg );
			sdk::c_warper->dispatch_message( &msg );
			continue;
		}

		sdk::c_warper->warp_frame( );
		sdk::c_warper->begin_frame( ); {
			/* initialize ui */
			sdk::input::update( );

			if ( sdk::input::input_sys::get( )->was_key_pressed( VK_INSERT ) ) {
				sdk::g::c_menu_open = !sdk::g::c_menu_open;
				sdk::c_notify->log( "state changed" );
			}

			/* setup */
			gui::ctx->animation = sdk::g::c_menu_open ? ( gui::ctx->animation + ( 1.0f / 0.2f ) * 0.02 ) :
				( ( gui::ctx->animation - ( 1.0f / 0.2f ) * 0.02 ) );

			gui::ctx->animation = std::clamp<float>( gui::ctx->animation, 0.f, 1.0f );

			if ( !sdk::g::c_menu_open )
				gui::ctx->colorpicker_info.hashed_id = 0;

			/* run there */
			gui::c_container->draw_menu( );

			/* think log drawing */
			sdk::c_notify->think( );
		}
		sdk::c_warper->end_frame( );

		/* render states and clear device */
		sdk::c_warper->set_render_state( device, D3DRS_ZENABLE, false );
		sdk::c_warper->set_render_state( device, D3DRS_ALPHABLENDENABLE, false );
		sdk::c_warper->set_render_state( device, D3DRS_SCISSORTESTENABLE, false );

		/* clear */
		sdk::c_warper->clear(
			device, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 23, 23, 23, 0 ), 1.0f, 0
		);

		/* init scene */
		if ( sdk::c_warper->begin_scene( device ) >= 0 ) {
			i::c_hooks->init_device( device, [ = ]( ) {
				/* here we are going to render our stuff */
				ImGui::Render( );
				ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
			} );


			/* unload external window */
			if ( GetAsyncKeyState( VK_END ) ) {
				ExitProcess( -1 );
			}

			/* end scene */
			sdk::c_warper->end_scene( device );
		}

		/* handle packet loss - i will handle it tmr */
		HRESULT result{ device->Present( NULL, NULL, NULL, NULL ) };
		if ( result == D3DERR_DEVICELOST && device->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET )
			i::c_hooks->reset_device( );
	}

	/* clean again */
	i::c_hooks->clean_device( );

	/* destroy it */
	sdk::c_warper->destroy_window( window );

	/* unregister class */
	sdk::c_warper->unregister_class( lpz_class, instance );

	/* stop it */
	return 0;
}