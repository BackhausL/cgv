#include <cgv/math/ftransform.h>

#include "placeable.h"

namespace cgv {
namespace render {

	// could use delegate-constructors
	placeable::placeable()
	    : position(vec3(0.0f)), pitch_yaw_roll(0.0f), scale(vec3{1.0f}),
	      model_matrix(cgv::math::identity4<float>())
	{
		// also possible: model_matrix.identity();
	}
	placeable::placeable(const vec3 &position)
	    : position(position), pitch_yaw_roll(0.0f), scale(vec3{1.0f})
	{
		calculate_model_matrix();
	}
	placeable::placeable(const vec3 &position, const vec3 &pitch_yaw_roll)
	    : position(position), pitch_yaw_roll(pitch_yaw_roll)
	{
		calculate_model_matrix();
	}

	placeable::placeable(const vec3 &position, const vec3 &pitch_yaw_roll,
	                     const vec3 &scale)
	    : position(position), pitch_yaw_roll(pitch_yaw_roll), scale(scale)
	{
		calculate_model_matrix();
	}

	void placeable::calculate_model_matrix()
	{
		auto pitch = cgv::math::rotate4(pitch_yaw_roll.x(), vec3(1.0f, 0.0f, 0.0f));
		auto yaw = cgv::math::rotate4(pitch_yaw_roll.y(), vec3(0.0f, 1.0f, 0.0f));
		auto roll = cgv::math::rotate4(pitch_yaw_roll.z(), vec3(0.0f, 0.0f, 1.0f));

		model_matrix = cgv::math::translate4(position) * roll * yaw * pitch *
		               cgv::math::scale4(scale);
	}

	vec3 placeable::get_position() const { return position; }
	void placeable::set_position(const vec3 &position)
	{
		this->position = position;
		calculate_model_matrix();
	}
	void placeable::set_position_x(const float x)
	{
		position.x() = x;
		calculate_model_matrix();
	}
	void placeable::set_position_y(const float y)
	{
		position.y() = y;
		calculate_model_matrix();
	}
	void placeable::set_position_z(const float z)
	{
		position.z() = z;
		calculate_model_matrix();
	}
	vec3 placeable::get_pitch_yaw_roll() const { return pitch_yaw_roll; }
	void placeable::set_pitch_yaw_roll(const vec3 &pyr)
	{
		pitch_yaw_roll = pyr;
		calculate_model_matrix();
	}
	void placeable::set_pitch(const float pitch)
	{
		pitch_yaw_roll.x() = pitch;
		calculate_model_matrix();
	}
	void placeable::set_yaw(const float yaw)
	{
		pitch_yaw_roll.y() = yaw;
		calculate_model_matrix();
	}
	void placeable::set_roll(const float roll)
	{
		pitch_yaw_roll.z() = roll;
		calculate_model_matrix();
	}
	vec3 placeable::get_scale() { return scale; }
	void placeable::set_scale(const vec3 &scale)
	{
		this->scale = scale;
		calculate_model_matrix();
	}
	void placeable::set_scale_x(const float x)
	{
		scale.x() = x;
		calculate_model_matrix();
	}
	void placeable::set_scale_y(const float y)
	{
		scale.y() = y;
		calculate_model_matrix();
	}
	void placeable::set_scale_z(const float z)
	{
		scale.z() = z;
		calculate_model_matrix();
	}
	mat4 placeable::get_model_matrix() const { return model_matrix; }
	void placeable::set_model_matrix(const mat4 &model_matrix)
	{
		this->model_matrix = model_matrix;
		calculate_model_matrix();
	}

} // namespace render
} // namespace cgv