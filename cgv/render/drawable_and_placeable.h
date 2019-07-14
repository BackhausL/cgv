#pragma once

#include <cgv/render/drawable.h>
#include <cgv/render/placeable.h>

#include "lib_begin.h"

namespace cgv {
namespace render {
// simple wrapper for drawable and placable in one
class CGV_API drawable_and_placeable : public drawable, public placeable {};

} // namespace render
} // namespace cgv

#include <cgv/config/lib_end.h>