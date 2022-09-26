#include "EnginePCH.h"
#include "MeshAnimation.h"

MeshAnimation::MeshAnimation()
{
	Name		= "";
	StartFrame	= 0;
	EndFrame	= 0;
	Speed		= 0;
	OneFrame	= 0;
	TotalFrame	= 0;
}

MeshAnimation::~MeshAnimation()
{
	//std::list<hsAnimation*>::iterator it = (*ChildAnimationList).begin();
	//for (it; it != (*ChildAnimationList).end(); ++it)
	//{
	//	(*it) = nullptr;
	//}
	//
	//map<string, LoadAnimation*>::iterator i = AnimationList.begin();
	//for (i; i != AnimationList.end(); ++i)
	//{
	//	(*i).second = nullptr;
	//}
}

void MeshAnimation::init(hsEngine* m_ENGINE)
{
	ENGINE = m_ENGINE;
}

void MeshAnimation::inputTotalAnimeData(string Name,LoadAnimation* AnimeData)
{
	//�ִϸ��̼��� �־��ش�
	AnimationList.insert({ Name,AnimeData});


	//���� �������ִ� ������ �ִϸ��̼��� ����
	list<hsAnimation*>* ChildAnime = &(AnimeData->Anime);
	list<hsAnimation*>::iterator it = (*ChildAnime).begin();
	for (it; it != (*ChildAnime).end(); it++)
	{
		hsAnimation* tempAnime = (*it);
		hsBone* tempBone = FindBone( (*it)->m_nodename);

		if(tempBone == nullptr){continue;}

		tempBone->AnimationList.insert({Name,*it});
		tempBone->isAnime = true;
	}
	int num = 0;
}

bool MeshAnimation::ChoiceAnimation(string m_Name)
{
 	Choice = FindAnimation(m_Name);
	ChoiceName = m_Name;

	if (Choice == nullptr)
	{
		return false;
	}
	else
	{
		//������ ������ ���缭 �������� ����
		Name		= Choice->Name;
		StartFrame	= Choice->StartFrame;
		EndFrame	= Choice->EndFrame;
		OneFrame	= Choice->OneFrame;
		TotalFrame	= Choice->TotalFrame;
		animeTime	= Choice->OneFrameTime;

		map<string, hsBone*>::iterator it = (*ChildList).begin();
		for (it; it != (*ChildList).end(); it++)
		{
			//�ִϸ��̼ǵ��� ��� ó�����·�
			it->second->AnimationReset();
			animeTime = 0;
			AnimeCount = 0;
			//�ִϸ��̼��� ���������
			it->second->Anime = it->second->AnimationList[m_Name];
		}

		return true;
	}
}

LoadAnimation* MeshAnimation::FindAnimation(string Name)
{
	//�ִϸ��̼� ����Ʈ���� �ִϸ��̼��� ã�Ƽ� ��������
	LoadAnimation* data;
	map<string, LoadAnimation*>::iterator it = AnimationList.begin();
	for (it; it != AnimationList.end(); it++)
	{
		if (it->first == Name)
		{
			return it->second;
		}
	}
	return nullptr;
}

hsAnimation* MeshAnimation::GetChildAnimation(string ChildName)
{
	//�ڽ��� �̸����´� �ִϸ��̼������� ��������
	std::list<hsAnimation*>::iterator it = (*ChildAnimationList).begin();
	for (it; it != (*ChildAnimationList).end(); it++)
	{
		hsAnimation* temp = (*it);
		if (temp->m_nodename == ChildName)
		{
			return temp;
		}
	}
	return nullptr;
}

hsBone* MeshAnimation::FindBone(string Name)
{
	map<string, hsBone*>::iterator it =(*ChildList).begin();
	for (it; it != (*ChildList).end(); it++)
	{
		if (it->first == Name)
		{
			return it->second;
		}
	}

	return nullptr;
}

void MeshAnimation::SetChildBoneList(map<string, hsBone*>* m_childList)
{
	ChildList = m_childList;
}

int MeshAnimation::PlayAnimation(string Name, float speed, bool Loop)
{
	//���� ���õ� �ִϸ��̼ǰ� ����ڰ� �����ų �ִϸ��̼ǿ� �̸��� ����������
	//����ڰ� ������ �ִϸ��̼����� ���������
	if (ChoiceName != Name)
	{
		ChoiceAnimation(Name);
	}

	if (animeTime >= Speed)
	{
		AnimeCount += speed * ENGINE->GetDeltaTime();
		animeTime = 0;
		AnimeFrame++;
	}

	int BoneCount = 0;
	std::map<string, hsBone*>::iterator it = (*ChildList).begin();
	for (it; it != (*ChildList).end(); it++)
	{
		if (it->second->MeshAnimation(AnimeCount) == true)
		{
			BoneCount++;
		}
	}

	//���� ����Ϸ��� �ִϸ��̼��� ���� ���Ǽ����� ũ�ų� ������
	//�� ��� ������ �ڽ��� �������ִ� �ִϸ��̼� ������ ���ȴٸ� �ϳ��� �ִϸ��̼��� �� ���ȴ�
	if (BoneCount >= (*ChildList).size())
	{
		//�ݺ� ����ؾ��Ѵٸ�
		if (Loop == true)
		{
			AnimeCount = StartFrame;
			std::map<string, hsBone*>::iterator it = (*ChildList).begin();
			for (it; it != (*ChildList).end(); it++)
			{
				it->second->AnimationReset();
			}
		}
	}
	
	return AnimeCount;
}

int MeshAnimation::GetEndFrame()
{
	return EndFrame;
}




