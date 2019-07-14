#pragma once

#include <array>

#include <cgv/math/fvec.h>
#include <cgv/render/render_types_namespace.h>

using namespace cgv::render::render_types_ns;

namespace cgv {
namespace data {
// no class for now, just typedef
// do not inherit from stl containers (no virtual destructors)
using rectangle = std::array<vec3, 4>;
static const rectangle null_rectangle = {
    vec3{0.0f, 0.0f, 0.0f}, vec3{0.0f, 0.0f, 0.0f}, vec3{0.0f, 0.0f, 0.0f},
    vec3{0.0f, 0.0f, 0.0f}};
} // namespace data
} // namespace cgv