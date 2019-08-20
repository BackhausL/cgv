#pragma once

namespace ViveSR
{
	enum Module_Status {
		SR_DISABLE			 = 0,
		SR_ENABLE			 = 1,
		SR_UPDATE			 = 2,
		SR_NOT_UPDATE		 = 3,
	};
	enum Module_Param {
		ENABLE_FPSCONTROL	 = 1001,
		SET_FPS				 = 1002,
	};
	enum SRModule_TYPE {
		ENGINE_SEETHROUGH				= 1,	// support device : VIVE Pro
		DEVICE_VIVE_HMD_DUALCAM			= 2,	// obsolete
		ENGINE_UNDISTORTION				= 3,	// obsolete
		ENGINE_DEPTH					= 4,
		ENGINE_RIGID_RECONSTRUCTION		= 5,
		ENGINE_HUMAN_DETECTION		    = 6,
		ENGINE_AI_VISION				= 7,
		ENGINE_MAX,
	};

	enum SRWorks_Link_Method {
		SR_NONE				 = -1,
		SR_PASSIVE			 = 0,
		SR_ACTIVE			 = 1,
	};
	typedef SRWorks_Link_Method SRWork_Link_Method;

	enum SR_Error {
		NOT_INITIAL			 = -2,
		FAILED				 = -1,
		WORK				 = 0,
		INVAILD_INPUT		 = 1,
		FILE_NOT_FOUND		 = 2,
		DATA_NOT_FOUND		 = 13,
		INITIAL_FAILED		 = 1001,
		NOT_IMPLEMENTED		 = 1003,
		NULL_POINTER		 = 1004,
		OVER_MAX_LENGTH		 = 1005,
		FILE_INVALID		 = 1006,
		UNINSTALL_STEAM		 = 1007,
		MEMCPY_FAIL			 = 1008,
		NOT_MATCH			 = 1009,
		NODE_NOT_EXIST		 = 1010,
		UNKONW_MODULE		 = 1011,
		MODULE_FULL			 = 1012,
		UNKNOW_TYPE			 = 1013,
		INVALID_MODULE		 = 1014,
		INVALID_TYPE		 = 1015,
		MEMORY_NOT_ENOUGH	 = 1016,
		BUZY				 = 1017,
		NOT_SUPPORT			 = 1018,
		INVALID_VALUE		 = 1019,
		COMING_SOON			 = 1020,
		INVALID_CHANGE		 = 1021, // this/these module parameter can't be changed dynamic
	};

	enum SR_LogLevel {
		SRLOG_LEVEL_0		 = 3,	// turn-off any log except error & warning
		SRLOG_LEVEL_1		 = 4,	// the lower level, the less log, default value
		SRLOG_LEVEL_2		 = 5,
		SRLOG_LEVEL_3		 = 6,
		SRLOG_LEVEL_MAX		 = 10,	// turn-on all log msg
	};

	enum SR_ProcessUnit {
		CPU = 1,
		GPU = 2,
	};

	enum SR_ImageType
	{
		DISTORTED = 0,
		UNDISTORTED = 1,
	};

	enum CameraParam
	{
		CX_L,
		CX_R,
		CY_L,
		CY_R,
		FOCAL_LENGTH_L,
		FOCAL_LENGTH_R,
		R_M0,
		R_M1,
		R_M2,
		R_M3,
		R_M4,
		R_M5,
		R_M6,
		R_M7,
		R_M8,
		T_M0,
		T_M1,
		T_M2,
	};

	namespace SeeThrough
	{
		enum Param
		{
			VR_INIT								= 0,
			VR_INIT_TYPE,
			OUTPUT_DISTORTED_WIDTH,
			OUTPUT_DISTORTED_HEIGHT,
			OUTPUT_DISTORTED_CHANNEL,
			OUTPUT_UNDISTORTED_WIDTH,
			OUTPUT_UNDISTORTED_HEIGHT,
			OUTPUT_UNDISTORTED_CHANNEL,
			OUTPUT_FPS,
			OFFSET_HEAD_TO_CAMERA,						// float[6]  (x0,y0,z0,x1,y1,z1)
			PLAY_AREA_RECT,								// float[12] (x0,y0,z0,x1,y1,z1,...)
			VIDEO_RES_NATIVE_PTR,
			VIDEO_RES_VIEW_NATIVE_PTR,
			IMAGE_NATIVE_TEXTURE_PTR_L,
			IMAGE_NATIVE_TEXTURE_PTR_R,
			CAMERA_BRIGHTNESS					= 100,
			CAMERA_CONTRAST,
			CAMERA_HUE,
			CAMERA_SATURATION,
			CAMERA_SHARPNESS,
			CAMERA_GAMMA,
			CAMERA_COLOR_ENABLE,
			CAMERA_WHITE_BALANCE,
			CAMERA_BACKLIGHT_COMPENSATION,
			CAMERA_GAIN,
			CAMERA_PAN,
			CAMERA_TILT,
			CAMERA_ROLL,
			CAMERA_ZOOM,
			CAMERA_EXPOSURE,
			CAMERA_IRIS,
			CAMERA_FOCUS,
			UNDISTORTION_MODE					= 200,
			UNDISTORTION_CX,
			UNDISTORTION_CY,
			UNDISTORTION_FOCAL_LENGTH,
			UNDISTORTION_FMAT_RM_L,
			UNDISTORTION_FMAT_RM_R,
			UNDISTORTION_INTRINSIC_L,
			UNDISTORTION_INTRINSIC_R,
			UNDISTORTION_R_RECTIFY_L,
			UNDISTORTION_R_RECTIFY_R,
			UNDISTORTION_COEFFS_L,
			UNDISTORTION_COEFFS_R,
			UNDISTORTION_P_NEWPROJ_L,
			UNDISTORTION_P_NEWPROJ_R,
			UNDISTORTION_MAP_SIZE,
			UNDISTORTION_MAP_L,
			UNDISTORTION_MAP_R,
			UNDISTORTION_CENTER,
			MESH_NATIVE_VERTEX_BUFFER_PTR_L,		// void *
			MESH_NATIVE_VERTEX_BUFFER_PTR_R,		// void *
			MESH_NATIVE_VERTEX_ARRAY_L,				// void *
			MESH_NATIVE_VERTEX_ARRAY_R,				// void *
			MESH_CALIB_NATIVE_VERTEX_BUFFER_PTR_L,	// void *
			MESH_CALIB_NATIVE_VERTEX_BUFFER_PTR_R,	// void *
			MESH_CALIB_NATIVE_VERTEX_ARRAY_L,		// void *
			MESH_CALIB_NATIVE_VERTEX_ARRAY_R,		// void *
			ENABLE_IMAGE_PLANE_MESH_L,				// bool
			ENABLE_IMAGE_PLANE_MESH_R,				// bool
			ENABLE_CALIB_IMAGE_PLANE_MESH_L,		// bool
			ENABLE_CALIB_IMAGE_PLANE_MESH_R,		// bool
		};
		enum CTRL_MODE {
			CTRL_AUTO				 = 0x01,
			CTRL_MANUAL				 = 0x02,
		};
		enum Cmd {
			RESET_AllCAMERA_QUALITY,
		};
		enum InitType {
			SCENE = 1,	   // Application will submit 3D frames
			BACKGROUND = 3, // Application should not start SteamVR if it's not already running, and should not
		};
		enum DataMask
		{
			DISTORTED_FRAME_LEFT    = 0,		// sizeof(char) * 612 * 460 * 4
			DISTORTED_FRAME_RIGHT   = 1,		// sizeof(char) * 612 * 460 * 4
			UNDISTORTED_FRAME_LEFT	= 2,		// sizeof(char) * 1150 * 750 * 4
			UNDISTORTED_FRAME_RIGHT = 3,		// sizeof(char) * 1150 * 750 * 4
			FRAME_SEQ				= 4,		// sizeof(unsigned int)
			TIME_STP				= 5,		// sizeof(unsigned int)
			POSE_LEFT				= 6,		// sizeof(float) * 16
			POSE_RIGHT				= 7,		// sizeof(float) * 16
			LUX_LEFT				= 8,		// sizeof(int)
			LUX_RIGHT				= 9,		// sizeof(int)
			COLOR_TEMPERATURE_LEFT	= 10,		// sizeof(int)
			COLOR_TEMPERATURE_RIGHT = 11,		// sizeof(int)
			EXPOSURE_TIME_LEFT		= 12,		// sizeof(int)
			EXPOSURE_TIME_RIGHT		= 13,		// sizeof(int)
			ANALOG_GAIN_LEFT		= 14,		// sizeof(int)
			ANALOG_GAIN_RIGHT		= 15,		// sizeof(int)
			DIGITAL_GAIN_LEFT		= 16,		// sizeof(int)
			DIGITAL_GAIN_RIGHT		= 17,		// sizeof(int)
			DM_MAX,
		};
		enum Callback {
			BASIC = 1001,
		};
	}
	namespace DualCamDevice
	{
		enum Param
		{
			VR_INIT							 = 0,
			VR_INIT_TYPE,
			OUTPUT_WIDTH,
			OUTPUT_HEIGHT,
			OUTPUT_CHANNEL,
			OUTPUT_FPS,
			OFFSET_HEAD_TO_CAMERA,					// float[6]  (x0,y0,z0,x1,y1,z1)
			PLAY_AREA_RECT,							// float[12] (x0,y0,z0,x1,y1,z1,...)
			VIDEO_RES_NATIVE_PTR,
			VIDEO_RES_VIEW_NATIVE_PTR,
			CAMERA_BRIGHTNESS				 = 100,
			CAMERA_CONTRAST,
			CAMERA_HUE,
			CAMERA_SATURATION,
			CAMERA_SHARPNESS,
			CAMERA_GAMMA,
			CAMERA_COLOR_ENABLE,
			CAMERA_WHITE_BALANCE,
			CAMERA_BACKLIGHT_COMPENSATION,
			CAMERA_GAIN,
			CAMERA_PAN,
			CAMERA_TILT,
			CAMERA_ROLL,
			CAMERA_ZOOM,
			CAMERA_EXPOSURE,
			CAMERA_IRIS,
			CAMERA_FOCUS,
		};
		enum CTRL_MODE {
			CTRL_AUTO			 = 0x01,
			CTRL_MANUAL			 = 0x02,
		};
		enum Cmd {
			RESET_AllCAMERA_QUALITY,
		};
		enum InitType {
			SCENE				 = 1,	// Application will submit 3D frames
			BACKGROUND			 = 3,	// Application should not start SteamVR if it's not already running, and should not
		};
		enum DataMask
		{
			LEFT_FRAME				= 0,		// sizeof(char) * 612 * 460 * 4
			RIGHT_FRAME				= 1,		// sizeof(char) * 612 * 460 * 4
			FRAME_SEQ				= 2,		// sizeof(unsigned int)
			TIME_STP				= 3,		// sizeof(unsigned int)
			LEFT_POSE				= 4,		// sizeof(float) * 16
			RIGHT_POSE				= 5,		// sizeof(float) * 16
			LUX_LEFT				= 6,		// sizeof(int)
			LUX_RIGHT				= 7,		// sizeof(int)
			COLOR_TEMPERATURE_LEFT	= 8,		// sizeof(int)
			COLOR_TEMPERATURE_RIGHT = 9,		// sizeof(int)
			EXPOSURE_TIME_LEFT		= 10,		// sizeof(int)
			EXPOSURE_TIME_RIGHT		= 11,		// sizeof(int)
			ANALOG_GAIN_LEFT		= 12,		// sizeof(int)
			ANALOG_GAIN_RIGHT		= 13,		// sizeof(int)
			DIGITAL_GAIN_LEFT		= 14,		// sizeof(int)
			DIGITAL_GAIN_RIGHT		= 15,		// sizeof(int)
			DM_MAX,
		};
		enum Callback {
			BASIC				 = 1001,
		};
	}
	namespace Undistorted
	{
		enum Param
		{
			OUTPUT_WIDTH,
			OUTPUT_HEIGHT,
			OUTPUT_CHAANEL,
			CX						 = 3,
			CY						 = 4,
			MODE					 = 5,
			FOCULENS				 = 6,
			FMAT_RM_L				 = 7,
			FMAT_RM_R				 = 8,
			INTRINSIC_L				 = 9,
			INTRINSIC_R				 = 10,
			R_RECTIFY_L				 = 13,
			R_RECTIFY_R				 = 14,
			COEFFS_L				 = 15,
			COEFFS_R				 = 16,
			P_NEWPROJ_L				 = 17,
			P_NEWPROJ_R				 = 18,
			MAP_UndistortionSize,
			MAP_Undistortion_L,
			MAP_Undistortion_R,
			UndistortionCenter,
			IMAGE_NativeTexPtr_L,
			IMAGE_NativeTexPtr_R,
		};
		enum DataMask
		{
			LEFT_FRAME				= 0,		// sizeof(char) * 1150 * 750 * 4
			RIGHT_FRAME				= 1,		// sizeof(char) * 1150 * 750 * 4
			FRAME_SEQ				= 2,		// sizeof(unsigned int)
			TIME_STP				= 3,		// sizeof(unsigned int)
			LEFT_POSE				= 4,		// sizeof(float) * 16
			RIGHT_POSE				= 5,		// sizeof(float) * 16
			LUX_LEFT				= 6,		// sizeof(int)
			LUX_RIGHT				= 7,		// sizeof(int)
			COLOR_TEMPERATURE_LEFT	= 8,		// sizeof(int)
			COLOR_TEMPERATURE_RIGHT = 9,		// sizeof(int)
			EXPOSURE_TIME_LEFT		= 10,		// sizeof(int)
			EXPOSURE_TIME_RIGHT		= 11,		// sizeof(int)
			ANALOG_GAIN_LEFT		= 12,		// sizeof(int)
			ANALOG_GAIN_RIGHT		= 13,		// sizeof(int)
			DIGITAL_GAIN_LEFT		= 14,		// sizeof(int)
			DIGITAL_GAIN_RIGHT		= 15,		// sizeof(int)
			DM_MAX,
		};
		enum Callback {
			BASIC = 1001,
		};
	}

	namespace Depth
	{
		enum Param {
			OUTPUT_WIDTH,
			OUTPUT_HEIGHT,
			OUTPUT_CHAANEL_0,
			OUTPUT_CHAANEL_1,
			TYPE,
			FOCULENS,
			BASELINE,
			COLLIDER_QUALITY,
			MESH_NEAR_DISTANCE,
			MESH_FAR_DISTANCE,
			DENOISE_MEDIAN_FILTER,	// range : 1, 3, 5; (default: 3)
			CONFIDENCE_THRESHOLD,	// range : 0 ~ 5; (default: 0.05)
		    DENOISE_GUIDED_FILTER,  // range : 1 ~ 7; (default: 3)
			DEPTH_USING_CASE,
		};
		enum DepthCase
		{
			DEFAULT,
			CLOSE_RANGE,
		};
		enum DataMask
		{
			LEFT_FRAME				= 0,		// sizeof(char) * 640 * 480 * 4
			DEPTH_MAP				= 1,		// sizeof(float) * 640 * 480 * 1
			FRAME_SEQ				= 2,		// sizeof(unsigned int)
			TIME_STP				= 3,		// sizeof(unsigned int)
			POSE					= 4,		// sizeof(float) * 16
			NUM_VERTICES			= 5,		// sizeof(unsigned int)
			BYTEPERVERT				= 6,		// sizeof(unsigned int)
			VERTICES				= 7,		// sizeof(float) * 640 * 480 * 3
			NUM_INDICES				= 8,		// sizeof(unsigned int)
			INDICES					= 9,		// sizeof(int) * 640 * 480 * 6
			LUX_LEFT				= 10,		// sizeof(int)
			COLOR_TEMPERATURE_LEFT	= 11,		// sizeof(int)
			EXPOSURE_TIME_LEFT		= 12,		// sizeof(int)
			ANALOG_GAIN_LEFT		= 13,		// sizeof(int)
			DIGITAL_GAIN_LEFT		= 14,		// sizeof(int)
			DM_MAX,
		};
		enum Cmd {
			EXTRACT_DEPTH_MESH						 = 0,
			ENABLE_SELECT_MESH_DISTANCE_RANGE,
			ENABLE_DEPTH_MESH_HOLE_FILLING,
			ENABLE_REFINEMENT,
			ENABLE_EDGE_ENHANCE,
			CHANGE_DEPTH_CASE,
		};
		enum Callback {
			BASIC = 1001,
		};
	}

	namespace RigidReconstruction
	{
		enum Param {
			VOXEL_SIZE							 = 0,
			COLOR_SIZE							 = 1,
			DATA_SOURCE							 = 2,
			DATASET_PATH						 = 3,
			RGB_IMAGE_EXT						 = 4,
			DATASET_FRAME						 = 5,
			MAX_DEPTH							 = 6,
			MIN_DEPTH							 = 7,
			FOCAL_LENGTH						 = 8,
			POINTCLOUD_POINTSIZE				 = 9,
			EXPORT_ADAPTIVE_MODEL				 = 10,
			ADAPTIVE_MAX_GRID					 = 11,
			ADAPTIVE_MIN_GRID					 = 12,
			ADAPTIVE_ERROR_THRES				 = 13,

			SECTOR_SIZE							 = 15,
			SECTOR_NUM_PER_SIDE					 = 16,

			ENABLE_FRUSTUM_CULLING				 = 20,

			CONFIG_FILEPATH						 = 21,
			CONFIG_QUALITY,
			CONFIG_EXPORT_COLLIDER,
			CONFIG_EXPORT_TEXTURE,

			DATA_CURRENT_POS					 = 31,
			LITE_POINT_CLOUD_MODE,
			FULL_POINT_CLOUD_MODE,
			LIVE_ADAPTIVE_MODE,
			ENABLE_COLLIDERINFO,
			FILE_EXPORT_ROOT_PATH,
			MESH_REFRESH_INTERVAL				 = 37,
			ENABLE_SECTOR_GROUPING				 = 38,

            SCENE_UNDERSTANDING_ENABLE           = 40,
            SCENE_UNDERSTANDING_MACHINE_VISION   = 41,
            SCENE_UNDERSTANDING_CONFIG           = 42,
            SCENE_UNDERSTANDING_REFINEMENT       = 43,

			VERTEX_BUFFER_NATIVE_PTR			 = 99,
			INDEX_BUFFER_NATIVE_PTR				 = 100,
		};

        enum SceneUnderstandingObjectType
        {
            FLOOR = 0,
            WALL = 1,
            CEILING = 2,
            CHAIR = 3,
            TABLE = 4,
            BED = 5,
        };

		enum DataMask
		{
			FRAME_SEQ		= 0,	// sizeof(unsigned int)
			POSEMTX44		= 1,	// sizeof(float) * 16
			NUM_VERTICES	= 2,	// sizeof(int)
			BYTEPERVERT		= 3,	// sizeof(int)
			VERTICES		= 4,	// sizeof(float) * 8 * 2500000
			NUM_INDICES		= 5,	// sizeof(int)
			INDICES			= 6,	// sizeof(int) * 2500000
			CLDTYPE			= 7,	// sizeof(int)
			COLLIDERNUM		= 8,	// sizeof(unsigned int)
			CLD_NUM_VERTS	= 9,	// sizeof(unsigned int) * 200
			CLD_NUMIDX		= 10,	// sizeof(unsigned int) * 200
			CLD_VERTICES	= 11,	// sizeof(float) * 3 * 50000
			CLD_INDICES		= 12,	// sizeof(int) * 100000
			SECTOR_NUM		= 13,   // sizeof(int)
			SECTOR_ID_LIST	= 14,   // sizeof(int) * 1000000
			SECTOR_VERT_NUM	= 15,   // sizeof(int) * 1000000
			SECTOR_IDX_NUM  = 16,   // sizeof(int) * 1000000
			DM_MAX,
		};

		enum Cmd {
			START							 = 0,
			STOP							 = 1,
			SHOW_INFO						 = 2,
			EXTRACT_POINT_CLOUD				 = 3,
			EXTRACT_VERTEX_NORMAL			 = 4,
			EXPORT_MODEL_RIGHT_HAND			 = 5,
			EXPORT_MODEL_FOR_UNITY			 = 6,
            EXPORT_SCENE_UNDERSTANDING_RIGHT_HAND  = 7,
            EXPORT_SCENE_UNDERSTANDING_FOR_UNITY   = 8,
		};

		enum Callback {
			EXPORT_PROGRESS					 = 1,
            SCENE_UNDERSTANDING_PROGRESS     = 2,
			BASIC							 = 1001,
		};

		enum ExportStage {
			EXTRACT_MODEL					 = 0x17,
			COMPACT_TEXTURE					 = 0x18,
			SAVE_MODEL						 = 0x19,
			EXTRACT_COLLIDER				 = 0x1A,
            SCENE_UNDERSTANDING_PASS_1       = 0x30, 
            SCENE_UNDERSTANDING_PASS_2       = 0x31,
		};
	}

	namespace HumanDetection
	{
		enum Param {
			OUTPUT_WIDTH,
			OUTPUT_HEIGHT,
			OUTPUT_CHANNEL_0,
			OUTPUT_CHANNEL_1,
			FOCULENS,
			BASELINE,
			MODULEINFO,
			PLAY_AREA_RECT,
			ENABLE_DEPTH_MASK,
			ENABLE_PLAY_AREA_MASK,
		};
		enum DataMask
		{
			LEFT_FRAME				= 0,		// sizeof(char) * 640 * 480 * 4
			MASK					= 1,		// sizeof(char) * 640 * 480 * 1
			DEPTH					= 2,		// sizeof(float) * 640 * 480 * 1
			FRAME_SEQ				= 3,		// sizeof(unsigned int)
			TIME_STP				= 4,		// sizeof(unsigned int)
			POSE					= 5,		// sizeof(float) * 16
			LUX_LEFT				= 6,		// sizeof(int)
			COLOR_TEMPERATURE_LEFT	= 7,		// sizeof(int)
			EXPOSURE_TIME_LEFT		= 8,		// sizeof(int)
			ANALOG_GAIN_LEFT		= 9,		// sizeof(int)
			DIGITAL_GAIN_LEFT		= 10,		// sizeof(int)
			DM_MAX,
		};

		enum Callback {
			BASIC = 1001,
		};
	}
	
	namespace AI_Vision
	{
		enum Param {
			MODULEINFO,		// This version only supports Model_Path being configured
			REFINE_TYPE,
		};
		enum DataMask {
			FRAME_LEFT			= 0,		// sizeof(char)*width*height*channel
			LABEL_LEFT				= 1,		// sizeof(char)*width*height
			FRAME_RIGHT				= 2,		// sizeof(char)*width*height*channel
			LABEL_RIGHT				= 3,		// sizeof(char)*width*height
			FRAME_SEQ					= 4,
			TIME_STP					= 5,
			POSE_LEFT				= 6,		// sizeof(float) * 16
			POSE_RIGHT					= 7,		// sizeof(float) * 16
			LUX_LEFT					= 8,		// sizeof(int)
			LUX_RIGHT					= 9,		// sizeof(int)
			COLOR_TEMPERATURE_LEFT	= 10,		// sizeof(int)
			COLOR_TEMPERATURE_RIGHT	= 11,		// sizeof(int)
			EXPOSURE_TIME_LEFT = 12,		// sizeof(int)
			EXPOSURE_TIME_RIGHT = 13,		// sizeof(int)
			ANALOG_GAIN_LEFT = 14,		// sizeof(int)
			ANALOG_GAIN_RIGHT = 15,		// sizeof(int)
			DIGITAL_GAIN_LEFT = 16,		// sizeof(int)
			DIGITAL_GAIN_RIGHT = 17,		// sizeof(int)
			DM_MAX,
		};
		enum Callback {
			BASIC = 1001,
		};
		enum SceneLabel {
			// label number 6
			NONE = 0,
			BED = 1,		
			CEILING = 2,	
			CHAIR_SOFA = 3,		
			FLOOR = 4,		
			TABLE = 5,		
			WALL = 6,		
		};
		enum SceneLabelRefineType {
			REFINE_NONE,
			REFINE_SMALL_HOMOGENEOUS_REGION,
			REFINE_LARGE_HOMOGENEOUS_REGION,
		};
		enum AI_Model {
			SCENE_SEMANTIC,
			HUMAN,
		};
		enum OutputMode {
			ID			    = 1, // use number to represent scene recognition result.
			MASK_ID		    = 2, // use number to represent scene recognition result, and focus on single item classify.
			VIS_ID		    = 4, // use color map to represent scene recognition result.
			VIS_W_ID		= 5, // alpha blending : VIS_ID + source image
			VIS_MASK_ID	    = 6, // use color map to represent scene recognition result, and focus on single item classify.
			VIS_MASK_W_ID	= 7, // alpha blending : VIS_MASK_ID + source image
		};
	}
}