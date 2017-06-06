#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include <QSortFilterProxyModel>
#include <QTemporaryFile>
#include "ui_evteditor.h"
#include "EvtJournal.h"
#include "recordsmodel.h"
#include "RecordRemover.h"

class EvtEditor : public QMainWindow
{
	Q_OBJECT

public:
	EvtEditor(QWidget *parent = Q_NULLPTR);
	~EvtEditor();

private:
	Ui::EvtEditorClass ui;
	bool OpenJournal(QString & filePath);
	bool SaveJournal(QString & filePath);
	bool CloseJournal();
	bool ExtractHeader(EvtJournal & journal);
	bool ExtractRecords(EvtJournal & journal);
	QString m_tempFileName;
	QByteArray m_JournalRawData;
	QMenu *			m_pTableContextMenu;
	QMessageBox *	m_ErrMBox;
	RecordsModel	m_currRecordsModel;
	RecordsModel	m_recoveredRecordsModel;
	QSortFilterProxyModel	m_proxyEvtModel;
	QSortFilterProxyModel	m_proxyRecEvtModel;
	QItemSelectionModel *	m_selectionModel;
	EvtJournal *	m_evtJournal;
	QAction * m_pDelRecordAction;

private slots:
	
	void fileOpenPressed();
	void onCurrTableClicked(const QModelIndex &);
	void provideContexMenu(const QPoint&);
	void rowSelectionChanged(const QModelIndex &, const QModelIndex &);
	void delRecordClicked(bool);
	void fileSavePressed();
};
