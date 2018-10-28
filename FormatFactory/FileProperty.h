#pragma once

#include <fstream>
using namespace std;

class AFX_EXT_CLASS FileProperty
{
public:
	FileProperty(void);
	~FileProperty(void);

	typedef enum tagFileType
	{
		FileType_Lidar = 0,
		FileType_Product,
		FileType_Sum
	}FileType;

	static const unsigned short FormatVersion = 1;

public:
	void Store(ofstream& ofp);
	void Restore(ifstream& ifp);
};

