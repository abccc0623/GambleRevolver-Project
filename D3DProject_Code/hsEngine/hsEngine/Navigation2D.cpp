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
	//2���� �迭�� ����
	NV_Map.resize(SizeX);
	for (int i = 0; i < SizeX; i++)
	{
		NV_Map.resize(SizeX);
		for (int j = 0; j <= SizeY; j++)
		{
			NV_Map[i].resize(SizeY);
		}
	}

	//�ܰ������� ��� ������ �����
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


	//�÷��̾� �������� ����4���� ������ ��带 �����ϰ� Open��忡 �־��ش�
	CheckNode(x, y + 1, 10); //�÷��̾��� ������ ����
	CheckNode(x, y - 1, 10); //�÷��̾��� �Ʒ����� ����
	CheckNode(x + 1, y, 10); //�÷��̾��� ������ ���� ����
	CheckNode(x - 1, y, 10); //�÷��̾��� ���� ���� ����

	
	CheckNode(x + 1, y + 1, 14); //�÷��̾��� ������ �� �밢��
	CheckNode(x + 1, y - 1, 14); //�÷��̾��� ������ �Ʒ� �밢��
	CheckNode(x - 1, y + 1, 14); //�÷��̾��� ���� �� �밢��
	CheckNode(x - 1, y - 1, 14); //�÷��̾��� ���� �Ʒ� �밢��
}

void Navigation2D::SearchNode()
{

	int MinNodeIndex = -1;
	int MinNodeF = 10000;


	//���̻� ���̾���...
	if(OpenNode.size() == 0)
	{
		return;
	}

	//���³���߿��� ���� �񱳿��� ���� �ּҺ���� ��带ã�´�
	for (int i = 0; i < OpenNode.size(); i++)
	{
		if (OpenNode[i] == nullptr) { continue; }

		if (MinNodeF > OpenNode[i]->H)
		{
			MinNodeF = OpenNode[i]->H;
			MinNodeIndex = i;
		}
	}
	
	//���� �ּҺ���� ��带 ã�Ҵ�
	Node* mNode = OpenNode[MinNodeIndex];
	//ã�� ���� �˻��� �Ϸ��߱⶧���� close���� �־��ְ� ���³�忡���� ������Ų��
	CloseNode.push_back(NowNode);

	//�׸��� ������ġ�� �������ش�
	NowPointX = mNode->POS_X;
	NowPointY = mNode->POS_Y;

	NowNode = mNode;
	OpenNode[MinNodeIndex] = nullptr;
}

bool Navigation2D::GetDistance(XMFLOAT3 mPos,float mdir)
{
	//ó�� ����� �Ÿ��� 0~1������ �Ÿ��� ��Ÿ���� mdir������ ���� �����´�
	float x = EndPointX - StartPointX;
	float y = EndPointY - (-StartPointY);
	float d = sqrt((x * x) + (y * y));
	float Start = MathHelper::Lerp<float>(0, d, mdir);

	//���� ���� ��ġ���� ������������ �Ÿ�
	float MYx = EndPointX - mPos.x;
	float MYy = EndPointY - (-mPos.z);
	float MYd = sqrt((MYx * MYx) + (MYy * MYy));

	//��θ� �̵��ϴٰ� �������� �Ÿ����� ª������ true ����
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
	//������ ���ü� �ֱ⶧���� ���밪���� ����
	float mx = abs(point.x);
	float my = abs(point.y);

	
	//�迭 �����Ѱ����� �ָ� ��ġ���ϰԵǸ� ��ġ�����ʴ´�
	if (mx < 0 || mx >= MaxSizeX || my < 0 || my >= MaxSizeY)
	{
		return false;
	}

	//���°��� �ݿø�ó������
	X = (int)floor(mx + 0.5f);
	Y = (int)floor(my + 0.5f);

	return true;
}

Node* Navigation2D::GetNode()
{
	//���Ǯ �ȿ� ��尡 �����Ұ�� 10���ø�
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
	//������ ������ ���Ǯ�� �����
	for (int i = 0; i < 20; i++)
	{
		NodePool.push_back(new Node);
	}
	GetNodeIndex = 0;
}

vector<XMFLOAT2>* Navigation2D::StartNavigation()
{
	reset = false;
	//�ٽñ���ã���� �ʱ�ȭ
	DataClear();

	NowNode = GetNode();
	NowNode->ParentNode = nullptr;
	NowNode->POS_X = StartPointX;
	NowNode->POS_Y = StartPointY;

	NowPointX = StartPointX;
	NowPointY = StartPointY;

	//�ִ� ��θ� ã�´�
	while(true)
	{
		if (NowPointX == EndPointX && NowPointY == EndPointY)
		{
			break;
		}

		//�ϴ� �������� �����ؼ� ���³�带 �����Ѵ�
		FindOpenNode();

		//���³�忡�� ���� ������ã�� �׳��� �̵��Ѵ�
		SearchNode();
	}

	//ã�� ����κ��� ��ġ���� �̾Ƴ���
	while (true)
	{
		if (NowNode == nullptr)
		{
			break;
		}

		FastRoad.push_back(XMFLOAT2(NowNode->POS_X, NowNode->POS_Y));
		NowNode = NowNode->ParentNode;
	}

	//���� ��ġ���� �������� �������ش� (�θ������ ó�������� Pos���� �о���µ� �̰��� �������ν�Ű�� �ִܰ��)
	std::reverse(FastRoad.begin(), FastRoad.end());


	//������ ���� ����Ʈ�� Ŭ����
	OpenNode.clear();
	CloseNode.clear();
	GetNodeIndex = 0;

	return &FastRoad;
}

bool Navigation2D::CheckNode(int x, int y, int Cost)
{
	///���³�忡 �������ִ� ������� Ȯ���Ѵ�

	//x��ǥ,y ��ǥ��  �迭�� ������ �Ѿ���
	if (x < 0 || x >= MaxSizeX || y < 0 || x >= MaxSizeX)
	{
		return false;
	}

	//������������ ���̴�
	if (NV_Map[x][y] == 1)
	{
		return false;
	}

	//�밢���� üũ�Ҷ�
	if (Cost == 14)
	{
		int dirx = x - NowNode->POS_X;
		int diry = y - NowNode->POS_Y;

		//�밢�������� ���μ����� ������Ʈ���ִٸ� �����°�
		bool x = FindObject(NowNode->POS_X+ dirx	, NowNode->POS_Y);
		bool y = FindObject(NowNode->POS_X		, NowNode->POS_Y + diry);
		if (x == true || y == true) { return false; }
	}

	for (int i = 0; i < CloseNode.size(); i++)
	{
		//Close��忡 ������尡 �ִٸ�
		if (CloseNode[i]->POS_X == x && CloseNode[i]->POS_Y == y)
		{
			return false;
		}
	}

	Node* mNode = GetNode();
	//��ġ�־���
	mNode->POS_X = x;
	mNode->POS_Y = y;

	//�θ�����
	mNode->ParentNode = NowNode;

	//����� ��ġ���� ���� ��ġ�� ����
	int mx = abs(mNode->POS_X - StartPointX);
	int my = abs(mNode->POS_Y - StartPointY);
	//�̰��� ���� ���� �̵� ���
	mNode->G = mNode->ParentNode->G + Cost;

	//���� �����ġ���� ��������ġ������
	mx = abs( EndPointX - mNode->POS_X);
	my = abs( EndPointY - mNode->POS_Y);

	//������ġ���� ������������ ���
	mNode->H = (int)sqrt( pow(mx, 2) + pow(my, 2)) * 14;

	//�������
	mNode->F = mNode->G + mNode->H;


	//��� �� ��带 ���³�忡 �ִ´�
	OpenNode.push_back(mNode);


	//��� �ƴ϶�� �����ִ� �����̴�
	return true;
}



void Navigation2D::SetStartPoint(float x, float y)
{
	//���������� �־���
	ChangeFloatMapPoint(StartPointX, StartPointY, XMFLOAT2(x, y));
}

void Navigation2D::SetEndPoint(float x, float y)
{
	//�������� �־���
	ChangeFloatMapPoint(EndPointX, EndPointY, XMFLOAT2(x, y));
}

bool  Navigation2D::NavigationMove(XMFLOAT3& Position, XMFLOAT3& Rot, float speed, float Distance)
{
	///ã�� ��� õõ�� �̵����ϰ� ȸ�����ϸ鼭 �̵��Ѵ�

	//������ã�� �ٽ� ��θ����������������� �̵������ʴ´�
	if (FastRoad.size() == 0) { return true;}


	if (FastRoadindex > FastRoad.size() - 1 || reset == true)
	{
		//���� �̰�δ� �ʿ����
		DataClear();
		//�������� ����
		return true;
	}


	//��������
	XMFLOAT3 Target = { 0,0,0 };
	Target.x = FastRoad[FastRoadindex].x;
	Target.y = 0;
	Target.z = -FastRoad[FastRoadindex].y;
	
	//���� ����
	XMFLOAT3 startpoint = { 0,0,0 };
	startpoint.x = Position.x;
	startpoint.y = 0;
	startpoint.z = Position.z;
	
	//�ϴ� ������ ���Ѵ�
	XMFLOAT3 point = startpoint - Target;
	
	//���⺤���� ũ�⸦ ���Ѵ�
	float nomal = sqrt(pow(point.x, 2) + pow(point.z, 2));
	
	
	///�������� ȸ��
	//���� ���� ����
	XMFLOAT2 startvec = { Position.x, Position.z };
	//�����ϴ� �� ����
	XMFLOAT2 Endvec = { FastRoad[FastRoadindex].x,-FastRoad[FastRoadindex].y };

	//�������κ����� �����ϴ°��� ���͸� ����
	float X = FastRoad[FastRoadindex].x - Position.x ;
	float Y = -FastRoad[FastRoadindex].y - Position.z;

	//�װ��� ������ ����
	float angle = -atan2(Y, X) * 180 / MathHelper::Pi;
	//�������� ó��ĳ���ʹ� x������ ���ư��ְ� -z������ �����ֱ⋚���� 90���� ���ش� (���������ٶ󺸰��ֵ����ϱ�����)
	Rot = { Rot.x,Rot.y,90 - angle };



	//��ǥ�� ����������� ������ũ����پ����̰� �װ��� �����ӽð����� �������
	//������������ �̵��Ѵ�
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
		//�������� ����
	}
	
	//��� �̵���
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
	///������Ʈ�� �����Ѵ� �����Ǹ� �̰��� ���������������̵ȴ�

	int ArrayX = 0;
	int ArrayY = 0;
	ChangeFloatMapPoint(ArrayX, ArrayY, XMFLOAT2(Pos.x, Pos.z));

	//���� �迭�� �ִ´�
	NV_Map[ArrayX][ArrayY] = 1;
}

void Navigation2D::SetObjectSetting(XMFLOAT2 pos, float sizeX, float sizeY)
{
	int ArrayX = 0;
	int ArrayY = 0;
	ChangeFloatMapPoint(ArrayX, ArrayY, XMFLOAT2(pos.x, pos.y));

	//���� �迭�� �ִ´�

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

	//���� �迭�� �ִ´�
	
	for (int i = 0; i < sizeX; i++)
	{
		bool isjump = false;
		for (int j = 0; j < sizeY; j++)
		{
			//��ĭ�� ���
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
	///���� Ȯ���ϴ� ��ǥ�� �����ִ������� üũ ������ true ������ false

	int pointX = 0;
	int pointY = 0;

	if (ChangeFloatMapPoint(pointX, pointY, XMFLOAT2(x, y))== false)
	{
		//�����ִ°ɷ� ����
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
