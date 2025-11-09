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

