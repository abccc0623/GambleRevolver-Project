#pragma once
/// <summary>
/// UI���� �����ҸŴ��� 
/// </summary>

class Canvas 
{

public:
	void init(hsEngine* _Engine, Factory *_Factory);



	void Update();
	void Render();
	void Debug();
	void Delete();

	//�����׸���Layout������ UI�� �־��ش�
	void PushUI(int layout, UI* ui);
	//����� ���̾ƿ� ����� ��������
	void CreateUILayout(int LayoutSize);
	
	//UI���� ��������
	void ClearUI();


	//UI�� �ʿ��� ȭ�����ǥ���� ���Ѵ�
	//�̰Ŵ� ������â�� ũ�Ⱑ���ҋ����� ���������ϱ⶧���� ����ƽ���� ������ OnResize�� �Ͼ�� ȣ��ɰ��̴�
	static void CreateScreenNumber(int Width,int Height);

	///ȭ������� ���Ѱ͵�
	//�����ȼ�ũ��
	static float OnePixelX;
	static float OnePixelY;

	//ȭ���� �������� ��ǥ
	static float MaxX;
	static float MaxY;

	//ȭ���� ���ʾƷ� ��ǥ
	static float MinX;
	static float MinY;

	//�̹����� 1:1 ������
	static float OneSizeX;
	static float OneSizeY;

public:
	///UI ���� �Լ���
	Image* Create_Image(string TextureName, int Layout, float Alpha, XMFLOAT3 Pos, XMFLOAT3 Scl = {1,1,1},UI_PIVOT pivot= UI_PIVOT::MID);
	Image* Create_Button(string TextureName, int Layout, float Alpha, XMFLOAT3 Pos, XMFLOAT3 Scl = { 1,1,1 }, UI_PIVOT pivot = UI_PIVOT::MID);
	Slider* Create_Slider(string Back,string Fill,string Front,string hand,XMFLOAT3 Pos,XMFLOAT3 Scl,UI_PIVOT pivot);
private:
	hsEngine* mENGINE;
	Factory* mFactory;


	vector<vector<UI*>> UILauoutList;
};