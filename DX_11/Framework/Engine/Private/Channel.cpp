#include "Channel.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize()
{
    return S_OK;
}
                                                                                                                                                                                                        /*현재 트랙 위치*/
HRESULT CChannel::TranslateAnimateMatrix(vector<_float4x4>& transfomationMatrices, _float CurrentTrackPosition, _bool Loop)
{

    _uint iCurrentKeyIndex = {};
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

    _matrix TransformationMatrix = XMMatrixAffineTransformation(keyFrame.vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), keyFrame.vRotation, keyFrame.vTranslation);
    XMStoreFloat4x4(&transfomationMatrices[m_iBoneIndex], TransformationMatrix);
    return S_OK;
}

void CChannel::Render_GUI()
{
}

CChannel* CChannel::Create()
{
    CChannel* instance = new CChannel;

    if (FAILED(instance->Initialize())) {
        Safe_Release(instance);
    }
    return instance;
}

void CChannel::Free()
{
    __super::Free();
}
