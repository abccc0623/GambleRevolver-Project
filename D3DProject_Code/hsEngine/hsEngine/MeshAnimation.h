#pragma once
///��Ű�� �Ž��� �������Ű�� �ִϸ��̼��� �����Ҽ��ֵ����ϴ� Ŭ����
class hsBone;
class MeshAnimation
{
public:
	MeshAnimation();
	~MeshAnimation();
	void init(hsEngine* ENGINE);

	//��Ű�� �Ž��� �������� ���ִϸ��̼��������� ����ִ� ������
	void inputTotalAnimeData(string Name,LoadAnimation* Anime);
	
	//����� �ִϸ��̼��� ������
	bool ChoiceAnimation(string Name);

	//�ִϸ��̼��� ã�´�
	LoadAnimation* FindAnimation(string Name);

	//�ڽĵ��� �ִϸ��̼������� �����´�
	hsAnimation* GetChildAnimation(string ChildName);

	hsBone* FindBone(string Name);

	//���� ������ �޾ƿ�
	void SetChildBoneList(map<string, hsBone*>* m_childList);

	//���õ� �ִϸ��̼� �����ӽð�����
	int PlayAnimation(string Name,float speed,bool Loop = true);

	//�� ����
	int GetEndFrame();
	
private:
	string Name;

	//�ִϸ��̼� ī��Ʈ
	float AnimeCount;
	//�ִϸ��̼� Ÿ��
	float animeTime;

	int AnimeFrame = 0;
	//���� ������
	int StartFrame;

	//��������
	int EndFrame;

	//���ǵ�
	int Speed;

	//�������Ӵ���
	int OneFrame;

	//��ü ������
	int TotalFrame;



	//���� ������ �ִϸ��̼�
	LoadAnimation* Choice;
	string ChoiceName;

	//�ִϸ��̼ǵ��� ��� �ִϸ��̼� ����Ʈ
	map<string, LoadAnimation*> AnimationList;
	//�ڽĵ��� �ִϸ��̼� ����Ʈ
	list<hsAnimation*>* ChildAnimationList;

	//�ڽĵ��� ����
	map<string, hsBone*>* ChildList;

	hsEngine* ENGINE;
};