#include "cShell.h"
#include "conio.h"//键盘
#include"iostream"

using namespace std;

extern char cmd;
cShell::cShell()
{
}


cShell::~cShell()
{
}



//==============================================

char cShell::getCommand()
{
	key = '0';
	if (_kbhit())
	{//有按键
		key = _getch();
		switch (key)
		{
		case 'q':			
			break;
		case 's':	
			cout << "start" << endl;
			cmd = 's'; //轨迹开始的标志
			break;
		default:
			printf("\n命令错误.\n");
		}
	}
	return key;

}
