#include "EvtJournalWrap.h"

int EvtJournalWrap::Open(String ^ filePath)
{
	//convert String^ to w_char
	pin_ptr<const wchar_t> w_Path = PtrToStringChars(filePath);

	return p_Journal->Open(w_Path);
	
}

String ^ EvtJournalWrap::GetVersion()
{
	return gcnew String(p_Journal->GetVersion().c_str());
}

unsigned int EvtJournalWrap::GetNumRecords()
{
	return p_Journal->GetNumRecords();
}

String ^ EvtJournalWrap::GetNumRecoveredRecords()
{
	return p_Journal->GetNumRecoverdRecords().ToString();
}

array <EventRecordWrap>^ EvtJournalWrap::GetRecords()
{
	std::vector<EventRecord> vecRecords = p_Journal->GetRecords();
	array<EventRecordWrap> ^ arrRec = gcnew array<EventRecordWrap>(vecRecords.size());

	for (int i = 0; i < vecRecords.size(); i++) {
		arrRec[i].RecordNumber		= Convert::ToString(vecRecords[i].RecordNumber);
		arrRec[i].EventTime			= DateTime(1970, 1, 1).AddSeconds(vecRecords[i].PosixTime);

		switch (vecRecords[i].EventType)
		{
		case 0x0001:
			arrRec[i].EventType = gcnew String("EVENTLOG_ERROR_TYPE");
			break;
		case 0x0010:
			arrRec[i].EventType = gcnew String("EVENTLOG_AUDIT_FAILURE");
			break;
		case 0x0008:
			arrRec[i].EventType = gcnew String("EVENTLOG_AUDIT_SUCCESS");
			break;
		case 0x0004:
			arrRec[i].EventType = gcnew String("EVENTLOG_INFORMATION_TYPE");
			break;
		case 0x0002:
			arrRec[i].EventType = gcnew String("EVENTLOG_WARNING_TYPE");
			break;
		default:
			arrRec[i].EventType = gcnew String("UNKNOWN_ERROR_");
			break;
		}

		/*arrRec[i].Strings = gcnew array<String^>(vecRecords[i].Strings.size());
		for (int stringIndex = 0; stringIndex < vecRecords[i].Strings.size(); stringIndex++)
		{
			arrRec[i].Strings[stringIndex] = gcnew String(vecRecords[i].Strings[stringIndex].c_str());
		}*/
		
		arrRec[i].EventCategory		= Convert::ToString(vecRecords[i].EventCategory);
		arrRec[i].EventID			= Convert::ToString(vecRecords[i].EventID);
		arrRec[i].ComputerName		= gcnew String(vecRecords[i].ComputerName.c_str());
		arrRec[i].SourceName		= gcnew String(vecRecords[i].SourceName.c_str());
		arrRec[i].UserSID			= gcnew String(vecRecords[i].UserSID.c_str());
		
		arrRec[i].Index = vecRecords[i].Index;

	}
	return arrRec;
}

array<EventRecordWrap>^ EvtJournalWrap::GetRecoveredRecords()
{
	std::vector<EventRecord> vecRecords = p_Journal->GetRecoveredRecords();
	return this->ParseEventRecords(&vecRecords);
}

void EvtJournalWrap::GetEventRecords() {
	p_Journal->GetRecords();
}

array<String^>^ EvtJournalWrap::GetRecordStrings(int recordIndex)
{
	std::vector<std::wstring> vecStrings = this->p_Journal->GetRecordStrings(recordIndex);
	array<String^>^ arrRecordStrings = gcnew array<String^>(vecStrings.size());
	for (int i = 0; i < vecStrings.size(); i++)
	{
		arrRecordStrings[i] = gcnew String(vecStrings[i].c_str());
	}
	return arrRecordStrings;
}

array<String^>^ EvtJournalWrap::GetRecoveredRecordStrings(int recordIndex)
{
	std::vector<std::wstring> vecStrings = this->p_Journal->GetRecoveredRecordStrings(recordIndex);
	array<String^>^ arrRecordStrings = gcnew array<String^>(vecStrings.size());
	for (int i = 0; i < vecStrings.size(); i++)
	{
		arrRecordStrings[i] = gcnew String(vecStrings[i].c_str());
	}
	return arrRecordStrings;
}

array<EventRecordWrap>^ EvtJournalWrap::ParseEventRecords(std::vector<EventRecord>* vecEvt)
{
	if (vecEvt == NULL) {
		return gcnew array<EventRecordWrap>(0);
	}

	array<EventRecordWrap> ^ arrRec = gcnew array<EventRecordWrap>(vecEvt->size());

	for (int i = 0; i < vecEvt->size(); i++) {
		arrRec[i].RecordNumber = Convert::ToString((*vecEvt)[i].RecordNumber);
		arrRec[i].EventTime = DateTime(1970, 1, 1).AddSeconds((*vecEvt)[i].PosixTime);

		switch ((*vecEvt)[i].EventType)
		{
		case 0x0001:
			arrRec[i].EventType = gcnew String("EVENTLOG_ERROR_TYPE");
			break;
		case 0x0010:
			arrRec[i].EventType = gcnew String("EVENTLOG_AUDIT_FAILURE");
			break;
		case 0x0008:
			arrRec[i].EventType = gcnew String("EVENTLOG_AUDIT_SUCCESS");
			break;
		case 0x0004:
			arrRec[i].EventType = gcnew String("EVENTLOG_INFORMATION_TYPE");
			break;
		case 0x0002:
			arrRec[i].EventType = gcnew String("EVENTLOG_WARNING_TYPE");
			break;
		default:
			arrRec[i].EventType = gcnew String("UNKNOWN_ERROR_");
			break;
		}

		/*arrRec[i].Strings = gcnew array<String^>(vecRecords[i].Strings.size());
		for (int stringIndex = 0; stringIndex < vecRecords[i].Strings.size(); stringIndex++)
		{
		arrRec[i].Strings[stringIndex] = gcnew String(vecRecords[i].Strings[stringIndex].c_str());
		}*/

		arrRec[i].EventCategory = Convert::ToString((*vecEvt)[i].EventCategory);
		arrRec[i].EventID = Convert::ToString((*vecEvt)[i].EventID);
		arrRec[i].ComputerName = gcnew String((*vecEvt)[i].ComputerName.c_str());
		arrRec[i].SourceName = gcnew String((*vecEvt)[i].SourceName.c_str());
		arrRec[i].UserSID = gcnew String((*vecEvt)[i].UserSID.c_str());

		arrRec[i].Index = (*vecEvt)[i].Index;
	}
	return arrRec;
}
