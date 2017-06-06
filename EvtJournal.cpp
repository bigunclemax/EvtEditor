#include "EvtJournal.h"

EvtJournal::EvtJournal()
{
	int res = libevt_file_initialize(&m_journalFile, &m_evtError);
}


EvtJournal::~EvtJournal()
{
	if (NULL != m_journalFile) {
		int res = libevt_file_free(&m_journalFile, &m_evtError);
	}
}

/* Opens a file journal by specified path
* Returns 1 if successful or -1 on error
*/
int EvtJournal::Open(const wchar_t* pathToFile)
{
	int res =0;
	if (-1 == libevt_file_open_wide(m_journalFile,
									pathToFile,
									LIBEVT_ACCESS_FLAG_READ,
									&m_evtError)) 
	{
		goto error;
	}
	
	//--read header--

	//get log version
	if (-1 == libevt_file_get_format_version(
		m_journalFile,
		&m_minorVer,
		&m_majorVer,
		&m_evtError)) 
	{
		goto error;
	}

	//check if journal file corrupted /?
	if (-1 == libevt_file_is_corrupted(
		m_journalFile,
		&m_evtError)) 
	{
		goto error;
	}

	//get number of records
	if (-1 == libevt_file_get_number_of_records(
		m_journalFile,
		&m_numRecords,
		&m_evtError)) 
	{
		goto error;
	}

	//get number of recovered records
	if (-1 == libevt_file_get_number_of_recovered_records(
		m_journalFile,
		&m_numRecoveredRecords,
		&m_evtError)) 
	{
		goto error;
	}
	
	//--read event records--

	if (-1 == this->ExtractRecordsFromFile()) 
	{
		goto error;
	};

	//--read recovered event records--

	if (-1 == this->ExtractRecoveredRecordsFromFile())
	{
		goto error;
	};

	return 1;
error:
	return -1;
}

void EvtJournal::Close()
{
	if (NULL != m_journalFile) {
		int res = libevt_file_free(&m_journalFile, &m_evtError);
	}

	m_vecEvtRecords.clear();
	m_vecEvtRecoveredRecords.clear();
	m_numRecords = 0;
	m_numRecoveredRecords = 0;
	
}

int EvtJournal::ExtractRecord(libevt_record_t * inEvtRecord, EventRecord * extractedRecord) 
{
	
	/* get the record offset */
	if (-1 == libevt_record_get_offset(
		inEvtRecord,
		reinterpret_cast<off64_t *>(&extractedRecord->RecordOffset),
		&m_evtError))
	{
		goto error;
	}

	/* get the identifier (record number)	*/
	if(-1 == libevt_record_get_identifier(
		inEvtRecord,
		&extractedRecord->RecordNumber,
		&m_evtError))
	{
		goto error;
	}

	/* Retrieves the event type	*/
	if (-1 == libevt_record_get_event_type(
		inEvtRecord,
		&extractedRecord->EventType,
		&m_evtError))
	{
		goto error;
	}

	/* Retrieves the event category	*/
	if (-1 == libevt_record_get_event_category(
		inEvtRecord,
		&extractedRecord->EventCategory,
		&m_evtError))
	{
		goto error;
	}

	/* Retrieves the creation time timestamp is a 32-bit POSIX date and time value	*/
	if (-1 == libevt_record_get_creation_time(
		inEvtRecord,
		&extractedRecord->PosixTime,
		&m_evtError))
	{
		goto error;
	}

	/* Retrieves the event identifier	*/
	if (-1 == libevt_record_get_event_identifier(
		inEvtRecord,
		&extractedRecord->EventID,
		&m_evtError))
	{
		goto error;
	}

	/* Retrieves the UTF-8 encoded computer name	*/
	///	libevt_record_get_utf8_computer_name_size
	///	libevt_record_get_utf8_computer_name

	if (-1 == ExtractStringFromRecord(
		libevt_record_get_utf8_computer_name_size,
		libevt_record_get_utf8_computer_name,
		inEvtRecord,
		&extractedRecord->ComputerName))
	{
		goto error;
	};

	/* Retrieves the UTF-16 encoded source name	*/
	///	int libevt_record_get_utf16_source_name
	///	int libevt_record_get_utf16_source_name_size

	if (-1 == ExtractStringFromRecord(
		libevt_record_get_utf8_source_name_size,
		libevt_record_get_utf8_source_name,
		inEvtRecord,
		&extractedRecord->SourceName))
	{
		goto error;
	};


	/* Retrieves the UTF-8 encoded user security identifier	*/
	/// libevt_record_get_utf8_user_security_identifier
	/// libevt_record_get_utf8_user_security_identifier_size
		
	if (-1 == ExtractStringFromRecord(
		libevt_record_get_utf8_user_security_identifier_size,
		libevt_record_get_utf8_user_security_identifier,
		inEvtRecord,
		&extractedRecord->UserSID))
	{
		goto error;
	};

	return 1;
error:
	return -1;
}	

int EvtJournal::ExtractRecordStrings(libevt_record_t * evt_record, std::vector<std::wstring>* vecStrings)
{

	int numStrings = 0;

	if (-1 == libevt_record_get_number_of_strings(
		evt_record,
		&numStrings,
		&m_evtError)) 
	{
		goto error;
	}

	libevt_error_t* evtError = NULL; //TODO: разобраться с типами ошибки
	size_t stringSize;

	for (int stringIndex = 0; stringIndex < numStrings; stringIndex++)
	{
		if (-1 == libevt_record_get_utf16_string_size(
			evt_record,
			stringIndex,
			&stringSize,
			&evtError))
		{
			goto error;
		}

		if (stringSize == 0) {
			goto error;
		}

		wchar_t* stringBuffer = new wchar_t[stringSize];

		if (-1 == libevt_record_get_utf16_string(
			evt_record,
			stringIndex,
			(uint16_t *)stringBuffer,
			stringSize,
			&evtError))
		{
			delete stringBuffer;
			goto error;
		}

		std::wstring wstr(stringBuffer);
		vecStrings->push_back(wstr);
		delete stringBuffer;

	}
	
	return 1;
error:
	return -1;
}

int EvtJournal::ExtractRecordsFromFile()
{
	//clear prev results
	m_vecEvtRecords.clear();
	m_vecEvtRecords.resize(m_numRecords);

	libevt_record_t* p_record;
	EventRecord extractedEvtRecord;

	for (unsigned int recordIndex = 0; recordIndex < m_numRecords; recordIndex++)
	{
		p_record = NULL;
		//get record by index
		if (1 == libevt_file_get_record(
			m_journalFile,
			recordIndex,
			&p_record,
			&m_evtError)) 
		{
			//m_vecRecords.push_back(p_record); 
			if (-1 == ExtractRecord(p_record, &extractedEvtRecord)) {
				//TODO: handle error
				return -1;
			}
			else {
				extractedEvtRecord.Index = recordIndex;
				//m_vecEvtRecords.push_back(extractedEvtRecord);
				m_vecEvtRecords[recordIndex] = extractedEvtRecord;
			}
			if (-1 == libevt_record_free(&p_record, &m_evtError)) {
				//TODO: handle error
				return -1;
			}
		}
		else
		{
			//TODO: check on error
			return -1;
		}
	}

	if (0 == m_vecEvtRecords.size()) {
		return 0;
	}

	return 1;
}

int EvtJournal::ExtractRecoveredRecordsFromFile()
{
	libevt_record_t* p_record;
	EventRecord extractedEvtRecord;

	for (unsigned int recordIndex = 0; recordIndex < m_numRecoveredRecords; recordIndex++)
	{
		p_record = NULL;
		//get record by index
		if (1 == libevt_file_get_recovered_record(
			m_journalFile,
			recordIndex,
			&p_record,
			&m_evtError))
		{
			//m_vecRecords.push_back(p_record); 
			if (-1 == ExtractRecord(p_record, &extractedEvtRecord)) {
				//TODO: handle error
				return -1;
			}
			else {
				extractedEvtRecord.Index = recordIndex;
				m_vecEvtRecoveredRecords.push_back(extractedEvtRecord);
			}
			if (-1 == libevt_record_free(&p_record, &m_evtError)) {
				//TODO: handle error
				return -1;
			}
		}
		else
		{
			//TODO: check on error
			return -1;
		}
	}

	if (0 == m_vecEvtRecords.size()) {
		return 0;
	}

	return 1;
}

std::vector<EventRecord> EvtJournal::GetRecords() const
{
	return m_vecEvtRecords;
}

std::vector<EventRecord> EvtJournal::GetRecoveredRecords() const
{
	return m_vecEvtRecoveredRecords;
}

std::vector<std::wstring> EvtJournal::GetRecordStrings(int recordIndex)
{
	std::vector<std::wstring> vecEvtStings;
	//get record from file by index
	libevt_record_t* p_record = NULL;
	//get record by index
	if (-1 == libevt_file_get_record(
		m_journalFile,
		recordIndex,
		&p_record,
		&m_evtError))
	{
		//TODO: check on error
		//
		goto error;
	}

	if (-1 == ExtractRecordStrings(p_record, &vecEvtStings)) {
		//TODO: handle error
		///Can't extract record strings
		goto error;
	}
		
	if (-1 == libevt_record_free(&p_record, &m_evtError)) {
		///Can't free memory
		//TODO: handle error
		goto error;
	}
	
	return vecEvtStings;
error:
	//return empty vector if error 
	return std::vector<std::wstring>();;
}

std::vector<std::wstring> EvtJournal::GetRecoveredRecordStrings(int recordIndex)
{
	std::vector<std::wstring> vecEvtStings;
	//get record from file by index
	libevt_record_t* p_record = NULL;
	//get record by index
	if (-1 == libevt_file_get_recovered_record(
		m_journalFile,
		recordIndex,
		&p_record,
		&m_evtError))
	{
		//TODO: check on error
		//
		goto error;
	}

	if (-1 == ExtractRecordStrings(p_record, &vecEvtStings)) {
		//TODO: handle error
		///Can't extract record strings
		goto error;
	}

	if (-1 == libevt_record_free(&p_record, &m_evtError)) {
		///Can't free memory
		//TODO: handle error
		goto error;
	}

	return vecEvtStings;
error:
	//return empty vector if error 
	return std::vector<std::wstring>();;
}

int EvtJournal::GetNumRecords() const
{
	return m_numRecords;
}

int EvtJournal::GetNumRecoverdRecords() const
{
	return m_numRecoveredRecords;
}

std::string EvtJournal::GetVersion() const
{
	return std::to_string(m_minorVer) + "." + std::to_string(m_majorVer);
}

int EvtJournal::ExtractStringFromRecord(
	int (*p_func_extract_size)(libevt_record_t *, size_t *, libevt_error_t **),
	int(*func_extract_string)(libevt_record_t *,uint8_t*, size_t, libevt_error_t **),
	libevt_record_t * event_record,
	std::string* outputStr)
{
	libevt_error_t* m_evtError = NULL;
	
	size_t stringSize = 0;
	if (-1 == p_func_extract_size(
		event_record,
		&stringSize,
		&m_evtError))
	{
		goto error;
	}

	if (stringSize == 0) {
		goto error;
	}

	char* stringBuffer = new char[stringSize];
	
	if (-1 == func_extract_string(
		event_record,
		(uint8_t *)stringBuffer,
		stringSize,
		&m_evtError))
	{
		delete stringBuffer;
		goto error;
	}

	*outputStr = std::string(stringBuffer);
	delete stringBuffer;
	
	return 1;
error:
	return -1;
}










//
//int EvtJournal::ExtractRecordComputerName(libevt_record_t* evt_record, std::string * in_out_str)
//{
//	libevt_error_t* m_evtError = NULL; //TODO: разобраться с типами ошибки
//
//	size_t stringSize = 0;
//	if (-1 == libevt_record_get_utf8_computer_name_size(
//		evt_record,
//		&stringSize,
//		&m_evtError))
//	{
//		goto error;
//	}
//
//	if (stringSize == 0) {
//		goto error;
//	}
//
//	char* stringBuffer = new char[stringSize];
//
//	if (-1 == libevt_record_get_utf8_computer_name(
//		evt_record,
//		(uint8_t *)stringBuffer,
//		stringSize,
//		&m_evtError))
//	{
//		goto error;
//	}
//
//	*in_out_str = std::string(stringBuffer);
//	delete stringBuffer;
//
//	return 1;
//error:
//	return -1;
//}