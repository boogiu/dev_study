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

	typedef struct DrawBasePacket {
		_float4x4* pWorldMatrix;
	}BASE_PACKET;

	typedef struct DrawPriorityPacket {
		
	}PRIORITY_PACKET;

	typedef struct DrawShadowPacket {

	}SHADOW_PACKET;

	/*불투명 패킷*/
	typedef struct DrawOpaquePacket : BASE_PACKET {
		_bool bSkinning = { false }; /*그래서 본이 있니?*/

		_uint DrawIndex = {}; /*몇번째 메시 그리는데?*/
		_uint MaterialIndex = {};/*그 메시는 뭐쓰는데*/

		class CModel* pModel = { nullptr };
		class CMaterial* pMaterial = { nullptr };

		variant<monostate, class CAnimator3D*, class CSkeletonFollower*> pPayLoad; /*추가적으로 넣고 싶은 것 있어?*/

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

	typedef struct DrawDebugPacket {
		_float4x4* pWorldMatrix;
		class CModel* pModel = { nullptr };
		class CDebugRender* pDebug = { nullptr };
	}DEBUG_PACKET;

	/*Audio*/
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

	/*Shader Param*/
	typedef struct tagShaderParameter {
		void* pData = { nullptr };
		string typeName = {};
		_uint iSize = {};
	}SHADER_PARAM;

}
