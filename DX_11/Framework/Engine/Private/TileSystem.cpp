#include "TileSystem.h"
#include "TileBlock.h"
#include "GameInstance.h"
#include "IRenderService.h"

#include "InstanceModel.h"
#include "Material.h"
#include "MaterialInstance.h"

CTileSystem::CTileSystem()
{
}


HRESULT CTileSystem::Initialize(const TILESYSTEM_INFO& tileInfo)
{
	m_tTileInfo = tileInfo;
	m_TileInfos.resize(m_tTileInfo.iTileCountX * m_tTileInfo.iTileCountZ, { 0,{
		m_tTileInfo.vWorldMin.y,m_tTileInfo.vWorldMin.y,m_tTileInfo.vWorldMin.y,m_tTileInfo.vWorldMin.y
		},nullptr });

	return S_OK;
}

void CTileSystem::Update(_float dt)
{
	if (m_pInstanceModel) {

		for (auto Index : m_DirtyTile) {
			auto info = m_TileInfos[Index.IndexX + Index.IndexZ * m_tTileInfo.iTileCountX];
			m_InstanceTiles[Index.IndexX + Index.IndexZ * m_tTileInfo.iTileCountX].fCornerHeight = {
				info.fCornerHeight[0],
				info.fCornerHeight[1],
				info.fCornerHeight[2],
				info.fCornerHeight[3]
			};
		}
		m_DirtyTile.clear();

		m_pInstanceModel->Update_Instance(m_pContext, m_InstanceTiles.data(), 0, static_cast<_uint>(m_InstanceTiles.size()));
		INSTANCE_PACKET packet;
		packet.pModel = m_pInstanceModel;
		packet.pMaterial = m_pTileMaterial;

		for (size_t i = 0; i < packet.pModel->Get_MeshCount(); i++)
		{
			if (!packet.pModel->isDrawable(i)) continue;
			packet.DrawIndex = i;
			packet.MaterialIndex = packet.pModel->Get_MaterialIndex(i);
			packet.pWorldMatrix = &m_pTileWorldMatrix;
			CGameInstance::GetInstance()->Get_RenderSystem()->Submit_Instance(packet);
		}

	}
}

HRESULT CTileSystem::Execute_InstanceModel(const string& levelKey, const string& modelKey, const string& materialKey)
{
	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();
	m_pContext = CGameInstance::GetInstance()->Get_Context();
	Safe_AddRef(m_pContext);

	m_pInstanceModel = CInstanceModel::Create();
	m_pTileMaterial = CMaterial::Create();

	INSTANCE_INIT_DESC instanceDesc = {};
	instanceDesc.ElementKey = "TileSystem";
	instanceDesc.ElementCount = VTX_TILEINSTANCE::iElementCount;
	instanceDesc.instanceStride = sizeof(INSTANCE_TILE);
	instanceDesc.pElementDesc = VTX_TILEINSTANCE::Elements;
	instanceDesc.instanceCount = m_TileInfos.size();

	vector<INSTANCE_INIT_DESC> pInstanceVector;
	m_instanceDesc.push_back(instanceDesc);
	XMStoreFloat4x4(&m_pTileWorldMatrix, XMMatrixIdentity());

	m_InstanceTiles.resize(m_TileInfos.size());
	m_pTileMaterial->Link_Material(levelKey, materialKey);
	for (auto& instance : m_pTileMaterial->Get_Material_Instance()) {
		instance->Override_Pass("Instancing");
	}

	m_pInstanceModel->Link_InstanceData(pDevice, m_instanceDesc, levelKey, modelKey);
	m_pInstanceModel->Link_InstanceMeshAll(0);
	_float sizex = m_tTileInfo.SizePerTile().x;
	_float sizez = m_tTileInfo.SizePerTile().z;
	for (size_t i = 0; i < m_tTileInfo.iTileCountZ; i++)
	{
		for (size_t j = 0; j < m_tTileInfo.iTileCountX; j++)
		{
			const _uint Index = i * m_tTileInfo.iTileCountX + j;

			m_InstanceTiles[Index].vLook = { 0,0,1,0 };
			m_InstanceTiles[Index].vUp = { 0,1,0,0 };
			m_InstanceTiles[Index].vRight = { 1,0,0,0 };
			m_InstanceTiles[Index].vTranslation = {
				m_tTileInfo.vWorldMin.x + j * sizex + sizex * 0.5f,
				m_tTileInfo.vWorldMin.y,
				m_tTileInfo.vWorldMin.z + i * sizez + sizez * 0.5f,
				1.f
			};
			m_InstanceTiles[Index].fCornerHeight = {
			m_TileInfos[Index].fCornerHeight[0],
			m_TileInfos[Index].fCornerHeight[1],
			m_TileInfos[Index].fCornerHeight[2],
			m_TileInfos[Index].fCornerHeight[3]
			};
		}
	}
	return S_OK;
}

HRESULT CTileSystem::Set_Material_ID(TILE_INDEX tileIndex, _float4 materialID)
{
	if (!Check_ValidIndex(tileIndex))
		return E_FAIL;

	m_InstanceTiles[tileIndex.IndexX + tileIndex.IndexZ * m_tTileInfo.iTileCountX].vMaterialType = materialID;
	return S_OK;
}

_float CTileSystem::Get_TileHeightByPosition(_float4 WorldPos)
{
	TILE_INDEX dstIndex = Get_IndexByPosition(WorldPos);
	if (!Check_ValidIndex(dstIndex))
		return 0.f;

	_float4 dstPos = Get_PositionByIndex(dstIndex, ANCHOR::Center);
	TILE_INFO info = m_TileInfos[dstIndex.IndexX + dstIndex.IndexZ * m_tTileInfo.iTileCountX];
	_float hSizeX = m_tTileInfo.SizePerTile().x * 0.5f;
	_float hSizeZ = m_tTileInfo.SizePerTile().z * 0.5f;

	_float3 CornerA = { dstPos.x - hSizeX, info.fCornerHeight[0], dstPos.z + hSizeZ }; // LT
	_float3 CornerB = { dstPos.x + hSizeX, info.fCornerHeight[1], dstPos.z + hSizeZ }; // RT
	_float3 CornerC = { dstPos.x + hSizeX, info.fCornerHeight[2], dstPos.z - hSizeZ }; // RB
	_float3 CornerD = { dstPos.x - hSizeX, info.fCornerHeight[3], dstPos.z - hSizeZ }; // LB

	// 타일 내부에서의 정규화 좌표 (0~1)
	_float localX = WorldPos.x - (dstPos.x - hSizeX);
	_float localZ = WorldPos.z - (dstPos.z - hSizeZ);
	_float normX = localX / (hSizeX * 2.f);
	_float normZ = localZ / (hSizeZ * 2.f);

	_float4 vPlane = {};

	// LT ↔ RB 기준: 직선 방정식은 v = u
	if (normZ > normX) {
		// 윗삼각형 (LT, RT, RB)
		XMStoreFloat4(&vPlane, XMPlaneFromPoints(
			XMLoadFloat3(&CornerA), // LT
			XMLoadFloat3(&CornerB), // RT
			XMLoadFloat3(&CornerC)  // RB
		));
	}
	else {
		// 아랫삼각형 (LT, RB, LB)
		XMStoreFloat4(&vPlane, XMPlaneFromPoints(
			XMLoadFloat3(&CornerA), // LT
			XMLoadFloat3(&CornerC), // RB
			XMLoadFloat3(&CornerD)  // LB
		));
	}

	// 평면식 ax + by + cz + d = 0  → y = (-a*x - c*z - d)/b
	_float fy = (-vPlane.x * WorldPos.x - vPlane.z * WorldPos.z - vPlane.w) / vPlane.y;

	return fy;
}


TILE_INDEX CTileSystem::Get_IndexByPosition(_float4 WorldPos)
{
	_float DistanceX = WorldPos.x - m_tTileInfo.vWorldMin.x;
	_float DistanceY = WorldPos.y - m_tTileInfo.vWorldMin.y;
	_float DistanceZ = WorldPos.z - m_tTileInfo.vWorldMin.z;

	_int    TileIndexX = static_cast<_int>(floorf(DistanceX / m_tTileInfo.SizePerTile().x));
	_int    TileIndexZ = static_cast<_int>(floorf(DistanceZ / m_tTileInfo.SizePerTile().z));

	TILE_INDEX index = {};

	if (TileIndexX < 0 || TileIndexX >= static_cast<_int>(m_tTileInfo.iTileCountX))
		index.IndexX = -1;
	else
		index.IndexX = TileIndexX;

	if (TileIndexZ < 0 || TileIndexZ >= static_cast<_int>(m_tTileInfo.iTileCountZ))
		index.IndexZ = -1;
	else
		index.IndexZ = TileIndexZ;

	return index;
}

_float4 CTileSystem::Get_PositionByIndex(TILE_INDEX tileIndex, ANCHOR anchor)
{
	_float3 vOrigin = m_tTileInfo.vWorldMin;
	_float3 result = {};

	_uint anchorFlags = static_cast<_uint>(anchor);

	if (anchorFlags & static_cast<_uint>(ANCHOR::Left))
		result.x = tileIndex.IndexX * m_tTileInfo.SizePerTile().x;
	else if (anchorFlags & static_cast<_uint>(ANCHOR::Right))
		result.x = (tileIndex.IndexX + 1) * m_tTileInfo.SizePerTile().x;
	else
		result.x = (tileIndex.IndexX * m_tTileInfo.SizePerTile().x) + (m_tTileInfo.SizePerTile().x * 0.5f);

	if (anchorFlags & static_cast<_uint>(ANCHOR::Top))
		result.z = (tileIndex.IndexZ + 1) * m_tTileInfo.SizePerTile().z;
	else if (anchorFlags & static_cast<_uint>(ANCHOR::Bottom))
		result.z = tileIndex.IndexZ * m_tTileInfo.SizePerTile().z;
	else
		result.z = (tileIndex.IndexZ * m_tTileInfo.SizePerTile().z) + (m_tTileInfo.SizePerTile().z * 0.5f);


	result.y = m_tTileInfo.vWorldMin.y;

	return _float4{ result.x, result.y, result.z, 1.f };
}
TILE_INDEX CTileSystem::Register_Tile(CTileBlock* block, TILE_INDEX index, _bool CanFail)
{
	if (!Check_ValidIndex(index))
		return TILE_INDEX();

	TILE_INDEX desireIndex = index;
	const _int totalCount = m_tTileInfo.iTileCountX * m_tTileInfo.iTileCountZ;
	_int tries = 0;

	while (tries++ < totalCount) {
		_int XZIndex = desireIndex.IndexX + desireIndex.IndexZ * m_tTileInfo.iTileCountX;

		if (XZIndex < 0 || XZIndex >= static_cast<_int>(m_TileInfos.size()))
			return TILE_INDEX{};

		if (m_TileInfos[XZIndex].pTileBlock == nullptr) {
			m_TileInfos[XZIndex].pTileBlock = block;
			block->Set_Index(desireIndex);
			block->Update_Position(m_tTileInfo);
			return desireIndex;
		}

		if (CanFail)
			return TILE_INDEX{};

		desireIndex.IndexX++;
		if (desireIndex.IndexX >= static_cast<_int>(m_tTileInfo.iTileCountX)) {
			desireIndex.IndexX = 0;
			desireIndex.IndexZ++;
		}

		if (desireIndex.IndexZ >= static_cast<_int>(m_tTileInfo.iTileCountZ))
			return TILE_INDEX{};
	}

	return TILE_INDEX{};
}

_uint CTileSystem::Get_NeighborInfoByIndex(TILE_INDEX index, vector<TILE_INFO>& container)
{
	if (!Check_ValidIndex(index))
		return 0;

	container.resize(9, {});
	_uint NeighborValid = {};

	//UPLEFT, UP, UPRIGHT, LEFT, CENTER, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT,END    
	if (Get_TileInfoByIndex({ index.IndexX - 1,index.IndexZ + 1 }, container[0]))
		NeighborValid |= static_cast<_uint>(NEIGHBOR_INDEX::UPLEFT);
	if (Get_TileInfoByIndex({ index.IndexX,index.IndexZ + 1 }, container[1]))
		NeighborValid |= static_cast<_uint>(NEIGHBOR_INDEX::UP);
	if (Get_TileInfoByIndex({ index.IndexX + 1,index.IndexZ + 1 }, container[2]))
		NeighborValid |= static_cast<_uint>(NEIGHBOR_INDEX::UPRIGHT);
	if (Get_TileInfoByIndex({ index.IndexX - 1,index.IndexZ }, container[3]))
		NeighborValid |= static_cast<_uint>(NEIGHBOR_INDEX::LEFT);
	if (Get_TileInfoByIndex({ index.IndexX,index.IndexZ }, container[4]))
		NeighborValid |= static_cast<_uint>(NEIGHBOR_INDEX::CENTER);
	if (Get_TileInfoByIndex({ index.IndexX + 1,index.IndexZ }, container[5]))
		NeighborValid |= static_cast<_uint>(NEIGHBOR_INDEX::RIGHT);
	if (Get_TileInfoByIndex({ index.IndexX - 1,index.IndexZ - 1 }, container[6]))
		NeighborValid |= static_cast<_uint>(NEIGHBOR_INDEX::DOWNLEFT);
	if (Get_TileInfoByIndex({ index.IndexX ,index.IndexZ - 1 }, container[7]))
		NeighborValid |= static_cast<_uint>(NEIGHBOR_INDEX::DOWN);
	if (Get_TileInfoByIndex({ index.IndexX + 1,index.IndexZ - 1 }, container[8]))
		NeighborValid |= static_cast<_uint>(NEIGHBOR_INDEX::DOWNRIGHT);

	return NeighborValid;
}

vector<class CTileBlock*> CTileSystem::Get_NeighborByIndex(TILE_INDEX index)
{
	vector<class CTileBlock*> neighBorTile;

	if (!Check_ValidIndex(index))
		return neighBorTile;

	neighBorTile.resize(9, nullptr);
	//UPLEFT, UP, UPRIGHT, LEFT, CENTER, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT,END    
	neighBorTile[0] = Get_TileBlockByIndex({ index.IndexX - 1,			index.IndexZ + 1 });//z위로 x왼;
	neighBorTile[1] = Get_TileBlockByIndex({ index.IndexX       ,			index.IndexZ + 1 });
	neighBorTile[2] = Get_TileBlockByIndex({ index.IndexX + 1,			index.IndexZ + 1 });
	neighBorTile[3] = Get_TileBlockByIndex({ index.IndexX - 1,			index.IndexZ });
	neighBorTile[4] = Get_TileBlockByIndex({ index.IndexX,				index.IndexZ });
	neighBorTile[5] = Get_TileBlockByIndex({ index.IndexX + 1,			index.IndexZ });
	neighBorTile[6] = Get_TileBlockByIndex({ index.IndexX - 1,			index.IndexZ - 1 });
	neighBorTile[7] = Get_TileBlockByIndex({ index.IndexX ,				index.IndexZ - 1 });
	neighBorTile[8] = Get_TileBlockByIndex({ index.IndexX + 1,			index.IndexZ - 1 });

	return neighBorTile;
}


vector<TILE_INDEX> CTileSystem::Get_IndeciesByArea(_float4 vMin, _float4 vMax)
{
	if (vMin.x > vMax.x) swap(vMin.x, vMax.x);
	if (vMin.y > vMax.y) swap(vMin.y, vMax.y);
	if (vMin.z > vMax.z) swap(vMin.z, vMax.z);

	TILE_INDEX MinIndex = Get_IndexByPosition(vMin);
	TILE_INDEX MaxIndex = Get_IndexByPosition(vMax);

	vector<TILE_INDEX> indices;

	if (!Check_ValidIndex(MinIndex) || !Check_ValidIndex(MinIndex)) { return indices; }

	for (int z = MinIndex.IndexZ; z <= MaxIndex.IndexZ - 1; ++z)
	{
		for (int x = MinIndex.IndexX; x <= MaxIndex.IndexX - 1; ++x)
		{
			TILE_INDEX idx;
			idx.IndexX = x;
			idx.IndexZ = z;
			if (!Check_ValidIndex(idx)) continue;

			indices.push_back(idx);
		}
	}
	return indices;
}

HRESULT CTileSystem::Add_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag)
{
	for (auto Index : indices) {
		if (!Check_ValidIndex(Index))
			continue;
		m_TileInfos[Index.IndexX + Index.IndexZ * m_tTileInfo.iTileCountX].TileFlag |= flag;
	}
	return S_OK;
}

HRESULT CTileSystem::Add_TileFlagByIndex(TILE_INDEX index, _uint flag)
{
	if (!Check_ValidIndex(index))
		return E_FAIL;

	m_TileInfos[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX].TileFlag |= flag;

	return S_OK;
}

HRESULT CTileSystem::Remove_TileFlagByIndex(vector<TILE_INDEX> indices, _uint flag)
{
	for (auto Index : indices) {
		if (!Check_ValidIndex(Index))
			continue;
		m_TileInfos[Index.IndexX + Index.IndexZ * m_tTileInfo.iTileCountX].TileFlag &= ~flag;
	}
	return S_OK;
}

HRESULT CTileSystem::Remove_TileFlagByIndex(TILE_INDEX index, _uint flag)
{
	if (!Check_ValidIndex(index))
		return E_FAIL;

	m_TileInfos[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX].TileFlag &= ~flag;

	return S_OK;
}

_uint CTileSystem::Get_TileFlagByIndex(TILE_INDEX index)
{
	return m_TileInfos[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX].TileFlag;
}

_bool CTileSystem::Check_TileFlagByPosition(_float4 WorldPos, _uint flag)
{
	TILE_INDEX Index = Get_IndexByPosition(WorldPos);
	if (!Check_ValidIndex(Index)) return false;

	_uint Flag = m_TileInfos[Index.IndexX + Index.IndexZ * m_tTileInfo.iTileCountX].TileFlag;
	return(Flag & flag) != 0;
}

TILE_INFO CTileSystem::Get_InfoByIndex(TILE_INDEX index)
{
	if (!Check_ValidIndex(index)) return TILE_INFO{};
	return m_TileInfos[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX];
}

INSTANCE_TILE CTileSystem::Get_InstanceInfoByIndex(TILE_INDEX index)
{
	if (!Check_ValidIndex(index)) return INSTANCE_TILE{};

	return m_InstanceTiles[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX];
}

void CTileSystem::Change_CornerHeight(TILE_INDEX index, _float leftTop, _float rightTop, _float rightBottom, _float leftBottom)
{
	auto& TileInfo = m_TileInfos[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX];
	TileInfo.fCornerHeight[0] = leftTop;
	TileInfo.fCornerHeight[1] = rightTop;
	TileInfo.fCornerHeight[2] = rightBottom;
	TileInfo.fCornerHeight[3] = leftBottom;

	m_DirtyTile.push_back(index);
}

static const TILE_INDEX CrossDir[] = { {+1,0},{-1,0},{0,+1},{0,-1} };
static const TILE_INDEX SquareDir[] ={{ +1,  0 }, { -1,  0 }, {  0, +1 }, {  0, -1 },{ +1, +1 }, { -1, +1 }, { +1, -1 }, { -1, -1 }};

/*맨해튼*/
vector<TILE_INDEX> CTileSystem::Request_Path_To(TILE_INDEX start, TILE_INDEX goal, _uint avoidMask)
{
	vector<TILE_INDEX> empty;
	if (start == goal)
		return { start };

	auto isNotToAvoid =
		[&](const TILE_INDEX& idx) -> _bool {
		_uint flag = Get_TileFlagByIndex(idx);
		if ((flag & avoidMask) != 0)
			return false;
		else
			return true;
		};

	unordered_map<TILE_INDEX, _float, TILE_INDEX_HASH> g_CostContainer; /*시작점에서 현재 도달지점 까지 도달하는 _float 비용*/
	unordered_map<TILE_INDEX, _float, TILE_INDEX_HASH> f_CostContainer;/*시작점에서 목표까지 도달하는 _float 비용*/
	// f[index] = g[index] + Heuristic(index, goal) => 이게 내가 구하고자 하는 것. 가장 작은애가 최단 거리임

	/*타일 -> 타일 부모*/
	unordered_map<TILE_INDEX, TILE_INDEX, TILE_INDEX_HASH> parent;
	/*이미 확인 된 타일 등*/
	unordered_set<TILE_INDEX, TILE_INDEX_HASH> closed;

	/*맵 안을 돌아보면서 해당 타일이 존재하는지 검사. 없으면 무한 때려버리기*/
	using MAP = unordered_map<TILE_INDEX, _float, TILE_INDEX_HASH>;
	auto getOrInf = [](const auto& MAP, const TILE_INDEX& INDEX) {
		auto it = MAP.find(INDEX);
		return (it == MAP.end()) ? numeric_limits<float>::infinity() : it->second;
		};

	/*큐 구조체*/
	struct QNode { _float fCost; TILE_INDEX index; };
	/*비교 펑터 ->우선 순위 큐는 큰값이 먼저다.(내부적으로 비교해서 false가 나오면 앞으로 넣음) 그래서 나는 펑터에서 작을 때 false가 나오게함*/
	struct Compare { _bool operator()(const QNode& a, const QNode& b) const { return a.fCost > b.fCost; } };
	priority_queue<QNode, vector<QNode>, Compare> open; /*탐색 후보군들-> 작은 애들부터 할거니까*/

	g_CostContainer[start] = 0.f;
	f_CostContainer[start] = Diagonal(start, goal);

	/*처음 시작 지점 기록 시작*/
	open.push({ f_CostContainer[start], start });

	/*탐색 후보군 다 사라지기 전까지*/
	while (!open.empty()) {
		/*오픈에서 꺼냄 1회차는 시작지점*/
		TILE_INDEX current = open.top().index;
		open.pop();

		if (current == goal) {/*만약 꺼내보니 목표지점이라면 경로 복원함*/
			return ReconstructPath(parent, current);
		}

		/*만약 이미 확인 된 친구라면? 즉 클로즈에 들어가 있으면 넘어가고 */
		if (closed.find(current) != closed.end()) continue;
		/*아니라면 이제부터라도 넣음*/
	
		closed.insert(current);
		// 8방향 이웃 검사함
		for (const auto& direction : SquareDir) {
			TILE_INDEX neighbor{ current.IndexX + direction.IndexX, current.IndexZ + direction.IndexZ };

			if (!isNotToAvoid(neighbor)) continue; //만약 이웃이 금지 목록이거나, 아니면 이미 검사한 애면 넘어감
			if (closed.find(neighbor) != closed.end()) continue;
			if (direction.IndexX != 0 && direction.IndexZ != 0)
			{
				TILE_INDEX adj1{ current.IndexX + direction.IndexX, current.IndexZ };
				TILE_INDEX adj2{ current.IndexX, current.IndexZ + direction.IndexZ };
				if (!isNotToAvoid(adj1) || !isNotToAvoid(adj2))
					continue;
			}
			if (!Check_ValidIndex(neighbor))continue;

			/*코스트 계산 시작 -> 이제야 꺼낸애를 확ㅇ니함*/
			// /*맵하튼 용 _float tentative = getOrInf(g_CostContainer, current) + 1.0f; // 4방향은 전부 비용 1
			_float cost = (direction.IndexX == 0 || direction.IndexZ == 0) ? 1.f : 1.41421356f;
			_float tentative = getOrInf(g_CostContainer, current) + cost;

			/*내 이웃 중에 혹시 나 +1 보다 적은 친구가 있니*/
			if (tentative < getOrInf(g_CostContainer, neighbor)) {
				parent[neighbor] = current; /*잇으면 내가 니 부모다->즉 너가 되면 내가 너 이전 타일이야~*/
				g_CostContainer[neighbor] = tentative; /*너는 코스트 기록 되었다.*/
				f_CostContainer[neighbor] = tentative + Diagonal(neighbor, goal); /*네 총 비용은 이정도 되겠지*/

				open.push({ f_CostContainer[neighbor], neighbor }); /*4개 우선 순위 넣었다?*/
			}
		}
	}

	// 다 꺼냈는데도 없네 : 경로 없음
	return empty;
}

HRESULT CTileSystem::Save_TileSystemData(const string& SavePath)
{
	ofstream ofs(SavePath.c_str(), ios::binary);
	if (!ofs.is_open())
		return E_FAIL;

	ofs.write(reinterpret_cast<const char*>(&m_tTileInfo), sizeof(TILESYSTEM_INFO));

	_uint infoCount = m_TileInfos.size();
	ofs.write(reinterpret_cast<const char*>(&infoCount), sizeof(_uint));
	for (size_t i = 0; i < infoCount; i++)
	{
		ofs.write(reinterpret_cast<const char*>(&m_TileInfos[i]), sizeof(TILE_INFO));
		ofs.write(reinterpret_cast<const char*>(&m_InstanceTiles[i]), sizeof(INSTANCE_TILE));
	}

	ofs.close();
	return S_OK;
}

HRESULT CTileSystem::Executer_SystemByData(const string& LoadPath)
{
	ifstream ifs(LoadPath.c_str(), ios::binary);
	if (!ifs.is_open())
		return E_FAIL;

	ifs.read(reinterpret_cast<char*>(&m_tTileInfo), sizeof(TILESYSTEM_INFO));

	_uint infoCount = {};
	ifs.read(reinterpret_cast<char*>(&infoCount), sizeof(_uint));
	m_TileInfos.resize(infoCount);
	m_InstanceTiles.resize(infoCount);
	for (size_t i = 0; i < infoCount; i++)
	{
		ifs.read(reinterpret_cast<char*>(&m_TileInfos[i]), sizeof(TILE_INFO));
		ifs.read(reinterpret_cast<char*>(&m_InstanceTiles[i]), sizeof(INSTANCE_TILE));
		m_TileInfos[i].pTileBlock = nullptr;
	}
	ifs.close();
	return S_OK;
}

/*경로 복원하기*/
vector<TILE_INDEX> CTileSystem::ReconstructPath(const unordered_map<TILE_INDEX, TILE_INDEX, TILE_INDEX_HASH>& parent, TILE_INDEX cur)
{
	vector<TILE_INDEX> result;
	result.push_back(cur);
	auto it = parent.find(cur);

	/*부모에서 최상위 부모로 올라갈 때까지*/
	while (it != parent.end())
	{
		cur = it->second;
		result.push_back(cur);
		it = parent.find(cur);
	}

	reverse(result.begin(), result.end());
	return result;
}


CTileBlock* CTileSystem::Get_TileBlockByIndex(TILE_INDEX index)
{
	if (!Check_ValidIndex(index))
		return nullptr;

	return m_TileInfos[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX].pTileBlock;
}

_bool CTileSystem::Get_TileInfoByIndex(TILE_INDEX index, TILE_INFO& info)
{
	if (!Check_ValidIndex(index))
		return false;

	info = m_TileInfos[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX];

	return true;
}

_bool CTileSystem::Check_ValidIndex(TILE_INDEX index)
{
	if (index.IndexX < 0 || index.IndexX >= static_cast<_int>(m_tTileInfo.iTileCountX))
		return false;
	if (index.IndexZ < 0 || index.IndexZ >= static_cast<_int>(m_tTileInfo.iTileCountZ))
		return false;

	return true;
}

TILE_INFO CTileSystem::Find_Info(TILE_INDEX index)
{
	if (!Check_ValidIndex(index))
		return TILE_INFO();

	return m_TileInfos[index.IndexX + index.IndexZ * m_tTileInfo.iTileCountX];
}

_float CTileSystem::Diagonal(const TILE_INDEX& a, const TILE_INDEX& b)
{
	
	_float dx = fabsf(float(a.IndexX - b.IndexX));
	_float dz = fabsf(float(a.IndexZ - b.IndexZ));
	_float D = 1.f;
	_float D2 = 1.41421356f;
	return D * (dx + dz) + (D2 - 2.f * D) * min(dx, dz);
}

CTileSystem* CTileSystem::Create(const TILESYSTEM_INFO& tileInfo)
{
	CTileSystem* instance = new CTileSystem();
	if (FAILED(instance->Initialize(tileInfo))) {
		Safe_Release(instance);
	}
	return instance;
}

CTileSystem* CTileSystem::CreateByData(const string& LoadPath)
{
	CTileSystem* instance = new CTileSystem();
	if (FAILED(instance->Executer_SystemByData(LoadPath))) {
		Safe_Release(instance);
	}
	return instance;
}

void CTileSystem::Free()
{
	__super::Free();

	Safe_Release(m_pInstanceModel);
	Safe_Release(m_pTileMaterial);
	Safe_Release(m_pContext);
}
