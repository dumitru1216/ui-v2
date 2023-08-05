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
	using c_point = POINT;
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
	using c_size = size_t;
	using c_msg = MSG;

	/* math */
	namespace math {
		struct vec2_t {
			/* default builders */
			vec2_t( ) : x( 0 ), y( 0 ) { }
			vec2_t( float x, float y ) : x( x ), y( y ) { }
			~vec2_t( ) { }

			/* operators */
			bool operator==( const vec2_t& src ) const {
				return ( src.x == this->x ) && ( src.y == y );
			}

			bool operator!=( const vec2_t& src ) const {
				return ( src.x != this->x ) || ( src.y != y );
			}

			float& operator[]( int i ) {
				return ( ( float* )this )[ i ];
			}

			float operator[]( int i ) const {
				return ( ( float* )this )[ i ];
			}

			/* vec - vec */
			vec2_t& operator-=( const vec2_t& v ) {
				this->x -= v.x; this->y -= v.y;
				return *this;
			}

			vec2_t& operator-=( float fl ) {
				this->x -= fl;
				this->y -= fl;

				return *this;
			}

			vec2_t& operator-( vec2_t v ) {
				vec2_t temp;
				temp.x = this->x - v.x;
				temp.y = this->y - v.y;

				return temp;
			}

			/* parameters */
			float x;
			float y;
		};
	}

	/* color */
	inline float alpha_mod = -1.f;
	namespace color {
		struct col_t {
			col_t( ) : r( 255 ), g( 255 ), b( 255 ), a( 255 ) { } /* default white */
			col_t( int r, int g, int b, int a = 255 ) : r( r ), g( g ), b( b ), a( a ) { }
			~col_t( ) { }

			std::uint32_t convert( ) {
				std::uint32_t out = 0;

				out = static_cast< std::uint32_t >( this->r ) << 0;
				out |= static_cast< std::uint32_t >( this->g ) << 8;
				out |= static_cast< std::uint32_t >( this->b ) << 16;
				out |= static_cast< std::uint32_t >( this->a ) << 24;

				return out;
			}

			col_t modify_alpha( int a ) {
				return col_t( r, g, b, a );
			}

			col_t darker( int amount ) {
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

		struct fcol_t {
			fcol_t( ) = default;
			fcol_t( float _r, float _g, float _b, float _a = 1.0f ) :
				r( _r ), g( _g ), b( _b ), a( _a ) { }

			fcol_t( int _r, int _g, int _b, int _a = 255 ) {
				set_color( _r, _g, _b, _a );
			}

			c_function set_color( float _r, float _g, float _b, float _a = 1.0f ) {
				r = _r;
				g = _g;
				b = _b;
				a = _a;
			}

			float r, g, b, a;
		};
	}

	namespace g {
		inline bool c_menu_open = false;
	}

	namespace interfaces {
		class c_global_vars {
		public:
			int c_frame_time{};
		};

		/* warpers */
		inline const auto c_globals = std::make_unique< c_global_vars >( );
	}
}