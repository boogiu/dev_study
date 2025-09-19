#include "Loader_Defines.h"
#include "AIChannel.h"
#include "ModelData.h"

CAIChannel::CAIChannel()
{
}

HRESULT CAIChannel::Initialize(const aiNodeAnim* pAIChannel, CModelData* pData)
{
	m_ChannelName = pAIChannel->mNodeName.data;
	m_iNumKeyFrames = max(pAIChannel->mNumPositionKeys, pAIChannel->mNumScalingKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumRotationKeys);
    m_iBoneIndex =  pData->Find_BoneIndexByName(m_ChannelName);
    m_boneName = pData->Find_BoneNameByIndex(m_iBoneIndex);

    _float3         vScale = {};
    _float4         vRotation = {};
    _float3         vTranslation = {};

    for (size_t i = 0; i < m_iNumKeyFrames; i++)
    {
        KEYFRAME            KeyFrame{};

        if (i < pAIChannel->mNumScalingKeys)
        {
            memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
            KeyFrame.fTrackPosition = pAIChannel->mScalingKeys[i].mTime;
        }

        if (i < pAIChannel->mNumRotationKeys)
        {
            vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
            vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
            vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
            vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
            KeyFrame.fTrackPosition = pAIChannel->mRotationKeys[i].mTime;
        }

        if (i < pAIChannel->mNumPositionKeys)
        {
            memcpy(&vTranslation, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
            KeyFrame.fTrackPosition = pAIChannel->mPositionKeys[i].mTime;
        }

        KeyFrame.vScale = vScale;
        KeyFrame.vRotation = vRotation;
        KeyFrame.vTranslation = vTranslation;

        m_KeyFrames.push_back(KeyFrame);
    }

	return S_OK;
}

void CAIChannel::Save_Channel(ofstream& ofs)
{
    ANIMATION_CHANNEL_HEADER channelHeader = {};
    strcpy_s(channelHeader.BoneName, m_boneName.data());
    channelHeader.iBoneIndex = m_iBoneIndex;
    channelHeader.iNumKeyFrames = m_iNumKeyFrames;

    ofs.write(reinterpret_cast<const char*>(&channelHeader), sizeof(channelHeader));

    for(auto& keyFrame : m_KeyFrames)
        ofs.write(reinterpret_cast<const char*>(&keyFrame), sizeof(KEYFRAME));
}

void CAIChannel::Render_GUI()
{
    string key = m_ChannelName + "&" + m_boneName;
    ImGui::Text(key.c_str());
}

CAIChannel* CAIChannel::Create(const aiNodeAnim* pAIChannel,  CModelData* pData)
{
	CAIChannel* instance = new CAIChannel();
	if (FAILED(instance->Initialize(pAIChannel, pData))) {
		Safe_Release(instance);
	}
	return instance;
}

void CAIChannel::Free()
{
    __super::Free();
}
