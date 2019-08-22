#include "vivesr_camera.h"

#include <iostream>
#include <string>
#include <atomic>

#ifdef WIN32
#include <Windows.h>
#endif

#define DISTORTED_IMAGE_W 612
#define DISTORTED_IMAGE_H 460
#define DISTORTED_IMAGE_C 4
#define UNDISTORTED_IMAGE_W 1150
#define UNDISTORTED_IMAGE_H 750
#define UNDISTORTED_IMAGE_C 4
#define DEPTH_DATA_W 640
#define DEPTH_DATA_H 480
#define DEPTH_DATA_C0 4
#define DEPTH_DATA_C1 1

vr::vivesr_camera* current_cam = nullptr;
std::atomic<bool> new_frame_available_atm = false;
std::atomic<int> ID_SEETHROUGH = -1;

namespace vr {
	void callback(int key);

	vivesr_camera::vivesr_camera()
		: framebuffer_size(0u), id_seethrough(-1), id_depth(-1), sr_state(-1) {
		frame_format = camera_frame_format::RGBA;
		frame_split = camera_frame_split::UP_DOWN;
		frame_flipped = true;

		distortion_type = distortion::DISTORTED;
	}

	vivesr_camera::~vivesr_camera() {
		current_cam = nullptr;
		stop_impl();
	}

	vivesr_camera::distortion vivesr_camera::get_distortion_type() const
	{
		return distortion_type;
	}

	bool vivesr_camera::initialize_impl() {
		current_cam = this;

		sr_state = ViveSR_Initial();
		if (sr_state != ViveSR::SR_Error::WORK) {
			fprintf(stderr, "ViveSR_Initial failed %d\n", sr_state);
			ViveSR_Stop();
			return false;
		}
		else {
			ViveSR_EnableLog(true);
			ViveSR_SetLogLevel(ViveSR::SR_LogLevel::SRLOG_LEVEL_MAX);

			sr_state = ViveSR_CreateModule(ViveSR::SRModule_TYPE::ENGINE_SEETHROUGH, &id_seethrough);
			fprintf(stderr, "ViveSR_CreateModule %d %s(%d)\n", id_seethrough, sr_state != ViveSR::SR_Error::WORK ? "failed" : "success", sr_state);
			if (sr_state != ViveSR::SR_Error::WORK) {
				ViveSR_Stop();
				return false;
			}
			ID_SEETHROUGH = id_seethrough;

			sr_state = ViveSR_CreateModule(ViveSR::SRModule_TYPE::ENGINE_DEPTH, &id_depth);
			fprintf(stderr, "ViveSR_CreateModule %d %s(%d)\n", id_depth, sr_state != ViveSR::SR_Error::WORK ? "failed" : "success", sr_state);
			if (sr_state != ViveSR::SR_Error::WORK) {
				ViveSR_Stop();
				return false;
			}

			// TODO: check if needed
			//Sleep(1000);

			ViveSR_GetCameraParams(&camera_parameters);

			ViveSR_SetParameterBool(id_seethrough, ViveSR::SeeThrough::Param::VR_INIT, true);
			ViveSR_SetParameterInt(id_seethrough, ViveSR::SeeThrough::Param::VR_INIT_TYPE, ViveSR::SeeThrough::InitType::SCENE);

			if (distortion_type == distortion::UNDISTORTED) {
				frame.resize(UNDISTORTED_IMAGE_W * UNDISTORTED_IMAGE_H * 2 * UNDISTORTED_IMAGE_C);
				frame_width = UNDISTORTED_IMAGE_W;
				frame_height = UNDISTORTED_IMAGE_H * 2u; // !important: 2 Images
			}
			else if (distortion_type == distortion::DISTORTED) {
				frame.resize(DISTORTED_IMAGE_W * DISTORTED_IMAGE_H * 2 * DISTORTED_IMAGE_C);
				frame_width = DISTORTED_IMAGE_W;
				frame_height = DISTORTED_IMAGE_H * 2u; // !important: 2 Images
			}

			return true;
		}
	}

	bool vivesr_camera::start_impl() {
		sr_state = ViveSR_StartModule(id_seethrough);
		if (sr_state != ViveSR::SR_Error::WORK) {
			fprintf(stderr, "ViveSR_StartModule %d failed(%d)\n", 0, sr_state);
			ViveSR_Stop();
			return false;
		}
		sr_state = ViveSR_StartModule(id_depth);
		if (sr_state != ViveSR::SR_Error::WORK) {
			fprintf(stderr, "ViveSR_StartModule %d failed(%d)\n", 1, sr_state);
			ViveSR_Stop();
			return false;
		}

		sr_state = ViveSR_ModuleLink(id_seethrough, id_depth, ViveSR::SRWorks_Link_Method::SR_ACTIVE);
		if (sr_state != ViveSR::SR_Error::WORK) {
			fprintf(stderr, "ViveSRs_link %d to %d failed(%d)\n", 0, 1, sr_state);
			ViveSR_Stop();
			return false;
		}

		if (sr_state != ViveSR::SR_Error::WORK) {
			fprintf(stderr, "ViveSR_Start failed\n");
			ViveSR_Stop();
			return false;
		}

		fprintf(stderr, "ViveSR_RegisterDataCallback %d\n", ViveSR_RegisterCallback(id_seethrough, ViveSR::SeeThrough::Callback::BASIC, callback));

		return true;
	}

	bool vivesr_camera::stop_impl() {
		fprintf(stderr, "ViveSR_UnregisterDataCallback %d\n", ViveSR_UnregisterCallback(ID_SEETHROUGH, ViveSR::SeeThrough::Callback::BASIC, callback));
		ViveSR_Stop();

		return true;
	}

	bool vivesr_camera::query_impl() {
		if (get_state() != camera_state::STARTED)
			return false;

		new_frame_available = false;

		if (new_frame_available_atm) {
			new_frame_available_atm = false;
			new_frame_available = true;
		}

		return true;
	}

	void callback(int key)
	{
		if (!current_cam) return;

		auto cam = static_cast<vivesr_camera*>(current_cam);

		auto half_image_size = 4 * current_cam->get_frame_width() *
			(current_cam->get_frame_height() / 2);

		auto data_ptr = current_cam->get_frame_ref().data();
		auto data_ptr_l = data_ptr;
		auto data_ptr_r = data_ptr + half_image_size;
		char* l, *r;

		if (cam->get_distortion_type() == vivesr_camera::distortion::UNDISTORTED) {
			ViveSR_GetPointer(key, ViveSR::SeeThrough::DataMask::UNDISTORTED_FRAME_LEFT, (void**)&l);
			ViveSR_GetPointer(key, ViveSR::SeeThrough::DataMask::UNDISTORTED_FRAME_RIGHT, (void**)&r);
		}
		else if (cam->get_distortion_type() == vivesr_camera::distortion::DISTORTED) {
			static int width, height, channel;
			static bool first = true;
			if (first) {
				ViveSR_GetParameterInt(ID_SEETHROUGH, ViveSR::SeeThrough::OUTPUT_DISTORTED_WIDTH, &width);
				ViveSR_GetParameterInt(ID_SEETHROUGH, ViveSR::SeeThrough::OUTPUT_DISTORTED_HEIGHT, &height);
				ViveSR_GetParameterInt(ID_SEETHROUGH, ViveSR::SeeThrough::OUTPUT_DISTORTED_CHANNEL, &channel);
				fprintf(stderr, "Distorted width :%d, height: %d, channel : %d\n", width, height, channel);
				first = false;
			}

			ViveSR_GetPointer(key, ViveSR::SeeThrough::DataMask::DISTORTED_FRAME_LEFT, (void**)&l);
			ViveSR_GetPointer(key, ViveSR::SeeThrough::DataMask::DISTORTED_FRAME_RIGHT, (void**)&r);
		}

		current_cam->lock_frame();
		{
			memcpy(data_ptr_l, l, half_image_size);
			memcpy(data_ptr_r, r, half_image_size);

			new_frame_available_atm = true;
		}
		current_cam->unlock_frame();
	}

} // namespace vr