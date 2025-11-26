#include "EffectInstance.h"
#include "EffectData.h"

#include "SpriteEmitter.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "IObjectService.h"
#include "Layer.h"
#include "SkeletalModel.h"
#include "Animator3D.h"

CEffectInstance::CEffectInstance()
{
}

void CEffectInstance::Play(CEffectData* data, const EffectRequestPacket& req)
{
	Reset();

	/*인스턴스 초기화 단계*/
	m_Data = data;
	m_fLifeTime = 0.f;
	m_fDuration = data->Get_Durate();
	m_bAlive = true;

	m_Transform.Apply_Request(req);
	m_Emitters.clear();

	auto& SpriteEmitterDatas = m_Data->Get_SpriteEmitterData();

	for (size_t i = 0; i < SpriteEmitterDatas.size(); ++i)
	{
		CEmitter* pEmitter = nullptr;
		pEmitter = CSpriteEmitter::Create(SpriteEmitterDatas[i]);
		m_Emitters.push_back(pEmitter);
	}
}

void CEffectInstance::MakePreset(const EffectDataPreset& preset, const EffectRequestPacket& req)
{
	Reset();

	m_Data = CEffectData::Create(preset);
	m_fLifeTime = 0.f;
	m_fDuration = m_Data->Get_Durate();
	m_bAlive = true;

	m_Transform.Apply_Request(req);
	m_Emitters.clear();

	auto& SpriteEmitterDatas = m_Data->Get_SpriteEmitterData();

	for (size_t i = 0; i < SpriteEmitterDatas.size(); ++i)
	{
		CEmitter* pEmitter = nullptr;
		pEmitter = CSpriteEmitter::Create(SpriteEmitterDatas[i]);
		m_Emitters.push_back(pEmitter);
	}
}

void CEffectInstance::Update(_float dt)
{
	if (!m_bAlive)
		return;

	m_fLifeTime += dt;

	if (m_fLifeTime >= m_fDuration && false == m_Data->isLoop()) {
		m_bAlive = false;
		return;
	}

	Update_Transform();

	for (auto emitter : m_Emitters)
		emitter->Update(dt, m_Transform);
}

void CEffectInstance::Render()
{
	if (!m_bAlive)
		return;

	for (auto emitter : m_Emitters)
		emitter->Render();
}

_bool CEffectInstance::IsAlive()
{
	return m_bAlive && (m_fLifeTime < m_fDuration);
}

void CEffectInstance::Reset()
{
	Safe_Release(m_Data);

	m_Request = {};
	m_fLifeTime = {};
	m_fDuration = {};
	m_bAlive = false;

	for (auto Emitter : m_Emitters)
	{
		Safe_Release(Emitter);
	}

	m_Transform = {};
}

void CEffectInstance::Update_Transform()
{
	_matrix finalMatrix = XMMatrixIdentity();

	_matrix LocalMatrix =
		XMMatrixScaling(m_Transform.vStartScale.x, m_Transform.vStartScale.y, m_Transform.vStartScale.z)
		* XMMatrixRotationQuaternion(XMLoadFloat4(&m_Transform.qStartRot))
		* XMMatrixTranslation(m_Transform.vStartPos.x, m_Transform.vStartPos.y, m_Transform.vStartPos.z);

	if (m_Request.objectID != 0) {
		CLayer* pLayer = CGameInstance::GetInstance()->Get_ObjectMgr()->Get_Layer({ m_Request.levelTag ,m_Request.layerTag });
		if (pLayer)
		{
			CGameObject* pObject = pLayer->Find_ObjectByID(m_Request.objectID);
			if (pObject)
			{
				XMMATRIX parentMat;

				if (holds_alternative<CAnimator3D*>(m_Request.pPayLoad))
				{
					auto pAnim = get<CAnimator3D*>(m_Request.pPayLoad);
					parentMat = XMLoadFloat4x4(pAnim->Get_BoneMatrixPtr(m_Request.boneName));
				}
				else if (holds_alternative<CSkeletalModel*>(m_Request.pPayLoad))
				{
					auto* pModel = get<CSkeletalModel*>(m_Request.pPayLoad);
					parentMat = XMLoadFloat4x4(pModel->Get_BoneMatrixPtr(m_Request.boneName));
				}
				else
				{
					parentMat = XMLoadFloat4x4(pObject->Get_Component<CTransform>()->Get_WorldMatrix_Ptr());
				}

				XMMATRIX offset =
					XMMatrixRotationQuaternion(XMLoadFloat4(&m_Transform.qLocalRot)) *
					XMMatrixTranslation(
						m_Transform.vLocalOffset.x,
						m_Transform.vLocalOffset.y,
						m_Transform.vLocalOffset.z);

				finalMatrix = LocalMatrix* offset * parentMat;
			}
		}
	}
	else
	{
		finalMatrix = LocalMatrix;
	}

	XMStoreFloat4x4(&m_Transform.WorldMatrix, finalMatrix);
}

CEffectInstance* CEffectInstance::Create()
{
	return new CEffectInstance;
}

void CEffectInstance::Free()
{
	__super::Free();

	Safe_Release(m_Data);

	for (auto Emitter : m_Emitters)
	{
		Safe_Release(Emitter);
	}
}
