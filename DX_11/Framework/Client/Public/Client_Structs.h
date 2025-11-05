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
}ITEM_DATA_DESC, TOOL_DATA_DESC;

struct EventMsgDesc {
	_float2 OpenSize = {};
	_float OpenSpeed = {};
	vector<wstring> textSequence;
	function<void()> OnClose;
};