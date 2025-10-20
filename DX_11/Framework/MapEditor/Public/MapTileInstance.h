#pragma once
#include "GameObject.h"
NS_BEGIN(MapEditor)
typedef struct tagInstanceTile
{
	_float4			vRight, vUp, vLook, vTranslation;
    _float4              vMaterialType;     //Diffuse/Normal//gradationx,gradationy
}INSTANCE_TILE;

typedef struct  tagVertexTileInstancing {
	static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[5] = {
		{ "INSTANCE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,		0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,	 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,		32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,		48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 4, DXGI_FORMAT_R32_FLOAT, 1,		                            64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
}VTX_TILEINSTANCE;

class CMapTileInstance :
    public CGameObject
{
private:
    CMapTileInstance();
    CMapTileInstance(const CMapTileInstance& rhs);
    virtual ~CMapTileInstance() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Add_Tile(_float4x4 matrix, _float4 materialType);
    void Add_Tile(_float4 position, _float4 materialType);

private:
    HRESULT ReadyTexture2DArray( );
    HRESULT CreateTexture2DArrayFromFiles(vector<string> TexturePath, vector<string> TextureKey, ID3D11ShaderResourceView** pSrv);

public:
    void Render_GUI() override;

private:
    vector<INSTANCE_TILE> m_Tiles;
    ID3D11ShaderResourceView* m_pTextureArrayDiffuse = { nullptr };
    ID3D11ShaderResourceView* m_pTextureArrayPalette = { nullptr };

public:
    static CMapTileInstance* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END