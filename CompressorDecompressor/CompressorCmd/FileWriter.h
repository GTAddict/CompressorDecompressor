#pragma once

#include <string>
#include <fstream>
#include <list>

struct Entry;

class FileWriter
{
public:
	FileWriter(std::string outputBinFilename, bool shouldWriteUncompressedData);
	~FileWriter();

	bool WriteToFile(const std::list<Entry>& xyzList, int bitSize);

private:
	void ComputeMinAndMax(const std::list<Entry>& list, Entry& min, Entry& max);
	Entry ComputeBucketSize(const Entry& minEntry, const Entry& maxEntry, int possibleValues);
	void WriteBuffer(std::list<Entry> xyzList, Entry min, Entry max, Entry bucketSize, int bitSize);

	std::ofstream	mBinFileHandle;
	std::ofstream	mUnCompressedBinFileHandle;
	bool			mShouldWriteUnCompressedData;
};

