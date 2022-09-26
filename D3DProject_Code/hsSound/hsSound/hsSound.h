#pragma once


/// <summary>
/// 기본적인 싱글톤 클래스 
/// </summary>
class hsSound
{

public:

	/// <summary>
	/// 싱글톤 클래스를 가져오는 함수
	/// </summary>
	/// <returns></returns>
	static hsSound* GM();

	/// <summary>
	/// 사운드 기본 초기화
	/// </summary>
	void init();

	/// <summary>
	/// BGM 사운드 로드
	/// </summary>
	/// <param name="path">사운드 파일경로</param>
	/// <param name="_Key">사운드 찾을때 사용할 키</param>
	void LoadSound_BGM(string path, string _Key);


	/// <summary> 
	/// 임팩트 사운드 로드
	/// </summary>
	/// <param name="path">사운드 파일 경로</param>
	/// <param name="_Key">사운드 키</param>
	void LoadSound_EX(string path, string _Key,bool _loop= false);



	/// <summary>
	/// 사운드 기본재생
	/// </summary>
	/// <param name="_key">사운드 키값</param>
	///  <param name="_loop">무한재생 여부</param>
	void Play_BGMSound(string _key);

	void Play_EXSound(string _key);

	/// <summary>
	/// BGM채널 볼륨 조절
	/// </summary>
	/// <param name="_volume">0~1 사이의 볼륨값</param>
	void Set_BGM_Volume(float _volume);

	/// <summary>
	/// BGM채널 사운드를 현재 사운드에서 0.1f 증가
	/// </summary>
	void Set_BGM_VolumeUP();

	/// <summary>
	///  BGM채널 사운드를 현재 사운드에서 0.1f 감소
	/// </summary>
	void Set_BGM_VolumeDown();


	/// <summary>
	/// EX채널 사운드 조절
	/// </summary>
	/// <param name="_volume">0~1 사이의 볼륨값</param>
	void Set_EX_Volume(float _volume);

	/// <summary>
	/// EX채널 사운드를 현재 사운드에서 0.1f 증가
	/// </summary>
	void Set_EX_VolumeUP();


	/// <summary>
	/// EX채널 사운드를 현재 사운드에서 0.1f 감소
	/// </summary>
	void Set_EX_VolumeDown();


	/// <summary>
	/// 모든 사운드의 볼륨을 
	/// </summary>
	/// <param name="_volume">0~1 사이의 볼륨</param>
	void Set_MASTER_Volume(float _volume);

	/// <summary>
	/// 마스터 볼륨의 사운드를 0.1f 증가
	/// </summary>
	void Set_MASTER_VolumeUP();

	/// <summary>
	/// 마스터 볼륨의 사운드를 0.1f 감소
	/// </summary>
	void Set_MASTER_VolumeDOWN();



	/// <summary>
	/// BGM 채널 멈춤
	/// </summary>
	void Stop_BGM();

	/// <summary>
	/// 일시중지
	/// </summary>
	void Paused_BGM(bool _play);


	/// <summary>
	/// EX 채널 멈춤 다음 재생시 처음부터
	/// </summary>
	void Stop_EX();

	/// <summary>
	/// EX 채널 일시정지
	/// </summary>
	/// <param name="_play">일시정지 여부</param>
	void Paused_EX(bool _play);



	/// <summary>
	/// 모든 채널 멈춤 다음 재생시 처음부터
	/// </summary>
	void Stop_ALL();


	/// <summary>
	/// 사운드관련 업데이트
	/// </summary>
	void Updata();


	void Sound2D_Update(float _x, float _y);
	void Play_Sound2D(string _key, float _x,float _y);
	void Sound2D_Max_Dis(float _dis);
private:
	/// <summary>
	/// 현재 재생중이 아닌 채널을 찾는다
	/// </summary>
	/// <returns></returns>
	Channel* FindChannel();


	static hsSound* instance;

	//사운드 시스템
	System* g_System;

	//사운드 리스트 
	map<string, Sound*>	SoundList;

	//채널들
	Channel* BGM_CH;
	Channel* EX_CH[7];


	//BGM 채널 리스트
	ChannelGroup* BGM_CH_Group;

	//효과음 채널 리스트
	ChannelGroup* EX_CH_Group;

	//마스터 채널 그룹
	ChannelGroup* Master;



	//BGM 사운드 볼륨
	float BGM_VOLUME;

	//효과음 사운드 볼륨
	float EX_VOLUME;

	//마스터 사운드볼륨
	float MASTER_VOLUME;

	//플레이어의 현재위치
	float POS_X;
	float POS_Y;

	//최대 사정거리
	float Max_Range;

	hsSound();
	~hsSound();
};