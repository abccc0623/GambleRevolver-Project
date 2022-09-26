#pragma once

/// <summary>
/// ���̷�Ʈ ���� Ű��ǲ Ŭ����
/// API���� �����Ǵ� Ű��ǲ�� ����ص� ����������
/// �������� ���α׷��ֹ���� Ű����� ���콺�� �ٷ궧 �޼����� �޾� ó��������
/// �̹���� �޼���ť�� ����ϹǷ� �߰����� ����ä�� D3D ��İ� ���Ҷ� ����� �ð��� �ɸ���
/// �̷��κе��� �ذ��ϱ����� �������� ������ �͵��� ����
/// </summary>
/// 


class Keyinput
{
public:
	~Keyinput();

	//�̱���..
	Keyinput();
	

	bool init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	void Delete();
	bool Updete();

	bool DownKey(unsigned char state);
	bool UpKey(unsigned char state);

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);
private:
	

	//Ű���� �б�
	bool ReadKeyboard();
	//���콺 �б�
	bool ReadMouse();
	void ProcessInput();


	IDirectInput8*			m_directInput;
	IDirectInputDevice8*	m_keyboard;
	IDirectInputDevice8*	m_mouse;

	BYTE m_keyboardState[256] = { 0, };
	DIMOUSESTATE m_mouseState;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;
};