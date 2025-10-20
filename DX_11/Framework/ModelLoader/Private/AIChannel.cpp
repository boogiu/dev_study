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

	_int Index = pData->Find_BoneIndexByName(m_ChannelName);
	if (Index == -1)
		return E_FAIL;

	m_iBoneIndex = static_cast<_uint> (pData->Find_BoneIndexByName(m_ChannelName));
	m_boneName = pData->Find_BoneNameByIndex(m_iBoneIndex);
	m_isRoot = pData->isRootBone(m_iBoneIndex);

	if (m_boneName.find("Skirt") != string::npos) {
		int i = 0;
	}

	if (m_boneName.find("Armature_Root")!=string::npos) {
		m_isModelRoot = true;
	}

	if (m_boneName.find("nw4f_root") != string::npos) {
		m_isRoot = true;
	}

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
	channelHeader.iBoneIndex = m_iBoneIndex;
	channelHeader.iNumKeyFrames = m_KeyFrames.size();
	channelHeader.isRootBoneChannel = m_isRoot;
	strcpy_s(channelHeader.BoneName, sizeof(channelHeader.BoneName), m_boneName.c_str());

	ofs.write(reinterpret_cast<const char*>(&channelHeader), sizeof(channelHeader));
	
	if (m_isRoot) {
		for (auto& keyFrame : m_KeyFrames) {

			XMMATRIX keyFramMatrix =
				XMMatrixScalingFromVector(XMLoadFloat3(&keyFrame.vScale)) *
				XMMatrixRotationQuaternion(XMLoadFloat4(&keyFrame.vRotation)) *
				XMMatrixTranslationFromVector(XMLoadFloat3(&keyFrame.vTranslation));

			XMMATRIX saveMatrix = XMMatrixMultiply(keyFramMatrix, XMMatrixRotationY(XMConvertToRadians(g_iExportPreRotate)));

			_vector outScale, outRot, outTrans;
			XMMatrixDecompose(&outScale, &outRot, &outTrans, saveMatrix);

			KEYFRAME SaveKeyFrame = keyFrame;

			XMStoreFloat3(&SaveKeyFrame.vScale, outScale);
			XMStoreFloat4(&SaveKeyFrame.vRotation, outRot);
			XMStoreFloat3(&SaveKeyFrame.vTranslation, outTrans);

			if (m_isModelRoot && m_RemoveMdlTrans) {
				keyFrame.vTranslation = { 0,0,0 };
			}
			ofs.write(reinterpret_cast<const char*>(&SaveKeyFrame), sizeof(KEYFRAME));
		}
	}
	else {
		for (auto& keyFrame : m_KeyFrames) {

			if (m_isModelRoot && m_RemoveMdlTrans) {
				keyFrame.vTranslation = { 0,0,0 };
			}
			ofs.write(reinterpret_cast<const char*>(&keyFrame), sizeof(KEYFRAME));
		}
	}
}

void CAIChannel::Render_GUI()
{
	string key ="Channel : "+ m_ChannelName + "/ Bone : " + m_boneName;
	ImGui::Text(key.c_str());

	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();

		ImGui::Text("Position:  %.3f, %.3f, %.3f",
			nowFrame.vTranslation.x,
			nowFrame.vTranslation.y,
			nowFrame.vTranslation.z);

		ImGui::Text("Scale:     %.3f, %.3f, %.3f",
			nowFrame.vScale.x,
			nowFrame.vScale.y,
			nowFrame.vScale.z);

		ImGui::Text("Rotation:  %.3f, %.3f, %.3f, %.3f",
			nowFrame.vRotation.x,
			nowFrame.vRotation.y,
			nowFrame.vRotation.z,
			nowFrame.vRotation.w);

		ImGui::EndTooltip();
	}
}

HRESULT CAIChannel::TranslateAnimateMatrix(vector<_float4x4>& transfomationMatrices, _float CurrentTrackPosition, _bool Loop)
{
	_uint iCurrentKeyIndex = { m_iNumKeyFrames - 1 };

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		if (i > 0 && !m_KeyFrames[i].IsBefore(CurrentTrackPosition)) {
			iCurrentKeyIndex = i - 1;
			break;
		}
	}

	_XMKeyFrame keyFrame = {};

	KEYFRAME LastKeyframe = m_KeyFrames.back();
	KEYFRAME firstKeyFrame = m_KeyFrames.front();

	if (CurrentTrackPosition >= LastKeyframe.fTrackPosition) {
		keyFrame.vScale = XMLoadFloat3(&LastKeyframe.vScale);
		keyFrame.vRotation = XMLoadFloat4(&LastKeyframe.vRotation);
		keyFrame.vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyframe.vTranslation), 1.0f);
	}

	else if (CurrentTrackPosition == 0) {
		keyFrame.vScale = XMLoadFloat3(&firstKeyFrame.vScale);
		keyFrame.vRotation = XMLoadFloat4(&firstKeyFrame.vRotation);
		keyFrame.vTranslation = XMVectorSetW(XMLoadFloat3(&firstKeyFrame.vTranslation), 1.0f);
	}

	else {
		keyFrame = m_KeyFrames[iCurrentKeyIndex].LerpKeyFram(m_KeyFrames[iCurrentKeyIndex + 1], CurrentTrackPosition);
	}

	if (m_isModelRoot && m_RemoveMdlTrans) {
		keyFrame.vTranslation = {0,0,0,0};
	}

	XMStoreFloat3(&nowFrame.vTranslation, keyFrame.vTranslation);
	XMStoreFloat3(&nowFrame.vScale, keyFrame.vScale);
	XMStoreFloat4(&nowFrame.vRotation, keyFrame.vRotation);

	_matrix TransformationMatrix = XMMatrixAffineTransformation(keyFrame.vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), keyFrame.vRotation, keyFrame.vTranslation);
	XMStoreFloat4x4(&transfomationMatrices[m_iBoneIndex], TransformationMatrix);

	/*보간된 것을 여기에 넣는 것임*/

	return S_OK;
}

HRESULT CAIChannel::ConvertAnimateMatrix(vector<_float4x4>& transfomationMatrices, 
	_float ConvertDuration, _float PrevTrackPosition,
	_float ConversionTrackPosition, CChannel* NextChannel)
{
	_uint iCurrentKeyIndex = { m_iNumKeyFrames - 1 };

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		if (i > 0 && !m_KeyFrames[i].IsBefore(PrevTrackPosition)) {
			iCurrentKeyIndex = i - 1;
			break;
		}
	}


	KEYFRAME SrcKeyFrame = m_KeyFrames[iCurrentKeyIndex];
	KEYFRAME DestKeyFrame = {};

	if (nullptr == NextChannel)
	{
		/*이때는 기준이 달라야 해.*/
		DestKeyFrame = m_KeyFrames.front();
	}
	else {
		DestKeyFrame = NextChannel->Get_KeyFrames().front();
	}

	/*만약 같은 본을 쓰게 된다면, 원래 진행하던대로 Lerp줌*/
	_XMKeyFrame keyFrame = {};
	keyFrame = SrcKeyFrame.LerpKeyFram(DestKeyFrame, ConversionTrackPosition, ConvertDuration);
	_matrix TransformationMatrix = XMMatrixAffineTransformation(keyFrame.vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), keyFrame.vRotation, keyFrame.vTranslation);
	XMStoreFloat4x4(&transfomationMatrices[m_iBoneIndex], TransformationMatrix);

	return S_OK;
}

CAIChannel* CAIChannel::Create(const aiNodeAnim* pAIChannel, CModelData* pData)
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
