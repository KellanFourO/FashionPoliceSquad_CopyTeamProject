 #pragma once
#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Define.h"
#include "Engine_Typedef.h"
#include "Engine_Enum.h"

class CMonsterState;
class CPlayerState;
class CBossState;

namespace Engine
{
	typedef struct tagVertexColor
	{
		_vec3		vPosition;
		_ulong		dwColor;
		static const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;
	}VTXCOL;



	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTexUV;
		static const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	}VTXTEX;


	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec3		vTexUV;
		_ulong		ulCount;
		static const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // 텍스처의 UV 좌표 값을 FLOAT형 3개로 표현하겠다는 매크로(괄호안의 숫자 0의 의미는 본래 버텍스에 텍스쳐 UV값이 여러개가 올 수 있는데 그중 0번째 값을 지정하겠다는 의미)
	}VTXCUBE;

	typedef struct tagVertexCrossTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec3		vTexUV;
		static const _ulong	FVF_CROSS = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	}VTXCROSS;


	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;

	union COLLIDER_ID
	{
		struct
		{
			_uint Left_id;
			_uint Right_id;
		};
		ULONGLONG ID;
	};

	typedef struct tagUIData
	{
		IDirect3DBaseTexture9* pTexture;
		_vec3				   vSize;
		_vec3				   vPos;


	}UIDATA;

	typedef struct tagDIALOGDATA
	{
		IDirect3DBaseTexture9* pTexture;
		_tchar*				   szText;
		_vec3				   vSize;
		_vec3				   vPos;

	}DIALOGDATA;


	typedef struct tagTerrainData   // Terrain Data 파일입출력용
	{
		_ulong	dwCntX;  //가로 버텍스 개수
		_ulong	dwCntY;  //세로 버텍스 개수
		_float	vtxITV;	 //버텍스 간격

	}TERRAINDATA;


	typedef struct tagCubeData	//만든 큐브 데이터
	{
		_uint					uITextureNum;	//텍스쳐 넘버
		_vec3					vSize;			//x, y, z 길이
		_vec3					vPos;			//x, y, z 중점
		_uint					iCubeIndex;		//몇 번째로 만들어지는 큐브?
		OBJ_TYPE				eOBJ_TYPE;		//OBJ 타입 enum class

	}CUBE;

	typedef struct tagOBJData	//만든 OBJ 데이터
	{
		IDirect3DBaseTexture9*	pBaseTexture;
		IDirect3DCubeTexture9*	pCubeTexture;
		_uint					uiTextureNum;	//텍스쳐 넘버
		_vec3					vSize;			//x, y, z 길이
		_vec3					vPos;			//x, y, z 중점
		_uint					iRotateCount;	//90도 단위로 회전 몇 번 시킬건데(Y축)
		_uint					iIndex;			//몇 번째로 만들어지는 녀석이냐
		OBJ_TYPE				eOBJ_TYPE;		//OBJ 타입 enum class (건물/OBJ큐브/OBJ평면)
		OBJ_ATTRIBUTE			eOBJ_Attribute;	//OBJ 속성(파괴/상호작용/장식)

		_uint					uiOBJ_HP;		//속성이 '파괴'일 경우 - 체력이 다 깎이면 파괴됨
		OBJ_INTERACTION			eOBJ_Interaction;//속성이 '상호작용'일 경우 : 로프액션 Pole 등

	}OBJData;

	typedef struct tagTriggerInfo
	{
		TRIGGER_NUMBER			eTrName;		//트리거 이름 열거체
		_vec3					vSize;			//x, y, z 길이
		_vec3					vPos;			//x, y, z 중점
		_uint					iIndex;			
		TRIGGER_CASE			eTrCase;		//한 번, 두 번, 반복 발동 중
		TRIGGER_TYPE			eTrType;		//발동 시 어떤 반응을 만들어낼 것인지
		TRIGGER_STATE			eTrSTATE;		//발동 전, 중, 후

	}TRIGGER;

	typedef struct tagCreatePoint
	{
		OBJData					defOBJData;
		MonsterType				eMonsterType;
	}C_POINT;

	typedef struct	SortCube {  //큐브 텍스쳐 정렬 목적 구조체
		char*					stFileName;		//파일 이름(정렬 용)
		_uint					iNameNumber;	//파일명 끝의 숫자
		IDirect3DCubeTexture9*	tTexture;
	}SORTCUBE;

	typedef struct	SortTexture {  //평면 텍스쳐 정렬 목적 구조체
		char*					stFileName;		//파일 이름(정렬 용)
		_uint					iNameNumber;	//파일명 끝의 숫자
		IDirect3DBaseTexture9*  tTexture;
	}SORTTEX;



	typedef struct tagCubeSize
	{
		_float					fX = VTXITV;
		_float					fY = VTXITV;
		_float					fZ = VTXITV;

	}CUBESIZE;

	typedef struct tagCubeRot
	{
		_float					fX;
		_float					fY;
		_float					fZ;
	}CUBEROT;

	typedef struct	Monster_Info {
		MonsterType			iMobType;	//몬스터 타입

		_vec3			vPos;		//위치

		_float			fHP;		//체력
		_float			fMaxHP;		//최대체력
		_float			fEXP;		//경험치
		_float			fLevel;		//레벨
		_float			Attack;		//몬스터 공격력
		_float			AggroRange;	//플레이어 감지하는 범위

		_bool			bAggro;		//어그로 끌린 상태로 시작할지
		_bool			bDead = false;		//주금
		_bool			bHit = false;		//쳐맞음

		_int			iFlags;		//이벤트용

		CMonsterState*  MonsterState;		//몬스터 상태
		CBossState*		BossState;
	}Mob_INFO;


	typedef struct		Player_Info {
		_vec3			vPos;		//위치
		_float			fStartDir;  //생성됐을때 보는 방향

		_float			fMoveSpeed;	//이동속도
		_float          fJumpHight;	//점프높이
		_float			fAttackDeley; //공격 주기

		_float			fAttack;	//공격력

		_float			fHP;		//체력
		_float			fMaxHP;		//최대체력
		_float			fMaXHP_Additional; // 추가 최대체력 (추가됨!)
		//(fHp는 fMaxHP + fMatHP_Additional 값이 최대이고, 시간이 갈수록 fMaxHP가 될 때까지 체력이 감소함)

		_float			fArmor;		//아머
		_float			fMaxArmor;	//최대체력

		_int			iLevel;		//레벨
		_int			iEXP;		//경험치

		_float			fMoney;		//소지금

		_float			fCurBullet; //현재 총알
		_float			fMaxBullet; //최대 총알



		CPlayerState*	PlayerState;	//플레이어 상태
		PLAYER_GUNTYPE	Player_GunType; // 들고있는 총

		_bool			bGameOver;	//플레이어 체력이 0이 됐을때 (추가됨!)

	}Player_INFO;

	typedef struct      PlayerGun_Info
	{
		const _tchar*	m_szGunName;
		_float			m_fFireSpeed;
		_float			m_fReloadSpeed;
		_int			m_iCurrentBullet;
		_int			m_iMaxBullet;
		_int			m_iReloadBullet;
	}Gun_INFO;

	typedef struct		Item_Info {
		ItemID		Item_ID;
		_vec3		vPos;
	};

	// 아이템은 인벤토리에 저장했다가 사용, 아머는 바로적용
	// 최대체력, 최대아머 추가
	// 아머가 먼저 깎이고, 체력이 깎여야함.
	// 테스트 코드 짜기


	//particle
	struct	Particle
	{
		D3DXVECTOR3 _Position; // 위치
		D3DCOLOR	_Color;    // 색
	};
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE ;

	//particle 속성
	typedef struct Attribute
	{
		Attribute()
		{
			lifeTime = 0.f;
			age = 0.f;
			isAlive = true;
		}
		D3DXVECTOR3 position;	// 월드 스페이스 파티클 위치
		D3DXVECTOR3 velocity;	//파티클의 속도 , 보통 초당 이동단위
		D3DXVECTOR3 acceleration;	//파티클의 가속, 보통은 초당 이동단위
		_float		lifeTime;	//파티클 소멸 전까지 유지 시간  ex) 레이저 빔 파티클은 특정한 시간 동안만 유지된다 등.
		_float		age;	//파티클의 현재 나이
		D3DXCOLOR	color;	//파티클 색
		D3DXCOLOR   colorFade;	//파티클의 컬러가 시간에 따라 바뀌는 방법
		bool		isAlive;	//파티클이 생존한 경우 True , 소멸한 경우 False
	}ATTRIBUTE;

	typedef struct      Ray_Info
	{
		_vec3 vRayPos;
		_vec3 vRayDir;
	}RAY;

}

#endif // Engine_Struct_h__
