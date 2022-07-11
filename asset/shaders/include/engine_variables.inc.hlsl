#ifndef ENGINE_VARIABLES_H
#define ENGINE_VARIABLES_H

#include "engine_constant.inc.hlsl"

cbuffer engine_per_frame : register(b0, space1) {
	float3 engine_light_ambient;
	float3 engine_light_pos;
	float3 engine_light_dir;
	float3 engine_light_intensity;
}

cbuffer engine_per_camera : register(b1, space1) {
	float4x4 engine_matrix_view;
	float4x4 engine_matrix_proj;
	float3	 engine_camera_pos;
}

cbuffer engine_per_object : register(b2, space1) {
	float4x4 engine_matrix_model;
	float4x4 engine_skeleton_matrix[ENGINE_MAX_BONE_PER_SKELETON];
}

 
#endif

