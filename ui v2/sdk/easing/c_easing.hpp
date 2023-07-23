#pragma once
#include "../../c_includes.hpp"

namespace sdk {
	namespace easing {
        #define M_PI 3.14159265358979323846

		__forceinline double in_sine( const double t ) {
			return sin( 1.5707963 * t );
		}

		__forceinline double out_sine( double t ) {
			return 1 + sin( 1.5707963 * ( --t ) );
		}

        __forceinline double in_out_sine( const double t ) {
            return 0.5 * ( 1 + sin( 3.1415926 * ( t - 0.5 ) ) );
        }

        __forceinline double in_quad( const double t ) {
            return t * t;
        }

        __forceinline double out_quad( const double t ) {
            return t * ( 2 - t );
        }

        __forceinline double in_out_quad( const double t ) {
            return t < 0.5 ? 2 * t * t : t * ( 4 - 2 * t ) - 1;
        }

        __forceinline double in_cubic( const double t ) {
            return t * t * t;
        }

        __forceinline double out_cubic( double t ) {
            return 1 + ( --t ) * t * t;
        }

        __forceinline double in_out_cubic( double t ) {
            return t < 0.5 ? 4 * t * t * t : 1 + ( --t ) * ( 2 * ( --t ) ) * ( 2 * t );
        }

        __forceinline double in_quart( double t ) {
            t *= t;
            return t * t;
        }

        __forceinline double out_quart( double t ) {
            t = ( --t ) * t;
            return 1 - t * t;
        }

        __forceinline double in_out_quart( double t ) {
            if ( t < 0.5 ) {
                t *= t;
                return 8 * t * t;
            } else {
                t = ( --t ) * t;
                return 1 - 8 * t * t;
            }
        }

        __forceinline double in_quint( const double t ) {
            const double t2 = t * t;
            return t * t2 * t2;
        }

        __forceinline double out_quint( double t ) {
            const double t2 = ( --t ) * t;
            return 1 + t * t2 * t2;
        }

        __forceinline double in_out_quint( double t ) {
            double t2;
            if ( t < 0.5 ) {
                t2 = t * t;
                return 16 * t * t2 * t2;
            } else {
                t2 = ( --t ) * t;
                return 1 + 16 * t * t2 * t2;
            }
        }

        __forceinline double in_expo( const double t ) {
            return ( pow( 2, 8 * t ) - 1 ) / 255;
        }

        __forceinline double out_expo( const double t ) {
            return 1 - pow( 2, -8 * t );
        }

        __forceinline double in_out_expo( const double t ) {
            if ( t < 0.5 ) {
                return ( pow( 2, 16 * t ) - 1 ) / 510;
            } else {
                return 1 - 0.5 * pow( 2, -16 * ( t - 0.5 ) );
            }
        }

        __forceinline double in_circ( const double t ) {
            return 1 - sqrt( 1 - t );
        }

        __forceinline double out_circ( const double t ) {
            return sqrt( t );
        }

        __forceinline double in_out_circ( const double t ) {
            if ( t < 0.5 ) {
                return ( 1 - sqrt( 1 - 2 * t ) ) * 0.5;
            } else {
                return ( 1 + sqrt( 2 * t - 1 ) ) * 0.5;
            }
        }

        __forceinline double in_back( const double t ) {
            return t * t * ( 2.70158 * t - 1.70158 );
        }

        __forceinline double out_back( double t ) {
            return 1 + ( --t ) * t * ( 2.70158 * t + 1.70158 );
        }

        __forceinline double in_out_back( double t ) {
            if ( t < 0.5 ) {
                return t * t * ( 7 * t - 2.5 ) * 2;
            } else {
                return 1 + ( --t ) * t * 2 * ( 7 * t + 2.5 );
            }
        }

        __forceinline double in_elastic( const double t ) {
            const double t2 = t * t;
            return t2 * t2 * sin( t * M_PI * 4.5 );
        }

        __forceinline double out_elastic( const double t ) {
            const double t2 = ( t - 1 ) * ( t - 1 );
            return 1 - t2 * t2 * cos( t * M_PI * 4.5 );
        }

        __forceinline double in_out_elastic( const double t ) {
            double t2;
            if ( t < 0.45 ) {
                t2 = t * t;
                return 8 * t2 * t2 * sin( t * M_PI * 9 );
            } else if ( t < 0.55 ) {
                return 0.5 + 0.75 * sin( t * M_PI * 4 );
            } else {
                t2 = ( t - 1 ) * ( t - 1 );
                return 1 - 8 * t2 * t2 * sin( t * M_PI * 9 );
            }
        }

        __forceinline double in_bounce( const double t ) {
            return pow( 2, 6 * ( t - 1 ) ) * abs( sin( t * M_PI * 3.5 ) );
        }

        __forceinline double out_bounce( const double t ) {
            return 1 - pow( 2, -6 * t ) * abs( cos( t * M_PI * 3.5 ) );
        }

        __forceinline double in_out_bounce( const double t ) {
            if ( t < 0.5 ) {
                return 8 * pow( 2, 8 * ( t - 1 ) ) * abs( sin( t * M_PI * 7 ) );
            } else {
                return 1 - 8 * pow( 2, -8 * t ) * abs( sin( t * M_PI * 7 ) );
            }
        }

	}
}