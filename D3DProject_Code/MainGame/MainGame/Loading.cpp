#include "pch.h"
#include "Loading.h"

Loading::Loading()
{

}

Loading::~Loading()
{

}

void Loading::init()
{
	///리소스 업로드를 위한 인트로 화면 나중에는 로딩씬도 띄워야할듯

	


	//모델 로드
	mFactory->SetResourece(LOAD_TYPE::ASE, "box.ASE", "box");
	//FBX 로드
	
	
	//Boss
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/Boss/Boss_BossRun.fbx",	  "Boss");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/Boss/Boss_BossPunch01.fbx", "Boss");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/Boss/Boss_BossPunch02.fbx", "Boss");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/Boss/Boss_BossPunch03.fbx", "Boss");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/Boss/Boss_BossDead.fbx",	 "Boss");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/Boss/Boss_BossGroggy1.fbx" ,"Boss");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/Boss/Boss_BossGroggy2.fbx" ,"Boss");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/Boss/Boss_BossIdle.fbx"	,"Boss");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/Boss/Boss_BossSkill01.fbx"	,"Boss");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/Boss/Boss_BossSkill02.fbx"	,"Boss");

	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/Boss/BossChip.fbx"	,"BossBullet");


	//EnemyA
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyA/EnemyA_EnemyAMove.fbx",	"EnemyA");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyA/EnemyA_EnemyAAttack1.fbx", "EnemyA");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyA/EnemyA_EnemyAAttack2.fbx", "EnemyA");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyA/EnemyA_EnemyADead.fbx",	"EnemyA");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyA/EnemyA_Weapon.fbx",		"EnemyA_Weapon");
	//EnemyB
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyB/EnemyB_EnemyBMove.fbx",	"EnemyB");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyB/EnemyB_EnemyBAttack2.fbx", "EnemyB");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyB/EnemyB_EnemyBAttack1.fbx", "EnemyB");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyB/EnemyB_GuardWalk.fbx",		"EnemyB");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyB/EnemyB_EnemyBDead.fbx",	"EnemyB");
	//EnemyC
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyC/EnemyC_EnemyCMove.fbx",	"EnemyC");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyC/EnemyC_EnemyCAttack.fbx",	"EnemyC");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyC/EnemyC_EnemyCReload.fbx",	"EnemyC");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyC/EnemyC_EnemyCDead.fbx",	"EnemyC");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyC/EnemyC_EnemyCTarget.fbx",	"EnemyC");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Enemy/EnemyC/EnemyC_Weapon.fbx",		"EnemyC_Weapon");

	//플레이어
	mFactory->SetResourece(LOAD_TYPE::FBX, "Player/Player_PlayerRun.fbx",			"Player");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Player/Player_PlayerRoll.fbx",			"Player");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Player/Player_PlayerAttack.fbx",		"Player");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Player/Player_PlayerReloadIdle.fbx",	"Player");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Player/Player_PlayerIdle.fbx",			"Player");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Player/Player_PlayerDead.fbx",			"Player");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Player/Player_PlayerSkill1.fbx",		"Player");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Player/Player_PlayerSkill2.fbx",		"Player");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Player/Player_PlayerReloadRun.fbx",		"Player");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Player/Pistol.fbx",						"Pistol");


	///스테이지 1
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_carpet_L_lower.fbx", "carpet_L_lower");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_carpet_L_upper.fbx", "carpet_L_upper");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_carpet_R_lower.fbx", "carpet_R_lower");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_cashier.fbx", "cashier");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_door_t1.fbx", "door_t1");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_enterance_01.fbx", "enterance_01");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_enterance_02.fbx", "enterance_02");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_enterance_03.fbx", "enterance_03");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_floor.fbx", "floor");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_piller_L.fbx", "piller_L");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_piller_M.fbx", "piller_M");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_piller_S.fbx", "piller_S");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_table_long_lower.fbx", "table_long_lower");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_table_long_upper.fbx", "table_long_upper");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_table_round_1-1.fbx", "table_round_1-1");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_table_round_1-2.fbx", "table_round_1-2");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_table_round_1-3.fbx", "table_round_1-3");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_table_round_1-4.fbx", "table_round_1-4");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_table_round_2-1.fbx", "table_round_2-1");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_table_round_2-2.fbx", "table_round_2-2");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_table_round_2-3.fbx", "table_round_2-3");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_table_round_2-4.fbx", "table_round_2-4");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_wall_L_lower.fbx", "wall_L_lower");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_wall_L_upper_t2.fbx", "wall_L_upper");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_wall_R_lower.fbx", "wall_R_lower");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage01/1s_wall_R_upper.fbx", "wall_R_upper");


	///스테이지 2
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_basicboard.fbx", "2s_basicboard");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_billboard.fbx",	"2s_billboard");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_Dia.fbx",		"2s_Dia");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_door.fbx",		"2s_door");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_enterance.fbx",	"2s_enterance");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_floor.fbx",		"2s_floor");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_pillar_R.fbx",		"2s_pillar_R");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_pillar_L.fbx",		"2s_pillar_L");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_pillar_seastory.fbx", "2s_pillar_seastory");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_scoreboard.fbx",		"2s_scoreboard");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_shark.fbx",			"2s_shark");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_slot_basic.fbx",		"2s_slot_basic");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_slot_basic_7_L.fbx", "2s_slot_basic_7_L");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_slot_basic_7_R.fbx", "2s_slot_basic_7_R");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_slot_round.fbx",		"2s_slot_round");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_slot_seastory.fbx",	"2s_slot_seastory");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_wall_L.fbx",			"2s_wall_L");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_wall_lower.fbx",		"2s_wall_lower");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_wall_R.fbx",			"2s_wall_R");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_wall_upper.fbx",		"2s_wall_upper");



	///보너스룸
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_carpet.fbx", "Reward_carpet");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_door.fbx", "Reward_door");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_enterance.fbx", "Reward_enterance");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_floor.fbx", "Reward_floor");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_Pillar_L_lower.fbx", "Reward_Pillar_L_lower");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_Pillar_R_upper.fbx", "Reward_Pillar_R_upper");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_threshold.fbx", "thresholdr");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_UI_pannel_Center.fbx", "UI_pannel_Center");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_UI_pannel_L.fbx", "UI_pannel_L");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_wall_high.fbx", "wall_high");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_wall_low.fbx", "wall_low");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_wood_long.fbx", "wood_long");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Reward_wood_short.fbx", "wood_short");

	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/bullet.fbx", "BonusRoom_bullet");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/heart.fbx", "BonusRoom_heart");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/plus_6.fbx", "BonusRoom_6");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/plus_10.fbx", "BonusRoom_10");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/Rolling.fbx", "BonusRoom_Rolling");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/acc.fbx", "BonusRoom_acc");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/chip.fbx", "BonusRoom_chip");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/jocker.fbx", "BonusRoom_jocker");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/specialslot.fbx",	"BonusRoom_specialslot");
	mFactory->SetResourece(LOAD_TYPE::FBX, "BonusRoom/lever.fbx",	"BonusRoom_lever");





	//텍스쳐 로드
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_carpet_L_lower_basecolor.dds", "carpet_L_lower");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_carpet_L_upper_basecolor.dds", "carpet_L_upper");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_carpet_R_lower_basecolor.dds", "carpet_R_lower");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_cashier_light_Real.dds", "cashier_light_Real");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_door_basecolor.dds", "door");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_enterance_01_light.dds", "enterance_01");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_enterance_02.dds", "enterance_02");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_enterance_03.dds", "enterance_03");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/floor_basecolor.dds", "floor");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_piller_L_basecolor.dds", "piller_L");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_piller_M_basecolor.dds", "piller_M");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_piller_S_basecolor.dds", "piller_S");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_table_long_lower_basecolor.dds", "table_long_lower");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_table_long_upper_basecolor.dds", "table_long_upper");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_table_round_1-1.dds", "table_round_1-1");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_table_round_1-2.dds", "table_round_1-2");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_table_round_1-3.dds", "table_round_1-3");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_table_round_1-4.dds", "table_round_1-4");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_table_round_2-1.dds", "table_round_2-1");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_table_round_2-2.dds", "table_round_2-2");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_table_round_2-3.dds", "table_round_2-3");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/1s_table_round_2-4.dds", "table_round_2-4");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/wall_L_lower_T1.dds", "wall_L_lower");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/wall_L_upper_t1_basecolor.dds", "wall_L_upper");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/wall_R_lower_basecolor.dds", "wall_R_lower");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/wall_R_upper_t1.dds", "wall_R_upper");


	//보너스룸텍스쳐
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_carpet_T.dds", "Reward_carpet");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_door_T.dds", "Reward_door");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_enterance_T.dds", "Reward_enterance");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_floor_T.dds", "Reward_floor");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_Pillar_L_lower_T.dds", "Reward_Pillar_L_lower");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_Pillar_R_upper_T.dds", "Reward_Pillar_R_upper");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_threshold_T.dds", "Reward_threshold");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_UI_pannel_T.dds", "Reward_UI_pannel");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_wall_high_T.dds", "Reward_wall_high");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_wall_low_T.dds", "Reward_wall_low");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_wood_long_T.dds", "Reward_wood_long");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Reward_wood_Short_T.dds", "Reward_wood_Short");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/acc_T.dds", "Reward_acc");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/bullet_T.dds", "Reward_bullet");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/chip_T.dds", "Reward_chip");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/heart_T.dds", "Reward_heart");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/jocker_T.dds", "Reward_jocker");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/lever_T.dds", "Reward_lever");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/Rolling_T.dds", "Reward_Rolling");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Bonus/specialslot_T.dds", "Reward_specialslot");
	
	
	//튜토리얼텍스쳐
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Tutorial/Tutorial_Carpet_T.dds",			"Tutorial_Carpet");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Tutorial/Tutorial_door_T.dds",				"Tutorial_door");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Tutorial/Tutorial_enterance_T.dds",		"Tutorial_enterance");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Tutorial/Tutorial_floor_T.dds",			"Tutorial_floor");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Tutorial/Tutorial_Pillar_L_lower_T.dds",	"Tutorial_Pillar_L_lower");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Tutorial/Tutorial_Pillar_R_upper_T.dds",	"Tutorial_Pillar_R_upper");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Tutorial/Tutorial_threshold_T.dds",		"Tutorial_threshold");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Tutorial/Tutorial_wall_high_T.dds",		"Tutorial_wall_high");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Tutorial/Tutorial_wall_low_T.dds",			"Tutorial_wall_low");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Tutorial/Tutorial_Wood_long_T.dds",		"Tutorial_Wood_long");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Tutorial/Tutorial_Wood_short_T.dds",		"Tutorial_Wood_short");



	//2스테이지 텍스쳐
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_basicboard_T.dds",		"2s_basicboard");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_billboard_T.dds",		"2s_billboard");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_Dia_T.dds",				"2s_Dia");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_door_T.dds",			"2s_door");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_floor_T.dds",			"2s_floor");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_enterance_T.dds",		"2s_enterance");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_pillar_L_T.dds",		"2s_pillar_L");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_pillar_R_T.dds",		"2s_pillar_R");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_pillar_seastory_T.dds", "2s_pillar_seastory");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_scoreboard_T.dds",		"2s_scoreboard");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_shark_T.dds",			"2s_shark");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_slot_basic_T.dds",		"2s_slot_basic");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_slot_round_T.dds",		"2s_slot_round");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_slot_seastory_T.dds",	"2s_slot_seastory");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_wall_L_T.dds",			"2s_wall_L");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_wall_lower_T.dds",		"2s_wall_lower");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_wall_R.dds",			"2s_wall_R");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage02/2s_wall_upper_T.dds",		"2s_wall_upper");











	//임팩트들
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/Back.dds", "Back");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Effect/Smoke1.dds", "Smoke");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Effect/sparkle.dds", "sparkle");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Effect/star01.dds", "star");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Effect/Shoot.dds", "Shoot");

	//나머지들
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "white.dds", "white");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Player_Pistol3.dds", "Pistol2");

	//AI 텍스쳐
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Enemy/enemy_b.dds", "EnemyB");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Enemy/enemy_c.dds", "EnemyC");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Enemy/enemy_boss.dds", "Boss");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Player.dds", "Player");



	//이미지폰트
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "ImgFont/0.dds", "0");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "ImgFont/1.dds", "1");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "ImgFont/2.dds", "2");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "ImgFont/3.dds", "3");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "ImgFont/4.dds", "4");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "ImgFont/5.dds", "5");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "ImgFont/6.dds", "6");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "ImgFont/7.dds", "7");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "ImgFont/8.dds", "8");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "ImgFont/9.dds", "9");


	ScreenManager::MoveScreen("Tutorial");
}
