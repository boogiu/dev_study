#include "Engine_Define.h"
#include "MyMath.h"

void MyMath::MyViewTrans(_matrix* pOut, _vec3* vEye, _vec3* vAt, _vec3* vUp)
{
	// 우선 행렬을 통해 기저 벡터를 카메라 기준 x,y,z로 변형을 시킨다. 그리고 이동을 시킨다.
	//이러면 카메라가 월드 상에 어떤 행렬을 받았는지 알 수 있게 됨.(왜냐면 기저벡터 변형 후에 이동 했으니까?)

	//카메라 기준 z -> 바라보는 곳 방향 벡터
	_vec3 vLook = *vAt - *vEye;
	D3DXVec3Normalize(&vLook, &vLook);

	//카메라 기준 x-> 룩 벡터와 월드 업 벡터()
	_vec3 vRight; 
	D3DXVec3Cross(&vRight, vUp, &vLook);

	//카메라 기준 y 
	_vec3 vMyUp;
	D3DXVec3Cross(&vMyUp, &vLook, &vRight);

	//기저 벡터 재설정
	pOut->_11 = vRight.x;	pOut->_12 = vRight.y; 	pOut->_13 = vRight.z; 	pOut->_14 = 0;
	pOut->_21 = vMyUp.x;	pOut->_22 = vMyUp.y;	pOut->_23 = vMyUp.z; 	pOut->_24 = 0;
	pOut->_31 = vLook.x;	pOut->_32 = vLook.y;	pOut->_33 = vLook.z; 	pOut->_34 = 0;

	//vEye만큼 이동
	pOut->_41 = vEye->x;	pOut->_42= vEye->y;		pOut->_43= vEye->z;		pOut->_44=1;

	D3DXMatrixInverse(pOut, 0, pOut);
}

//앵글은 라디안으로
void MyMath::MyProjectionTrans(_matrix* pOut, float angle, float aspect, float zNear, float zFar)
{
	//투영 변환
	//-> 종횡비 매트릭스 곱해주고
	// z나누기 
	// 1/탄젠트 (세타/2) 


	//틀린코드
	// _matrix aspectMat;
	// D3DXMatrixIdentity(&aspectMat); //초기화
	// 
	// //x와y값 비율 
	// aspectMat._11 = (1 / tanf(angle / 2)) * (1 / aspect);
	// aspectMat._22 = 1 / tanf(angle / 2);
	// //z값에 영향을 받는 인자
	// aspectMat._33 = 1 / (zFar - zNear);
	// aspectMat._43 = 1 - (1 / (zFar - zNear) * zFar);
	// //z남기기
	// aspectMat._34=1; 
	_matrix aspectMat;
	D3DXMatrixIdentity(&aspectMat);

	float f = 1.0f / tanf(angle / 2.0f); // 공통 계수

	aspectMat._11 = f / aspect;          // x 스케일
	aspectMat._22 = f;                   // y 스케일

	aspectMat._33 = zFar / (zFar - zNear);
	aspectMat._34 = 1.0f;

	aspectMat._43 = -zNear * zFar / (zFar - zNear);
	aspectMat._44 = 0.0f;

	*pOut = aspectMat;

	//		0, 0, zNear, 1과 행렬 곱한다고 했을 때, = > 0;
	//		0, 0, zFar, 1과 행렬 곱한다고 했을 때, = > 1;
	//
	//(zNear* fx +  gx /x)/zNear= 0;
	//(zFar* fx + gx)/zFar =1;
	// 
	//int subs = (zFar - zNear); //1/fx
	//1 / subs => fx;
	//gx = 1 - zFar(fx);
}
