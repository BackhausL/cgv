#include "vr_camera.h"

#include <iostream>
#include <string>

namespace vr {
vr_camera::vr_camera()
    : num_cameras(0u), frame_width(0u), frame_height(0u),
      frame_format(camera_frame_format::RGBA), frame_split(camera_frame_split::NONE), new_frame_available(false) {}

bool vr_camera::initialize() {
  // possible in error state, as it might be temporary
  if (!(state == camera_state::UNINITIALIZED || state == camera_state::ERROR)) {
    std::cerr
        << "Tried initializing an already initialized or running vr_camera"
        << std::endl;
    return false;
  }

  auto initialized = initialize_impl();

  if (initialized) {
    state = camera_state::INITIALIZED;
    return true;
  } else {
    state = camera_state::ERROR;
    return false;
  }
}

bool vr_camera::start() {
  if (state != camera_state::INITIALIZED) {
    std::cerr
        << "Tried starting an uninitialized, running or erroneous vr_camera"
        << std::endl;
    return false;
  }

  auto started = start_impl();

  if (started) {
    state = camera_state::STARTED;
    return true;
  } else {
    state = camera_state::ERROR;
    return false;
  }
}

bool vr_camera::stop() {
  if (state != camera_state::STARTED) {
    std::cerr
        << "Tried stopping an uninitialized, stopped or erroneous vr_camera"
        << std::endl;
    return false;
  }

  auto stopped = stop_impl();

  if (stopped) {
    state = camera_state::INITIALIZED;
    return true;
  } else {
    state = camera_state::ERROR;
    return false;
  }
}

bool vr_camera::is_new_frame_available() const { return new_frame_available; }

std::vector<unsigned char> vr_camera::get_frame() const { return frame; }

std::vector<unsigned char>& vr_camera::get_frame_ref()
{
	return frame;
}

uint32_t vr_camera::get_frame_width() const
{
	return frame_width;
}

uint32_t vr_camera::get_frame_height() const
{
	return frame_height;
}

camera_frame_format vr_camera::get_frame_format() const { return frame_format; }

camera_frame_split vr_camera::get_frame_split() const
{
	return frame_split;
}

camera_state vr_camera::get_state() const { return state; }

uint8_t vr_camera::get_num_cameras() const { return num_cameras; }

} // namespace vr