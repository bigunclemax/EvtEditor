#include "RecordRemover.h"
#include <QDebug>


RecordRemover::RecordRemover()
{
	m_pJournal = new ifstream();
}

RecordRemover::RecordRemover(char * path)
{
	m_pJournal = new ifstream(path, std::ifstream::ate | std::ifstream::binary);
	if (m_pJournal->is_open())
	{
		m_inVector.reserve(m_pJournal->tellg());
		m_pJournal->seekg(0,m_pJournal->beg);
		m_inVector.assign(std::istreambuf_iterator<char>(*m_pJournal),
			std::istreambuf_iterator<char>());
	}
}

bool RecordRemover::Open(char * path)
{
	m_pJournal->open(path,ios::binary);
	if (!m_pJournal->is_open())
	{
		return false;
	}

	m_pJournal->seekg(0,m_pJournal->end);
	m_inVector.reserve(m_pJournal->tellg());
	m_pJournal->seekg(0,m_pJournal->beg);

	m_inVector.assign(std::istreambuf_iterator<char>(*m_pJournal), std::istreambuf_iterator<char>());
	

	return true;
}

bool RecordRemover::Open(QByteArray & byteArray)
{
	m_inVector = std::vector<char>(byteArray.begin(), byteArray.end());
	return true;
}

void RecordRemover::Close()
{
	if (m_pJournal->is_open())
	{
		m_pJournal->close();
	}
}

bool RecordRemover::Save(char * path)
{
	ofstream file(path,ios::binary);
	if (!file.is_open()) 
	{
		return false;
	}

	

	std::copy(m_inVector.begin(), m_inVector.end(), std::ostreambuf_iterator<char>(file));

	/*for (int i = 0; i < m_inVector.size(); i++)
	{
		file.write(,sizeof(EVT_SIGN_SIZE));
	}

	uint32_t buff;
	while ((!m_pJournal->fail()) || (!m_pJournal->eof()))
	{
		m_pJournal->read(reinterpret_cast<char*>(&buff), sizeof(EVT_SIGN_SIZE));
		m_inVector.push_back(buff);
	}
	return false;*/
	file.close();

	return true;
}

bool RecordRemover::Save(QByteArray & byteArray)
{
	byteArray = QByteArray(reinterpret_cast<const char*>(&m_inVector[0]),m_inVector.size());
	return true;
}

uint32_t RecordRemover::GetOffsetByID(uint32_t id)
{
	uint32_t idx = EVT_HEADER_SIZE; //skip header
	while (idx < (m_inVector.size() - 2 * DW_SIZE)) {
		if (*(reinterpret_cast<const uint32_t*>(&m_inVector[idx + DW_SIZE])) != EVT_SIGNATURE)
		{
			idx += DW_SIZE;
			continue;
		}
		if (*(reinterpret_cast<const uint32_t*>(&m_inVector[idx + DW_SIZE*2])) == id)
		{
			return idx;  // go to record start
		}
		idx += *reinterpret_cast<const uint32_t*>(&m_inVector[idx]);
	}
	return 0;
}

bool RecordRemover::RemoveRecord3(uint32_t id)
{

	int eofOffset = getEndOffset();

	int startOffset = getStartOffset();

	//rotate all
	rotate(m_inVector.begin() + EVT_HEADER_SIZE, m_inVector.begin() + startOffset, m_inVector.end());

	uint32_t delOffset = GetOffsetByID(id);
	uint32_t delLength = *reinterpret_cast<const uint32_t*>(&m_inVector[delOffset]);
	
	//zeroed record
	for (int i = 0; i < delLength; i++)
	{
		m_inVector[delOffset + i] = 0;
	}

	//fix eof offsets
	if ((eofOffset - startOffset) < EVT_HEADER_SIZE)
	{
		eofOffset = m_inVector.size() - (startOffset - eofOffset);
	}
	else
	{
		eofOffset -= startOffset;
	}

	*(reinterpret_cast<uint32_t*>(&m_inVector[eofOffset]) + 5) = EVT_HEADER_SIZE;
	*(reinterpret_cast<uint32_t*>(&m_inVector[eofOffset]) + 6) = eofOffset - delLength;
	
	rotate(m_inVector.begin() + delOffset, m_inVector.begin() + delOffset + delLength, m_inVector.end());

	return true;
}
bool RecordRemover::RemoveRecord2(uint32_t id) 
{
	int eofOffset = getEndOffset();
	int delOffset = findRecordOffset(id);
	int delLength = recordLength(delOffset);
	int journalSize = m_inVector.size();
	
	int nextForEofOffset = eofOffset + EVT_EOF_SIZE;
	int nextForDelOffset = delOffset + delLength;

	//zeroed record
	for (int i = 0; i < delLength; i++)
	{
		m_inVector[i + delOffset] = 0;
	}

	//fix next for eof
	if ((nextForEofOffset) > journalSize)
	{
		nextForEofOffset -=  (journalSize - EVT_HEADER_SIZE);
	}

	//fix eof offsets
	int newEofOffset = 0;
	if ((eofOffset - delLength) < EVT_HEADER_SIZE)
	{
		newEofOffset = m_inVector.size() - (delLength - eofOffset);
	}
	else
	{
		newEofOffset = eofOffset -delLength;
	}
	uint32_t * ptr_eof_offset = (reinterpret_cast<uint32_t*>(&m_inVector[eofOffset]) + 6);
	*ptr_eof_offset = eofOffset - delLength;

	if (nextForEofOffset > delOffset)
	{
		//rotate
		rotate(m_inVector.begin() + delOffset, m_inVector.begin() + nextForDelOffset, m_inVector.begin() + nextForEofOffset);
	}
	else
	{
		rotate(m_inVector.begin() + delOffset, m_inVector.begin() + nextForDelOffset, m_inVector.end());
		uint32_t upperSize = nextForEofOffset - EVT_HEADER_SIZE;
		if (upperSize <= delLength) 
		{
			swap_ranges(m_inVector.begin() + EVT_HEADER_SIZE, m_inVector.begin() + nextForEofOffset, m_inVector.end() - delLength);
		}
		else
		{
			swap_ranges(m_inVector.begin() + EVT_HEADER_SIZE, m_inVector.begin() + delLength, m_inVector.end() - delLength);
			rotate(m_inVector.begin() + EVT_HEADER_SIZE, m_inVector.begin() + EVT_HEADER_SIZE + delLength, m_inVector.begin() + nextForEofOffset);
		}
	}
	return true;
}

bool RecordRemover::RemoveRecord(uint32_t id)
{
//	readJournal();
//	copyHeader();

	//get EOF_RECORD_OFFSET
	uint32_t eofOffset = getEndOffset();
	uint32_t delOffset = findRecordOffset(id);
	uint32_t delLength = recordLength(delOffset);
	uint32_t journalSize = m_inVector.size();

	uint32_t nextForEofOffset = eofOffset + EVT_EOF_SIZE;

	for (int i = 0; i < delLength; i++)
	{
		m_inVector[i + delOffset] = 0;
	}

	if ((delOffset < eofOffset) && ((nextForEofOffset) <= journalSize))
	{
		//zeroed removed record
		//fix last record offset
		uint32_t * ptr_eof_offset = (reinterpret_cast<uint32_t*>(&m_inVector[eofOffset]) + 6);
		*ptr_eof_offset = eofOffset - delLength;
		//rotate
		uint32_t nextForDelOffset = delOffset + delLength;
		

		rotate(m_inVector.begin() + delOffset, m_inVector.begin() + nextForDelOffset, m_inVector.begin() + nextForEofOffset);
	}
	else
	{
		//fix last record offset
		uint32_t * ptr_eof_offset = (reinterpret_cast<uint32_t*>(&m_inVector[eofOffset]) + 6);
		if (eofOffset > EVT_HEADER_SIZE + delLength) 
		{
			*ptr_eof_offset = eofOffset - delLength;
		}
		else
		{
			*ptr_eof_offset = journalSize + eofOffset - delLength;
		}
		
		//fix next for eof
		if ((nextForEofOffset) > journalSize)
		{
			nextForEofOffset = eofOffset + EVT_EOF_SIZE - journalSize + EVT_HEADER_SIZE;
		}
				
		//copy  part from start del_record to endfile
		uint32_t downPartSize = journalSize - delOffset;
		uint32_t upperPartSize = nextForEofOffset - EVT_HEADER_SIZE;

		vector<char> tmp;
		tmp.resize(downPartSize + upperPartSize);

		copy(m_inVector.begin() + delOffset, m_inVector.end(), tmp.begin());
		
		//copy  part from start_file to next_from_eof
		copy(m_inVector.begin() + EVT_HEADER_SIZE, m_inVector.begin() + nextForEofOffset, tmp.begin()+ downPartSize);
		rotate(tmp.begin(), tmp.begin() + delLength, tmp.end());
		//zeroed

		//back copy part from start del_record to endfile
		copy(tmp.begin(), tmp.begin() + downPartSize, m_inVector.begin() + delOffset);
		copy(tmp.begin()+ downPartSize, tmp.end(), m_inVector.begin() + EVT_HEADER_SIZE);

	}

/*

	if (delOffset < eofOffset)
	{
		*(reinterpret_cast<uint32_t*>(&m_inVector[endRecOffset]) + 6) = endRecOffset - recLength;
		rotate(m_inVector.begin() + remRecOffset, m_inVector.begin() + remRecOffset + recLength, m_inVector.begin() + endRecOffset + EVT_EOF_SIZE);
	}


	uint32_t remRecOffset = findRecordOffset(id);
	uint32_t startRecOffset = getStartOffset();
	uint32_t endRecOffset = getEndOffset();
	uint32_t recLength = recordLength(remRecOffset);

	if (remRecOffset < endRecOffset)
	{
		*(reinterpret_cast<uint32_t*>(&m_inVector[endRecOffset]) + 6) = endRecOffset - recLength;
		rotate(m_inVector.begin() + remRecOffset, m_inVector.begin() + remRecOffset + recLength, m_inVector.begin() + endRecOffset + EVT_EOF_SIZE);
	}
	else 
	{
		rotate(m_inVector.begin() + remRecOffset, m_inVector.begin() + remRecOffset + recLength, m_inVector.end());
		
		if (recLength >= endRecOffset + EVT_EOF_SIZE)
		{
			*(reinterpret_cast<uint32_t*>(&m_inVector[endRecOffset]) + 6) = m_inVector.size() - recLength;
			swap_ranges(m_inVector.begin(), m_inVector.begin() + EVT_EOF_SIZE -1, m_inVector.end() - recLength);
		}
		else 
		{
			*(reinterpret_cast<uint32_t*>(&m_inVector[endRecOffset]) + 6) = endRecOffset - recLength;
			swap_ranges(m_inVector.begin(), m_inVector.begin() + recLength -1, m_inVector.end()  - recLength);
			rotate(m_inVector.begin(), m_inVector.begin() + recLength, m_inVector.begin() + endRecOffset + EVT_EOF_SIZE);
		}
	}*/
	return true;
}

void RecordRemover::ReadJournal(char * path)
{
	m_pJournal->open(path, ios::binary);
	if (!m_pJournal->is_open())
	{
		return;
	}

	m_pJournal->seekg(0, m_pJournal->end);
	m_inVector.reserve(m_pJournal->tellg());
	m_pJournal->seekg(0, m_pJournal->beg);

	m_inVector.assign(std::istreambuf_iterator<char>(*m_pJournal), std::istreambuf_iterator<char>());

	uint32_t start_offset = getStartOffset();
	uint32_t eof_offset = getEndOffset();
	uint32_t journal_size = m_inVector.size();

	int record_offset = start_offset;
	
	int counter = 0;
	while (true)
	{
		int record_length = *reinterpret_cast<uint32_t*>(&m_inVector[record_offset]);
		
		//alloc buffer
		char * record_buffer;
		record_buffer = new char[record_length];

		int record_end_offset = 0;
		
		int buff_idx = 0;
		if (record_offset + record_length >= journal_size)
		{
			//record wrapped
			record_end_offset = EVT_HEADER_SIZE + (record_length - (journal_size - record_offset));
			
			for (int i = record_offset; i < journal_size; i++, buff_idx++)
			{
				record_buffer[buff_idx] = m_inVector[i];
			}
			for (int i = EVT_HEADER_SIZE; i < record_end_offset; i++, buff_idx++)
			{
				record_buffer[buff_idx] = m_inVector[i];
			}
		}
		else
		{
			record_end_offset = record_offset + record_length;
			for (int i = record_offset; i < record_end_offset; i++, buff_idx++)
			{
				record_buffer[buff_idx] = m_inVector[i];
			}
		}

		//new record
		
		Record * rcrd = new Record(record_buffer, record_length);
		qDebug() << "# " << counter++ << "Record ID: " << rcrd->header->RecordNumber;

		//free buffer
		delete record_buffer;

		//check if 0x00000027 pattern
		if (0x00000027 == *reinterpret_cast<uint32_t*>(&m_inVector[record_end_offset]))
		{
			record_offset = EVT_HEADER_SIZE;
		}
		else
		{
			record_offset = record_end_offset;
		}

		//check if eof
		if (eof_offset == record_offset)
		{
			break;
		}
	}
}



int RecordRemover::findRecordOffset(uint32_t id)
{
	for (int idx = EVT_HEADER_SIZE; idx < m_inVector.size(); idx += DW_SIZE)
	{
		if (*reinterpret_cast<const uint32_t*>(&m_inVector[idx]) != EVT_SIGNATURE)
		{
			continue;
		}
		
		if (*reinterpret_cast<const uint32_t*>(&m_inVector[idx + DW_SIZE]) != id)
		{
			continue;
		}
		return (idx - DW_SIZE);  // go to record start
	}
	return -1;
}

uint32_t RecordRemover::getStartOffset()
{
	int eofRecOffset = this->eofRecordOffset();
	
	const uint32_t * startRecordOffset_DW = reinterpret_cast<const uint32_t*>(&m_inVector[eofRecOffset]);

	return *(startRecordOffset_DW+5);
}

uint32_t RecordRemover::getEndOffset()
{
	int eofRecOffset = this->eofRecordOffset();

	const uint32_t * startRecordOffset_DW = reinterpret_cast<const uint32_t*>(&m_inVector[eofRecOffset]);

	return *(startRecordOffset_DW + 6);
}

int RecordRemover::eofRecordOffset()
{
	for (int idx = EVT_HEADER_SIZE; idx < m_inVector.size() - EVT_EOF_SIZE; idx += DW_SIZE)
	{
		const uint32_t * data = reinterpret_cast<const uint32_t*>(&m_inVector[idx]);

		if (*data == 0x11111111)
		{
			if ((*(data + 1) == 0x22222222) &&
				(*(data + 2) == 0x33333333) &&
				(*(data + 3) == 0x44444444))
			{
				return (idx - DW_SIZE);	//go back to record start
			}
		}
	}
	return -1;
}

int RecordRemover::recordLength(uint32_t offset)
{
	return *reinterpret_cast<const uint32_t*>(&m_inVector[offset]);
}

bool RecordRemover::copyHeader()
{
	if (m_inVector.size() < EVT_HEADER_SIZE_DW) {
		return false;
	}

	for (int i = 0; i < EVT_HEADER_SIZE_DW; i++)
	{
		m_outVector[i] = m_inVector[i];
	}
	return true;
}

bool RecordRemover::readJournal()
{
	if (!m_pJournal->is_open())
	{
		return false;
	}

	m_inVector.clear();
	m_pJournal->seekg(0, m_pJournal->beg);
	
	uint32_t buff;
	while ((!m_pJournal->fail()) || (!m_pJournal->eof()))
	{
		m_pJournal->read(reinterpret_cast<char*>(&buff), sizeof(EVT_SIGN_SIZE));
		m_inVector.push_back(buff);
	}
	return true;
}

RecordRemover::~RecordRemover()
{
	delete m_pJournal;
}

void RecordRemover::RE(uint32_t recOffset, uint32_t prevRecOffset)
{
	if (0 == prevRecOffset) 
	{
		//first record!
		//need update EOF record!
		return;
	}
		
	uint32_t prevRecLength = *reinterpret_cast<const uint32_t*>(&m_inVector[prevRecOffset]);
	uint32_t recLength = *reinterpret_cast<const uint32_t*>(&m_inVector[recOffset]);
	uint32_t journalSize = m_inVector.size();
	uint32_t endPrevRecOffset;
	uint32_t endRecOffset;

	//find  pevious_record end offset
	if (prevRecOffset + prevRecLength > journalSize) 
	{
		endPrevRecOffset = EVT_HEADER_SIZE + (prevRecLength - (journalSize - prevRecOffset));
	}
	else 
	{
		endPrevRecOffset = prevRecOffset + prevRecLength;
	}

	//find record end offset
	if (recOffset + recLength > journalSize)
	{
		endRecOffset = EVT_HEADER_SIZE + (recLength - (journalSize - recOffset));
	}
	else
	{
		endRecOffset = recOffset + recLength;
	}

	//zeroed prev length at the end of record
	m_inVector[endPrevRecOffset -4] = 0x00000000;

	//zeroed from to ...
	int startOffset = endPrevRecOffset;
	int endOffset = endRecOffset;

	uint32_t written = 0;
	if (startOffset > endOffset) 
	{
		
		for (int i = startOffset; i < journalSize; i++, written++)
		{
			m_inVector[i] = 0x00;
		}
		for (int i = EVT_HEADER_SIZE; i < endOffset; i++, written++)
		{
			m_inVector[i] = 0x00;
		}
	}
	else
	{
		for (int i = startOffset; i < endOffset; i++, written++)
		{
			m_inVector[i] = 0x00;
		}
	}

	//update prev record length
	*reinterpret_cast<uint32_t*>(&m_inVector[endOffset - 4])  = written + prevRecLength;
	*reinterpret_cast<uint32_t*>(&m_inVector[prevRecOffset]) = written + prevRecLength;
}