// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H


// 여기에 미리 컴파일하려는 헤더 추가

#include "EnumClass.h"


#include "SOUND_pch.h"
#include "../../hsEngine/hsEngine/EnginePCH.h"
#include "framework.h"

#include "SystemClass.h"
#include <map>
#include <vector>

#include "Bullet.h"
#include "Trigger.h"
#include "Player.h"
#include "EnemyB.h"
#include "EnemyC.h"
#include "EnemyA.h"
#include "Boss.h"
#include "Table.h"

#include "SlotMachine.h"
#include "HPBox.h"
#include "BulletBox.h"

#include "Tutorial.h"
#include "Stage01.h"
#include "Stage02.h"
#include "Stage03.h"
#include "Title.h"
#include "Loading.h"
#include "BonusRoom.h"
#include "GameOver.h"
#include "Win.h"
#include "CountFont.h"


#include "GameManager.h"
#include "StructData.h"
#endif //PCH_H
