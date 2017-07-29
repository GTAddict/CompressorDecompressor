#include "FileParser.h"
#include "Entry.h"

#define MAX_CHARACTERS 255

FileParser::FileParser(std::string filePath)
{
	mFileHandle.open(filePath, std::fstream::in | std::ios::binary);
}

std::list<Entry> FileParser::Parse()
{
	std::list<Entry> xyzList;
	char singleEntryBuffer[MAX_CHARACTERS];

	if (mFileHandle.good())
	{
		while (!mFileHandle.eof())
		{
			Entry entry;

			mFileHandle >> singleEntryBuffer;	// Index, skip

			mFileHandle >> singleEntryBuffer;	// x
			entry.mX = atof(singleEntryBuffer);

			mFileHandle >> singleEntryBuffer;	// y
			entry.mY = atof(singleEntryBuffer);

			mFileHandle >> singleEntryBuffer;	// z
			entry.mZ = atof(singleEntryBuffer);

			mFileHandle >> singleEntryBuffer;	// u, skip
			mFileHandle >> singleEntryBuffer;	// v, skip
			mFileHandle >> singleEntryBuffer;	// textureName, skip
			mFileHandle >> singleEntryBuffer;	// r, skip
			mFileHandle >> singleEntryBuffer;	// g, skip
			mFileHandle >> singleEntryBuffer;	// b, skip

			xyzList.push_back(entry);

		}
	}

	return xyzList;
}

FileParser::~FileParser()
{
	if (mFileHandle.is_open())
	{
		mFileHandle.close();
	}
}
