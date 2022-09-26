#pragma once

//오브젝트를 만들때 마다 3개의 변수를 넘겨줘야하는데 그게 귀찮아서 하나로 뭉침
struct DeviceData
{
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RasterizerState* mSolidRS;
	ID3D11RasterizerState* mWireframeRS;
};
struct EngineBaseViewData
{
	ID3D11RenderTargetView* RTV;	//랜더 타겟
	ID3D11DepthStencilView* DSV;	//뎁스 스텐실뷰
	D3D11_VIEWPORT			VP;		//뷰포트
};


//오브젝트 업데이트 할떄 넘겨줘야하는 데이터
struct hsViewData 
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;
};

//모델을 로드할때 필요한 구조체
struct LoadData
{
	//경로
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


	///오브젝트 상속될때 필요한 변수들
	bool parent; //부모객체 있는지 여부
	Vector3 POS; //위치
	Vector3 ROT;  //회전
	Vector3 SCALE; //크기

	XMMATRIX V_TM; //로컬 행렬
	XMMATRIX W_TM;

	
	std::string ParentName;
	std::string ChildName;
	std::string MyName;

	//스키닝 구조체
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

	//애니메이션 정보
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
	UINT ObjType;	//오브젝트 타입
	UINT HitType;	//충돌한 오브젝트 타입
};

struct hsSphereCollider
{
	BoundingSphere BoundingSphereCollider;
	UINT ObjType; //오브젝트 타입
	UINT HitType; //충돌한 오브젝트 타입
	bool isCollision = true;
};

struct Particle_float
{
	//랜덤이 아니면 이거
	Particle_float(float data = 0)
	{
		Data = data;
		Min = 0;
		Max = 0;
		isRandom = false;
	}

	//랜덤이면 이거
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
			//정수 자리
			int RandomNumber_n = Min + (rand()) % (int)Max;
			//소수점 자리
			int RandomNumber = 0   + (rand()) % (int)10;
			//둘이 합쳐 소수점으로
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
	//랜덤이 아닐때
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

	//랜덤일때
	Particle_float3(Particle_float x, Particle_float y, Particle_float z)
	{
		DataX = x;
		DataY = y;
		DataZ = z;
	}

	//3개 모두 공통된 랜덤일때
	Particle_float3(int min,int max)
	{
		//정수 자리
		int RandomNumber_n = min + (rand()) % (int)max;
		//소수점 자리
		int RandomNumber = 0 + (rand()) % (int)10;
		//둘이 합쳐 소수점으로
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


