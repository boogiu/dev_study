#include "BaseField.h"
#include "Client_Defines.h"

#include "StaticModel.h"
#include "InstanceModel.h"

#include "Material.h"
#include "MaterialInstance.h"

#include "GameInstance.h"
#include "ITileService.h"
#include "IResourceService.h"
#include "Texture.h"

CBaseField::CBaseField()
{
}

CBaseField::CBaseField(const CBaseField& rhs)
	:CGameObject(rhs)
{
}

HRESULT CBaseField::Initialize_Prototype()
{
	__super::Initialize();
	//Add_Component<CStaticModel>();
	Add_Component<CInstanceModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CBaseField::Initialize(INIT_DESC * pArg)
{
	__super::Initialize(pArg);
	__super::Initialize(pArg);
	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();

	CInstanceModel::INSTANCE_INIT_DESC instanceDesc = {};
	instanceDesc.ElementCount = 5;
	instanceDesc.ElementKey = "GrassInstance";
	instanceDesc.instanceCount = 20000;
	instanceDesc.instanceStride = sizeof(INSTANCE_TILE);
	instanceDesc.pElementDesc = VTX_TILEINSTANCE::Elements;

	vector<CInstanceModel::INSTANCE_INIT_DESC> pVector;
	pVector.push_back(instanceDesc);


	ReadyTexture2DArray();

	CMaterial* pMaterial = Get_Component<CMaterial>();
	pMaterial->Link_Material(G_GlobalLevelKey, "Base_0.mat");

	Get_Component<CInstanceModel>()->Link_InstanceData(pDevice, pVector, G_GlobalLevelKey, "Base_0.model");
	Get_Component<CInstanceModel>()->Link_InstanceWithMesh(0, 0);
	Get_Component<CInstanceModel>()->Link_InstanceWithMesh(1, 0);

	m_Tiles.reserve(20000);
	Override_Pass();
	return S_OK;
}

void CBaseField::Priority_Update(_float dt)
{
}

void CBaseField::Update(_float dt)
{
	ID3D11DeviceContext* pContext = CGameInstance::GetInstance()->Get_Context();

	// SRV 클리어 (OpacityTexture, MaskTexture, 기타 이펙트 잔류 제거)
	ID3D11ShaderResourceView* nullSRV[128] = { nullptr };
	pContext->PSSetShaderResources(0, 128, nullSRV);

	Get_Component<CInstanceModel>()->Update_Instance(pContext, m_Tiles.data(), 0, static_cast<_uint>(m_Tiles.size()));
}

void CBaseField::Late_Update(_float dt)
{
}

void CBaseField::Render_GUI()
{
	__super::Render_GUI();
}

HRESULT CBaseField::ReadyTexture2DArray()
{
	vector<string> TextureAlbPath = {
	"../../Resources/Textures/Alb/mRoadBrick_Alb.dds",
	"../../Resources/Textures/Alb/mRoadFanPattern_Alb.dds",
	"../../Resources/Textures/Alb/mRoadStone_Alb.dds",
	"../../Resources/Textures/Alb/mRoadTile_Alb.dds",
	"../../Resources/Textures/Alb/mRoadWood_Alb.dds"
	};
	vector<string> TextureAlbKey = {
	"mRoadBrick_Alb.dds",
	"mRoadFanPattern_Alb.dds",
	"mRoadStone_Alb.dds",
	"mRoadTile_Alb.dds",
	"mRoadWood_Alb.dds"
	};

	CreateTexture2DArrayFromFiles(TextureAlbPath, TextureAlbKey, &m_pTextureArrayDiffuse);

	vector<string> TexturePalettePath = {
		"../../Resources/Textures/Grd/mGrass_Grd.dds",
		"../../Resources/Textures/Grd/mGrass_GrdEdge.dds",
		"../../Resources/Textures/Grd/mRoadBrick_Grd.dds",
		"../../Resources/Textures/Grd/mRoadDarkSoil_Grd.dds",
		"../../Resources/Textures/Grd/mRoadFanPattern_Grd.dds",
		"../../Resources/Textures/Grd/mRoadSand_Grd.dds",
		"../../Resources/Textures/Grd/mRoadSoil_Grd.dds",
		"../../Resources/Textures/Grd/mRoadStone_Grd.dds",
		"../../Resources/Textures/Grd/mRoadTile_Grd.dds",
		"../../Resources/Textures/Grd/mRoadWood_Grd.dds",
	};
	vector<string> TexturePaletteKey = {
		"mGrass_Grd.dds",
		"mGrass_GrdEdge.dds",
		"mRoadBrick_Grd.dds",
		"mRoadDarkSoil_Grd.dds",
		"mRoadFanPattern_Grd.dds",
		"mRoadSand_Grd.dds",
		"mRoadSoil_Grd.dds",
		"mRoadStone_Grd.dds",
		"mRoadTile_Grd.dds",
		"mRoadWood_Grd.dds",
	};

	CreateTexture2DArrayFromFiles(TexturePalettePath, TexturePaletteKey, &m_pTextureArrayPalette);

	return S_OK;
}

HRESULT CBaseField::CreateTexture2DArrayFromFiles(vector<string> TexturePath, vector<string> TextureKey, ID3D11ShaderResourceView** pSrv)
{
	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();
	ID3D11DeviceContext* pContext = CGameInstance::GetInstance()->Get_Context();

	for (size_t i = 0; i < TextureKey.size(); i++)
	{
		CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(TextureKey[i], TexturePath[i]);
	}

	vector<CTexture*> Textures;
	vector<ID3D11Texture2D*> Textures2D;

	for (size_t i = 0; i < TextureKey.size(); i++)
	{
		CTexture* pTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture(G_GlobalLevelKey, TextureKey[i]);
		ID3D11Resource* pResource = { nullptr };
		pTexture->Get_SRV()->GetResource(&pResource);

		ID3D11Texture2D* pTexture2D;
		pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTexture2D);
		Textures2D.push_back(pTexture2D);
		Safe_Release(pResource);
	}

	D3D11_TEXTURE2D_DESC baseDesc;
	Textures2D[0]->GetDesc(&baseDesc);

	D3D11_TEXTURE2D_DESC arrayDesc = baseDesc;
	arrayDesc.ArraySize = static_cast<UINT>(Textures2D.size());
	arrayDesc.Usage = D3D11_USAGE_DEFAULT;
	arrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	arrayDesc.CPUAccessFlags = 0;
	arrayDesc.MiscFlags = 0;

	ID3D11Texture2D* texArray;
	HRESULT hr = pDevice->CreateTexture2D(&arrayDesc, nullptr, &texArray);

	if (FAILED(hr)) return hr;

	for (UINT i = 0; i < Textures2D.size(); ++i)
	{

		for (UINT mip = 0; mip < baseDesc.MipLevels; ++mip)
		{
			UINT destSubresource = D3D11CalcSubresource(mip, i, baseDesc.MipLevels);
			pContext->CopySubresourceRegion(texArray, destSubresource, 0, 0, 0, Textures2D[i], mip, nullptr);
		}
	}

	for (auto tex2D : Textures2D)
		if (tex2D) Safe_Release(tex2D);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = arrayDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = arrayDesc.MipLevels;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = arrayDesc.ArraySize;

	hr = pDevice->CreateShaderResourceView(texArray, &srvDesc, pSrv);
	Safe_Release(texArray);

	if (FAILED(hr)) return hr;

	return S_OK;
}

void CBaseField::Load_Tile(INSTANCE_TILE tile)
{
	m_Tiles.push_back(tile);
}
void CBaseField::Override_Pass()
{
	CMaterial* pMaterial = Get_Component<CMaterial>();

	SHADER_PARAM TileAlbedo = {};
	TileAlbedo.iSize = 0;
	TileAlbedo.typeName = "Texture2DArray";
	TileAlbedo.pData = m_pTextureArrayDiffuse;

	SHADER_PARAM TilePalette = {};
	TilePalette.iSize = 0;
	TilePalette.typeName = "Texture2DArray";
	TilePalette.pData = m_pTextureArrayPalette;

	for (auto& instance : pMaterial->Get_Material_Instance()) {
		instance->Override_Pass("Instancing");
		instance->Set_Param("g_TileAlbedo", TileAlbedo);
	}

	for (auto& instance : pMaterial->Get_Material_Instance()) {
		instance->Override_Pass("Instancing");
	}
	if (pMaterial->Get_MaterialInstanceByName("mGrassXlu")) {
		pMaterial->Get_MaterialInstanceByName("mGrassXlu")->Override_Pass("Edge");
		pMaterial->Get_MaterialInstanceByName("mGrassXlu")->Set_Param("g_TilePalette", TilePalette);
	}
}

CBaseField* CBaseField::Create()
{
	CBaseField* instance = new CBaseField();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CBaseField");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CBaseField::Clone(INIT_DESC* pArg)
{
	CBaseField* instance = new CBaseField(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CBaseField");
		Safe_Release(instance);
	}

	return instance;
}

void CBaseField::Free()
{
	__super::Free();
	Safe_Release(m_pTextureArrayDiffuse);
	Safe_Release(m_pTextureArrayPalette);
}
