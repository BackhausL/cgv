#pragma once

#include <memory>
#include <vector>
#include <chrono>

#include <vr/vr_camera.h>

#include "openvr.h"

#include "lib_begin.h"

///@ingroup VR
///@{

/**@file
  defines camera class for camera provided by openvr_kit
*/
///
namespace vr {
class CGV_API openvr_camera : public vr_camera {
public:
  openvr_camera(vr::IVRSystem *hmd);
  virtual ~openvr_camera();

  bool query();

protected:
  vr::IVRSystem *hmd;

private:
  vr::IVRTrackedCamera *tracked_camera;
  vr::TrackedCameraHandle_t tracked_camera_handle;
  uint32_t last_frame_sequence;
  vr::CameraVideoStreamFrameHeader_t current_frame_header;

  bool initialize_impl();
  bool start_impl();
  bool stop_impl();

  float query_rate;
};

} // namespace vr
///@}

#include <cgv/config/lib_end.h>
