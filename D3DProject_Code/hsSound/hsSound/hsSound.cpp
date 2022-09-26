// hsSound.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "SOUND_pch.h"
#include <iostream>
hsSound* hsSound::instance = 0;
hsSound* hsSound::GM()
{
	if (instance == nullptr)
	{
		instance = new hsSound();
	}

	return instance;
}

void hsSound::init()
{
	FMOD::System_Create(&g_System);
	g_System->init(100, FMOD_INIT_NORMAL, nullptr);

	//채널 그룹을 만든다
	g_System->createChannelGroup("EX", &EX_CH_Group);
	g_System->createChannelGroup("BGM", &BGM_CH_Group);

	//마스터 그룹을 만든다
	g_System->getMasterChannelGroup(&Master);


	//마스터그룹에 채널을 등록시킨다
	Master->addGroup(EX_CH_Group);
	Master->addGroup(BGM_CH_Group);


	//모든 채널들을 그룹에 등록시킴
	BGM_CH->setChannelGroup(BGM_CH_Group);
	for (int i = 0; i < 6; i++)
	{
		EX_CH[i]->setChannelGroup(EX_CH_Group);
	}
}

//배경음은 무조건 무한으로 재생
void hsSound::LoadSound_BGM(string path, string _Key)                     
{
	//STL::Map 에 사운드를 넣어놓는다
	Sound* _sound;
	//Channel* ch;

	g_System->createSound(path.c_str(), FMOD_LOOP_NORMAL, nullptr, &_sound);


	//사운드와 채널을 같은키로 넣는다
	SoundList.insert({ _Key, _sound });
}

//사운드로드
void hsSound::LoadSound_EX(string path, string _Key, bool _loop)
{
	//새로운 사운드를 만들고 등록
	Sound* sd;

	if (_loop == false)
	{
		g_System->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &sd);
	}
	else
	{
		g_System->createSound(path.c_str(), FMOD_LOOP_NORMAL, nullptr, &sd);
	}


	//사운드와 채널을 같은키로 넣는다
	SoundList.insert({ _Key, sd });
}


//사운드를 BGM 채널로 실행시켜준다
//실패하면 리턴
void hsSound::Play_BGMSound(string _key)
{
	bool play;

	//이미 다른 BGM 채널에 재생중이라면 
	if (BGM_CH->isPlaying(&play) == true)
	{
		BGM_CH->stop();
	}
	
	//실패할경우
	if (g_System->playSound(FMOD_CHANNEL_REUSE, SoundList[_key], false, &BGM_CH) != FMOD_OK)
	{
		return;
	}

	if (BGM_CH != nullptr)
	{
		BGM_CH->setVolume(BGM_VOLUME);
		BGM_CH->setChannelGroup(BGM_CH_Group);
	}
}

//사운드를 EX채널로 실행시켜준다
//실패하면 리턴
void hsSound::Play_EXSound(string _key)
{
	//현재 사용하지 않는 채널을 가져옴
	Channel* temp = FindChannel();


	//실패할경우
	if (g_System->playSound(FMOD_CHANNEL_REUSE, SoundList[_key], false, &temp) != FMOD_OK)
	{
		return;
	}

	if (temp != nullptr)
	{
		EX_CH_Group->setVolume(EX_VOLUME);
		temp->setChannelGroup(EX_CH_Group);
	}
}


//BGM 채널에 원하는 볼륨크기 넣기
void hsSound::Set_BGM_Volume(float _volume)
{
	if (_volume < 0 && _volume >1) { return; }

	BGM_VOLUME = _volume;
	BGM_CH_Group->setVolume(BGM_VOLUME);
}

//사운드 증가
void hsSound::Set_BGM_VolumeUP()
{
	BGM_VOLUME += 0.1f;
	BGM_CH_Group->setVolume(BGM_VOLUME);
	if (BGM_VOLUME > 1) { BGM_VOLUME = 1; }
}

//사운드 감소
void hsSound::Set_BGM_VolumeDown()
{
	BGM_VOLUME -= 0.1f;
	BGM_CH_Group->setVolume(BGM_VOLUME);
	if (BGM_VOLUME < 0) { BGM_VOLUME = 0; }
}

//EX채널에 원하는 볼륨크기 넣기
void hsSound::Set_EX_Volume(float _volume)
{
	if (_volume < 0 && _volume >1) { return; }

	EX_VOLUME = _volume;
}

//효과음 사운드 증가
void hsSound::Set_EX_VolumeUP()
{
	EX_VOLUME += 0.1f;

	if (EX_VOLUME > 1) { EX_VOLUME = 1; }
}

//효과음 사운드 감소
void hsSound::Set_EX_VolumeDown()
{
	EX_VOLUME -= 0.1f;

	if (EX_VOLUME < 0) { EX_VOLUME = 0; }
}

//모든사운드 볼륨 조절
void hsSound::Set_MASTER_Volume(float _volume)
{
	if (_volume < 0 && _volume >1) { return; }

	MASTER_VOLUME = _volume;
	Master->setVolume(MASTER_VOLUME);
}

//모든 사운드 증가
void hsSound::Set_MASTER_VolumeUP()
{
	MASTER_VOLUME += 0.1f;
	Master->setVolume(MASTER_VOLUME);

	if (MASTER_VOLUME > 1) { MASTER_VOLUME = 1; }
}

//모든 사운드 감소
void hsSound::Set_MASTER_VolumeDOWN()
{
	MASTER_VOLUME -= 0.1f;
	Master->setVolume(MASTER_VOLUME);

	
	if (MASTER_VOLUME < 0) { MASTER_VOLUME = 0; }
}

//BGM 멈춤 (이후 재생시 처음부터 재생)
void hsSound::Stop_BGM()
{
	//모든 BGM 재생멈춤
	if (BGM_CH_Group != nullptr)
	{
		BGM_CH_Group->stop();
	}
	BGM_CH->stop();
}

//일시중지 여부
void hsSound::Paused_BGM(bool _play)
{
	if (BGM_CH_Group != nullptr)
	{
		BGM_CH_Group->setPaused(_play);
	}
}

//모든 효과음 멈춤
void hsSound::Stop_EX()
{
	if (EX_CH_Group != nullptr)
	{
		EX_CH_Group->stop();
	}
}

//모든 효과음 일시정지
void hsSound::Paused_EX(bool _play)
{
	if (EX_CH_Group != nullptr)
	{
		EX_CH_Group->setPaused(_play);
	}
}

//모든사운드 멈춤
void hsSound::Stop_ALL()
{
	//모든 사운드 멈춤
	if (Master != nullptr)
	{
		Master->stop();
	}
}


void hsSound::Updata()
{
	g_System->update();
	EX_CH_Group->setVolume(EX_VOLUME);
}

void hsSound::Sound2D_Update(float _x,float _y)
{
	POS_X = _x;
	POS_Y = _y;
	std::cout << "플레이어 현재위치"<< "x:"<< POS_X<<"y:"<<POS_Y<< std::endl;
}

void hsSound::Play_Sound2D(string _key, float _x, float _y)
{
	//현재 사용하지 않는 채널을 가져옴
	Channel* temp = FindChannel();


	float X = POS_X - _x;
	float Y = POS_Y - _y;

	float dir = sqrt(pow(X, 2) + pow(Y, 2));


	//사정거리 밖으로 나갈시에 소리를재생하지않음
 	if(dir > Max_Range)
	{
		return;
	}

	cout <<"플레이어와 나와의 거리:"<<dir << endl;

	float Percent = 1 - (dir / Max_Range) ;

	cout <<"플레이어와 나와의 거리 퍼센트:"<< Percent << endl;





	//실패할경우
	if (g_System->playSound(FMOD_CHANNEL_REUSE, SoundList[_key], false, &temp) != FMOD_OK)
	{
		return;
	}
	EX_CH_Group->setVolume(Percent);
	temp->setChannelGroup(EX_CH_Group);
}

void hsSound::Sound2D_Max_Dis(float _dis)
{
	Max_Range = _dis;
}

Channel* hsSound::FindChannel()
{
	for (int i = 0; i < 6; i++)
	{
		bool play;
		EX_CH[i]->isPlaying(&play);

		//재생중인 채널이 아니면 채널을 가져옴
		if (play != true)
		{
			return EX_CH[i];
		}
	}
}

hsSound::hsSound()
{
	//초기화
	g_System = nullptr;
	EX_CH_Group = nullptr;
	BGM_CH_Group = nullptr;
	Master = nullptr;

	BGM_CH = nullptr;
	for (int i = 0; i < 7; i++)
	{
		EX_CH[i] = nullptr;
	}
	POS_X = 0;
	POS_Y = 0;

	BGM_VOLUME = 1;
	EX_VOLUME = 1;
	MASTER_VOLUME = 1;
	Max_Range = 10;
}

hsSound::~hsSound()
{
	if (BGM_CH_Group != nullptr)
	{
		BGM_CH->stop();
		BGM_CH_Group->release();
	}

	if (EX_CH_Group != nullptr)
	{
		for (int i = 0; i < 6; i++)
		{
			EX_CH[i]->stop();
			EX_CH_Group->release();
		}
	}

	map<string, Sound*>::iterator it = SoundList.begin();
	for (it; it == SoundList.end(); it++)
	{
		delete &it;
	}

	g_System->release();
	g_System->close();
}
