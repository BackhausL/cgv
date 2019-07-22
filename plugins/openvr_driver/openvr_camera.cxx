#include "openvr_camera.h"

#include <iostream>
#include <string>

namespace vr {
	openvr_camera::openvr_camera(vr::IVRSystem *hmd)
		: hmd(hmd), tracked_camera(nullptr),
		tracked_camera_handle(INVALID_TRACKED_CAMERA_HANDLE), framebuffer_size(0u), query_rate(60.0f) {
		frame_format = camera_frame_format::RGBA;
		frame_split = camera_frame_split::UP_DOWN;

		distortion_type = EVRTrackedCameraFrameType::VRTrackedCameraFrameType_Distorted;
	}

	openvr_camera::~openvr_camera() {}

	bool openvr_camera::initialize_impl() {

		tracked_camera = vr::VRTrackedCamera();

		if (!tracked_camera) {
			std::cerr << "Camera could not be initialized!" << std::endl;
			return false;
		}

		bool has_camera = false;
		vr::EVRTrackedCameraError camera_error =
			tracked_camera->HasCamera(vr::k_unTrackedDeviceIndex_Hmd, &has_camera);
		if (camera_error != vr::VRTrackedCameraError_None || !has_camera) {
			std::cerr << "No Tracked Camera Available! (%s) "
				<< tracked_camera->GetCameraErrorNameFromEnum(camera_error)
				<< std::endl;
			return false;
		}

		/*Accessing the FW description is just a further check to ensure camera
		communication is valid as expected.*/
		vr::ETrackedPropertyError propertyError;
		char buffer[128];
		hmd->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd,
			vr::Prop_CameraFirmwareDescription_String,
			buffer, sizeof(buffer), &propertyError);
		if (propertyError != vr::TrackedProp_Success) {
			std::cerr << "Failed to get tracked camera firmware description!"
				<< std::endl;
			return false;
		}

		return true;
	}

	bool openvr_camera::start_impl() { // Allocate for camera frame buffer
									   // requirements
		uint32_t camera_framebuffer_size = 0;
		if (tracked_camera->GetCameraFrameSize(
			vr::k_unTrackedDeviceIndex_Hmd,
			distortion_type, &frame_width, &frame_height,
			&camera_framebuffer_size) != vr::VRTrackedCameraError_None) {
			std::cerr << "GetCameraFrameBounds() Failed!" << std::endl;
			return false;
		}

		if (camera_framebuffer_size &&
			camera_framebuffer_size !=
			framebuffer_size) {

			framebuffer_size = camera_framebuffer_size;

			switch (frame_format) {
			case camera_frame_format::RGBA: {
				frame.resize(framebuffer_size);
				break;
			}
			default:
				throw std::logic_error("Openvr camera invalid frame format");
			}

			frame.shrink_to_fit();
		}

		last_frame_sequence = 0;

		tracked_camera->AcquireVideoStreamingService(vr::k_unTrackedDeviceIndex_Hmd,
			&tracked_camera_handle);
		if (tracked_camera_handle == INVALID_TRACKED_CAMERA_HANDLE) {
			std::cerr << "AcquireVideoStreamingService() Failed!" << std::endl;
			return false;
		}

		// vr::HmdVector2_t focal_length, center;
		// auto e = tracked_camera->GetCameraIntrinsics(
		//    vr::k_unTrackedDeviceIndex_Hmd, 0,
		//    distortion_type, &focal_length, &center);

		return true;
	}

	bool openvr_camera::stop_impl() {
		tracked_camera->ReleaseVideoStreamingService(tracked_camera_handle);
		tracked_camera_handle = INVALID_TRACKED_CAMERA_HANDLE;

		return true;
	}

	bool openvr_camera::query() {
		if (!tracked_camera || !tracked_camera_handle ||
			get_state() != camera_state::STARTED)
			return false;

		new_frame_available = false;

		// TODO: timeout (2s)
		if (false) {
			std::cerr << "No Video Frames Arriving!" << std::endl;
		}

		// get the frame header only
		vr::CameraVideoStreamFrameHeader_t frame_header;
		vr::EVRTrackedCameraError camera_error =
			tracked_camera->GetVideoStreamFrameBuffer(
				tracked_camera_handle, distortion_type,
				nullptr, 0, &frame_header, sizeof(frame_header));
		if (camera_error != vr::VRTrackedCameraError_None)
			return false;

		if (frame_header.nFrameSequence == last_frame_sequence) {
			// frame hasn't changed yet, nothing to do
			return false;
		}

		// Frame has changed, do the more expensive frame buffer copy
		camera_error = tracked_camera->GetVideoStreamFrameBuffer(
			tracked_camera_handle, distortion_type,
			frame.data(), framebuffer_size, &frame_header, sizeof(frame_header));
		if (camera_error != vr::VRTrackedCameraError_None)
			return false;

		last_frame_sequence = frame_header.nFrameSequence;
		current_frame_header = frame_header;

		switch (frame_format) {
		case camera_frame_format::RGBA: {
			frame.resize(frame_width * frame_height * 4u);
			break;
		}
		default:
			throw std::logic_error("vr camera invalid frame format");
		}

		/// this would store ready to use texture in "tex" instead
		/*glUInt_t tex;
		camera_error = tracked_camera->GetVideoStreamTextureGL(tracked_camera_handle,
		distortion_type, &tex, &frame_header,
		sizeof(frame_header)); if (camera_error != vr::VRTrackedCameraError_None)
				return 0;*/

		last_frame_sequence = frame_header.nFrameSequence;
		current_frame_header = frame_header;
		new_frame_available = true;

		return true;
	}

} // namespace vr