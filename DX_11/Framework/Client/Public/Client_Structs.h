#pragma once

typedef struct tagItemDataDesc {
	_int ItemIndex = {};
	_int fullCount = {};
	itemType TypeTag = {  };
	string IconName = {};
	wstring ItemName = {};
	string FileName = { "None" };
	string modelName = {};
	string materialName = {};
	string Additionaldata = {};
}ITEM_DATA_DESC, TOOL_DATA_DESC;

typedef struct tagInsectDataDesc{
	_int InsectIndex = {};
	string InsectFileName = {};
	string modelName = {};
	string materialName = {};
	wstring InsectName = {};
	string ItemFile = {};
	string IconName= {};
	wstring Comment = {};
} INSECT_DATA_DESC;

typedef struct tagFishDataDesc {
	_int FishIndex = {};
	string FishFileName = {};
	string modelName = {};
	string materialName = {};
	wstring FishName = {};
	string ItemFile = {};
	string IconName = {};
	wstring Comment = {};
	string sizeTag = {};
}FISH_DATA_DESC;

typedef struct tagNpcDataDesc {
	_int NpcIndex = {};
	_int NpcID= {};
	wstring NpcName = {};
	string FolderPath = {};
	string ModelName = {};
	string MaterialName = {};
	string NpcKey = {};
	string TopName = {};
	string BottomName = {};
	string BaseToolName = {};
} NPC_DATA_DESC;

/*선택지*/
typedef struct tagChoiceDataDesc
{
	_int ChoiceIndex = 0;					// 선택지 번호
	wstring Text;									// 선택지 텍스트
	_int NextSequenceID = -1;		// 선택 후 이동할 시퀀스 ID
}CHOICE_DATA_DESC;

struct PostActionMsg {
	string Type = {};							// "GiveItem", "UnlockQuest", "MoveNpc", "TriggerEvent"
	string Param1 = {};						// 대상 이름, 아이템 태그 등
	string Param2 = {};						// 수량이나 세부 데이터
	_int NextSequenceID = -1;		// PostAction 수행 후 이어질 시퀀스
} ;

typedef struct tagSequenceDataDesc
{
	_int NpcID = {};

	_int SequenceID = {};
	string EntryCondition = {};

	_int LineIndex = {};
	_float pauseTime = {  };

	wstring Text = {};
	string Emotion = {};
	string Motion = {};
	string Voice = {};
	string Camera = {};

	vector<CHOICE_DATA_DESC> choiceSelection;

	_int NextSequenceID = { -1 };        // -1  자동 전이 없음
	string NextCondition = {};				// (선택지나 조건 분기용)

	PostActionMsg postActionMsg;

} SEQUENCE_DATA_DESC;

struct CraftData {
    ITEM_DATA_DESC Result_Item = {};
    string RecipeImage = {};

    ITEM_DATA_DESC ResourceItem_01 = {};
    _uint ResourceItemCount_01 = {};
    _uint nowCount_01 = {};

    ITEM_DATA_DESC ResourceItem_02 = {};
    _uint ResourceItemCount_02 = {};
    _uint nowCount_02 = {};

    ITEM_DATA_DESC ResourceItem_03 = {};
    _uint ResourceItemCount_03 = {};
    _uint nowCount_03 = {};

    _bool isAbleToCraft() {
        return
            (ResourceItemCount_01 <= nowCount_01) &&
            (ResourceItemCount_02 <= nowCount_02) &&
            (ResourceItemCount_03 <= nowCount_02);
    }
    _bool Has(wstring itemName) {
        return
            (ResourceItem_01.ItemName == itemName) ||
            (ResourceItem_02.ItemName == itemName) ||
            (ResourceItem_03.ItemName == itemName);
    }
    void Add(wstring itemName, _uint Count)
    {
        if (ResourceItem_01.ItemName == itemName)
        {
            nowCount_01 += Count;
            return;
        }

        if (ResourceItem_02.ItemName == itemName)
        {
            nowCount_02 += Count;
            return;
        }

        if (ResourceItem_03.ItemName == itemName)
        {
            nowCount_03 += Count;
            return;
        }
    }
	
	void Reset() {
		nowCount_01 =0;
		nowCount_02 = 0;
		nowCount_03 = 0;
	}
};

struct EffectData {
	_float4 ReqPosition = {};
	_float4 FxPosition = {};
	string Additional = {};
	EffectData() {};
	EffectData(_float4 fxPosition, _float4 reqPosition) 
		: FxPosition(fxPosition), ReqPosition(reqPosition){}
	EffectData(_float4 fxPosition, _float4 reqPosition,string additional) 
		: FxPosition(fxPosition), ReqPosition(reqPosition),Additional(additional){}
};


typedef struct  tagInstanceParticle
{
	_float4			vRight, vUp, vLook, vTranslation;
	_float4              vColor;
	_float2              vLifetime;
}INSTANCE_PARTICLE;

typedef struct   tagVertexParticleInstancing {
	static constexpr unsigned int					iElementCount = { 6 };
	static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[6] = {
		{ "INSTANCE", 0,	DXGI_FORMAT_R32G32B32A32_FLOAT, 1,0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 1,	DXGI_FORMAT_R32G32B32A32_FLOAT, 1,16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 2,	DXGI_FORMAT_R32G32B32A32_FLOAT, 1,32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 3,	DXGI_FORMAT_R32G32B32A32_FLOAT, 1,48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 4,	DXGI_FORMAT_R32G32B32A32_FLOAT, 1,64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 5,	DXGI_FORMAT_R32G32_FLOAT, 1,80, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
}VTX_PARTICLEINSTANCE;
