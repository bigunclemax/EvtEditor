#pragma once

#include "EvtJournal.h"

using namespace System;

ref class JournalsManager
{
public:
	JournalsManager();
	int openJournal(String^ filePath);
	
	String^ CurrentJournalVersion(); //get current journal minor and major version
	const EvtJournal* m_currentJournal;
private:
	
	System::Collections::Generic::List<IntPtr> m_journalsArr;
	
};

