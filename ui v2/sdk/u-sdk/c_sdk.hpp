#pragma once
#include "../../c_includes.hpp"

namespace sdk {
	using c_str = std::string;
	using c_str_vec = std::vector<std::string>;
	using c_uint32 = std::uint32_t;

	/* client funcs */
	using c_function = void;

	/* other */
	using c_hwnd = HWND;
	using c_device_x = LPDIRECT3D9;
	using c_device = LPDIRECT3DDEVICE9;
	using c_dev = IDirect3DDevice9;
	using c_viewport = D3DVIEWPORT9;
	using c_dword = DWORD;
	using c_result = HRESULT;
	using c_lresult = LRESULT;
	using c_atom = ATOM;
	using c_bool = BOOL;
	using c_rect = RECT;
	using c_device_parameter = D3DPRESENT_PARAMETERS;
	using c_instance = HINSTANCE;
	using c_lstr = LPCTSTR;

	/* math */
	namespace math {
		struct vec2_t {
			/* default builders */
			vec2_t( ) : x( 0 ), y( 0 ) { }
			vec2_t( float x, float y ) : x( x ), y( y ) { }
			~vec2_t( ) { }

			/* parameters */
			float x;
			float y;
		};
	}

	/* color */
	namespace color {
		struct col_t {
			col_t( ) : r( 255 ), g( 255 ), b( 255 ), a( 255 ) { } /* default white */
			col_t( int r, int g, int b, int a = 255 ) : r( r ), g( g ), b( b ), a( a ) { }
			~col_t( ) { }

			std::uint32_t g_convert( ) {
				std::uint32_t out = 0;

				out = static_cast< std::uint32_t >( this->r ) << 0;
				out |= static_cast< std::uint32_t >( this->g ) << 8;
				out |= static_cast< std::uint32_t >( this->b ) << 16;
				out |= static_cast< std::uint32_t >( this->a ) << 24;

				return out;
			}

			col_t g_modify_alpha( int a ) {
				return col_t( r, g, b, a );
			}

			col_t g_darker( int amount ) {
				int red = r;
				int green = g;
				int blue = b;

				// reduce the RGB values by the given amount
				red = std::fmax( 0, red - amount );
				green = std::fmax( 0, green - amount );
				blue = std::fmax( 0, blue - amount );

				return col_t( red, green, blue, a );
			}

			int r, g, b, a;
		};
	}
}