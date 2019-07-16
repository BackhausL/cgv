#pragma once

#include <cgv/render/render_types_namespace.h>

using namespace cgv::render::render_types_ns;

#include "lib_begin.h"

namespace cgv {
namespace render {

	class CGV_API placeable {
	  private:
		vec3 position;
		vec3 pitch_yaw_roll;
		vec3 scale;
		mat4 model_matrix;

		void calculate_model_matrix();

	  public:
		placeable();
		placeable(const vec3 &position);
		placeable(const vec3 &position, const vec3 &pitch_yaw_roll);
		placeable(const vec3 &position, const vec3 &pitch_yaw_roll, const vec3 &scale);

		vec3 get_position() const;
		void set_position(const vec3 &position);
		void set_position_x(const float x);
		void set_position_y(const float y);
		void set_position_z(const float z);

		vec3 get_pitch_yaw_roll() const;
		void set_pitch_yaw_roll(const vec3 &pyr);
		void set_pitch(const float pitch);
		void set_yaw(const float yaw);
		void set_roll(const float roll);

		vec3 get_scale();
		void set_scale(const vec3 &scale);
		void set_scale_x(const float x);
		void set_scale_y(const float y);
		void set_scale_z(const float z);

		mat4 get_model_matrix() const;
		void set_model_matrix(const mat4 &model_matrix);
	};

} // namespace render
} // namespace cgv

#include <cgv/config/lib_end.h>