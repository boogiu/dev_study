#include "Engine_Define.h"
#include "CReflectRenderer.h"
#include "CGraphicDev.h"
#include "CResourceMgr.h"
#include "CSceneMgr.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "CStateCache.h"
#include "CGameObject.h"

CReflectRenderer::CReflectRenderer()
{
	m_tPLane = { 0.0f, 0.0f, 1.0f, 0.0f }; // 반사 평면
}

CReflectRenderer::~CReflectRenderer()
{
}

CReflectRenderer* CReflectRenderer::Create()
{
	CReflectRenderer* instance = new CReflectRenderer;

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CReflectRenderer::Ready_Component()
{
	m_pDevice = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (!m_pDevice)
		return E_FAIL;

	m_pDevice->AddRef();

	return S_OK;
}

void CReflectRenderer::Update_Component(float& dt)
{
	Compute_Plane();
	Get_CandidateObject();
	Set_TargetObject();
}

void CReflectRenderer::LateUpdate_Component(float& dt)
{
		CRenderMgr::GetInstance()->Add_Renderer(this);
}


void CReflectRenderer::Render(LPDIRECT3DDEVICE9 pDevice)
{
	
	m_pCache->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 기본 앞면만

	//우선 스텐실 버퍼 설정하기
	m_pCache->SetRenderState(D3DRS_STENCILENABLE, TRUE); //스텐실 버퍼 쓰기 시작
	m_pCache->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); //테스트  -> 항상 통과
	m_pCache->SetRenderState(D3DRS_STENCILREF, 0x1); //참조값
	m_pCache->SetRenderState(D3DRS_STENCILMASK, 0xffffffff); //전체 마스크
	m_pCache->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff); //쓰기 마스크

	//테스트 실패한 것들은 그대로 두고, 성공한 것들은 1로 =>그럼
	m_pCache->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE); //스텐실 성공한 경우 1
	m_pCache->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP); //z테스트 실패시 킵
	m_pCache->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP); //스텐실 테스트 실패시 킵


	if (!m_pMesh) return;
	m_pCache->SetFVF(m_pMesh->GetFVF());

	//거울을 그릴 것임(스텐실에)
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	auto& vectoMat = m_pMaterial->Get_Material();

	//알파 블렌딩 비활성화(이건 픽셀을 그리지 않는 것임. 왜냐면 소스 블렌드가 제로이니까)
	m_pCache->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pCache->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	m_pCache->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// z버퍼 쓰기 막고. 안그럼 거울 뒤로 생김(왜냐면 스텐실 뿐만 아니라 z버퍼 영역에도 그리게 되니까)
	m_pCache->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); //z버퍼 갱신 정지

	//거울을 그리는 작업임.(스텐실에 -> 즉 거울 만큼만 1이됨)
	for (int i = 0; i < m_SubsetIdx.size(); ++i) {
		m_pMesh->GetMesh()->DrawSubset(m_SubsetIdx[i]);
	}

	//비친 것처럼 하기 위해 알파블렌딩을 시작한다.
	m_pCache->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	m_pCache->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	// 복구 작업 -> z버퍼 쓰기 가능
	m_pCache->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// 스텐실 상에서 같은 것들은(1) 그대로 두기
	m_pCache->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	m_pCache->SetRenderState(D3DRS_STENCILREF, 0x1); 
	m_pCache->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL); //참조값과 같은 것만 통과
	m_pCache->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP); //그러고 값변화 없음

	//반대로 나올 것이기에 시계방향 반대로 해주고
	m_pCache->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	
	for (CGameObject*& target : m_Targets) {
		//이제부터 반사될 물체를 그릴 것임.
		D3DXMATRIX W, R; // 월드-> 트랜스*반사행렬
		D3DXMatrixReflect(&R, &m_tPLane); //반사 행렬

		//반사 물체
		_matrix worldMat = target->Get_Component<CTransform>()->Get_WorldMatrix();
		W = worldMat * R;    
		pDevice->SetTransform(D3DTS_WORLD, &W);   

		//z버퍼 싹 비우고(1로) 왜냐면 지금 부터 그릴 반사 물체는 z버퍼가 기존 그린 것들에 밀려버리면 안되니끼
		m_pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.f, 0);
		
		//여기서 반사 물체를 그려준다.
		CMeshRenderer* targetRenderer = target->Get_Component<CMeshRenderer>();
		if (!targetRenderer)continue;

		LPD3DXMESH pMesh = targetRenderer->Get_Mesh()->GetMesh();
		if (!pMesh) continue;

		const auto& materials = m_pMaterial ? targetRenderer->Get_Material()->Get_Material() : vector<MATTEX>();
		DWORD subsetCount = targetRenderer->Get_Mesh()->GetSubsetCount();
	
		m_pCache->SetFVF(pMesh->GetFVF());

		for (DWORD i = 0; i < subsetCount; ++i)
		{
			// 머티리얼 설정
			if (i < materials.size()) {
				pDevice->SetMaterial(&materials[i].material);
				pDevice->SetTexture(0, materials[i].texture);
			}

			pMesh->DrawSubset(i);
		}
	}

	//나머지를 원상복귀 시켜준다.
	m_pCache->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pCache->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pCache->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pCache->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_pCache->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CComponent* CReflectRenderer::Clone() const
{
	return nullptr;
}

void CReflectRenderer::Set_Mesh(const wstring& key, vector<DWORD> subsetIdx)
{
	m_pMesh = dynamic_cast<CMesh*>(CResourceMgr::GetInstance()->Find_Mesh(key));

	if (!m_pMesh) {
		MessageBoxW(nullptr, L"잘못된 렌더러 타입 반환. 키 확인 필요", L"Error", MB_OK);
		return;
	}
	m_pMesh->AddRef();

	DWORD subsetCount = m_pMesh->GetSubsetCount();

	for (DWORD subset : subsetIdx) {
		if (subset > subsetCount - 1) {
			MessageBoxW(nullptr, L"서브셋 사이즈 불일치 ", L"Error", MB_OK);
			return;
		}
	}

	m_SubsetIdx = subsetIdx;

	m_pMaterial = CResourceMgr::GetInstance()->Find_Material(key);
	if (!m_pMaterial) {
		MessageBoxW(nullptr, L"잘못된 메쉬 타입 반환. 키 확인 필요", L"Error", MB_OK);
		return;
	}

}

void CReflectRenderer::Compute_Plane()
{
	// 거울(=자기 자신) Transform
	const _matrix& W = m_pTransform->Get_WorldMatrix();

	// 1) 로컬 +Z 축  월드 법선
	_vec3 nLocal = { 0,0,1.f };
	_vec3 nWorld;
	D3DXVec3TransformNormal(&nWorld, &nLocal, &W);
	D3DXVec3Normalize(&nWorld, &nWorld); //이건 방향 벡터니까

	// 2) 평면 위 한 점 (Transform 피벗이 면 위라고 가정)
	_vec3 p0 = { W._41, W._42, W._43 };
	D3DXPlaneFromPointNormal(&m_tPLane, &p0, &nWorld);

	// 3) D 계산, 평면 저장
	float D = -D3DXVec3Dot(&nWorld, &p0);
	m_tPLane = D3DXPLANE(nWorld.x, nWorld.y, nWorld.z, D);
}

void CReflectRenderer::Get_CandidateObject()
{
	m_Candidate.clear();
	CSceneMgr::GetInstance()->Get_NowScene();
}

void CReflectRenderer::Set_TargetObject()
{
	m_Targets.clear();
	if (m_Candidate.empty()) return;

	for (CGameObject* obj : m_Candidate) {

		_matrix objMat = obj->Get_Component<CTransform>()->Get_WorldMatrix();
		_vec3 objPos = obj->Get_Component<CTransform>()->Get_Pos();
		_vec3 worldPos;

		D3DXVec3TransformCoord(&worldPos, &objPos, &objMat);
		float dist = D3DXPlaneDotCoord(&m_tPLane, &worldPos);

		if (dist >= 1.f) {
			m_Targets.push_back(obj);
		}
	}
}

void CReflectRenderer::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pMesh);
	Safe_Release(m_pMaterial);
}
