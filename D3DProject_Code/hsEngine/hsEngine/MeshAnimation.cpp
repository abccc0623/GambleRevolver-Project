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
	//애니메이션을 넣어준다
	AnimationList.insert({ Name,AnimeData});


	//내가 가지고있는 본마다 애니메이션을 연결
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
		//선택한 정보에 맞춰서 변수들을 만듬
		Name		= Choice->Name;
		StartFrame	= Choice->StartFrame;
		EndFrame	= Choice->EndFrame;
		OneFrame	= Choice->OneFrame;
		TotalFrame	= Choice->TotalFrame;
		animeTime	= Choice->OneFrameTime;

		map<string, hsBone*>::iterator it = (*ChildList).begin();
		for (it; it != (*ChildList).end(); it++)
		{
			//애니메이션들을 모두 처음상태로
			it->second->AnimationReset();
			animeTime = 0;
			AnimeCount = 0;
			//애니메이션을 변경시켜줌
			it->second->Anime = it->second->AnimationList[m_Name];
		}

		return true;
	}
}

LoadAnimation* MeshAnimation::FindAnimation(string Name)
{
	//애니메이션 리스트에서 애니메이션을 찾아서 내보내줌
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
	//자식의 이름에맞는 애니메이션정보를 내보내줌
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
	//현재 선택된 애니메이션과 사용자가 재생시킬 애니메이션에 이름이 같지않으면
	//사용자가 선택한 애니메이션으로 변경시켜줌
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

	//만약 재생완료한 애니메이션의 수가 본의수보다 크거나 같으면
	//즉 모든 본들이 자신이 가지고있는 애니메이션 끝까지 돌렸다면 하나의 애니메이션을 다 돌렸다
	if (BoneCount >= (*ChildList).size())
	{
		//반복 재생해야한다면
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




