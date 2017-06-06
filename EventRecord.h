#pragma once

#include <string>

struct EventRecord
{
	uint32_t Index; // record index in file
	uint64_t RecordOffset;
	uint32_t RecordNumber;
	uint32_t PosixTime;
	uint16_t EventType;
	uint16_t EventCategory;
	uint32_t EventID;
	std::string ComputerName;
	std::string SourceName;
	std::string UserSID;
	/*std::vector < std::string > Strings;*/
};