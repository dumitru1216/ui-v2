#pragma once
#include "../../c_includes.hpp"

#define draw_list ImGui::GetBackgroundDrawList()
#define im_vec_2 ImVec2

namespace sdk {
	namespace drawing {
		enum c_fonts {
			smallest_pixel,
			verdana,
			verdanab,
			tahoma,
			icon,
			default_font
		};

		c_function rect_filled( int x, int y, int w, int h, color::col_t c, int rounding = 0.f );
		c_function image( int x, int y, int w, int h, ImTextureID user_texture_id );
		c_function rect( int x, int y, int w, int h, color::col_t c, int rounding = 0.f );
		c_function gradient_vertical( int x, int y, int w, int h, color::col_t c, color::col_t c1 );
		c_function gradient_horizontal( int x, int y, int w, int h, color::col_t c, color::col_t c1 );
		c_function text( float x, float y, color::col_t c, int font_index, const char* text, bool drop_shadow = false );
		c_function init_fonts( );

		im_vec_2 get_text_size( const char* text, int font_index );
	}

	namespace view_port {
		/* for viewport */
		class c_view_port {
		public:
			c_view_port( c_dev* device ) {
				this->dev = device;
				this->vertex_shader = vertex_shader;

				device->GetViewport( &screen_size );
			}

			IDirect3DVertexShader9* vertex_shader;
			c_viewport screen_size;
		public:
			c_function init_device( );
		public:
			c_viewport get_viewport( ) {
				c_viewport vp;
				dev->GetViewport( &vp );
				return vp;
			}

			/* viewports */
			c_function set_viewport_dx( c_viewport vp ) {
				dev->SetViewport( &vp );
			}
		private:
			c_dev* dev;
			bool b_ready = false;
		};
		extern c_view_port* view_port;
	}
}