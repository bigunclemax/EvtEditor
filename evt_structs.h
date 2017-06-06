#pragma once

#define EVT_HEADER_SIZE 0x30
#define EVT_EOF_SIZE	0x28

typedef struct _LOGHEADER {
	ULONG HeaderSize;
	ULONG Signature;
	ULONG MajorVersion;
	ULONG MinorVersion;
	ULONG StartOffset;
	ULONG EndOffset;
	ULONG CurrentRecordNumber;
	ULONG OldestRecordNumber;
	ULONG MaxSize;
	ULONG Flags;
	ULONG Retention;
	ULONG EndHeaderSize;
} LOGHEADER, *PLOGHEADER;

typedef struct _LOGEOF {
	ULONG RecordSizeBeginning;
	ULONG One;
	ULONG Two;
	ULONG Three;
	ULONG Four;
	ULONG BeginRecord;
	ULONG EndRecord;
	ULONG CurrentRecordNumber;
	ULONG OldestRecordNumber;
	ULONG RecordSizeEnd;
} LOGEOF, *PLOGEOF;

typedef struct _LOGRECORD {
	DWORD Length;
	DWORD Reserved;
	DWORD RecordNumber;
	DWORD TimeGenerated;
	DWORD TimeWritten;
	DWORD EventID;
	WORD  EventType;
	WORD  NumStrings;
	WORD  EventCategory;
	WORD  ReservedFlags;
	DWORD ClosingRecordNumber;
	DWORD StringOffset;
	DWORD UserSidLength;
	DWORD UserSidOffset;
	DWORD DataLength;
	DWORD DataOffset;
} LOGRECORD, *POGRECORD;

