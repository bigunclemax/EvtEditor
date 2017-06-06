#pragma once

public value struct EventRecordWrap {
	int Index;
	System::String^ RecordNumber;
	System::DateTime EventTime;
	System::String^ EventType;
	System::String^ EventCategory;
	System::String^ EventID;
	System::String^ ComputerName;
	System::String^ SourceName;
	System::String^ UserSID;
	/*array<System::String^>^	Strings;*/
};

