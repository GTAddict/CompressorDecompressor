#include <fstream>
#include <bitset>
#include <list>
#include "Entry.h"
#include "FileHeader.h"
#include <iostream>

using namespace std;
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		return -1;
	}
/*
	cin >> argv[1];
	cin >> argv[2];
	*/
	ifstream filin(argv[1], ios::binary | ios::in );
	ifstream origFilin(argv[2], ios::binary | ios::in);

	//ifstream filin("C:\\Users\\kyeragudipati\\dev\\Students\\CH13\\kyeragudipati\\ProgrammingI\\Nov9\\CompressorDecompressor\\data\\verts5.bin", ios::binary | ios::in);
	//ifstream origFilin("C:\\Users\\kyeragudipati\\dev\\Students\\CH13\\kyeragudipati\\ProgrammingI\\Nov9\\CompressorDecompressor\\data\\verts.uncompressed", ios::binary | ios::in);

	filin.seekg(0, ios::end);
	int fileSize = (int) filin.tellg();
	filin.seekg(0, ios::beg);

	FileHeader h;
	filin.read((char*)&h, sizeof(FileHeader));
	int numItems = h.numItems;
	int bitsize = h.bitSize;

	cout << bitsize << ";";
	cout << fileSize << ";";

	char inputChar;
	int currentBitInStream = 0;
	std::bitset<32> finalInt;
	bool read = false;
	std::list<Entry> finalList;
	int finalSection;

	filin.read(&inputChar, sizeof(char));
	std::bitset<8> currentChar(inputChar);
	
	for (int count = 0; count < numItems; ++count)
	{
		Entry entry;

		for (int value = 0; value < 3; ++value)
		{
			finalInt.reset();
			for (int i = 0; i < bitsize; ++i)
			{
				if (currentBitInStream == 8)
				{
					currentBitInStream = 0;
					filin.read((char*)&inputChar, sizeof(char));
					std::bitset<8> tempChar(inputChar);
					currentChar = tempChar;
				}

				finalInt[i] = currentChar[currentBitInStream];
				currentBitInStream++;
			}

			finalSection = finalInt.to_ulong();

			if (value == 0)
			{
				entry.mX = h.min.mX + (finalSection * h.bucketSize.mX);
			}
			else if (value == 1)
			{
				entry.mY = h.min.mY + (finalSection * h.bucketSize.mY);
			}
			else if (value == 2)
			{
				entry.mZ = h.min.mZ + (finalSection * h.bucketSize.mZ);
			}

		}

		finalList.push_back(entry);
	}

	int  i = finalList.size();

	std::list<Entry> originalEntries;

	Entry e;

	for (unsigned int i = 0; i < h.numItems; ++i)
	{
		origFilin.read((char*)&e, sizeof(Entry));
		originalEntries.push_back(e);
	}

	if (originalEntries.size() != finalList.size())
	{
		return -1;
	}

	int size = originalEntries.size();
	double squaredSum = 0;

	std::list<Entry>::iterator origIt = originalEntries.begin(), finalIt = finalList.begin();

	for (; origIt != originalEntries.end(); ++origIt, ++finalIt)
	{
		squaredSum += pow((origIt->mX - finalIt->mX), 2);
		squaredSum += pow((origIt->mY - finalIt->mY), 2);
		squaredSum += pow((origIt->mZ - finalIt->mZ), 2);
	}

	double meanSquaredSum = squaredSum / (size * 3);
	double RMS = sqrt(meanSquaredSum);

	cout << RMS << endl;

	filin.close();
	origFilin.close();
	
}