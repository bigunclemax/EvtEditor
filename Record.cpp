#include "Record.h"



Record::Record()
{
}

Record::Record(vector<char> inRecordData)
{
	m_rawData = new char[inRecordData.size()];
	std::copy(inRecordData.begin(), inRecordData.end(), m_rawData);
	header = reinterpret_cast<LOGRECORD * >(m_rawData);
}

Record::Record(char * input_buffer, int input_buffer_size)
{
	m_rawData = new char[input_buffer_size];
	memcpy(m_rawData, input_buffer, input_buffer_size);
	header = reinterpret_cast<LOGRECORD * >(m_rawData);
}


Record::~Record()
{
	delete m_rawData;
}

list<string> Record::GetStrings()
{

	return list<string>();
}
