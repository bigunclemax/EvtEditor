#pragma once

#include <vcclr.h>
#include "EvtJournal.h"
#include "EventRecordWrap.h"

using namespace System;

ref class EvtJournalWrap
{
public:
	EvtJournal* p_Journal;
	EvtJournalWrap() : p_Journal(new EvtJournal()) {};
	int Open(String^ filePath);

	String^ GetVersion();
	unsigned int GetNumRecords();
	String^ GetNumRecoveredRecords();
	array <EventRecordWrap>^ GetRecords();
	array <EventRecordWrap>^ GetRecoveredRecords();
	void GetEventRecords();

	array<String^>^ GetRecordStrings(int);
	array<String^>^ GetRecoveredRecordStrings(int);

private:
	array<EventRecordWrap>^ ParseEventRecords(std::vector<EventRecord>* );
};