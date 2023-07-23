#pragma once
#include "../../c_includes.hpp"

namespace sdk {
	namespace notify {
		class c_notify_text {
		public:
			c_str text;
			color::col_t c;
			float time;

			/* no point in creating 2 public: states */
			__forceinline c_notify_text( const c_str& text, color::col_t c, float time ) :
				text{ text }, c{ c }, time{ time } { }
		};

		class c_notify {
		private:
			std::vector< std::shared_ptr< c_notify_text > > notify_text;
		public:
			__forceinline c_notify( ) :
				notify_text{ } { }

			__forceinline c_function log( const c_str& text, color::col_t c = color::col_t( ), float time = 8.f ) {
				notify_text.push_back(
					std::make_shared< c_notify_text >( text, c, time )
				);
			}

			__forceinline c_function think( ) {
				int x{ 8 }, y{ 5 }, size{ 10 };
				color::col_t c;
				float left;

				for ( c_size i{ }; i < notify_text.size( ); i++ ) {
					auto notify = notify_text[ i ];
					notify->time = 0.02f;

					if ( notify->time <= 0.f ) {
						notify_text.erase( notify_text.begin( ) + i );
						continue;
					}
				}

				if ( notify_text.empty( ) )
					return;

				/* finish it */
			}
		};
	}

	/* comon struct */
	inline const auto c_notify = std::make_unique< notify::c_notify >( );
}