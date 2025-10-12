#include "TileModel.h"

/*
타일 모델은 타일을 위주로 렌더링하는 모델
이걸 따로 만든 이유는 
1)		타일의 팔레트 시스템을 위한 인덱스
2)	인스턴싱
3)	동일한 UV와 다른 텍스처 매핑을 위함
*/

CTileModel::CTileModel()
{
}

CTileModel::CTileModel(const CTileModel& rhs)
	:CModel(rhs)
{
}

HRESULT CTileModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTileModel::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}


const D3D11_INPUT_ELEMENT_DESC* CTileModel::Get_ElementDesc(_uint DrawIndex)
{
	return VTXNORMTEX::Elements;
}

const _uint CTileModel::Get_ElementCount(_uint DrawIndex)
{
	return VTXNORMTEX::iElementCount;
}

const string_view CTileModel::Get_ElementKey(_uint DrawIndex)
{
	return VTXNORMTEX::Key;
}

HRESULT CTileModel::Draw(ID3D11DeviceContext* pContext, _uint Index)
{
	return S_OK;
}

HRESULT CTileModel::Link_Model(const string& levelKey, const string& modelDataKey)
{
	return S_OK;
}

_uint CTileModel::Get_MeshCount()
{
	return 1;
}

_uint CTileModel::Get_MaterialIndex(_uint Index)
{
	return _uint();
}

_bool CTileModel::isDrawable(_uint Index)
{
	return true;
}

BOUNDING_BOX CTileModel::Get_LocalBoundingBox()
{
	return BOUNDING_BOX();
}

vector<BOUNDING_BOX> CTileModel::Get_MeshBoundingBox()
{
	return vector<BOUNDING_BOX>();
}

BOUNDING_BOX CTileModel::Get_WorldBoundingBox()
{
	return BOUNDING_BOX();
}

_bool CTileModel::isReadyToDraw()
{
	return _bool();
}

void CTileModel::Render_GUI()
{
}

CTileModel* CTileModel::Create()
{
	CTileModel* instance = new CTileModel();

	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("CTileModel Create Failed : CTileModel");
		Safe_Release(instance);
	}

	return instance;
}

CComponent* CTileModel::Clone()
{
	CTileModel* instance = new CTileModel(*this);
	return instance;
}

void CTileModel::Free()
{
	__super::Free();
}