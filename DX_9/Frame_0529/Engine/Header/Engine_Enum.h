#pragma once
namespace Engine {
	enum WINMODE { MODE_FULL, MODE_WIN };
	
	enum COMPONENTID {ID_DYNAMIC, ID_STATIC, ID_END};
	
	enum INFO {INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS,INFO_END};
	enum ROTATION {ROT_X,ROT_Y,ROT_Z, ROT_END};

	enum TEXTUREID{TEX_NORMAL, TEX_CUBE, TEX_END};
	enum RENDERID {RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI,RENDER_END};
	
	enum MOUSEKEYSTATE{DIM_LB,DIM_RB,DIM_MB,DIM_END};
	enum MOUSEMOVESTATE{DIMS_X,DIMD_Y,DIMD_Z,DIMS_END};

	enum class OBJ_TYPE{};
	enum class LIGHT_TYPE{DIRECTION, POINT,SPOTLIGHT};

	enum class COM_UPDATE{DYNAMIC,STATIC};
    enum class COM_TYPE {
        TRANSFORM,     // 위치, 회전, 스케일
        RENDERER,      // 렌더링용
        MESH,          // 정점/인덱스 버퍼
        CAMERA,        // 뷰/프로젝션 계산
        LIGHT,         // 광원
        TERRAIN,       // 지형
        RIGIDBODY,     // 물리 기반 움직임

        COLLIDER,      // 충돌 영역 (AABB/Sphere 등)
        ANIMATOR,      // 애니메이션 (스켈레톤 포함)
        AUDIO_SOURCE,  // 사운드 재생
        SCRIPT,        // 커스텀 로직, FSM
        PARTICLE_SYSTEM, // 파티클 이펙트
        SKYBOX,        // 하늘 배경
        BILLBOARD,     // 카메라 빌보드
        POST_PROCESSOR,// 블룸, DOF 등 후처리
        NAV_AGENT,     // 내비게이션 에이전트
        DECAL,         // 표면 이미지 투사
        LIFETIME,      // 시간 지나면 파괴
        TAG,           // 그룹 태그, 이름 등
        // 마지막은 개수 확인용
        END
    };

}