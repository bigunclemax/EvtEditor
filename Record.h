#pragma once
#include <vector>
#include <Windows.h>
#include <list>
#include "evt_structs.h"

using namespace std;

class Record
{
public:
	Record();
	Record(vector<char> inRecordData);
	Record(char* input_buffer, int input_buffer_size);
	~Record();
	list<string> GetStrings();
	
	LOGRECORD * header;
	
protected:
	char * m_rawData;

};

