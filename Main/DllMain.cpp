
/*
Source by: https://github.com/clxxe
Modify by: https://github.com/edok3q
Simple Rust Cheat.
If you used this source. Don't forget to credit me as an author
*/


#include "Utils/Il2cpp/Hook/Hook.hpp"
#include "Hooks/Hooks.hpp"
#include <Windows.h>

auto DllMain(void*, std::uint32_t call_reason, void*) -> bool {
	if (call_reason != 1) return false;

	/*Unity Il2cpp Init*/
	il2cpp_lib::init();

	/*Init hooks*/
	impl::hooks::ddraw_ongui.setup(xrs("UnityEngine::DDraw.OnGUI()"), &impl::hooks::hk_ddraw_ongui, 0); 
	impl::hooks::bp_client_input.setup(xrs("BasePlayer.ClientInput()"), &impl::hooks::hk_bp_client_input); 

	/*Beep After Inject*/
	Beep(330, 100);

	return true;
}

