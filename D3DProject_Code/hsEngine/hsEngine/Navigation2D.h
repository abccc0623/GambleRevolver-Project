#pragma once
struct Node
{
	int POS_X;
	int POS_Y;

	//부모노드
	Node* ParentNode;

	//현재까지 이동한거리
	int G;
	//목표까지의 절때값 거리
	int H;
	//G+H의 값
	int F;
};

class Navigation2D
{
public:
	//거리 검사를 할때 필요한 노드
	Navigation2D();
	~Navigation2D();

	vector<XMFLOAT2>* StartNavigation();

	//시작지점 설정 (배열이기때문에 float형이 들어오면 따로 처리)
	void SetStartPoint(float x, float y);

	//끝지점 설정(배열이기때문에 float형이 들어오면 따로 처리)
	void SetEndPoint(float x, float y);

	//A*로 뽑은 위치값으로 천천히 이동시킨다
	//Distance = 0 끝지점 Distance = 1 처음지점 Distance = 0.5f 중간지점
	bool NavigationMove(XMFLOAT3& Position, XMFLOAT3& Rot, float speed,float Distance = 0);

	//데이터를 클리어해준다
	void DataClear();
public:
	///스테틱 함수들
	//맵배열을 초기화 시킨다
	static void ClearMap();

	//맵 사이즈를 만들어준다
	static void CreateMapSize(int SizeX, int SizeY,bool Side = true);

	//맵배열에 오브젝트를 넣어줌 그곳은 못가는곳이 될것임
	static void SetObjectSetting(XMFLOAT3 Pos);
	//맵배열에 오브젝트를 넣어줌 처음지점에 증가량까지
	static void SetObjectSetting(XMFLOAT2 pos,float sizeX,float sizeY);
	//맵배열에 오브젝트를 넣어줌 처음지점에 증가량까지 한칸씩떨어뜨림
	static void SetObjectSetting_jump(XMFLOAT2 pos, float sizeX, float sizeY);


	//좌표를 넣었을때 그좌표에 벽이있는지 벽이면 true 빈곳이면 false
	static bool FindObject(float x, float y);

	//들어온 float 형을 맵에 포인트에맞게 변경해줌
	static bool ChangeFloatMapPoint(int& X, int& Y, XMFLOAT2 point);
private:
	///노드 찾기용 함수들
	//이좌표에 갈수있는지 체크
	bool CheckNode(int x, int y,int Cost);
	//대각선 체크
	
	//나의 방향으로 8방향 노드를 검색한다
	void FindOpenNode();

	//오픈노드에서 내가 갈수있는곳을 찾는다
	void SearchNode();

	//현재 나의 지점과 목적지에 거리를 구하고 0~1사이의 지점을 가져온다
	bool GetDistance(XMFLOAT3 Pos,float dir);

	Node* GetNode();
	void CreateNode();

	static vector<vector<int>> NV_Map;
private:
	//노드들을 관리할 풀
	vector<Node*>NodePool;
	int GetNodeIndex;

	//맵사이즈

	//조사를 하고있는 노드 리스트
	vector<Node*> OpenNode;
	//조사가 끝난 노드 리스트
	vector<Node*> CloseNode;

	//A* 알고리즘으로 얻어낸 최단경로
	vector<XMFLOAT2>FastRoad;

	//맵 사이즈
	static int MaxSizeX;
	static int MaxSizeY;

	//시작지점
	int StartPointX;
	int StartPointY;

	//끝지점
	int EndPointX;
	int EndPointY;

	//현재위치
	int NowPointX;
	int NowPointY;


	bool reset;
	Node* NowNode;
	int FastRoadindex = 1;
};
