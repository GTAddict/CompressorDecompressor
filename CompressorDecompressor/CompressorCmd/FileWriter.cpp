#include "FileWriter.h"
#include "Entry.h"
#include "FileHeader.h"
#include <bitset>

#include <iostream>

FileWriter::FileWriter(std::string outputBinFilename, bool shouldWriteUncompressedData)
{
	mShouldWriteUnCompressedData = shouldWriteUncompressedData;

	mBinFileHandle.open(outputBinFilename, std::ios::binary | std::ios::out | std::ios::trunc);

	if (mShouldWriteUnCompressedData)
	{
		mUnCompressedBinFileHandle.open(outputBinFilename.replace(outputBinFilename.rfind("."), std::string::npos, ".uncompressed"),
			std::ios::binary | std::ios::out | std::ios::trunc);
	}
}

FileWriter::~FileWriter()
{
	if (mBinFileHandle.is_open())
	{
		mBinFileHandle.close();
	}

	if (mUnCompressedBinFileHandle.is_open())
	{
		mUnCompressedBinFileHandle.close();
	}
}

bool FileWriter::WriteToFile(const std::list<Entry>& xyzList, int bitSize)
{
	if (!mBinFileHandle.good() || !mUnCompressedBinFileHandle.good())
	{
		return false;
	}

	if (mShouldWriteUnCompressedData)
	{
		for each (Entry entry in xyzList)
		{
			mUnCompressedBinFileHandle.write((char*)&entry, sizeof(Entry));
		}
	}

	Entry minEntry, maxEntry;
	minEntry.mX = minEntry.mY = minEntry.mZ = 100;
	maxEntry.mX = maxEntry.mY = maxEntry.mZ = 0;

	ComputeMinAndMax(xyzList, minEntry, maxEntry);
	int possibleValues = (int) pow(2, bitSize);
	Entry bucketSize = ComputeBucketSize(minEntry, maxEntry, possibleValues);

	WriteBuffer(xyzList, minEntry, maxEntry, bucketSize, bitSize);

	return true;
}

void FileWriter::ComputeMinAndMax(const std::list<Entry>& list, Entry& min, Entry& max)
{
	for each (Entry entry in list)
	{
		if (entry.mX > max.mX)
		{
			max.mX = entry.mX;
		}
		if (entry.mX < min.mX)
		{
			min.mX = entry.mX;
		}

		if (entry.mY > max.mY)
		{
			max.mY = entry.mY;
		}
		if (entry.mY < min.mY)
		{
			min.mY = entry.mY;
		}

		if (entry.mZ > max.mZ)
		{
			max.mZ = entry.mZ;
		}
		if (entry.mZ < min.mZ)
		{
			min.mZ = entry.mZ;
		}
	}
}

Entry FileWriter::ComputeBucketSize(const Entry& minEntry, const Entry& maxEntry, int possibleValues)
{
	// We want to split the data into buckets of possibleValues - 1 sections.
	
	Entry bucketSize;
	bucketSize.mX = (maxEntry.mX - minEntry.mX) / (possibleValues - 1);
	bucketSize.mY = (maxEntry.mY - minEntry.mY) / (possibleValues - 1);
	bucketSize.mZ = (maxEntry.mZ - minEntry.mZ) / (possibleValues - 1);

	return bucketSize;

}

void FileWriter::WriteBuffer(std::list<Entry> xyzList, Entry min, Entry max, Entry bucketSize, int bitSize)
{
	FileHeader h;
	h.numItems = xyzList.size();
	h.bitSize = bitSize;
	h.min = min;
	h.bucketSize = bucketSize;

	mBinFileHandle.write((char*)&h, sizeof(FileHeader));

	char outputChar;
	std::bitset<8> outputByte;
	int currentBit = 0;

	// Let's do it just for x first.
	for each (Entry entry in xyzList)
	{
		int section;
		double entryValues[] = { entry.mX, entry.mY, entry.mZ };
		double minValues[] = { min.mX, min.mY, min.mZ };
		double bucketValues[] = { bucketSize.mX, bucketSize.mY, bucketSize.mZ };

		for (int count = 0; count < 3; ++count)
		{
			section = (int)((entryValues[count] - minValues[count] + (bucketValues[count] / 2)) / bucketValues[count]);	// add half bucket size to round to section
			std::bitset<32> numBitSet(section);

			for (int i = 0; i < bitSize; ++i)
			{
				if (currentBit == 8)
				{
					outputChar = (char)outputByte.to_ulong();
					mBinFileHandle.write((char*)&outputChar, sizeof(char));
					outputByte.reset();
					currentBit = 0;
				}
				outputByte[currentBit] = numBitSet[i];
				++currentBit;
			}
		}
	}

	// Pad out remaining bytes
	while (currentBit != 8)
	{
		outputByte[currentBit++] = 0;
	}
	outputChar = (char) outputByte.to_ulong();
	mBinFileHandle.write((char*)&outputChar, sizeof(char));
}