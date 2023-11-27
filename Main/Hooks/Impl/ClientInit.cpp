#include "../Hooks.hpp"

/*
Source by: https://github.com/clxxe
Modify by: https://github.com/edok3q
If you used this source. Don't forget to credit me as an author
*/

auto impl::hooks::hk_bce_client_init( c_base_player* instance, void* entity ) -> void{
	//Return bp_client_init
	return impl::hooks::bce_client_init.get_original< decltype( &impl::hooks::hk_bce_client_init ) >( )( instance, entity );
}