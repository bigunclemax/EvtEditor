#pragma once

#include <list>
#include <fstream>
#include <QDebug>
#include "Record.h"

using namespace std;

class EvtLogJournal
{
public:
	EvtLogJournal();
	~EvtLogJournal();
	void SaveFile(char * path);
	void ReadFile(char * path);
	void Clear();
	const list<Record*>& Records();

	PLOGHEADER header;
private:
	list<Record *> _m_recordsList;
	char* _m_header_buffer;
	int findEofOffset(vector<char>& in_vec);

};

