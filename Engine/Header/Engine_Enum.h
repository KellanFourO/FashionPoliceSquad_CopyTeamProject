#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	// Dynamic 컴포넌트 경우 매 프레임마다 갱신해야하는 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };
	enum INFO {	INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_AXIS, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };


	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA,RENDER_ALPHATEST, RENDER_ALPHA, RENDER_UI,RENDER_BLOCK, RENDER_END };
	enum CAMERAID { CAM_FIRST, CAM_THIRD, CAM_CENTER, CAM_END };

	enum OBJ_STATE {OBJ_NOEVENT, OBJ_DEAD, OBJ_END};

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum class StateMachine { IDLE, WALK, ATTACK, HIT, DEAD, JUMP, ROLL, GET, FALL, PUSH, SHIELD, STATE_END };

	enum class LAYERTAG {ENVIRONMENT, GAMELOGIC, CAMERA, EFFECT, UI, LAYERTAG_END};

	enum class SCENETAG {LOGO,LOADING ,STAGE,STAGE2,LOBBY, MAPTOOL, BOSS_STAGE, SCENETAG_END };

	enum class COMPONENTTAG
	{
		BUFFER, TRANSFORM,
		TEXTURE, CALCULATOR, LIGHT, CAMERA, BASICSTAT, MONSTERAI,
		ANIMATOR, PARTICLE, BILLBOARD, STATEMACHINE, INVENTORY, FONT, RIGIDBODY, COLLIDER, COMPONENT_END
	};
	enum class OBJECTTAG
	{
		PLAYER, PROGRESSVALUE, PROGRESSFRAME, FPSCAMERA, DCAMERA, FLYCAMERA, MONSTER, PLAYERBULLET, BOSS, ITEM, MONSTERBULLET, PLAYER_LAZER, PLAYER_GUN,
		UI, MISSION, PORTRAIT, CROSSHAIR, CARDLIST, CARD, CARDTEXTURE, DIALOG, BOSSBULLET, OBJECT, RAY, SKYBOX, EFFECT, BACKGOUND, TERRAIN, WIREFRAME,
		PLAYER_HAND, BUILD_CUBE, BUILD_OBJ, TRIGGER, MAPCURSOR, DEAD_OBJ, DEAD_MONSTER, PARTICLE, OBJECT_END
	};

	enum CHANNELID { SOUND_EFFECT, SOUND_BGM, SOUND_PLAYER, MAXCHANNEL };


	//유진) OBJ 관련 enum
	enum class OBJ_TYPE { BUILDING_TYPE, CUBE_TYPE, PLANE_TYPE, OBJ_TYPE_END };
	enum class OBJ_ATTRIBUTE { DES_OBJ, INTER_OBJ, LIGHT_OBJ, BILL_OBJ, NONE_OBJ, TRIGGER_OBJ, C_POINT_OBJ, MOVING_OBJ, ATTRIBUTE_END };
							//파괴OBJ, 상호작용OBJ, 조명OBJ, 빌보드 OBJ, 장식 OBJ, 트리거 OBJ, CreatePoint, 무빙 OBJ
	enum class OBJ_INTERACTION { OBJ_LOCK_ON, OBJ_LOCK_OFF, INTER_NONE, INTER_END };
			//상호작용 OBJ에서	  LOCK_ON,	   LOCK_OFF,	상호작용 비해당  을 의미
	enum class CAMERA_TYPE { NOTMOVE_CAMERA, MOVE_CAMERA, CAMERA_END };
	enum class LIGHT_TYPE { LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_SPOT, LIGHT_END };
	enum class MonsterType { BIGDADDY, DULLSUIT, KCIKBOARD, SMOKER, BOSS, MOBTYPE_END };//C POINT (몬스터 Create를 위한 ) 구분 enum
	
	enum class TRIGGER_NUMBER { TR0, TR1, TR2, TR3, TR4, TR5, TR6, TR7, TR8, TR9, TR10,
								TR11, TR12, TR13, TR14, TR15, TR16, TR17, TR18, TR19, TR20, TR_END };
	enum class TRIGGER_CASE { TR_ONCE, TR_TWICE, TR_REPEAT, TR_CASE_END };
	enum class TRIGGER_TYPE { TR_MOVING, TR_UI, TR_DAMAGE, TR_MONSTER, TR_NOTHING, TR_TYPE_END };
	enum class TRIGGER_STATE { TR_BEFORE, TR_NOW, TR_AFTER, TR_STATE_END };




	//TODO - 승용 UI 타입별 태그 정리 시작
	enum class UI_TYPE { HP, VALUE, SPRITE, BULLET, FONT, DIALOG, BASIC, UI_TYPE_END};
	enum class CARD_TYPE { FORCE, SPEED, INTELLIGENCE, STRENGTH, BONUS, BOMB, CARD_TYPE_END};
	enum class CARD_DIR { CARD_LEFT, CARD_CENTER, CARD_RIGHT, CARD_END };
	enum class DIALOGTAG { STORY_ST1_INTRO, STORY_ST1_DEVELOP, STORY_ST1_TURN, STORY_ST1_CONCLU, QUEST_1, QUEST_2, ST1_BOSS_START};
	enum class PORTRAITTAG {PORT_DES, PORT_DESNEON, PORT_HALEY, PORT_DEEPCOAT, PORT_BAUSS, PORT_TURNCOAT};
	enum class BULLETTYPE { SHOTGUN_BULLET, SHOTGUN_RAZER, ASSERTRIFLE_BULLET, ASSERTRIFLE_BOMB,  BULLETTYPE_END};
	enum class PLAYER_GUNTYPE { SHOTGUN, ASSERTRIFLE, PLAYER_GUNTYPE_END};

	//TODO - 승용 UI 타입별 태그 정리 끝

	enum class COLORTAG{RED,ORANGE,YELLOW,GREEN,DEEPGREEN,BLUE,PURPLE,PINK};

	//enum class TIMERTAG {,TIMERTAG_END};

	//enum PROTOTAG {PROTO_TERRAINTEX,PROTO_TRANSFORM,PROTO_TERRAINTEXTURE,PROTO_RCTEX,PROTO_LOGOTEXTURE,PROTO_BRIFCASE,PROTO_DYEHARD,PROTO_EFFECTTEXTURE,PROTO_CALCULATOR,PROTO_TEXTURETILE,};
	//필요한 enum class들 자기 이름 밑에다가 직접 만들것 ! 그리고 이름이 겹칠경우 문제가 생기니 이니셜 꼭 붙여서 만들것
	//EX) enum MOUSEMOVESTATE { JYP_DIMS_X, JYP_DIMS_Y, JYP_DIMS_Z, JYP_DIMS_END };



	//준호 SM
	//충돌


	//유진 YG

	//승용 SKY




	enum class PlayerStateID	//	플레이어 표정 바꿔줄때 받으면 되는 상태
								//	Player_Info& PlayerINFO = dynamic_cast<CPlayer*>(pPlayer)->Get_INFO();
								//	PlayerStateID stateID = PlayerINFO.PlayerState->StateID;
	{ Player_IDLE, Player_Right, Player_Left, Player_Hit, Player_Heal, Player_Armor, Player_Lv_Up};
	enum class MonsterState {
		IDLE, AGGRO_ON, HIT, DEAD, MOBSTATE_END
	};

	enum class MonsterHit
	{};
	enum class GunID        // 총 ID (dynamic_cast<CGun*>(pGun)->GUN_ID == GunID::DYEHARD) 같은 방법으로 찾으면 됨
	{ DYEHARD, RIFLE, GUN_ID_END };
	enum class ItemID		// 아이템 ID
	{	COCKTAIL, BIG_COCKTAIL, WATCH, NECKTIE, CAN, CLOSET,
		SUNGLASSES, HIP_SUNGLASSES,
		SISORS_RED, SISORS_GREEN, SISORS_BLUE, ITEM_ID_END
	};

	//재연 JYP

	enum class BOSSPHASE { PHASE_1, PHASE_2, PHASE_3, PHASE_END};


};
#endif // Engine_Enum_h__
