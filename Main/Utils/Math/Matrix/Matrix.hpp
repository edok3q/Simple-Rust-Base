#pragma once

/*
Source by: https://github.com/clxxe
Modify by: https://github.com/edok3q
If you used this source. Don't forget to credit me as an author
*/

namespace math
{
	struct mat4x4_t
	{
		mat4x4_t( )
			: m{ { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 } }
		{}

		mat4x4_t( const mat4x4_t& ) = default;

		auto transpose( ) -> mat4x4_t {
			mat4x4_t m;

			for ( int i = 0; i < 4; i++ )
				for ( int j = 0; j < 4; j++ )
					m.m[ i ][ j ] = this->m[ j ][ i ];

			return m;
		}

		auto is_empty( ) -> bool {
			if ( !m[ 3 ][ 0 ] && !m[ 3 ][ 1 ] && !m[ 3 ][ 2 ] && !m[ 2 ][ 1 ] && !m[ 2 ][ 0 ] && !m[ 2 ][ 2 ] )
				return true;

			return false;
		}

		vector_t operator*( const vector_t& vec ) {
			mat4x4_t m;

			m[ 3 ][ 0 ] = vec.x;
			m[ 3 ][ 1 ] = vec.y;
			m[ 3 ][ 2 ] = vec.z;

			m[ 0 ][ 0 ] = 1;
			m[ 1 ][ 1 ] = 1;
			m[ 2 ][ 2 ] = 1;


			m[ 0 ][ 3 ] = 0.0f;
			m[ 1 ][ 3 ] = 0.0f;
			m[ 2 ][ 3 ] = 0.0f;
			m[ 3 ][ 3 ] = 1.0f;

			auto result = m * ( *this );

			return vector_t{ result[ 3 ][ 0 ], result[ 3 ][ 1 ], result[ 3 ][ 2 ] };
		}

		mat4x4_t operator*( const mat4x4_t& _m2 ) {
			auto _m = *this;

			mat4x4_t out;
			out[ 0 ][ 0 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 0 ];
			out[ 0 ][ 1 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 1 ];
			out[ 0 ][ 2 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 2 ];
			out[ 0 ][ 3 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 3 ];
			out[ 1 ][ 0 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 0 ];
			out[ 1 ][ 1 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 1 ];
			out[ 1 ][ 2 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 2 ];
			out[ 1 ][ 3 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 3 ];
			out[ 2 ][ 0 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 0 ];
			out[ 2 ][ 1 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 1 ];
			out[ 2 ][ 2 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 2 ];
			out[ 2 ][ 3 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 3 ];
			out[ 3 ][ 0 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 0 ];
			out[ 3 ][ 1 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 1 ];
			out[ 3 ][ 2 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 2 ];
			out[ 3 ][ 3 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 3 ];

			return out;
		}

		float* operator[]( size_t i ) { return m[ i ]; }
		const float* operator[]( size_t i ) const { return m[ i ]; }

		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[ 4 ][ 4 ];
		};
	};
}