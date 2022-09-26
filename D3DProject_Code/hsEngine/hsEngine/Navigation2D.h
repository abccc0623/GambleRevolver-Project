#pragma once
struct Node
{
	int POS_X;
	int POS_Y;

	//�θ���
	Node* ParentNode;

	//������� �̵��ѰŸ�
	int G;
	//��ǥ������ ������ �Ÿ�
	int H;
	//G+H�� ��
	int F;
};

class Navigation2D
{
public:
	//�Ÿ� �˻縦 �Ҷ� �ʿ��� ���
	Navigation2D();
	~Navigation2D();

	vector<XMFLOAT2>* StartNavigation();

	//�������� ���� (�迭�̱⶧���� float���� ������ ���� ó��)
	void SetStartPoint(float x, float y);

	//������ ����(�迭�̱⶧���� float���� ������ ���� ó��)
	void SetEndPoint(float x, float y);

	//A*�� ���� ��ġ������ õõ�� �̵���Ų��
	//Distance = 0 ������ Distance = 1 ó������ Distance = 0.5f �߰�����
	bool NavigationMove(XMFLOAT3& Position, XMFLOAT3& Rot, float speed,float Distance = 0);

	//�����͸� Ŭ�������ش�
	void DataClear();
public:
	///����ƽ �Լ���
	//�ʹ迭�� �ʱ�ȭ ��Ų��
	static void ClearMap();

	//�� ����� ������ش�
	static void CreateMapSize(int SizeX, int SizeY,bool Side = true);

	//�ʹ迭�� ������Ʈ�� �־��� �װ��� �����°��� �ɰ���
	static void SetObjectSetting(XMFLOAT3 Pos);
	//�ʹ迭�� ������Ʈ�� �־��� ó�������� ����������
	static void SetObjectSetting(XMFLOAT2 pos,float sizeX,float sizeY);
	//�ʹ迭�� ������Ʈ�� �־��� ó�������� ���������� ��ĭ������߸�
	static void SetObjectSetting_jump(XMFLOAT2 pos, float sizeX, float sizeY);


	//��ǥ�� �־����� ����ǥ�� �����ִ��� ���̸� true ����̸� false
	static bool FindObject(float x, float y);

	//���� float ���� �ʿ� ����Ʈ���°� ��������
	static bool ChangeFloatMapPoint(int& X, int& Y, XMFLOAT2 point);
private:
	///��� ã��� �Լ���
	//����ǥ�� �����ִ��� üũ
	bool CheckNode(int x, int y,int Cost);
	//�밢�� üũ
	
	//���� �������� 8���� ��带 �˻��Ѵ�
	void FindOpenNode();

	//���³�忡�� ���� �����ִ°��� ã�´�
	void SearchNode();

	//���� ���� ������ �������� �Ÿ��� ���ϰ� 0~1������ ������ �����´�
	bool GetDistance(XMFLOAT3 Pos,float dir);

	Node* GetNode();
	void CreateNode();

	static vector<vector<int>> NV_Map;
private:
	//������ ������ Ǯ
	vector<Node*>NodePool;
	int GetNodeIndex;

	//�ʻ�����

	//���縦 �ϰ��ִ� ��� ����Ʈ
	vector<Node*> OpenNode;
	//���簡 ���� ��� ����Ʈ
	vector<Node*> CloseNode;

	//A* �˰������� �� �ִܰ��
	vector<XMFLOAT2>FastRoad;

	//�� ������
	static int MaxSizeX;
	static int MaxSizeY;

	//��������
	int StartPointX;
	int StartPointY;

	//������
	int EndPointX;
	int EndPointY;

	//������ġ
	int NowPointX;
	int NowPointY;


	bool reset;
	Node* NowNode;
	int FastRoadindex = 1;
};
