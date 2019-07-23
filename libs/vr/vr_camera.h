#pragma once

#include <vector>
#include <chrono>

#include "lib_begin.h"

///@ingroup VR
///@{

/**@file
  defines camera class for camera provided by vr_kit
*/
///
namespace vr {
/// different status values for a camera
enum class camera_state : uint8_t {
  UNINITIALIZED,
  INITIALIZED,
  STARTED,
  ERROR
};
enum class camera_frame_format : uint8_t { RGBA = 0 };
enum class camera_frame_split : uint8_t { NONE = 0, LEFT_RIGHT, UP_DOWN };

class CGV_API vr_camera {

public:
  vr_camera();
  virtual ~vr_camera() = default;

  bool initialize();
  bool start();
  bool stop();

  /// query camera for next frame
  /// implementations shall set new_frame_available & frame data
  bool query();
  bool is_new_frame_available() const;

  float get_query_limit() const;
  void set_query_limit(float fps);

  /// get raw frame, format described by get_frame_format()
  std::vector<unsigned char> get_frame() const;
  std::vector<unsigned char> &get_frame_ref();
  uint32_t get_frame_width() const;
  uint32_t get_frame_height() const;
  camera_frame_format get_frame_format() const;
  camera_frame_split get_frame_split() const;

  camera_state get_state() const;
  uint8_t get_num_cameras() const;


protected:
  uint8_t num_cameras;

  std::vector<unsigned char> frame;
  uint32_t frame_width;
  uint32_t frame_height;
  camera_frame_format frame_format;
  camera_frame_split frame_split;
  bool new_frame_available;

private:
  camera_state state;
  float query_limit;
  std::chrono::high_resolution_clock::time_point last_query_timepoint;

  virtual bool initialize_impl() = 0;
  virtual bool start_impl() = 0;
  virtual bool stop_impl() = 0;

  virtual bool query_impl() = 0;
};

} // namespace vr
///@}

#include <cgv/config/lib_end.h>
