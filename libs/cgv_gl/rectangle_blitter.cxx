#include "rectangle_blitter.h"

#include <cgv_gl/gl/gl.h>

namespace cgv {
namespace render {

rectangle_blitter::rectangle_blitter() : rect(null_rectangle) {}

rectangle_blitter::rectangle_blitter(const rectangle &rect) : rect(rect) {}

rectangle_blitter::rectangle_blitter(std::shared_ptr<texture> tex)
    : rect(null_rectangle), tex(tex) {}

rectangle_blitter::rectangle_blitter(const rectangle &rect,
                                     std::shared_ptr<texture> tex)
    : rect(rect), tex(tex) {}

rectangle_blitter::~rectangle_blitter() {}

bool rectangle_blitter::init(context &ctx) {

  bool success = vao.create(ctx);
  success = prog.build_program(ctx, "rectangle_blit.glpr", true) && success;

  prog.set_uniform(ctx, "tex", 0);

  std::vector<vec3> v = {rect[0], rect[1], rect[2], rect[3]};
  v_buf.create(ctx, v.data(), v.size());

  if (!success) {
    std::cerr << "couldn't initialize rectangle blitter" << std::endl;
  }

  return success;
}

void rectangle_blitter::init_frame(context &ctx) {}

void rectangle_blitter::draw(context &ctx) {
  if (!tex)
    return;

  vao.enable(ctx);

  // TODO: check if order is right, probably not
  std::vector<vec3> v = {rect[0], rect[1], rect[2], rect[3]};
  v_buf.replace(ctx, 0, v.data(), v.size());
  type_descriptor v_type(cgv::type::info::TI_FLT32, 3u, false);
  vao.set_attribute_array(ctx, 0, v_type, v_buf, 0u, 0u, 0u);

  prog.set_uniform(ctx, "fullscreen", false);

  ctx.push_modelview_matrix();
  {
    ctx.mul_modelview_matrix(get_model_matrix());
    prog.enable(ctx);
    tex->enable(ctx, 0);

    // attributeless rendering
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    tex->disable(ctx);
    prog.disable(ctx);
  }
  ctx.pop_modelview_matrix();

  vao.disable(ctx);
}

void rectangle_blitter::draw_fullscreen(context &ctx) {
  if (!tex)
    return;

  vao.enable(ctx);

  std::vector<vec3> v = {vec3(-1.0, 1.0, 0.0), vec3(-1.0, -1.0, 0.0),
                         vec3(1.0, 1.0, 0.0), vec3(1.0, -1.0, 0.0)};
  v_buf.replace(ctx, 0, v.data(), v.size());
  type_descriptor v_type(cgv::type::info::TI_FLT32, 3u, false);
  vao.set_attribute_array(ctx, 0, v_type, v_buf, 0u, 0u, 0u);

  prog.set_uniform(ctx, "fullscreen", true);

  prog.enable(ctx);
  tex->enable(ctx, 0);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  tex->disable(ctx);
  prog.disable(ctx);

  vao.disable(ctx);
}

void rectangle_blitter::clear(context &ctx) {
  // do not delete texture

  prog.destruct(ctx);

  v_buf.destruct(ctx);
  vao.destruct(ctx);
}

std::shared_ptr<texture> rectangle_blitter::get_texture() const { return tex; }

void rectangle_blitter::set_texture(std::shared_ptr<texture> t) { tex = t; }

rectangle rectangle_blitter::get_rectangle() const { return rect; }

void rectangle_blitter::set_rectangle(const rectangle &rectangle) {
  rect = rectangle;
}

} // namespace render
} // namespace cgv