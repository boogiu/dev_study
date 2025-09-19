#pragma once
#include "Engine_Defines.h"

namespace Engine {

	typedef struct tagRenderKey {
		_uint ShaderID = {};
		_uint MaterialID = {};

		bool operator<(const tagRenderKey& rhs) {
			if (ShaderID != rhs.ShaderID) return ShaderID < rhs.ShaderID;
			if (MaterialID != rhs.MaterialID) return MaterialID < rhs.MaterialID;
			return false;
		}
	}RENDERKEY;

	typedef struct DrawPriorityPacket {
		
	}PRIORITY_PACKET;

	typedef struct DrawShadowPacket {

	}SHADOW_PACKET;

	typedef struct DrawOpaquePacket {
		_bool bSkinning = { false };

		_uint DrawIndex = {};
		_uint MaterialIndex = {};

		_float4x4* pWorldMatrix;
		class CModel* pModel = { nullptr };
		class CMaterial* pMaterial = { nullptr };
		class CAnimator3D* pAnimator = { nullptr };
		RENDERKEY GetKey() const;
	}OPAQUE_PACKET;

	typedef struct DrawUIPacket {
		_uint DrawIndex = {};
		_uint MaterialIndex = {};
		_float4x4* pWorldMatrix;
		_float4x4* pViewMatrix;
		class CModel* pModel = { nullptr };
		class CMaterial* pMaterial = { nullptr };
	}UI_PACKET;


	typedef struct tagAudioPacket {
		_bool isPaused = { false };
		_bool is3DAttribute = { true };
		_uint iLoopCount = { 0};

		_float fVolume = { 1.f };
		SOUND_GROUP m_eGroup = {};

		class CSoundData* pSound = {};
		FMOD::Channel** ppChannelToUpdate = { nullptr };
		_float3 vPosition = {};
	}AUDIO_PACKET;

}
