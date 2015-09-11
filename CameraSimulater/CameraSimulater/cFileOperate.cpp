#include "cFileOperate.h"


cFileOperate::cFileOperate(string txtpath)
{
	myfile = ifstream(txtpath);
	if (!myfile) 
	{
		cout << "Unable to open myfile";
		getchar();
		exit(0);
	}

}


cFileOperate::~cFileOperate()
{
}

int cFileOperate::LoadFullFile()
{
	float a, b,c,d;

	int i = 0;

	while (!myfile.eof() && i < 500)
	{
		myfile.getline(buffer, 100);
		sscanf_s(buffer, "%f %f %f %f", &a, &b, &c, &d);
	//	cout << a << " " << b << " "<< c << " " << d << endl;
		data_Read[i][0] = a;
		data_Read[i][1] = b;
		data_Read[i][2] = c;
		data_Read[i][3] = d;
		i++;
	}
	myfile.close();

	return i;
}
