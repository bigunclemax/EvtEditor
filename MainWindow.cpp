#include "MainWindow.h"



void EvtEditor::MainWindow::ShowJournalData(String ^ journalPath)
{
	
	this->m_journal = gcnew EvtJournalWrap();
	this->m_journal->Open(journalPath);
	ClearTable();
	FillHeaderData();
	
	m_evtArray = this->m_journal->GetRecords();
	FillRecordsData(
		this->dataGridViewJournalContent,
		m_evtArray);
	
	m_evtRecoveredArray = this->m_journal->GetRecoveredRecords();
	FillRecordsData(
		this->dataGridViewRecoverdEvents,
		m_evtRecoveredArray);
}

void EvtEditor::MainWindow::FillRecordsData(System::Windows::Forms::DataGridView ^ dataView, array<EventRecordWrap>^ m_evtArray)
{
	dataView->Rows->Add(m_evtArray->Length);	
	for (int i = 0; i < m_evtArray->Length; i++)
	{
		dataView->Rows[i]->Cells[0]->Value = m_evtArray[i].RecordNumber;
		dataView->Rows[i]->Cells[1]->Value = m_evtArray[i].EventID;
		dataView->Rows[i]->Cells[2]->Value = m_evtArray[i].EventTime.ToLocalTime().ToString("G");
		dataView->Rows[i]->Cells[3]->Value = m_evtArray[i].EventType;
		dataView->Rows[i]->Cells[4]->Value = m_evtArray[i].EventCategory;
		dataView->Rows[i]->Cells[5]->Value = m_evtArray[i].ComputerName;
		dataView->Rows[i]->Cells[6]->Value = m_evtArray[i].SourceName;
		dataView->Rows[i]->Cells[7]->Value = m_evtArray[i].UserSID;
	}
}

void EvtEditor::MainWindow::FillHeaderData()
{
	this->textBoxJournalVersion->Text = m_journal->GetVersion();
	this->textBoxNumRecords->Text = m_journal->GetNumRecords().ToString();
	this->textBoxNumRcvrd->Text = m_journal->GetNumRecoveredRecords();
}

bool EvtEditor::MainWindow::IsANonHeaderCell(System::Windows::Forms::DataGridViewCellEventArgs^  cellEvent)
{
	if (cellEvent->RowIndex != -1)
	{
		return true;
	}
	return false;	
}

void EvtEditor::MainWindow::ShowRecordStrings(System::Windows::Forms::DataGridViewCellEventArgs^ cellEvent)
{
	int recordRow = cellEvent->RowIndex;
	String^ selectedRecordNumber;
	array<EventRecordWrap>^ evtArray;

	switch (this->tabControl1->SelectedIndex)
	{
	case 0:
		selectedRecordNumber = this->dataGridViewJournalContent->Rows[recordRow]->Cells[0]->Value->ToString();
		evtArray = m_evtArray;
		break;
	case 1:
		selectedRecordNumber = this->dataGridViewRecoverdEvents->Rows[recordRow]->Cells[0]->Value->ToString();
		evtArray = m_evtRecoveredArray;
		break;
	default:
		return;
	}
	
	//find record by record number
	int i = 0;
	for (i = 0; i < evtArray->Length; i++)
	{
		if (String::Compare(evtArray[i].RecordNumber, selectedRecordNumber) == 0) {
			break;
		};
	}

	array<String^>^ strArr;
	switch (this->tabControl1->SelectedIndex)
	{
	case 0:
		strArr = this->m_journal->GetRecordStrings(evtArray[i].Index);
		break;
	case 1:
		strArr = this->m_journal->GetRecoveredRecordStrings(evtArray[i].Index);
		break;
	default:
		return;
	}

	this->textBoxRecordStrings->Clear();
	for (int i = 0; i < strArr->Length; i++)
	{
		this->textBoxRecordStrings->AppendText(strArr[i] + "\r\n");
	}

}

void EvtEditor::MainWindow::ClearTable()
{
	this->dataGridViewJournalContent->Rows->Clear();
	this->dataGridViewJournalContent->Refresh();
	this->dataGridViewRecoverdEvents->Rows->Clear();
	this->dataGridViewRecoverdEvents->Refresh();
}
