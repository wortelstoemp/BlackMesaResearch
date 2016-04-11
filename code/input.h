// Author(s): Tom

struct InputSystem
{
	enum KeyCode
	{
		KEY_UP = 0,
		KEY_DOWN = 1,
		KEY_LEFT = 2,
		KEY_RIGHT = 3,
		KEY_F = 4,
		KEY_L = 5,				
	};
	
	static const int MAX_KEYS = 6;
	bool keys[MAX_KEYS] = {false};
	bool downKeys[MAX_KEYS] = {false};
	bool upKeys[MAX_KEYS] = {false};
	
	inline bool isKey(KeyCode keyCode)
	{
		return keys[keyCode];
	}
	
	inline bool isKeyDown(KeyCode keyCode)
	{
		return downKeys[keyCode];
	}
	
	inline bool isKeyUp(KeyCode keyCode)
	{
		return upKeys[keyCode];
	}
	
	inline void ResetKeys()
	{
		for (int i = 0; i < MAX_KEYS; i++)
			keys[i] = false;
		for (int i = 0; i < MAX_KEYS; i++)
			downKeys[i] = false;
		for (int i = 0; i < MAX_KEYS; i++)
			upKeys[i] = false;
	}
};