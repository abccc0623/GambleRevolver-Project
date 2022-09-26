#pragma once
///스키닝 매쉬에 상속을시키면 애니메이션을 가동할수있도록하는 클래스
class hsBone;
class MeshAnimation
{
public:
	MeshAnimation();
	~MeshAnimation();
	void init(hsEngine* ENGINE);

	//스키닝 매쉬에 전달해줄 모든애니메이션정보들이 담겨있는 데이터
	void inputTotalAnimeData(string Name,LoadAnimation* Anime);
	
	//재생할 애니메이션을 선택함
	bool ChoiceAnimation(string Name);

	//애니메이션을 찾는다
	LoadAnimation* FindAnimation(string Name);

	//자식들의 애니메이션정보를 가져온다
	hsAnimation* GetChildAnimation(string ChildName);

	hsBone* FindBone(string Name);

	//본의 정보를 받아옴
	void SetChildBoneList(map<string, hsBone*>* m_childList);

	//선택된 애니메이션 프레임시간리턴
	int PlayAnimation(string Name,float speed,bool Loop = true);

	//끝 지점
	int GetEndFrame();
	
private:
	string Name;

	//애니메이션 카운트
	float AnimeCount;
	//애니메이션 타임
	float animeTime;

	int AnimeFrame = 0;
	//시작 프레임
	int StartFrame;

	//끝프레임
	int EndFrame;

	//스피드
	int Speed;

	//한프레임단위
	int OneFrame;

	//전체 프레임
	int TotalFrame;



	//현재 선택한 애니메이션
	LoadAnimation* Choice;
	string ChoiceName;

	//애니메이션들이 담길 애니메이션 리스트
	map<string, LoadAnimation*> AnimationList;
	//자식들의 애니메이션 리스트
	list<hsAnimation*>* ChildAnimationList;

	//자식들의 정보
	map<string, hsBone*>* ChildList;

	hsEngine* ENGINE;
};