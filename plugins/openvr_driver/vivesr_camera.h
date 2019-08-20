#pragma once

#include <memory>
#include <vector>
#include <chrono>

#include <vr/vr_camera.h>

#include "ViveSR_API.h"
#include "ViveSR_Enums.h"

#include "lib_begin.h"

///@ingroup VR
///@{

/**@file
  defines camera class for camera provided by vivesr_kit
*/
///
namespace vr {
	class CGV_API vivesr_camera : public vr_camera {
	public:
		vivesr_camera();
		virtual ~vivesr_camera();

		enum distortion { DISTORTED, UNDISTORTED };

		distortion get_distortion_type() const;

	protected:
		int id_seethrough;
		int id_depth;

		CameraParams camera_parameters;



	private:
		uint32_t last_frame_sequence;
		uint32_t framebuffer_size;
		distortion distortion_type;
		int sr_state;

		bool initialize_impl() override;
		bool start_impl() override;
		bool stop_impl() override;
		bool query_impl() override;

	};

} // namespace vr
///@}

#include <cgv/config/lib_end.h>
