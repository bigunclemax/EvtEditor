#pragma once

#include <QAbstractTableModel>
#include <QDate>
#include <QIcon>

#include "EventRecord.h"

#define TH_EVT_NUMBER		"Record number"
#define TH_EVT_ID			"Event ID"
#define	TH_EVT_TIME			"Event time"
#define TH_EVT_TYPE			"Event type"
#define TH_EVT_CATEGORY		"Event category"
#define TH_EVN_COMP_NAME	"Computer name"
#define	TH_SOURCE_NAME		"Source name"
#define TH_USR_SID			"User SID"

class RecordsModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	void	addRecords(const QVector<EventRecord>&);
	void	addRecords(const std::vector<EventRecord>&);
	void	clear();
	int		rowCount(const QModelIndex &parent = QModelIndex()) const;
	int		columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int		getRecordIndex(const QModelIndex &) const;
	int		size() const;
	uint64_t getRecordOffset(int recordIdx) const;
	uint32_t getRecordID(int recordIdx) const;


private:
	QVector<EventRecord> m_records;
	QStringList m_tableHeaders = { 
		tr(TH_EVT_NUMBER),
		tr(TH_EVT_ID),
		tr(TH_EVT_TIME),
		tr(TH_EVT_TYPE),
		tr(TH_EVT_CATEGORY),
		tr(TH_EVN_COMP_NAME),
		tr(TH_SOURCE_NAME),
		tr(TH_USR_SID)
	};
};