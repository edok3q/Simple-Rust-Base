#pragma once
#include "../../Include.hpp"
#include "../../Utils/Xorstr/Xorstr.hpp"
#include "../../Utils/Il2cpp/Il2cpp_Lib.hpp"

/*
Source by: https://github.com/clxxe
Modify by: https://github.com/edok3q
If you used this source. Don't forget to credit me as an author
*/


namespace unity
{
	inline std::uintptr_t asset_bundle, camera;

	struct rect_t{
		float x, y, w, h;
		rect_t(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h) { }
		rect_t() : x(0), y(0), w(0), h(0) { }

		auto contains(math::vector_2d_t point) -> bool{
			return point.x >= x && point.x < (x + w) && point.y >= y && point.y < (y + h);
		}
	};

	class c_screen {
	public:
		static auto get_width( ) -> int {
			METHOD( get_screen_width_fn, xrs("UnityEngine::Screen.get_width()"), 0, xrs(""), -1, int ( * )( ) );
			return get_screen_width_fn( );
		}

		static auto get_height( ) -> int {
			METHOD( get_screen_height_fn, xrs("UnityEngine::Screen.get_height()"), 0, xrs(""), -1, int ( * )( ) );
			return get_screen_height_fn( );
		}
	};

	inline auto view_matrix( ) -> math::mat4x4_t
	{
		ICALL( get_main_camera_fn, xrs("UnityEngine.Camera::get_main()"), std::uintptr_t( * )( ) );

		if ( !camera ) {
			camera = get_main_camera_fn( );
		}

		auto camera_ = *reinterpret_cast< std::uintptr_t* >( camera + 0x10 );
		if ( !camera_ ) {
			return {};
		}

		auto matrix = *reinterpret_cast< math::mat4x4_t* >( camera_ + 0x2e4 );
		if ( !matrix.m ) {
			return {};
		}

		return matrix;
	}

	inline auto screen_transform( math::vector_t wrld ) -> math::vector_t
	{
		auto matrix = view_matrix( );

		if ( !matrix.m ) {
			return { 0,0,0 };
		}

		math::vector_t out;
		const auto temp = matrix.transpose( );

		auto trans = math::vector_t{ temp[ 3 ][ 0 ], temp[ 3 ][ 1 ], temp[ 3 ][ 2 ] };
		auto up = math::vector_t{ temp[ 1 ][ 0 ], temp[ 1 ][ 1 ], temp[ 1 ][ 2 ] };
		auto right = math::vector_t{ temp[ 0 ][ 0 ], temp[ 0 ][ 1 ], temp[ 0 ][ 2 ] };

		float w = trans.dot( wrld ) + temp[ 3 ][ 3 ];

		if ( w < 0.001f ) {
			return math::vector_t( 0, 0, 0 );
		}

		float x = right.dot( wrld ) + temp[ 0 ][ 3 ];
		float y = up.dot( wrld ) + temp[ 1 ][ 3 ];

		out.x = ( static_cast< float >( unity::c_screen::get_width( ) ) / 2 )* ( 1 + x / w );
		out.y = ( static_cast< float >( unity::c_screen::get_height( ) ) / 2 )* ( 1 - y / w );
		out.z = 0.0f;

		return out;
	}
}