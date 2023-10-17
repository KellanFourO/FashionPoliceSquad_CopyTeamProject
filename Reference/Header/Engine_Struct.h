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
		static const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // �ؽ�ó�� UV ��ǥ ���� FLOAT�� 3���� ǥ���ϰڴٴ� ��ũ��(��ȣ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؽ��� UV���� �������� �� �� �ִµ� ���� 0��° ���� �����ϰڴٴ� �ǹ�)
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


	typedef struct tagTerrainData   // Terrain Data ��������¿�
	{
		_ulong	dwCntX;  //���� ���ؽ� ����
		_ulong	dwCntY;  //���� ���ؽ� ����
		_float	vtxITV;	 //���ؽ� ����

	}TERRAINDATA;


	typedef struct tagCubeData	//���� ť�� ������
	{
		_uint					uITextureNum;	//�ؽ��� �ѹ�
		_vec3					vSize;			//x, y, z ����
		_vec3					vPos;			//x, y, z ����
		_uint					iCubeIndex;		//�� ��°�� ��������� ť��?
		OBJ_TYPE				eOBJ_TYPE;		//OBJ Ÿ�� enum class

	}CUBE;

	typedef struct tagOBJData	//���� OBJ ������
	{
		IDirect3DBaseTexture9*	pBaseTexture;
		IDirect3DCubeTexture9*	pCubeTexture;
		_uint					uiTextureNum;	//�ؽ��� �ѹ�
		_vec3					vSize;			//x, y, z ����
		_vec3					vPos;			//x, y, z ����
		_uint					iRotateCount;	//90�� ������ ȸ�� �� �� ��ų�ǵ�(Y��)
		_uint					iIndex;			//�� ��°�� ��������� �༮�̳�
		OBJ_TYPE				eOBJ_TYPE;		//OBJ Ÿ�� enum class (�ǹ�/OBJť��/OBJ���)
		OBJ_ATTRIBUTE			eOBJ_Attribute;	//OBJ �Ӽ�(�ı�/��ȣ�ۿ�/���)

		_uint					uiOBJ_HP;		//�Ӽ��� '�ı�'�� ��� - ü���� �� ���̸� �ı���
		OBJ_INTERACTION			eOBJ_Interaction;//�Ӽ��� '��ȣ�ۿ�'�� ��� : �����׼� Pole ��

	}OBJData;

	typedef struct tagTriggerInfo
	{
		TRIGGER_NUMBER			eTrName;		//Ʈ���� �̸� ����ü
		_vec3					vSize;			//x, y, z ����
		_vec3					vPos;			//x, y, z ����
		_uint					iIndex;			
		TRIGGER_CASE			eTrCase;		//�� ��, �� ��, �ݺ� �ߵ� ��
		TRIGGER_TYPE			eTrType;		//�ߵ� �� � ������ ���� ������
		TRIGGER_STATE			eTrSTATE;		//�ߵ� ��, ��, ��

	}TRIGGER;

	typedef struct tagCreatePoint
	{
		OBJData					defOBJData;
		MonsterType				eMonsterType;
	}C_POINT;

	typedef struct	SortCube {  //ť�� �ؽ��� ���� ���� ����ü
		char*					stFileName;		//���� �̸�(���� ��)
		_uint					iNameNumber;	//���ϸ� ���� ����
		IDirect3DCubeTexture9*	tTexture;
	}SORTCUBE;

	typedef struct	SortTexture {  //��� �ؽ��� ���� ���� ����ü
		char*					stFileName;		//���� �̸�(���� ��)
		_uint					iNameNumber;	//���ϸ� ���� ����
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
		MonsterType			iMobType;	//���� Ÿ��

		_vec3			vPos;		//��ġ

		_float			fHP;		//ü��
		_float			fMaxHP;		//�ִ�ü��
		_float			fEXP;		//����ġ
		_float			fLevel;		//����
		_float			Attack;		//���� ���ݷ�
		_float			AggroRange;	//�÷��̾� �����ϴ� ����

		_bool			bAggro;		//��׷� ���� ���·� ��������
		_bool			bDead = false;		//�ֱ�
		_bool			bHit = false;		//�ĸ���

		_int			iFlags;		//�̺�Ʈ��

		CMonsterState*  MonsterState;		//���� ����
		CBossState*		BossState;
	}Mob_INFO;


	typedef struct		Player_Info {
		_vec3			vPos;		//��ġ
		_float			fStartDir;  //���������� ���� ����

		_float			fMoveSpeed;	//�̵��ӵ�
		_float          fJumpHight;	//��������
		_float			fAttackDeley; //���� �ֱ�

		_float			fAttack;	//���ݷ�

		_float			fHP;		//ü��
		_float			fMaxHP;		//�ִ�ü��
		_float			fMaXHP_Additional; // �߰� �ִ�ü�� (�߰���!)
		//(fHp�� fMaxHP + fMatHP_Additional ���� �ִ��̰�, �ð��� ������ fMaxHP�� �� ������ ü���� ������)

		_float			fArmor;		//�Ƹ�
		_float			fMaxArmor;	//�ִ�ü��

		_int			iLevel;		//����
		_int			iEXP;		//����ġ

		_float			fMoney;		//������

		_float			fCurBullet; //���� �Ѿ�
		_float			fMaxBullet; //�ִ� �Ѿ�



		CPlayerState*	PlayerState;	//�÷��̾� ����
		PLAYER_GUNTYPE	Player_GunType; // ����ִ� ��

		_bool			bGameOver;	//�÷��̾� ü���� 0�� ������ (�߰���!)

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

	// �������� �κ��丮�� �����ߴٰ� ���, �ƸӴ� �ٷ�����
	// �ִ�ü��, �ִ�Ƹ� �߰�
	// �ƸӰ� ���� ���̰�, ü���� �𿩾���.
	// �׽�Ʈ �ڵ� ¥��


	//particle
	struct	Particle
	{
		D3DXVECTOR3 _Position; // ��ġ
		D3DCOLOR	_Color;    // ��
	};
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE ;

	//particle �Ӽ�
	typedef struct Attribute
	{
		Attribute()
		{
			lifeTime = 0.f;
			age = 0.f;
			isAlive = true;
		}
		D3DXVECTOR3 position;	// ���� �����̽� ��ƼŬ ��ġ
		D3DXVECTOR3 velocity;	//��ƼŬ�� �ӵ� , ���� �ʴ� �̵�����
		D3DXVECTOR3 acceleration;	//��ƼŬ�� ����, ������ �ʴ� �̵�����
		_float		lifeTime;	//��ƼŬ �Ҹ� ������ ���� �ð�  ex) ������ �� ��ƼŬ�� Ư���� �ð� ���ȸ� �����ȴ� ��.
		_float		age;	//��ƼŬ�� ���� ����
		D3DXCOLOR	color;	//��ƼŬ ��
		D3DXCOLOR   colorFade;	//��ƼŬ�� �÷��� �ð��� ���� �ٲ�� ���
		bool		isAlive;	//��ƼŬ�� ������ ��� True , �Ҹ��� ��� False
	}ATTRIBUTE;

	typedef struct      Ray_Info
	{
		_vec3 vRayPos;
		_vec3 vRayDir;
	}RAY;

}

#endif // Engine_Struct_h__
