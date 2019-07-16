#pragma once

#include <memory>

#include <cgv/data/rectangle.h>
#include <cgv/render/attribute_array_binding.h>
#include <cgv/render/drawable_and_placeable.h>
#include <cgv/render/render_types_namespace.h>
#include <cgv/render/shader_program.h>
#include <cgv/render/vertex_buffer.h>

using namespace cgv::data;
using namespace cgv::math;
using namespace cgv::render;
using namespace cgv::render::render_types_ns;

#include "gl/lib_begin.h"

namespace cgv {
namespace render {
class CGV_API rectangle_blitter : public drawable_and_placeable {
private:
  rectangle rect;

  // does not own texture
  std::shared_ptr<texture> tex;

  attribute_array_binding vao;
  vertex_buffer v_buf{VertexBufferType::VBT_VERTICES,
                      VertexBufferUsage::VBU_DYNAMIC_DRAW};

  shader_program prog;

public:
  rectangle_blitter();
  rectangle_blitter(const rectangle &rect);
  rectangle_blitter(std::shared_ptr<texture> tex);
  rectangle_blitter(const rectangle &rect, std::shared_ptr<texture> tex);
  ~rectangle_blitter();

  bool init(context &ctx);
  void init_frame(context &ctx);

  // draw rectangle at position/orientation given by placeable
  // draw with member texture
  void draw(context &ctx);
  // draw with given texture
  void draw(context &ctx, const rectangle &rect, texture &texture);

  // draw fullscreen, ignores placeable attributes
  // draw with member texture
  void draw_fullscreen(context &ctx);
  // draw with given texture
  void draw_fullscreen(context &ctx, texture &texture);

  void clear(context &ctx);

  std::shared_ptr<texture> get_texture() const;
  void set_texture(std::shared_ptr<texture> t);

  rectangle get_rectangle() const;
  void set_rectangle(const rectangle &rectangle);

private:
  void draw_impl(context &ctx, const rectangle &rectangle, texture &texture);
  void draw_fullscreen_impl(context &ctx, texture &texture);
};

} // namespace render
} // namespace cgv

#include <cgv/config/lib_end.h>