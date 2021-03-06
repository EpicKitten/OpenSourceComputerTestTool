#define WIN32_LEAN_AND_MEAN //If we're not using it, remove it
#include <Windows.h> //For Windows Specific Functions
#include <iostream> //For std
#include <string> //For specific string functions
#define _WIN32_WINNT 0x0400
HHOOK hKeyboardHook;
HANDLE hThread;
DWORD dwThread;
DWORD SHIFT_key = 0; //Varible for holding data if they key was tagged with any SHIFT modifier
DWORD LSHIFT_key = 0; //Varible for holding data if they key was tagged with left SHIFT modifier
DWORD RSHIFT_key = 0; //Varible for holding data if they key was tagged with right SHIFT modifier
DWORD CTRL_key = 0; //Varible for holding data if they key was tagged with any CTRL modifier
DWORD LCTRL_key = 0; //Varible for holding data if they key was tagged with left CTRL modifier
DWORD RCTRL_key = 0; //Varible for holding data if they key was tagged with right CTRL modifier
DWORD ALT_key = 0; //Varible for holding data if they key was tagged with any SHIFT modifier
DWORD LALT_key = 0; //Varible for holding data if they key was tagged with left ALT modifier
DWORD RALT_key = 0; //Varible for holding data if they key was tagged with right ALT modifier
bool A_key = false,
B_key = false,
C_key = false,
D_key = false,
E_key = false,
F_key = false,
G_key = false,
H_key = false,
I_key = false,
J_key = false,
K_key = false,
L_key = false,
M_key = false,
N_key = false,
O_key = false,
P_key = false,
Q_key = false,
R_key = false,
S_key = false,
T_key = false,
U_key = false,
V_key = false,
W_key = false,
X_key = false,
Y_key = false,
Z_key = false,
Tab_key = false,
Enter_key = false,
Space_key = false,
LeftControl_key = false,
RightControl_key = false,
LeftShift_key = false,
RightShift_key = false,
LeftAlt_key = false,
RightAlt_key = false,
Up_key = false,
Left_key = false,
Right_key = false,
Down_key = false,
One_key = false,
Two_key = false,
Three_key = false,
Four_key = false,
Five_key = false,
Six_key = false,
Seven_key = false,
Eight_key = false,
Nine_key = false,
Zero_key = false,
LeftBracket_key = false,
RightBracket_key = false,
LeftSlash_key = false,
Minus_key = false,
Equal_key = false,
Backspace_key = false,
Titde_key = false,
LeftArrow_key = false,
RightArrow_key = false,
QuestionMark_key = false,
Semicolan_key = false,
Qoute_key = false,
LeftWin_key = false,
RightWin_key = false,
CapsLock_key = false;
int A_keyAmount = 0,
B_keyAmount = 0,
C_keyAmount = 0,
D_keyAmount = 0,
E_keyAmount = 0,
F_keyAmount = 0,
G_keyAmount = 0,
H_keyAmount = 0,
I_keyAmount = 0,
J_keyAmount = 0,
K_keyAmount = 0,
L_keyAmount = 0,
M_keyAmount = 0,
N_keyAmount = 0,
O_keyAmount = 0,
P_keyAmount = 0,
Q_keyAmount = 0,
R_keyAmount = 0,
S_keyAmount = 0,
T_keyAmount = 0,
U_keyAmount = 0,
V_keyAmount = 0,
W_keyAmount = 0,
X_keyAmount = 0,
Y_keyAmount = 0,
Z_keyAmount = 0,
Tab_keyAmount = 0,
Enter_keyAmount = 0,
Space_keyAmount = 0,
LeftControl_keyAmount = 0,
RightControl_keyAmount = 0,
LeftShift_keyAmount = 0,
RightShift_keyAmount = 0,
LeftAlt_keyAmount = 0,
RightAlt_keyAmount = 0,
Up_keyAmount = 0,
Left_keyAmount = 0,
Right_keyAmount = 0,
Down_keyAmount = 0,
One_keyAmount = 0,
Two_keyAmount = 0,
Three_keyAmount = 0,
Four_keyAmount = 0,
Five_keyAmount = 0,
Six_keyAmount = 0,
Seven_keyAmount = 0,
Eight_keyAmount = 0,
Nine_keyAmount = 0,
Zero_keyAmount = 0,
LeftBracket_keyAmount = 0,
RightBracket_keyAmount = 0,
LeftSlash_keyAmount = 0,
Minus_keyAmount = 0,
Equal_keyAmount = 0,
Backspace_keyAmount = 0,
Titde_keyAmount = 0,
LeftArrow_keyAmount = 0,
RightArrow_keyAmount = 0,
QuestionMark_keyAmount = 0,
Semicolan_keyAmount = 0,
Qoute_keyAmount = 0,
LeftWin_keyAmount = 0,
RightWin_keyAmount = 0,
CapsLock_keyAmount = 0;

BOOL isRunningKeyboardTest = false;
//Displays the status of a key pressed when called
void DisplayKeyStatus(std::string keyName, bool keyEnabled, int keyPressAmount, bool onlyfail)
{
	if (onlyfail && keyEnabled)
	{
		return;
	}
	HANDLE colorhandle = GetStdHandle(STD_OUTPUT_HANDLE); //Get a handle on the output text we're modifying
	WORD wOldColorAttrs; //Create a varible to hold old color information
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo; //Create a varible to hold old console information
	GetConsoleScreenBufferInfo(colorhandle, &csbiInfo); //Get the current console information containing the color data
	wOldColorAttrs = csbiInfo.wAttributes; //Move the current color data into our varible
	SetConsoleTextAttribute(colorhandle, FOREGROUND_GREEN | 0 | FOREGROUND_INTENSITY);//Set the console to a bright green on black background for easy visability
	std::string keyEnabledWord = "YES";//Set the default status as yes
	if (keyEnabled == false)//If Windows didnt detect the key press...
	{
		SetConsoleTextAttribute(colorhandle, FOREGROUND_RED | 0 | FOREGROUND_INTENSITY);//Set the color to bright red on black background to alert the user
		keyEnabledWord = "NO";//Change the status to no
	}
	std::cout << keyName << " Status | Detected: " << keyEnabledWord << " Amount: " << keyPressAmount << std::endl;//Print the text to console
	SetConsoleTextAttribute(colorhandle, wOldColorAttrs);//Change the console back to orginal colors
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN))) // If the key message is readable by the below code and is either a SYSKEYDOWN or a KEYDOWn, continue
	{
		KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*)lParam);//Put the lParam into a KBDLLHOOKSTRUCT to read information about the key press
		DWORD dwMsg = 1;
		dwMsg += hooked_key.scanCode << 16; //Shift the scanCode over 16 bits and add that to the dwMessage
		dwMsg += hooked_key.flags << 24; //Shift the flags over 24 bits and add that to the dwMessage
		char lpszKeyName[1024] = { 0 }; //Create a KeyName array with the size of 1024 and add a 0 to it
		int keycode = hooked_key.vkCode;//Set the varible key to the vkCode from the Windows API trigger
		SHIFT_key = GetAsyncKeyState(VK_SHIFT); //Since WinAPI probably uses GetAsyncKeyState we can ask Windows if when we got the key command, any Shift key was pressed down
		LSHIFT_key = GetAsyncKeyState(VK_LSHIFT); //Since WinAPI probably uses GetAsyncKeyState we can ask Windows if when we got the key command, the left Shift key was pressed down
		RSHIFT_key = GetAsyncKeyState(VK_RSHIFT); //Since WinAPI probably uses GetAsyncKeyState we can ask Windows if when we got the key command, the right Shift key was pressed down
		CTRL_key = GetAsyncKeyState(VK_CONTROL); //Since WinAPI probably uses GetAsyncKeyState we can ask Windows if when we got the key command, any Control key was pressed down
		LCTRL_key = GetAsyncKeyState(VK_LCONTROL); //Since WinAPI probably uses GetAsyncKeyState we can ask Windows if when we got the key command, the left Control key was pressed down
		RCTRL_key = GetAsyncKeyState(VK_RCONTROL); //Since WinAPI probably uses GetAsyncKeyState we can ask Windows if when we got the key command, the right Control key was pressed down
		ALT_key = GetAsyncKeyState(VK_MENU); //Since WinAPI probably uses GetAsyncKeyState we can ask Windows if when we got the key command, any Alt key was pressed down
		LALT_key = GetAsyncKeyState(VK_LMENU); //Since WinAPI probably uses GetAsyncKeyState we can ask Windows if when we got the key command, the left Alt key was pressed down
		RALT_key = GetAsyncKeyState(VK_RMENU); //Since WinAPI probably uses GetAsyncKeyState we can ask Windows if when we got the key command, the right Alt key was pressed down

		printf("Actual Keycode = %d\n", keycode); //State the key that was pressed down
		printf("Decoded Keycode = %c\n", keycode); //State the key that was pressed down from its keycode

		//Switch on the keycode for each supported key, change that it has been detected, add to the amount it's been pressed (to detect faulty/failing keyboards, e.g I hit F 100 times but windows only saw 56 of those times, then we know the keyboard is faulty)
		switch (keycode)
		{
			case 0x30:
				Zero_key = true;
				Zero_keyAmount++;
				break;
			case 0x31:
				One_key = true;
				One_keyAmount++;
				break;
			case 0x32:
				Two_key = true;
				Two_keyAmount++;
				break;
			case 0x33:
				Three_key = true;
				Three_keyAmount++;
				break;
			case 0x34:
				Four_key = true;
				Four_keyAmount++;
				break;
			case 0x35:
				Five_key = true;
				Five_keyAmount++;
				break;
			case 0x36:
				Six_key = true;
				Six_keyAmount++;
				break;
			case 0x37:
				Seven_key = true;
				Seven_keyAmount++;
				break;
			case 0x38:
				Eight_key = true;
				Eight_keyAmount++;
				break;
			case 0x39:
				Nine_key = true;
				Nine_keyAmount++;
				break;
			case 0x41:
				A_key = true;
				A_keyAmount++;
				break;
			case 0x42:
				B_key = true;
				B_keyAmount++;
				break;
			case 0x43:
				C_key = true;
				C_keyAmount++;
				break;
			case 0x44:
				D_key = true;
				D_keyAmount++;
				break;
			case 0x45:
				E_key = true;
				E_keyAmount++;
				break;
			case 0x46:
				F_key = true;
				F_keyAmount++;
				break;
			case 0x47:
				G_key = true;
				G_keyAmount++;
				break;
			case 0x48:
				H_key = true;
				H_keyAmount++;
				break;
			case 0x49:
				I_key = true;
				I_keyAmount++;
				break;
			case 0x4A:
				J_key = true;
				J_keyAmount++;
				break;
			case 0x4B:
				K_key = true;
				K_keyAmount++;
				break;
			case 0x4C:
				L_key = true;
				L_keyAmount++;
				break;
			case 0x4D:
				M_key = true;
				M_keyAmount++;
				break;
			case 0x4E:
				N_key = true;
				N_keyAmount++;
				break;
			case 0x4F:
				O_key = true;
				O_keyAmount++;
				break;
			case 0x50:
				P_key = true;
				P_keyAmount++;
				break;
			case 0x51:
				Q_key = true;
				Q_keyAmount++;
				break;
			case 0x52:
				R_key = true;
				R_keyAmount++;
				break;
			case 0x53:
				S_key = true;
				S_keyAmount++;
				break;
			case 0x54:
				T_key = true;
				T_keyAmount++;
				break;
			case 0x55:
				U_key = true;
				U_keyAmount++;
				break;
			case 0x56:
				V_key = true;
				V_keyAmount++;
				break;
			case 0x57:
				W_key = true;
				W_keyAmount++;
				break;
			case 0x58:
				X_key = true;
				X_keyAmount++;
				break;
			case 0x59:
				Y_key = true;
				Y_keyAmount++;
			case 0x5A:
				Z_key = true;
				Z_keyAmount++;
				break;
		}
		if (keycode >= 'A' && keycode <= 'Z') //If the key was within the A to Z range, continue
		{

			if (SHIFT_key >= 0) //If the shift key was pressed (above or equal to 0) 
			{
				keycode += 32;
			}
			//Print status of all keys and amount presses
			if (CTRL_key != 0 && keycode == 's')//Ctrl+S(tatus)
			{
				DisplayKeyStatus("A", A_key, A_keyAmount, false);
				DisplayKeyStatus("B", B_key, B_keyAmount, false);
				DisplayKeyStatus("C", C_key, C_keyAmount, false);
				DisplayKeyStatus("E", E_key, E_keyAmount, false);
				DisplayKeyStatus("F", F_key, F_keyAmount, false);
				DisplayKeyStatus("G", G_key, G_keyAmount, false);
				DisplayKeyStatus("H", H_key, H_keyAmount, false);
				DisplayKeyStatus("I", I_key, I_keyAmount, false);
				DisplayKeyStatus("J", J_key, J_keyAmount, false);
				DisplayKeyStatus("K", K_key, K_keyAmount, false);
				DisplayKeyStatus("L", L_key, L_keyAmount, false);
				DisplayKeyStatus("M", M_key, M_keyAmount, false);
				DisplayKeyStatus("N", N_key, N_keyAmount, false);
				DisplayKeyStatus("O", O_key, O_keyAmount, false);
				DisplayKeyStatus("P", P_key, P_keyAmount, false);
				DisplayKeyStatus("Q", Q_key, Q_keyAmount, false);
				DisplayKeyStatus("R", R_key, R_keyAmount, false);
				DisplayKeyStatus("S", S_key, S_keyAmount, false);
				DisplayKeyStatus("T", T_key, T_keyAmount, false);
				DisplayKeyStatus("U", U_key, U_keyAmount, false);
				DisplayKeyStatus("V", V_key, V_keyAmount, false);
				DisplayKeyStatus("W", W_key, W_keyAmount, false);
				DisplayKeyStatus("X", X_key, X_keyAmount, false);
				DisplayKeyStatus("Y", Y_key, Y_keyAmount, false);
				DisplayKeyStatus("Z", Z_key, Z_keyAmount, false);
			}
			//Quit this and contiune
			if (CTRL_key != 0 && keycode == 'c')//Ctrl+c(ontuine)
			{
				goto EXITKEYBOARDHOOK;
				//TerminateThread(hThread, -3);
				//PostQuitMessage(0);
			}
		}
	}
	SHIFT_key = 0;
	LSHIFT_key = 0;
	RSHIFT_key = 0;
	CTRL_key = 0;
	LCTRL_key = 0;
	RCTRL_key = 0;
	ALT_key = 0;
	LALT_key = 0;
	RALT_key = 0;
	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

DWORD WINAPI KeyboardTestHandler()
{
	isRunningKeyboardTest = true;
	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, NULL, NULL); //Add a Windows hook
	std::cout << "[KeyboardTestHandler] Keyboard test is ready for input!" << std::endl;
	MSG message;
	while (GetMessage(&message, NULL, 0, 0))//Check for new messages
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	EXITKEYBOARDHOOK:
	UnhookWindowsHookEx(hKeyboardHook);//If the message tells us to quit, dump the hook
}
int KeyboardTest()
{
	hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)KeyboardTestHandler, NULL, NULL, &dwThread);//Create a thread
	if (hThread)
	{
		return WaitForSingleObject(hThread, INFINITE);//Wait for the thread to die
	}
	else
	{
		if (A_key && B_key && C_key && D_key && E_key && F_key && G_key && H_key && I_key && J_key && K_key && L_key && M_key && N_key && O_key && P_key && Q_key && R_key && S_key && T_key && U_key && V_key && W_key && X_key && Y_key && Z_key)
		{
			std::cout << "[KeyboardTestHandler] Keyboard test passed!" << std::endl;
		}
		else
		{
			std::cout << "[KeyboardTestHandler] Keyboard test failed!" << std::endl;
			DisplayKeyStatus("A", A_key, A_keyAmount, true);
			DisplayKeyStatus("B", B_key, B_keyAmount, true);
			DisplayKeyStatus("C", C_key, C_keyAmount, true);
			DisplayKeyStatus("E", E_key, E_keyAmount, true);
			DisplayKeyStatus("F", F_key, F_keyAmount, true);
			DisplayKeyStatus("G", G_key, G_keyAmount, true);
			DisplayKeyStatus("H", H_key, H_keyAmount, true);
			DisplayKeyStatus("I", I_key, I_keyAmount, true);
			DisplayKeyStatus("J", J_key, J_keyAmount, true);
			DisplayKeyStatus("K", K_key, K_keyAmount, true);
			DisplayKeyStatus("L", L_key, L_keyAmount, true);
			DisplayKeyStatus("M", M_key, M_keyAmount, true);
			DisplayKeyStatus("N", N_key, N_keyAmount, true);
			DisplayKeyStatus("O", O_key, O_keyAmount, true);
			DisplayKeyStatus("P", P_key, P_keyAmount, true);
			DisplayKeyStatus("Q", Q_key, Q_keyAmount, true);
			DisplayKeyStatus("R", R_key, R_keyAmount, true);
			DisplayKeyStatus("S", S_key, S_keyAmount, true);
			DisplayKeyStatus("T", T_key, T_keyAmount, true);
			DisplayKeyStatus("U", U_key, U_keyAmount, true);
			DisplayKeyStatus("V", V_key, V_keyAmount, true);
			DisplayKeyStatus("W", W_key, W_keyAmount, true);
			DisplayKeyStatus("X", X_key, X_keyAmount, true);
			DisplayKeyStatus("Y", Y_key, Y_keyAmount, true);
			DisplayKeyStatus("Z", Z_key, Z_keyAmount, true);
			std::cin.get();
		}
	}
}

//This is the message displayed when the tool runs
void StartUpMessage()
{
	std::cout << "***************************************************" << std::endl;
	std::cout << "**                    OSCTT                      **" << std::endl;
	std::cout << "***************************************************" << std::endl;
	std::cout << "** Open Source Computer Test Tool is a computer  **" << std::endl;
	std::cout << "** testing tool for testing mice, keyboards, etc **" << std::endl;
	std::cout << "***************************************************" << std::endl;
	std::cout << "** Wait 5 seconds for this tool to start...      **" << std::endl;
	std::cout << "***************************************************" << std::endl;
	//Sleep(5000);
}
int main()
{
	StartUpMessage();
	KeyboardTest();
	std::cin.get();
}

