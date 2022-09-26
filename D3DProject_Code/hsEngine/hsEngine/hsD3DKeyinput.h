#pragma once

/// <summary>
/// 다이렉트 전용 키인풋 클래스
/// API에서 제공되는 키인풋을 사용해도 무방하지만
/// 윈도우즈 프로그래밍방식은 키보드와 마우스를 다룰때 메세지를 받아 처리하지만
/// 이방식은 메세지큐를 사용하므로 중간에서 가로채는 D3D 방식과 비교할때 상당한 시간이 걸린다
/// 이런부분들을 해결하기위해 전용으로 만들어논 것들을 정리
/// </summary>
/// 


class Keyinput
{
public:
	~Keyinput();

	//싱글톤..
	Keyinput();
	

	bool init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	void Delete();
	bool Updete();

	bool DownKey(unsigned char state);
	bool UpKey(unsigned char state);

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);
private:
	

	//키보드 읽기
	bool ReadKeyboard();
	//마우스 읽기
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