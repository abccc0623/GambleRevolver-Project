#include "EnginePCH.h"
#include "Factory.h"


std::vector<ID3D11InputLayout*> Factory::LayoutList = std::vector<ID3D11InputLayout*>();
Factory::Factory()
{
	ENGINE = nullptr;
	Resource = nullptr;
}

Factory::~Factory()
{
	ENGINE = nullptr;

	

	delete Resource;
}

void Factory::init(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;

	Resource = new ResourceManager();
	Resource->init(ENGINE);

	shaderManager = new ShaderManager();
	shaderManager->init(ENGINE);
}


void Factory::SetResourece(LOAD_TYPE Type, string _path, string ReName)
{
	Resource->SetResource(Type, _path, ReName);
}

ID3D11ShaderResourceView* Factory::GetTexture(string Name)
{
	return Resource->GetTexture(Name);
}

void Factory::SetLoadPath(LOAD_TYPE Type, string _path)
{
	Resource->SetLoadPath(Type, _path);
}

void Factory::SetShaderLoadPath(string _path)
{
	shaderManager->SetShaderFilePath(_path);
}

void Factory::ClearMesh(string Name)
{
	Resource->ClearMesh(Name);
}

void Factory::ClearAnimation(string Name)
{
	Resource->ClearAnimation(Name);
}

ShaderData Factory::GetShader(string Name)
{
	return shaderManager->GetShader(Name);
}

hsCamera* Factory::Create_Camera(float x, float y, float z)
{
	hsCamera* Cam = new hsCamera(x,y,z);
	Cam->SetDebugShader(shaderManager->GetShader("Debug"));
	Cam->SetContextBufferList(shaderManager->GetContextBufferList());
	Cam->init(ENGINE);
	return Cam;
}

LightManager* Factory::Create_LightManager( float x, float y, float z)
{
	LightManager* temp = new LightManager(x,y,z);
	temp->SetShaderData(shaderManager->GetShader("PointLight"));
	//temp->init(ENGINE);
	return temp;
}

LightObject* Factory::Create_LightObj(int Type,float x, float y, float z)
{
	LightObject* temp = new LightObject(x, y, z);
	temp->SetDebugShader(shaderManager->GetShader("Debug"));
	temp->init(ENGINE, Type);
	return temp;
}

hsDirLight* Factory::Create_DirLight(float x, float y, float z)
{
	hsDirLight* temp = new hsDirLight(x, y, z);
	temp->SetDebugShader(shaderManager->GetShader("Debug"));
	temp->SetShaderData(shaderManager->GetShader("DirLight"));

	temp->init(ENGINE);
	return temp;
}

ParticleSystem* Factory::Create_Particle(Bulider _bulider, float x, float y, float z)
{
	ShaderData data = shaderManager->GetShader(_bulider.shader);
	ShaderData Debug = shaderManager->GetShader("Debug");
	ID3D11ShaderResourceView* text = Resource->GetTexture(_bulider.Textures);

	ParticleSystem* temp = new ParticleSystem(x, y, z);
	temp->SetDebugShader(Debug);
	temp->SetShaderData(data);
	temp->SetTexture(text);

	temp->init(ENGINE);
	return temp;
}

TextureObj* Factory::Create_TextureObj(Bulider _bulider,float x, float y, float z)
{
	ShaderData data = shaderManager->GetShader(_bulider.shader);
	ID3D11ShaderResourceView* text = Resource->GetTexture(_bulider.Textures);

	TextureObj* temp = new TextureObj(x,y,z);
	temp->SetShaderData(data);
	temp->SetDebugShader(shaderManager->GetShader("Debug"));
	temp->SetTextures(text);

	temp->init(ENGINE);
	return temp;
}

Image* Factory::Create_Image(Bulider _bulider, float x, float y, float z)
{
	ShaderData data = shaderManager->GetShader(_bulider.shader);
	ID3D11ShaderResourceView* text = Resource->GetTexture(_bulider.Textures);

	Image* temp = new Image(x, y, z);
	temp->SetShaderData(data);
	temp->SetTexture(text);
	
	temp->init(ENGINE);
	return temp;
}

SkyBox* Factory::Create_SkyBox(float x, float y, float z)
{
	
	LoadMesh* mesh = Resource->GetMesh("box");
	ID3D11ShaderResourceView* text = Resource->GetTexture("Sky");
	ShaderData data = shaderManager->GetShader("Sky");


	SkyBox* temp = new SkyBox(x, y, z);
	temp->SetShaderData(data);
	temp->init(ENGINE, mesh->meshList[0]);
	temp->SetSkyTextures(text);
	return temp;
}

Grid* Factory::Create_Grid(float SizeX, float SizeY, XMFLOAT4 Color,XMFLOAT3 Pos)
{
	Grid* temp = new Grid(Pos.x, Pos.y, Pos.z);
	temp->SetDebugShader(shaderManager->GetShader("Debug"));
	temp->SetOption(SizeX, SizeY, Color);
	temp->init(ENGINE);
	return temp;
}




















