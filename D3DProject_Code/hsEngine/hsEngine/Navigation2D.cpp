#include "EnginePCH.h"
#include "Navigation2D.h"


vector<vector<int>> Navigation2D::NV_Map;

int Navigation2D::MaxSizeX = 0;
int Navigation2D::MaxSizeY = 0;
Navigation2D::Navigation2D()
{
	StartPointX = 0;
	StartPointY = 0;

	EndPointX = 0;
	EndPointY = 0;

	NowPointX = 0;
	NowPointY = 0;

	CreateNode();
}

Navigation2D::~Navigation2D()
{
	for (int i = 0; i < NodePool.size(); i++)
	{
		delete NodePool[i];
	}
	NodePool.clear();
	OpenNode.clear();
	CloseNode.clear();
	FastRoad.clear();
}

void Navigation2D::ClearMap()
{
	NV_Map.clear();
}

void Navigation2D::CreateMapSize(int SizeX, int SizeY ,bool Side)
{
	//2차원 배열을 만듬
	NV_Map.resize(SizeX);
	for (int i = 0; i < SizeX; i++)
	{
		NV_Map.resize(SizeX);
		for (int j = 0; j <= SizeY; j++)
		{
			NV_Map[i].resize(SizeY);
		}
	}

	//외각선들을 모두 벽으로 세운다
	if (Side == true)
	{
		for (int i = 0; i<= SizeX - 1; i++)
		{
			NV_Map[i][0] = 1;
			NV_Map[i][SizeY - 1] = 1;
		}
	
		for (int j = 0; j< SizeY - 1; j++)
		{
			NV_Map[0][j] = 1;
			NV_Map[SizeX -1][j] = 1;
		}
	}



	MaxSizeX = SizeX;
	MaxSizeY = SizeY;
}

void Navigation2D::FindOpenNode()
{
	int x = NowPointX;
	int y = NowPointY;


	//플레이어 기준으로 방향4개의 방향의 노드를 조사하고 Open노드에 넣어준다
	CheckNode(x, y + 1, 10); //플레이어의 윗방향 조사
	CheckNode(x, y - 1, 10); //플레이어의 아래방향 조사
	CheckNode(x + 1, y, 10); //플레이어의 오른쪽 방향 조사
	CheckNode(x - 1, y, 10); //플레이어의 왼쪽 방향 조사

	
	CheckNode(x + 1, y + 1, 14); //플레이어의 오른쪽 위 대각선
	CheckNode(x + 1, y - 1, 14); //플레이어의 오른쪽 아래 대각선
	CheckNode(x - 1, y + 1, 14); //플레이어의 왼쪽 위 대각선
	CheckNode(x - 1, y - 1, 14); //플레이어의 왼쪽 아래 대각선
}

void Navigation2D::SearchNode()
{

	int MinNodeIndex = -1;
	int MinNodeF = 10000;


	//더이상 길이없다...
	if(OpenNode.size() == 0)
	{
		return;
	}

	//오픈노드중에서 값을 비교에서 가장 최소비용의 노드를찾는다
	for (int i = 0; i < OpenNode.size(); i++)
	{
		if (OpenNode[i] == nullptr) { continue; }

		if (MinNodeF > OpenNode[i]->H)
		{
			MinNodeF = OpenNode[i]->H;
			MinNodeIndex = i;
		}
	}
	
	//가장 최소비용의 노드를 찾았다
	Node* mNode = OpenNode[MinNodeIndex];
	//찾은 노드는 검색을 완료했기때문에 close노드로 넣어주고 오픈노드에서는 삭제시킨다
	CloseNode.push_back(NowNode);

	//그리고 시작위치를 변경해준다
	NowPointX = mNode->POS_X;
	NowPointY = mNode->POS_Y;

	NowNode = mNode;
	OpenNode[MinNodeIndex] = nullptr;
}

bool Navigation2D::GetDistance(XMFLOAT3 mPos,float mdir)
{
	//처음 경로의 거리를 0~1까지의 거리로 나타내고 mdir지점의 값을 가져온다
	float x = EndPointX - StartPointX;
	float y = EndPointY - (-StartPointY);
	float d = sqrt((x * x) + (y * y));
	float Start = MathHelper::Lerp<float>(0, d, mdir);

	//현재 나의 위치에서 끝지점에서의 거리
	float MYx = EndPointX - mPos.x;
	float MYy = EndPointY - (-mPos.z);
	float MYd = sqrt((MYx * MYx) + (MYy * MYy));

	//경로를 이동하다가 조건을둔 거리보다 짧아지면 true 리턴
	if (MYd <= Start)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Navigation2D::ChangeFloatMapPoint(int& X, int& Y, XMFLOAT2 point)
{
	//음수가 들어올수 있기때문에 절대값으로 변경
	float mx = abs(point.x);
	float my = abs(point.y);

	
	//배열 선언한곳보다 멀리 배치를하게되면 배치하지않는다
	if (mx < 0 || mx >= MaxSizeX || my < 0 || my >= MaxSizeY)
	{
		return false;
	}

	//들어온값을 반올림처리해줌
	X = (int)floor(mx + 0.5f);
	Y = (int)floor(my + 0.5f);

	return true;
}

Node* Navigation2D::GetNode()
{
	//노드풀 안에 노드가 부족할경우 10개늘림
	if (GetNodeIndex == NodePool.size() - 1)
	{
		for (int i = 0; i < 10; i++)
		{
			NodePool.push_back(new Node);
		}
	}

	Node* mNode = NodePool[GetNodeIndex];
	mNode->F = 0;
	mNode->G = 0;
	mNode->H = 0;
	mNode->ParentNode = nullptr;
	mNode->POS_X = 0;
	mNode->POS_Y = 0;

	GetNodeIndex++;
	return mNode;
}

void Navigation2D::CreateNode()
{
	//노드들을 관리할 노드풀을 만든다
	for (int i = 0; i < 20; i++)
	{
		NodePool.push_back(new Node);
	}
	GetNodeIndex = 0;
}

vector<XMFLOAT2>* Navigation2D::StartNavigation()
{
	reset = false;
	//다시길을찾을때 초기화
	DataClear();

	NowNode = GetNode();
	NowNode->ParentNode = nullptr;
	NowNode->POS_X = StartPointX;
	NowNode->POS_Y = StartPointY;

	NowPointX = StartPointX;
	NowPointY = StartPointY;

	//최단 경로를 찾는다
	while(true)
	{
		if (NowPointX == EndPointX && NowPointY == EndPointY)
		{
			break;
		}

		//일단 내주위를 조사해서 오픈노드를 생성한다
		FindOpenNode();

		//오픈노드에서 내가 갈곳을찾고 그노드로 이동한다
		SearchNode();
	}

	//찾은 노드들로부터 위치값을 뽑아낸다
	while (true)
	{
		if (NowNode == nullptr)
		{
			break;
		}

		FastRoad.push_back(XMFLOAT2(NowNode->POS_X, NowNode->POS_Y));
		NowNode = NowNode->ParentNode;
	}

	//뽑은 위치값을 역순으로 변경해준다 (부모노드부터 처음노드까지 Pos값을 읽어오는데 이것을 역순으로시키면 최단경로)
	std::reverse(FastRoad.begin(), FastRoad.end());


	//다음을 위해 리스트들 클리어
	OpenNode.clear();
	CloseNode.clear();
	GetNodeIndex = 0;

	return &FastRoad;
}

bool Navigation2D::CheckNode(int x, int y, int Cost)
{
	///오픈노드에 넣을수있는 노드인지 확인한다

	//x좌표,y 좌표가  배열의 범위를 넘었다
	if (x < 0 || x >= MaxSizeX || y < 0 || x >= MaxSizeX)
	{
		return false;
	}

	//지나갈수없는 벽이다
	if (NV_Map[x][y] == 1)
	{
		return false;
	}

	//대각선을 체크할때
	if (Cost == 14)
	{
		int dirx = x - NowNode->POS_X;
		int diry = y - NowNode->POS_Y;

		//대각선방향의 가로세로의 오브젝트가있다면 못가는곳
		bool x = FindObject(NowNode->POS_X+ dirx	, NowNode->POS_Y);
		bool y = FindObject(NowNode->POS_X		, NowNode->POS_Y + diry);
		if (x == true || y == true) { return false; }
	}

	for (int i = 0; i < CloseNode.size(); i++)
	{
		//Close노드에 같은노드가 있다면
		if (CloseNode[i]->POS_X == x && CloseNode[i]->POS_Y == y)
		{
			return false;
		}
	}

	Node* mNode = GetNode();
	//위치넣어줌
	mNode->POS_X = x;
	mNode->POS_Y = y;

	//부모지정
	mNode->ParentNode = NowNode;

	//노드의 위치에서 나의 위치를 뺀다
	int mx = abs(mNode->POS_X - StartPointX);
	int my = abs(mNode->POS_Y - StartPointY);
	//이것이 현재 나의 이동 비용
	mNode->G = mNode->ParentNode->G + Cost;

	//현재 노드위치에서 끝지점위치를뺀다
	mx = abs( EndPointX - mNode->POS_X);
	my = abs( EndPointY - mNode->POS_Y);

	//현재위치에서 끝지점까지의 비용
	mNode->H = (int)sqrt( pow(mx, 2) + pow(my, 2)) * 14;

	//최종비용
	mNode->F = mNode->G + mNode->H;


	//계산 한 노드를 오픈노드에 넣는다
	OpenNode.push_back(mNode);


	//모두 아니라면 갈수있는 지역이다
	return true;
}



void Navigation2D::SetStartPoint(float x, float y)
{
	//시작지점을 넣어줌
	ChangeFloatMapPoint(StartPointX, StartPointY, XMFLOAT2(x, y));
}

void Navigation2D::SetEndPoint(float x, float y)
{
	//끝지점을 넣어줌
	ChangeFloatMapPoint(EndPointX, EndPointY, XMFLOAT2(x, y));
}

bool  Navigation2D::NavigationMove(XMFLOAT3& Position, XMFLOAT3& Rot, float speed, float Distance)
{
	///찾은 길로 천천히 이동도하고 회전도하면서 이동한다

	//길을다찾고 다시 경로를지정해주지않으면 이동하지않는다
	if (FastRoad.size() == 0) { return true;}


	if (FastRoadindex > FastRoad.size() - 1 || reset == true)
	{
		//이제 이경로는 필요없음
		DataClear();
		//목적지에 도착
		return true;
	}


	//도착지점
	XMFLOAT3 Target = { 0,0,0 };
	Target.x = FastRoad[FastRoadindex].x;
	Target.y = 0;
	Target.z = -FastRoad[FastRoadindex].y;
	
	//현재 지점
	XMFLOAT3 startpoint = { 0,0,0 };
	startpoint.x = Position.x;
	startpoint.y = 0;
	startpoint.z = Position.z;
	
	//일단 방향을 구한다
	XMFLOAT3 point = startpoint - Target;
	
	//방향벡터의 크기를 구한다
	float nomal = sqrt(pow(point.x, 2) + pow(point.z, 2));
	
	
	///지점으로 회전
	//현재 지점 벡터
	XMFLOAT2 startvec = { Position.x, Position.z };
	//가야하는 곳 벡터
	XMFLOAT2 Endvec = { FastRoad[FastRoadindex].x,-FastRoad[FastRoadindex].y };

	//원점으로부터의 가야하는곳의 벡터를 구함
	float X = FastRoad[FastRoadindex].x - Position.x ;
	float Y = -FastRoad[FastRoadindex].y - Position.z;

	//그곳의 각도를 구함
	float angle = -atan2(Y, X) * 180 / MathHelper::Pi;
	//각도설정 처음캐릭터는 x축으로 돌아가있고 -z방향을 보고있기떄문에 90도를 빼준다 (오른쪽을바라보고있도록하기위해)
	Rot = { Rot.x,Rot.y,90 - angle };



	//목표에 가까워질수록 벡터의크기는줄어들것이고 그값이 프레임시간보다 작을경우
	//다음목적지로 이동한다
	if (nomal >= speed)
	{
		XMFLOAT3 dir = { -point.x,0,-point.z };
		dir.x /= nomal;
		dir.y /= nomal;
		dir.z /= nomal;
	
		Position.x += (dir.x * speed);
		Position.y += (dir.y * speed);
		Position.z += (dir.z * speed);	
		reset = GetDistance(Position, Distance);
	}
	else 
	{
		FastRoadindex++;
		//시작지점 벡터
	}
	
	//계속 이동중
	return false;
}

void Navigation2D::DataClear()
{
	FastRoadindex = 1;
	GetNodeIndex = 0;
	FastRoad.clear();
}

void Navigation2D::SetObjectSetting(XMFLOAT3 Pos)
{
	///오브젝트를 설정한다 설정되면 이곳을 못지나가는지역이된다

	int ArrayX = 0;
	int ArrayY = 0;
	ChangeFloatMapPoint(ArrayX, ArrayY, XMFLOAT2(Pos.x, Pos.z));

	//이제 배열에 넣는다
	NV_Map[ArrayX][ArrayY] = 1;
}

void Navigation2D::SetObjectSetting(XMFLOAT2 pos, float sizeX, float sizeY)
{
	int ArrayX = 0;
	int ArrayY = 0;
	ChangeFloatMapPoint(ArrayX, ArrayY, XMFLOAT2(pos.x, pos.y));

	//이제 배열에 넣는다

	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			NV_Map[ArrayX + i][ArrayY + j] = 1;
		}
	}
}

void Navigation2D::SetObjectSetting_jump(XMFLOAT2 pos, float sizeX, float sizeY)
{
	int ArrayX = 0;
	int ArrayY = 0;
	ChangeFloatMapPoint(ArrayX, ArrayY, XMFLOAT2(pos.x, pos.y));

	//이제 배열에 넣는다
	
	for (int i = 0; i < sizeX; i++)
	{
		bool isjump = false;
		for (int j = 0; j < sizeY; j++)
		{
			//한칸씩 띄움
			if (isjump == true)
			{
				isjump = false;
				continue;
			}

			if (isjump == false)
			{
				NV_Map[ArrayX + i][ArrayY + j] = 1;
				isjump = true;
			}
		}
	}
}

bool Navigation2D::FindObject(float x, float y)
{
	///현재 확인하는 좌표에 벽이있는지없는 체크 있으면 true 없으면 false

	int pointX = 0;
	int pointY = 0;

	if (ChangeFloatMapPoint(pointX, pointY, XMFLOAT2(x, y))== false)
	{
		//벽이있는걸로 생각
		return false;
	}


	int temp = NV_Map[pointX][pointY];


	if (temp == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
