#include "EvtLogJournal.h"



EvtLogJournal::EvtLogJournal()
{
}


EvtLogJournal::~EvtLogJournal()
{
	this->Clear();
}

void EvtLogJournal::SaveFile(char * path)
{
}

void EvtLogJournal::ReadFile(char * path)
{
	ifstream f_journal(path, ios::binary);
	
	if (!f_journal.is_open())
	{
		return;
	}

	f_journal.seekg(0, f_journal.end);
	vector<char> in_vector(f_journal.tellg());
	f_journal.seekg(0, f_journal.beg);

	//read file
	in_vector.assign(std::istreambuf_iterator<char>(f_journal), std::istreambuf_iterator<char>());

	f_journal.close();

	uint32_t journal_size = in_vector.size();

	// copy header
	for (int i = 0; i < EVT_HEADER_SIZE; i++)
	{
		_m_header_buffer[i] = in_vector[i];
	}
	header = reinterpret_cast<PLOGHEADER>(_m_header_buffer);

	// find eof record
	uint32_t eof_offset = findEofOffset(in_vector);
	PLOGEOF eof_record = reinterpret_cast<PLOGEOF>(&in_vector[eof_offset]);
	
	uint32_t start_offset = eof_record->BeginRecord;

	int record_offset = start_offset;

	int counter = 0;
	while (true)
	{
		int record_length = *reinterpret_cast<uint32_t*>(&in_vector[record_offset]);

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
				record_buffer[buff_idx] = in_vector[i];
			}
			for (int i = EVT_HEADER_SIZE; i < record_end_offset; i++, buff_idx++)
			{
				record_buffer[buff_idx] = in_vector[i];
			}
		}
		else
		{
			record_end_offset = record_offset + record_length;
			for (int i = record_offset; i < record_end_offset; i++, buff_idx++)
			{
				record_buffer[buff_idx] = in_vector[i];
			}
		}

		//new record

		Record * rcrd = new Record(record_buffer, record_length);
		_m_recordsList.push_back(rcrd);
		qDebug() << "# " << counter++ << "Record ID: " << rcrd->header->RecordNumber;

		//free buffer
		delete record_buffer;

		//check if 0x00000027 pattern
		if (0x00000027 == *reinterpret_cast<uint32_t*>(&in_vector[record_end_offset]))
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

void EvtLogJournal::Clear()
{
	//clear header
	delete _m_header_buffer;
	//clear all data
	for (list<Record*>::iterator iter = _m_recordsList.begin(); iter != _m_recordsList.end(); iter++)
	{
		delete *iter;
	}
	//clear list
	_m_recordsList.clear();
}

const list<Record*>& EvtLogJournal::Records()
{
	return _m_recordsList;
}

int EvtLogJournal::findEofOffset(vector<char>& in_vec)
{
	for (int idx = EVT_HEADER_SIZE; idx < in_vec.size() - EVT_EOF_SIZE; idx += 4)
	{
		const uint32_t * data = reinterpret_cast<const uint32_t*>(&in_vec[idx]);

		if (*data == 0x11111111)
		{
			if ((*(data + 1) == 0x22222222) &&
				(*(data + 2) == 0x33333333) &&
				(*(data + 3) == 0x44444444))
			{
				return (idx - 4);	//go back to record start
			}
		}
	}
	return 0;
}
