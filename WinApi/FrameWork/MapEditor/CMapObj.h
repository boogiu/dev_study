#pragma once
#include "../MapEditor/CTile.h"


class CMapObj :
    public CTile
{
public:
    CMapObj();
    CMapObj(int Col, int Row);
    CMapObj(TILE_INFO _info);
    ~CMapObj();

public:
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();

    typedef struct tagColRow {
        int Col;
        int Row;

        tagColRow(int col, int row) : Col(col), Row(row) {};

        tagColRow(tagColRow& rhs) = default;

        bool operator==(const tagColRow& rhs) const {
            return (Col == rhs.Col) && (Row == rhs.Row);
        }
    }MapObj;

};

