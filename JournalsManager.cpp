#include "JournalsManager.h"
#include <vcclr.h>


JournalsManager::JournalsManager()
{
}

//1 if success
//-1 if error
int JournalsManager::openJournal(String^ filePath)
{		
	
	
	EvtJournal* p_journal = new EvtJournal();
	/*p_journal->OpenFile(w_Path);*/
	/*p_journal->ReadRecords();*/


	//if it successfully opend then add it to journals 
	m_journalsArr.Add(IntPtr(p_journal));

	//set last opend journal as current journal
	m_currentJournal = p_journal;
	return 1;
}

String ^ JournalsManager::CurrentJournalVersion()
{
	EvtJournal dfgd;
	throw gcnew System::NotImplementedException();
	// TODO: insert return statement here
}

