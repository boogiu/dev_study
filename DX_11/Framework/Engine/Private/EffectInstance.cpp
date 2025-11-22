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
	/*인스턴스 초기화 단계*/
	m_Data = data;
	m_fLifeTime = 0.f;
	m_fDuration = data->Get_Durate();
	m_bAlive = true;

	m_Transform.Apply_Request(req);
	m_Emitters.clear();
	auto& EmitterDatas = data->Get_EmitterData();
	m_Emitters.resize(EmitterDatas.size());

	for (auto& data : EmitterDatas)
	{
		CEmitter* pEmitter = { nullptr };
		switch (data->eType)
		{
		case Engine::EmitterType::Sprite:
			pEmitter =  CSpriteEmitter::Create(data);
			break;
		case Engine::EmitterType::Mesh:
			break;
		case Engine::EmitterType::Particle:
			break;
		default:
			break;
		}

		if (pEmitter)
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

				finalMatrix = LocalMatrix * parentMat * offset;
			}
		}
	}
	else
	{
		finalMatrix = LocalMatrix;
	}
}

CEffectInstance* CEffectInstance::Create()
{
	return new CEffectInstance;
}

void CEffectInstance::Free()
{
	__super::Free();
}
