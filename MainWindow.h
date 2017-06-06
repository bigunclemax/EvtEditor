#pragma once
//#include "JournalsManager.h"
#include "EvtJournalWrap.h"

namespace EvtEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainWindow
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	private: System::Windows::Forms::TextBox^  textBox4;
	public:
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBoxNumRcvrd;

	private: System::Windows::Forms::Label^  labelNumOfRcvrdRec;
	private: System::Windows::Forms::TextBox^  textBoxNumRecords;

	private: System::Windows::Forms::Label^  labelNumOfRecords;
	private: System::Windows::Forms::DataGridView^  dataGridViewJournalContent;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColRecordNumber;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnEvtID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnEventTime;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnEventType;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnEventCategory;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnComputerName;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnSourceName;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnUserSid;
	private: System::Windows::Forms::TextBox^  textBoxRecordStrings;

	private: System::Windows::Forms::GroupBox^  groupBoxEventStrings;
	private: System::Windows::Forms::TextBox^  textBoxJournalVersion;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::DataGridView^  dataGridViewRecoverdEvents;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn5;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn6;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn7;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn8;

	private:
		EvtJournalWrap^ m_journal;
		array<EventRecordWrap>^ m_evtArray;
		array<EventRecordWrap>^ m_evtRecoveredArray;
		void ShowJournalData(String^ journalPath); 
		void FillRecordsData(DataGridView^,array<EventRecordWrap>^);
		void FillHeaderData();
		bool IsANonHeaderCell(System::Windows::Forms::DataGridViewCellEventArgs^ cellEvent);
		void ShowRecordStrings(System::Windows::Forms::DataGridViewCellEventArgs^ cellEvent);
		void ClearTable();

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::GroupBox^  groupBox1;

	private: System::Windows::Forms::Label^  label1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBoxNumRcvrd = (gcnew System::Windows::Forms::TextBox());
			this->labelNumOfRcvrdRec = (gcnew System::Windows::Forms::Label());
			this->textBoxNumRecords = (gcnew System::Windows::Forms::TextBox());
			this->labelNumOfRecords = (gcnew System::Windows::Forms::Label());
			this->textBoxJournalVersion = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->dataGridViewJournalContent = (gcnew System::Windows::Forms::DataGridView());
			this->ColRecordNumber = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnEvtID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnEventTime = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnEventType = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnEventCategory = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnComputerName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnSourceName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnUserSid = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->textBoxRecordStrings = (gcnew System::Windows::Forms::TextBox());
			this->groupBoxEventStrings = (gcnew System::Windows::Forms::GroupBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->dataGridViewRecoverdEvents = (gcnew System::Windows::Forms::DataGridView());
			this->dataGridViewTextBoxColumn1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn6 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn7 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn8 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->menuStrip1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewJournalContent))->BeginInit();
			this->groupBoxEventStrings->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewRecoverdEvents))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(936, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(109, 22);
			this->openToolStripMenuItem->Text = L"Open..";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindow::openToolStripMenuItem_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->Filter = L"Evt files (*.evt)|*.evt|All files (*.*)|*.*";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MainWindow::openFileDialog1_FileOk);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->textBox4);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->textBoxNumRcvrd);
			this->groupBox1->Controls->Add(this->labelNumOfRcvrdRec);
			this->groupBox1->Controls->Add(this->textBoxNumRecords);
			this->groupBox1->Controls->Add(this->labelNumOfRecords);
			this->groupBox1->Controls->Add(this->textBoxJournalVersion);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(13, 28);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(305, 131);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Evt header";
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(162, 89);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(100, 20);
			this->textBox4->TabIndex = 7;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(84, 92);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(63, 13);
			this->label2->TabIndex = 6;
			this->label2->Text = L"Is corrupted";
			// 
			// textBoxNumRcvrd
			// 
			this->textBoxNumRcvrd->Enabled = false;
			this->textBoxNumRcvrd->Location = System::Drawing::Point(162, 64);
			this->textBoxNumRcvrd->Name = L"textBoxNumRcvrd";
			this->textBoxNumRcvrd->Size = System::Drawing::Size(100, 20);
			this->textBoxNumRcvrd->TabIndex = 5;
			// 
			// labelNumOfRcvrdRec
			// 
			this->labelNumOfRcvrdRec->AutoSize = true;
			this->labelNumOfRcvrdRec->Location = System::Drawing::Point(8, 67);
			this->labelNumOfRcvrdRec->Name = L"labelNumOfRcvrdRec";
			this->labelNumOfRcvrdRec->Size = System::Drawing::Size(139, 13);
			this->labelNumOfRcvrdRec->TabIndex = 4;
			this->labelNumOfRcvrdRec->Text = L"Number of recorded records";
			// 
			// textBoxNumRecords
			// 
			this->textBoxNumRecords->Enabled = false;
			this->textBoxNumRecords->Location = System::Drawing::Point(162, 41);
			this->textBoxNumRecords->Name = L"textBoxNumRecords";
			this->textBoxNumRecords->Size = System::Drawing::Size(100, 20);
			this->textBoxNumRecords->TabIndex = 3;
			// 
			// labelNumOfRecords
			// 
			this->labelNumOfRecords->AutoSize = true;
			this->labelNumOfRecords->Location = System::Drawing::Point(53, 44);
			this->labelNumOfRecords->Name = L"labelNumOfRecords";
			this->labelNumOfRecords->Size = System::Drawing::Size(94, 13);
			this->labelNumOfRecords->TabIndex = 2;
			this->labelNumOfRecords->Text = L"Number of records";
			// 
			// textBoxJournalVersion
			// 
			this->textBoxJournalVersion->Enabled = false;
			this->textBoxJournalVersion->Location = System::Drawing::Point(162, 17);
			this->textBoxJournalVersion->Name = L"textBoxJournalVersion";
			this->textBoxJournalVersion->Size = System::Drawing::Size(100, 20);
			this->textBoxJournalVersion->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(105, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(42, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Version";
			// 
			// dataGridViewJournalContent
			// 
			this->dataGridViewJournalContent->AllowUserToAddRows = false;
			this->dataGridViewJournalContent->AllowUserToDeleteRows = false;
			this->dataGridViewJournalContent->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dataGridViewJournalContent->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewJournalContent->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(8) {
				this->ColRecordNumber,
					this->ColumnEvtID, this->ColumnEventTime, this->ColumnEventType, this->ColumnEventCategory, this->ColumnComputerName, this->ColumnSourceName,
					this->ColumnUserSid
			});
			this->dataGridViewJournalContent->Location = System::Drawing::Point(0, 0);
			this->dataGridViewJournalContent->Name = L"dataGridViewJournalContent";
			this->dataGridViewJournalContent->Size = System::Drawing::Size(903, 294);
			this->dataGridViewJournalContent->TabIndex = 2;
			this->dataGridViewJournalContent->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainWindow::dataGridViewJournalContent_CellContentClick);
			// 
			// ColRecordNumber
			// 
			this->ColRecordNumber->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->ColRecordNumber->HeaderText = L"Record number";
			this->ColRecordNumber->Name = L"ColRecordNumber";
			// 
			// ColumnEvtID
			// 
			this->ColumnEvtID->HeaderText = L"Event ID";
			this->ColumnEvtID->Name = L"ColumnEvtID";
			// 
			// ColumnEventTime
			// 
			this->ColumnEventTime->HeaderText = L"Event time";
			this->ColumnEventTime->Name = L"ColumnEventTime";
			// 
			// ColumnEventType
			// 
			this->ColumnEventType->HeaderText = L"Event type";
			this->ColumnEventType->Name = L"ColumnEventType";
			// 
			// ColumnEventCategory
			// 
			this->ColumnEventCategory->HeaderText = L"Event category";
			this->ColumnEventCategory->Name = L"ColumnEventCategory";
			// 
			// ColumnComputerName
			// 
			this->ColumnComputerName->HeaderText = L"Computer name";
			this->ColumnComputerName->Name = L"ColumnComputerName";
			// 
			// ColumnSourceName
			// 
			this->ColumnSourceName->HeaderText = L"Source name";
			this->ColumnSourceName->Name = L"ColumnSourceName";
			// 
			// ColumnUserSid
			// 
			this->ColumnUserSid->HeaderText = L"User SID";
			this->ColumnUserSid->Name = L"ColumnUserSid";
			// 
			// textBoxRecordStrings
			// 
			this->textBoxRecordStrings->Location = System::Drawing::Point(6, 20);
			this->textBoxRecordStrings->Multiline = true;
			this->textBoxRecordStrings->Name = L"textBoxRecordStrings";
			this->textBoxRecordStrings->Size = System::Drawing::Size(543, 105);
			this->textBoxRecordStrings->TabIndex = 3;
			// 
			// groupBoxEventStrings
			// 
			this->groupBoxEventStrings->Controls->Add(this->textBoxRecordStrings);
			this->groupBoxEventStrings->Location = System::Drawing::Point(369, 28);
			this->groupBoxEventStrings->Name = L"groupBoxEventStrings";
			this->groupBoxEventStrings->Size = System::Drawing::Size(555, 131);
			this->groupBoxEventStrings->TabIndex = 4;
			this->groupBoxEventStrings->TabStop = false;
			this->groupBoxEventStrings->Text = L"Event strings";
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(13, 165);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(911, 316);
			this->tabControl1->TabIndex = 5;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->dataGridViewJournalContent);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(903, 290);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Current evt records";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->dataGridViewRecoverdEvents);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(903, 290);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Recovered event records";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// dataGridViewRecoverdEvents
			// 
			this->dataGridViewRecoverdEvents->AllowUserToAddRows = false;
			this->dataGridViewRecoverdEvents->AllowUserToDeleteRows = false;
			this->dataGridViewRecoverdEvents->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dataGridViewRecoverdEvents->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewRecoverdEvents->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(8) {
				this->dataGridViewTextBoxColumn1,
					this->dataGridViewTextBoxColumn2, this->dataGridViewTextBoxColumn3, this->dataGridViewTextBoxColumn4, this->dataGridViewTextBoxColumn5,
					this->dataGridViewTextBoxColumn6, this->dataGridViewTextBoxColumn7, this->dataGridViewTextBoxColumn8
			});
			this->dataGridViewRecoverdEvents->Location = System::Drawing::Point(0, -2);
			this->dataGridViewRecoverdEvents->Name = L"dataGridViewRecoverdEvents";
			this->dataGridViewRecoverdEvents->Size = System::Drawing::Size(903, 294);
			this->dataGridViewRecoverdEvents->TabIndex = 3;
			this->dataGridViewRecoverdEvents->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainWindow::dataGridViewRecoveredEvents_CellContentClick);
			// 
			// dataGridViewTextBoxColumn1
			// 
			this->dataGridViewTextBoxColumn1->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->dataGridViewTextBoxColumn1->HeaderText = L"Record number";
			this->dataGridViewTextBoxColumn1->Name = L"dataGridViewTextBoxColumn1";
			// 
			// dataGridViewTextBoxColumn2
			// 
			this->dataGridViewTextBoxColumn2->HeaderText = L"Event ID";
			this->dataGridViewTextBoxColumn2->Name = L"dataGridViewTextBoxColumn2";
			// 
			// dataGridViewTextBoxColumn3
			// 
			this->dataGridViewTextBoxColumn3->HeaderText = L"Event time";
			this->dataGridViewTextBoxColumn3->Name = L"dataGridViewTextBoxColumn3";
			// 
			// dataGridViewTextBoxColumn4
			// 
			this->dataGridViewTextBoxColumn4->HeaderText = L"Event type";
			this->dataGridViewTextBoxColumn4->Name = L"dataGridViewTextBoxColumn4";
			// 
			// dataGridViewTextBoxColumn5
			// 
			this->dataGridViewTextBoxColumn5->HeaderText = L"Event category";
			this->dataGridViewTextBoxColumn5->Name = L"dataGridViewTextBoxColumn5";
			// 
			// dataGridViewTextBoxColumn6
			// 
			this->dataGridViewTextBoxColumn6->HeaderText = L"Computer name";
			this->dataGridViewTextBoxColumn6->Name = L"dataGridViewTextBoxColumn6";
			// 
			// dataGridViewTextBoxColumn7
			// 
			this->dataGridViewTextBoxColumn7->HeaderText = L"Source name";
			this->dataGridViewTextBoxColumn7->Name = L"dataGridViewTextBoxColumn7";
			// 
			// dataGridViewTextBoxColumn8
			// 
			this->dataGridViewTextBoxColumn8->HeaderText = L"User SID";
			this->dataGridViewTextBoxColumn8->Name = L"dataGridViewTextBoxColumn8";
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(936, 493);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->groupBoxEventStrings);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->menuStrip1);
			this->HelpButton = true;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainWindow";
			this->Text = L"Evt Editor";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewJournalContent))->EndInit();
			this->groupBoxEventStrings->ResumeLayout(false);
			this->groupBoxEventStrings->PerformLayout();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewRecoverdEvents))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
			return;
		}
		
		this->ShowJournalData(openFileDialog1->FileName);
		
	}
	private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
	}
	

private: System::Void dataGridViewJournalContent_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
	
	if (IsANonHeaderCell(e))
	{
		ShowRecordStrings(e);	
	}
}

private: System::Void dataGridViewRecoveredEvents_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {

	if (IsANonHeaderCell(e))
	{
		ShowRecordStrings(e);
	}
}
};
}
