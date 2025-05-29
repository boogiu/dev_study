#pragma once

typedef struct tagPolygon {
    D3DXVECTOR3 m_vLocalPos;
    D3DXVECTOR3 m_vWorldPos;
 } POLYGON;

typedef struct tagRectangle {
    D3DXVECTOR3 m_vCenterPosLocal;
    D3DXVECTOR3 m_vCenterPosWorld;
    D3DXVECTOR3 m_vScale;
    D3DXVECTOR3 m_vRotate;

    POLYGON tLT;
    POLYGON tLB;
    POLYGON tRT;
    POLYGON tRB;
} RECTANGLE;