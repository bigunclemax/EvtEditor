#include "evteditor.h"

EvtEditor::EvtEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	m_pTableContextMenu = new QMenu(this);

	m_pDelRecordAction = new QAction("Delete record", this);
	m_pTableContextMenu->addAction(m_pDelRecordAction);

	ui.tableView_EvtRecords->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.tableView_EvtRecords->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	
	
	m_ErrMBox = new QMessageBox(
		QMessageBox::Critical,
		tr("Error"),
		"",
		QMessageBox::Ok);
	
	QObject::connect(ui.actionOpen, SIGNAL(triggered(bool)), this, SLOT(fileOpenPressed()));
	QObject::connect(ui.actionSave_as, SIGNAL(triggered(bool)), this, SLOT(fileSavePressed()));
	QObject::connect(
		m_pDelRecordAction,
		SIGNAL(triggered(bool)),
		this,
		SLOT(delRecordClicked(bool)));
	QObject::connect(ui.tableView_EvtRecords, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onCurrTableClicked(const QModelIndex &)));
	QObject::connect(ui.tableView_EvtRecords, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(provideContexMenu(const QPoint&)));

	m_proxyEvtModel.setSourceModel(&m_currRecordsModel);
	m_selectionModel = new QItemSelectionModel(&m_proxyEvtModel);

	m_proxyRecEvtModel.setSourceModel(&m_recoveredRecordsModel);

	ui.tableView_EvtRecords->setModel(&m_proxyEvtModel);
	ui.tableView_EvtRecords->setSelectionModel(m_selectionModel);
	ui.tableView_RecoveredRecords->setModel(&m_proxyRecEvtModel);
	
	QObject::connect(
		m_selectionModel,
		SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
		SLOT(rowSelectionChanged(const QModelIndex &, const QModelIndex &))
	);
}

void EvtEditor::fileOpenPressed()
{
	QString pathToJournal = QFileDialog::getOpenFileName(
		this,
		"Open evt journal",
		"",
		"*.evt");
	
	if (pathToJournal.isNull()) 
	{
		m_ErrMBox->setText("Wrong evt journal path");
		m_ErrMBox->exec();
		return;
	}

	this->OpenJournal(pathToJournal);
}

EvtEditor::~EvtEditor()
{
	delete m_ErrMBox;
}

bool EvtEditor::OpenJournal(QString & journalPath)
{
	//open evt journal
	QFile journalFile(journalPath);
	if (!journalFile.open(QIODevice::ReadOnly))
	{
		m_ErrMBox->setText("Can't open evt journal");
		m_ErrMBox->exec();
		return false;
	}

	//create temp file
	QTemporaryFile journalTemp;
	if (!journalTemp.open())
	{
		m_ErrMBox->setText("Can't create tmp file");
		m_ErrMBox->exec();
		return false;
	}

	m_JournalRawData = journalFile.readAll();
	journalTemp.write(m_JournalRawData);
	journalFile.close();
	journalTemp.close();

	m_tempFileName = journalTemp.fileName();

	//QString to wchar_t
	m_evtJournal = new EvtJournal();
	wchar_t * wPath =  new wchar_t[m_tempFileName.length()+1];
	QDir::toNativeSeparators(m_tempFileName).toWCharArray(wPath);
	wPath[m_tempFileName.length()] = 0;

	int res = m_evtJournal->Open(wPath);
	delete(wPath);
		
	if (res == -1)
	{
		m_ErrMBox->setText("Can't open evt journal");
		m_ErrMBox->exec();
		return false;
	}

	ExtractHeader(*m_evtJournal);
	ExtractRecords(*m_evtJournal);
	return true;
}

bool EvtEditor::SaveJournal(QString & filePath)
{
	QFile::copy(m_tempFileName, filePath);
	return true;
}

bool EvtEditor::CloseJournal()
{
	m_evtJournal->Close();
	delete m_evtJournal;
	return true;
}

bool EvtEditor::ExtractHeader(EvtJournal & journal)
{
	// TODO: check if journal is open
	ui.lineEdit_NumRecords->setText(QString::number(journal.GetNumRecords()));
	ui.lineEdit_NumRecoveredRecords->setText(QString::number(journal.GetNumRecoverdRecords()));
	return true;
}

bool EvtEditor::ExtractRecords(EvtJournal & journal)
{
	// TODO: check if journal is open
	
	m_currRecordsModel.addRecords(journal.GetRecords());
	m_recoveredRecordsModel.addRecords(journal.GetRecoveredRecords());
	
	return true;
}

void EvtEditor::onCurrTableClicked(const QModelIndex & index)
{
	ui.plainTextEdit_RecStrings->setPlainText("");
	int recIdx = m_currRecordsModel.getRecordIndex(index);
	std::vector<std::wstring> recStrings = m_evtJournal->GetRecordStrings(recIdx);

	for (int i = 0; i < recStrings.size(); i++)
	{
		QString qtString = QString::fromStdWString(recStrings[i]);
		ui.plainTextEdit_RecStrings->appendPlainText(qtString);
	}
}

void EvtEditor::provideContexMenu(const QPoint& position)
{
	QItemSelectionModel * selectionModel = ui.tableView_EvtRecords->selectionModel();
	if (selectionModel->selectedRows().size() == 1) 
	{
		m_pTableContextMenu->exec(ui.tableView_EvtRecords->viewport()->mapToGlobal(position));
	}
}

void EvtEditor::rowSelectionChanged(const QModelIndex & current, const QModelIndex & previous)
{
	ui.plainTextEdit_RecStrings->setPlainText("");
	int recIdx = m_currRecordsModel.getRecordIndex(current);
	std::vector<std::wstring> recStrings = m_evtJournal->GetRecordStrings(recIdx);

	for (int i = 0; i < recStrings.size(); i++)
	{
		QString qtString = QString::fromStdWString(recStrings[i]);
		ui.plainTextEdit_RecStrings->appendPlainText(qtString);
	}
}

void EvtEditor::delRecordClicked(bool)
{
	QModelIndexList selectionRows =  m_selectionModel->selectedRows();
	if (selectionRows.size() == 1) 
	{
		QModelIndex mapedIndex =  m_proxyEvtModel.mapToSource(selectionRows[0]);

		int recordIdx = m_currRecordsModel.getRecordIndex(mapedIndex);
		
		if (recordIdx == 0)
		{
			return; //TODO: while skip first record
		}

		int recOffset = m_currRecordsModel.getRecordOffset(recordIdx) ;
		int prevRecOffset = m_currRecordsModel.getRecordOffset(recordIdx-1);
		
		m_evtJournal->Close();
		//get erase data
		RecordRemover remover;
		remover.Open(m_JournalRawData);
		remover.RE(recOffset, prevRecOffset);
		remover.Save(m_JournalRawData);
		remover.Close();
		//fix record ID's
		uint32_t delRecordID = m_currRecordsModel.getRecordID(recordIdx);
		for (int i = recordIdx+1; i < m_currRecordsModel.size(); i++, delRecordID++)
		{
			uint recOffset = static_cast<uint>(m_currRecordsModel.getRecordOffset(i));
			//*(reinterpret_cast<uint32_t *>(&m_JournalRawData[recOffset])+2) = delRecordID;
			
			m_JournalRawData.replace(recOffset+8,4,QByteArray(reinterpret_cast<const char*>(&delRecordID),4));
		}

		//commit to temp file
		QFile tmpJournal(m_tempFileName);
		if (!tmpJournal.open(QIODevice::WriteOnly))
		{
			m_ErrMBox->setText("Can't open tmp journal");
			m_ErrMBox->exec();
		}
		else 
		{
			tmpJournal.write(m_JournalRawData);
			tmpJournal.close();
		}
		
		//reload evt journal
		this->OpenJournal(m_tempFileName);
	}
}

void EvtEditor::fileSavePressed()
{
	QString pathToJournal = QFileDialog::getSaveFileName(
		this,
		"Save evt journal",
		"",
		"*.evt");

	if (pathToJournal.isNull())
	{
		m_ErrMBox->setText("Incorrect new evt journal path");
		m_ErrMBox->exec();
		return;
	}

	this->SaveJournal(pathToJournal);
}
