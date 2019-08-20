///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//                           License Agreement
//                     For Vive Super Reality Library
//
// Copyright (c) 2017,		HTC Corporation
// Copyright (c) 2013,		OpenCV Foundation
// Copyright (c) 2000-2008, Intel Corporation
// Copyright (c) 2009-2010, Willow Garage Inc.
// Copyright (c) 2006,		fixstars
// Copyright (c) 2002-2007, Milan Ikits <milan ikits[]ieee org>
// Copyright (c) 2002-2007, Marcelo E. Magallon <mmagallo[]debian org>
// Copyright (c) 2002,		Lev Povalahev
// All rights reserved. Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
///////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <stdint.h>
#define VIVE_SR_API __declspec(dllexport)
#define SR_PATH_MAX 260	

struct CameraParams
{
	double Cx_L;
	double Cx_R;
	double Cy_L;
	double Cy_R;
	double FocalLength_L;
	double FocalLength_R;
	double Rotation[9];
	double Translation[3];
};

struct DataInfo
{
	void *ptr;
	int mask;
	void *description;
};

struct ParamInfo {
	int32_t nStatus;
	int32_t nDefaultValue;
	int32_t nMin;
	int32_t nMax;
	int32_t nStep;
	int32_t nDefaultMode;
	int32_t nValue;
	int32_t nMode;
};

// TODO: replace with int array in future release
struct SceneUnderstandingConfig {
    union
    {
        struct 
        {
            int32_t nFloorMaxInst;
            int32_t nWallMaxInst;
            int32_t nCeilingMaxInst;
            int32_t nChairMaxInst;
            int32_t nTableMaxInst;
            int32_t nBedMaxInst;
        };
        int32_t nMaxDetectInstance[6];
    };
};

struct AI_VisionModuleInfo
{
    char Model_Path[SR_PATH_MAX];				// AI model folder path ex: "C/Users/Name/Downloads/Vive-SRWorks-x.x.x.x"
	int32_t Model_PathLength;
	int32_t IsDualCam;
	int32_t Img_Type;					// ViveSR::SR_ImageType  : DISTORTED, UNDISTORTED
	int32_t	Img_W;
	int32_t	Img_H;
	int32_t	Img_CH;						// SRWorks using RGBA colorspace
	int32_t	Img_Crop_W;
	int32_t	Img_Crop_H;
	int32_t	Img_Crop_Center_X;
	int32_t	Img_Crop_Center_Y;
	int32_t	Label_W;
	int32_t	Label_H;
	int32_t	Label_CH;
	int32_t	Label_Size;
	int32_t	Label_Number;				// total label type(ViveSR::AI_Vision::SceneLabel) number
	int32_t Prob_Size;					// label probability table size, float array [Label_number][Label_W][Label_H]
	int32_t	Label_OutputMode;			// ViveSR::AI_Vision::OutputMode
	int32_t Target_Mask;				// Using MASK_ID, VIS_MASK_ID, VIS_MASK_W_ID as Label_OutputMode, can choese one label in SceneLabel as target
	int32_t AI_Model;					// ViveSR::AI_Vision::AI_Mode
	int32_t ProcessUnit;				// 1: CPU, 2: GPU
	int32_t Threshold;					// threshold of labeling probability
	// AI model information
	float Model_MeanR;
	float Model_MeanG;
	float Model_MeanB;
	float Model_VarR;
	float Model_VarG;
	float Model_VarB;
};

struct HumanDetectionModuleInfo
{
	char Model_Path[SR_PATH_MAX];		// AI model folder path ex: "C/Users/Name/Downloads/Vive-SRWorks-x.x.x.x"
	int32_t Model_PathLength;
	int32_t ProcessUnit;				// 1: CPU, 2: GPU
};

// Event types for UnitySetGraphicsDevice
enum GfxDeviceEventType {
	kGfxDeviceEventInitialize = 0,
	kGfxDeviceEventShutdown,
	kGfxDeviceEventBeforeReset,
	kGfxDeviceEventAfterReset,
};

// You can find out all function return code definition at SRWorks SDK package path : 
// $(SRWorks_SDK_Package_Path)\Vive-SRWorks-VersionNumber)\C\headers\ViveSR_Enums.h(enum SR_Error)
extern "C" {
	/* Get the version of this library */
	VIVE_SR_API char* ViveSR_GetVersion();

	VIVE_SR_API int ViveSR_CreateModule(int ModuleType, int *ModuleID);

	VIVE_SR_API int ViveSR_ModuleLink(int from, int to, int mode);

	VIVE_SR_API int ViveSR_GetMultiData(int moduleID, DataInfo *data, unsigned int size);

	VIVE_SR_API int ViveSR_GetMultiDataSize(int moduleID, DataInfo *data, unsigned int size);

	VIVE_SR_API int ViveSR_Initial();

	VIVE_SR_API int ViveSR_EnableLog(bool flag);

	VIVE_SR_API int ViveSR_SetLogLevel(int level);

	/* Initialize all settings and then start to capture and process images. */
	VIVE_SR_API int ViveSR_StartModule(int ModuleID);

	/* Stop all process and release all settings. */
	VIVE_SR_API int ViveSR_Stop();

	VIVE_SR_API int ViveSR_RegisterCallback(int moduleID, int type, void *callback);
	VIVE_SR_API int ViveSR_UnregisterCallback(int moduleID, int type, void *callback);

	VIVE_SR_API int ViveSR_GetCameraParams(CameraParams *parameter);

	VIVE_SR_API int ViveSR_GetParameterBool(int moduleID, int type, bool *parameter);
	VIVE_SR_API int ViveSR_SetParameterBool(int moduleID, int type, bool parameter);

	VIVE_SR_API int ViveSR_GetParameterInt(int moduleID, int type, int *parameter);
	VIVE_SR_API int ViveSR_SetParameterInt(int moduleID, int type, int parameter);

	VIVE_SR_API int ViveSR_GetParameterFloat(int moduleID, int type, float *parameter);
	VIVE_SR_API int ViveSR_SetParameterFloat(int moduleID, int type, float parameter);

	VIVE_SR_API int ViveSR_GetParameterDouble(int moduleID, int type, double *parameter);
	VIVE_SR_API int ViveSR_SetParameterDouble(int moduleID, int type, double parameter);

	VIVE_SR_API int ViveSR_GetParameterString(int moduleID, int type, char **parameter);
	VIVE_SR_API int ViveSR_SetParameterString(int moduleID, int type, char *parameter);

	VIVE_SR_API int ViveSR_GetParameterNativePtr(int moduleID, int type, void **parameter);
	VIVE_SR_API int ViveSR_SetParameterNativePtr(int moduleID, int type, void *parameter);

	VIVE_SR_API int ViveSR_GetParameterStruct(int moduleID, int type, void **parameter);
	VIVE_SR_API int ViveSR_SetParameterStruct(int moduleID, int type, void *parameter);

	VIVE_SR_API int ViveSR_GetParameterFloatArray(int moduleID, int type, float **parameter);
	VIVE_SR_API int ViveSR_SetParameterFloatArray(int moduleID, int type, float *parameter);

	VIVE_SR_API int ViveSR_SetCommandBool(int moduleID, int type, bool content);
	VIVE_SR_API int ViveSR_SetCommandInt(int moduleID, int type, int content);
	VIVE_SR_API int ViveSR_SetCommandFloat(int moduleID, int type, float content);
	VIVE_SR_API int ViveSR_SetCommandString(int moduleID, int type, char* content /*, int length*/);
	VIVE_SR_API int ViveSR_SetCommandFloat3(int moduleID, int type, float content0, float content1, float content2);
	VIVE_SR_API int ViveSR_ChangeModuleLinkStatus(int from, int to, int mode);

	VIVE_SR_API int ViveSR_GetPointer(int key, int type, void **ptr);

	VIVE_SR_API void UnitySetGraphicsDevice(void* device, int deviceType, int eventType);
	VIVE_SR_API void UnityRenderEvent(int eventID);
}
