#include "Client_Defines.h"
#include "Player.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "ITileService.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "Animator3D.h"
#include "ObjectContainer.h"

#include "Target_Camera.h"
#include "ICameraService.h"
#include "Camera.h"

CPlayer::CPlayer()
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	:CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	HRESULT hr = Add_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", "PlayerBody.model");
	hr = Add_Component<CMaterial>()->Link_Material("GamePlay_Level", "PlayerBody.mat");
	Add_Component<CAnimator3D>();

	return hr;
}

HRESULT CPlayer::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	Get_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", "PlayerBody.model");
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Act_WatchCStd.anim", "Player", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Move_Walk_F.anim", "Player", true);
	Get_Component<CAnimator3D>()->Chane_Animation("Act_WatchCStd.anim");

	return S_OK;
}

void CPlayer::Priority_Update(_float dt)
{
	_float4 NowMove = Get_Position();

	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_DOWN)) {
		NowMove.z += 1.f;
	}
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_UP)) {
		NowMove.z -= 1.f;
	}
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_LEFT)) {
		NowMove.x += 1.f;
	}
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_RIGHT)) {
		NowMove.x -= 1.f;
	}
	//m_pTransform->LookAt(XMLoadFloat4(&NowMove));

	Can_Walk = CGameInstance::GetInstance()->Get_TileSystem()->Check_TileFlagByPosition(NowMove, static_cast<_uint>(TILE_FLAG::WALKABLE));

	if (Can_Walk) {
		//m_pTransform->Translate(m_pTransform->Dir(STATE::LOOK));
		m_pTransform->Set_Pos({ NowMove.x, NowMove.y, NowMove.z });
	}

}

void CPlayer::Update(_float dt)
{
	Get_Component<CAnimator3D>()->Update_Animation(dt);
}

void CPlayer::Late_Update(_float dt)
{

}

void CPlayer::Render_GUI()
{
	__super::Render_GUI();
}

CPlayer* CPlayer::Create()
{
	CPlayer* instance = new CPlayer();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CPlayer");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CPlayer::Clone(INIT_DESC* pArg)
{
	CPlayer* instance = new CPlayer(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CPlayer");
		Safe_Release(instance);
	}

	return instance;
}

void CPlayer::Free()
{
	__super::Free();
}