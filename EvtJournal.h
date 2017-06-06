#pragma once

#include <vector>
#include <string>
#include "libevt.h"
#include "libevt\error.h"
#include "EventRecord.h"


class EvtJournal
{
public:
	EvtJournal();
	~EvtJournal();
	int Open(const wchar_t *);
	void Close();
	int GetNumRecords() const;
	int GetNumRecoverdRecords() const;
	std::string GetVersion() const;
	std::vector<EventRecord> GetRecords() const;
	std::vector<EventRecord> GetRecoveredRecords() const;
	std::vector<std::wstring> GetRecordStrings(int);
	std::vector<std::wstring> GetRecoveredRecordStrings(int);

protected:
	//std::vector<libevt_record_t*> m_vecRecords;
	std::vector<EventRecord> m_vecEvtRecords;
	std::vector<EventRecord> m_vecEvtRecoveredRecords;
	libevt_file_t* m_journalFile = NULL;
	libevt_error_t* m_evtError = NULL;
	int m_numRecords = 0;
	int m_numRecoveredRecords = 0;
	unsigned int m_minorVer;
	unsigned int m_majorVer;

	int ExtractRecordsFromFile();
	int ExtractRecoveredRecordsFromFile();
	int ExtractRecord(libevt_record_t *, EventRecord*);
	int ExtractRecordStrings(libevt_record_t *, std::vector<std::wstring>* vecStrings);
	int ExtractStringFromRecord(
		int(*p_func_extract_size)(libevt_record_t *, size_t *, libevt_error_t **),
		int(*func_extract_string)(libevt_record_t *, uint8_t*, size_t, libevt_error_t **),
		libevt_record_t * event_record,
		std::string* outputStr);
};
