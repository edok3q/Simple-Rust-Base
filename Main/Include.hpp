#pragma once

/*
Source by: https://github.com/clxxe
Modify by: https://github.com/edok3q
If you used this source. Don't forget to credit me as an author
*/

//Utils
#include "Utils/Il2cpp/Il2cpp_Lib.hpp"
#include "Utils/Math/Vector/Vector.hpp"
#include "Utils/Math/Matrix/Matrix.hpp"
#include "Utils/Il2cpp/Managed.hpp"

//Unity
#include "Rust/Unity/Rendering/Color/Color.hpp"
#include "Rust/Unity/Unity_Sdk.hpp"
#include "Rust/Unity/Rendering/Unity_Gui.hpp"

#define DECLARE_MEMBER(type, klass, name) type & name() { FIELD(klass, ##name) return *reinterpret_cast<type*>(this + name); }

//Classes
#include "Rust/Classes/BaseWeapon.hpp"
#include "Rust/Classes/Transform.hpp"
#include "Rust/Classes/BasePlayer.hpp"

//Features
#include "Rust/Features/Visuals/Visuals.hpp"



