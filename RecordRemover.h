#pragma once

#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>    // std::swap_ranges
#include <QByteArray>
#include "Record.h"

using namespace std;

#define EVT_SIGNATURE	0x654c664c //"eLfL signature"
#define EVT_SIGN_SIZE	4
#define EVT_HEADER_SIZE_DW 12 // 48/4
#define SIZE_IN_DW(i)	i>>2
#define DW_SIZE			4
#define EVT_HEADER_SIZE 0x30
#define EVT_EOF_SIZE	0x28

class RecordRemover
{
public:
	RecordRemover();
	RecordRemover(char*);
	~RecordRemover();

	void RE(uint32_t recOffset, uint32_t prevRecOffset);

	bool Open(char *);
	bool Open(QByteArray &);
	void Close();
	bool Save(char *);
	bool Save(QByteArray&);
	bool RemoveRecord3(uint32_t id);
	bool RemoveRecord2(uint32_t id);
	bool RemoveRecord(uint32_t);
	void ReadJournal(char * path);

private:
	
	ifstream * m_pJournal;
	vector<char> m_inVector;
	vector<uint32_t> m_outVector;
	uint32_t * m_pOutputBuff;

	int findRecordOffset(uint32_t);
	uint32_t getStartOffset();
	uint32_t getEndOffset();
	int		eofRecordOffset();
	int		recordLength(uint32_t );
	bool	copyHeader();
	bool	readJournal();
	uint32_t GetOffsetByID(uint32_t id);


};

