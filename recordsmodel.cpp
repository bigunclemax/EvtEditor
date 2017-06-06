#include "recordsmodel.h"

void RecordsModel::addRecords(const QVector<EventRecord>&)
{
	emit layoutAboutToBeChanged();
	/*foreach(DiscoveredPort port, ports) {
		m_ports.push_back(port);
	};*/
	emit layoutChanged();
}

void RecordsModel::addRecords(const std::vector<EventRecord>& inRecordsVec)
{
	emit layoutAboutToBeChanged();
	m_records = QVector<EventRecord>::fromStdVector(inRecordsVec);
	emit layoutChanged();
}

void RecordsModel::clear()
{
	emit layoutAboutToBeChanged();
	m_records.clear();
	emit layoutChanged();
}

int RecordsModel::rowCount(const QModelIndex & parent) const
{
	return m_records.size();
}

int RecordsModel::columnCount(const QModelIndex & parent) const
{
	return m_tableHeaders.size();
}

QVariant RecordsModel::data(const QModelIndex & index, int nRole) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	if (nRole == Qt::DecorationRole)
	{
		if (!m_tableHeaders[index.column()].compare(TH_EVT_ID))
		{
			uint32_t eventID = m_records[index.row()].EventID;
			uint32_t sev = (eventID >> 29) & 0x3;
			switch (sev)
			{
			case 0:
				//Success
				return QIcon(":/EvtEditor/Resources/Ok_16.png");
			case 1:
				//Informational
				return QIcon(":/EvtEditor/Resources/Info_16.png");
			case 2:
				//Warning
				return QIcon(":/EvtEditor/Resources/Warn_16.png");
			case 3:
				//Error
				return QIcon(":/EvtEditor/Resources/Error_16.png");
			default:
				return QVariant();
			}
		}
	}
	if (nRole == Qt::TextAlignmentRole)
	{
		return Qt::AlignCenter;
	}

	if (nRole != Qt::DisplayRole)
	{
		return QVariant();
	}

	if (!m_tableHeaders[index.column()].compare(TH_EVT_CATEGORY))
	{
		return m_records[index.row()].EventCategory;
	}
	if (!m_tableHeaders[index.column()].compare(TH_EVT_TYPE))
	{
		switch (m_records[index.row()].EventType)
		{
		case 0x0001:
			return	"EVENTLOG_ERROR_TYPE";
		case 0x0010:
			return	"EVENTLOG_AUDIT_FAILURE";
		case 0x0008:
			return "EVENTLOG_AUDIT_SUCCESS";
		case 0x0004:
			return "EVENTLOG_INFORMATION_TYPE";
		case 0x0002:
			return "EVENTLOG_WARNING_TYPE";
		default:
			return "UNKNOWN_ERROR_";
		}
	}
	if (!m_tableHeaders[index.column()].compare(TH_EVT_ID))
	{
		uint32_t eventID = m_records[index.row()].EventID;
		QString outStr;

		if ((eventID >> 28) & 0x1)
		{
			outStr = QString("S ");
		}
		else 
		{
			outStr = QString("C ");
		}
		
		outStr = outStr + QString("ID: ")+QString::number(eventID & 0xFFFF);

		return outStr;
	}
	if (!m_tableHeaders[index.column()].compare(TH_SOURCE_NAME))
	{
		return QString::fromStdString(m_records[index.row()].SourceName);
	}
	if (!m_tableHeaders[index.column()].compare(TH_EVN_COMP_NAME))
	{
		return QString::fromStdString(m_records[index.row()].ComputerName);
	}
	if (!m_tableHeaders[index.column()].compare(TH_USR_SID))
	{
		return QString::fromStdString(m_records[index.row()].UserSID);
	}
	if (!m_tableHeaders[index.column()].compare(TH_EVT_NUMBER))
	{
		return m_records[index.row()].RecordNumber;
	}
	if (!m_tableHeaders[index.column()].compare(TH_EVT_TIME))
	{
		QDateTime time;
		time.setSecsSinceEpoch(m_records[index.row()].PosixTime);
		return time.toString("dd.MM.yyyy hh:mm:ss");
	}

	return QVariant();
}

QVariant RecordsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) {
		return QVariant();
	}
	if (orientation == Qt::Vertical) {
		return QVariant();
	}
	return m_tableHeaders[section];
}

int RecordsModel::getRecordIndex(const QModelIndex & idx) const
{
	if (!idx.isValid())
	{
		return -1;
	}

	return m_records[idx.row()].Index;
}

int RecordsModel::size() const
{
	return m_records.size();
}

uint64_t RecordsModel::getRecordOffset(int recordIndex) const
{
	if ((recordIndex < 0) || (recordIndex >= m_records.size())) 
	{
		return 0;
	}
	return m_records[recordIndex].RecordOffset;
}

uint32_t RecordsModel::getRecordID(int recordIdx) const
{
	if ((recordIdx < 0) || (recordIdx >= m_records.size()))
	{
		return 0;
	}
	return m_records[recordIdx].RecordNumber;
}
