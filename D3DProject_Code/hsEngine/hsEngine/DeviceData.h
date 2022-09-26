#pragma once

//������Ʈ�� ���鶧 ���� 3���� ������ �Ѱ�����ϴµ� �װ� �����Ƽ� �ϳ��� ��ħ
struct DeviceData
{
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RasterizerState* mSolidRS;
	ID3D11RasterizerState* mWireframeRS;
};
struct EngineBaseViewData
{
	ID3D11RenderTargetView* RTV;	//���� Ÿ��
	ID3D11DepthStencilView* DSV;	//���� ���ٽǺ�
	D3D11_VIEWPORT			VP;		//����Ʈ
};


//������Ʈ ������Ʈ �ҋ� �Ѱ�����ϴ� ������
struct hsViewData 
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;
};

//���� �ε��Ҷ� �ʿ��� ����ü
struct LoadData
{
	//���
	std::string path;
	UINT vcount;
	UINT tcount;

	int IndexCount;
	std::vector<UINT> vertex_index;

	std::vector<Vector3> vertex_pos;
	std::vector<Vector2> vertex_tex;
	std::vector<Vector3> vertex_nomal;
	std::vector<Vector3> vertex_tangentU;
	std::vector<Vector4> vertex_weights;
	std::vector< std::vector<UINT> > vertex_bonIndex;


	///������Ʈ ��ӵɶ� �ʿ��� ������
	bool parent; //�θ�ü �ִ��� ����
	Vector3 POS; //��ġ
	Vector3 ROT;  //ȸ��
	Vector3 SCALE; //ũ��

	XMMATRIX V_TM; //���� ���
	XMMATRIX W_TM;

	
	std::string ParentName;
	std::string ChildName;
	std::string MyName;

	//��Ű�� ����ü
	bool isSkinnde;
	bool isBone;
	map<string,int> BoneNumberList;
	vector<ASEParser::Bone*>			m_bone_list;
	vector<ASEParser::VertexWeight*>	m_wvertexs;
};

struct LoadMesh
{
	std::string meshName;
	std::vector<LoadData*> meshList;

	//�ִϸ��̼� ����
	std::list<hsAnimation*> anim;
};
struct FBX_ParserData
{
	XMFLOAT3	pos;
	XMFLOAT2	uv;
	XMFLOAT3	nomal;
	//int			index;
};

struct FBX_Parser
{
	XMFLOAT3	pos;
	XMFLOAT3	nomal;
	XMFLOAT2	uv;
	int			index;
};

struct LoadFX 
{
	ID3DX11Effect* mFX;
	std::vector<char> compiledShader;
	int size;
};

struct LoadAnimation
{	
	string Name;
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

	double OneFrameTime;
	std::list<hsAnimation*> Anime;
};

struct Bone 
{
	string MyName		= "";
	string ParentName	= "";
	int index;

	XMFLOAT4X4 TransfromTm;
	XMFLOAT4X4 ParentTM;

	vector<int>WeightIndex;
	vector<double>Weight;
};


struct Bulider
{
	ID3D11InputLayout* Layout	= nullptr;

	string Textures		= "";
	string SubTextures	= "";
	string Nomal		= "";
	string Mask			= "";
	string Sky			= "";
	string ASE_MESH		= "";
	string FBX_MESH		= "";
	string Anim			= "";
	string shader		= "";
	string mesh			= "";
	std::vector<string> AnimList;
};

struct hsBoxCollider
{
	BoundingBox BoundingBoxCollider;
	UINT ObjType;	//������Ʈ Ÿ��
	UINT HitType;	//�浹�� ������Ʈ Ÿ��
};

struct hsSphereCollider
{
	BoundingSphere BoundingSphereCollider;
	UINT ObjType; //������Ʈ Ÿ��
	UINT HitType; //�浹�� ������Ʈ Ÿ��
	bool isCollision = true;
};

struct Particle_float
{
	//������ �ƴϸ� �̰�
	Particle_float(float data = 0)
	{
		Data = data;
		Min = 0;
		Max = 0;
		isRandom = false;
	}

	//�����̸� �̰�
	Particle_float(int min,int max)
	{
		Data = 0;
		Min = min;
		Max = max;
		isRandom = true;
	}

	float  GetData()
	{
		if (isRandom == false)
		{
			return Data;
		}
		else
		{
			//���� �ڸ�
			int RandomNumber_n = Min + (rand()) % (int)Max;
			//�Ҽ��� �ڸ�
			int RandomNumber = 0   + (rand()) % (int)10;
			//���� ���� �Ҽ�������
			float num = RandomNumber_n + (RandomNumber * 0.1f);
			
			return num + Data;
		}
	}

	float Min;
	float Max;
	float Data;
	bool isRandom;
};
struct Particle_float3
{
	//������ �ƴҶ�
	Particle_float3(float x, float y, float z)
	{
		DataX = Particle_float(x);
		DataY = Particle_float(y);
		DataZ = Particle_float(z);
	}

	Particle_float3()
	{
		DataX = Particle_float(0);
		DataY = Particle_float(0);
		DataZ = Particle_float(0);
	}

	//�����϶�
	Particle_float3(Particle_float x, Particle_float y, Particle_float z)
	{
		DataX = x;
		DataY = y;
		DataZ = z;
	}

	//3�� ��� ����� �����϶�
	Particle_float3(int min,int max)
	{
		//���� �ڸ�
		int RandomNumber_n = min + (rand()) % (int)max;
		//�Ҽ��� �ڸ�
		int RandomNumber = 0 + (rand()) % (int)10;
		//���� ���� �Ҽ�������
		float num = RandomNumber_n + (RandomNumber * 0.1f);

		DataX = Particle_float(num);
		DataY = Particle_float(num);
		DataZ = Particle_float(num);
	}

	XMFLOAT3 GetData()
	{
		XMFLOAT3 number;
		number.x  = DataX.GetData();
		number.y  = DataY.GetData();
		number.z  = DataZ.GetData();
		return number;
	}

	Particle_float DataX;
	Particle_float DataY;
	Particle_float DataZ;
};


