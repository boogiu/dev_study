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


typedef struct tagNpcDataDesc {
	_int NpcIndex = {};
	_int NpcID= {};
	wstring NpcName = {};
	string FolderPath = {};
	string ModelName = {};
	string MaterialName = {};
} NPC_DATA_DESC;

typedef struct tagCothDataDesc {

} CLOTH_DATA_DESC;


/*선택지*/
typedef struct tagChoiceDataDesc
{
	_int ChoiceIndex = 0;				 // 선택지 번호
	wstring Text;								// 선택지 텍스트
	_int NextSequenceID = -1;	// 선택 후 이동할 시퀀스 ID
}CHOICE_DATA_DESC;

typedef struct tagPostActionDataDesc
{
	string Type;				// 행동 이름
	string Param1;			 // 인자 1
	_int         Param2 = 0; // 인자 2
	_int         NextSequenceID = -1;
}POST_ACTION_DATA_DESC;


typedef struct tagSequenceDataDesc {
	_int NpcID = {};
	_int SequenceID = {};
	_int LineIndex = {};
	wstring Text = {};
	string Emotion = {};
	string Motion = {};
	string Voice = {};
	_float pauseTime = {  };

	POST_ACTION_DATA_DESC postAction;
	vector<CHOICE_DATA_DESC> choiceSelection;
} SEQUENCE_DATA_DESC;