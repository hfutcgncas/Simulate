#include "cShell.h"
#include "conio.h"//����
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
	{//�а���
		key = _getch();
		switch (key)
		{
		case 'q':			
			break;
		case 's':	
			cout << "start" << endl;
			cmd = 's'; //�켣��ʼ�ı�־
			break;
		default:
			printf("\n�������.\n");
		}
	}
	return key;

}
