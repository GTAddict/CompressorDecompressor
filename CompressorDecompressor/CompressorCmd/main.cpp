#include <iostream>
#include "FileParser.h"
#include "FileWriter.h"
#include "Entry.h"
#include <list>

enum class ARGTYPES
{
	INVALID = -1,
	ARG_PROGRAM_NAME = 0,
	ARG_FULLINPUTFILENAME,
	ARG_FULLOUTPUTFILENAME,
	ARG_BITSIZE,
	ARG_UNCOMPRESSED_DATA_FILE,
	COUNT
};

int main(int argc, char* argv[])
{
	std::string inputFileNameWithPath;
	std::string outputFileNameWithPath;
	int bitSize;
	bool shouldWriteUncompressedData;

	if (argc == (int)ARGTYPES::COUNT)
	{
		inputFileNameWithPath = std::string(argv[int(ARGTYPES::ARG_FULLINPUTFILENAME)]);
		outputFileNameWithPath = std::string(argv[int(ARGTYPES::ARG_FULLOUTPUTFILENAME)]);
		bitSize = atoi(argv[int(ARGTYPES::ARG_BITSIZE)]);

#pragma warning(push)
#pragma warning(disable:4800) // C4800 : 'int' : forcing value to bool 'true' or 'false' (performance warning)
		shouldWriteUncompressedData = atoi(argv[int(ARGTYPES::ARG_UNCOMPRESSED_DATA_FILE)]) != 0;
#pragma warning(pop) // The recommended fix is to change expression to use type bool or to specify expr != 0 at the end.
		// The first resolution is not possible and there is no atob; the second is just confusing.

	}
	else
	{
		using namespace std;
		char fullFileName[255];

		cout << "Invalid number of arguments. Expected "<< (int)ARGTYPES::COUNT - 1 <<", received " << argc - 1 << endl;
		
		cout << "Enter the full input file name with path:\n";
		cin >> fullFileName;
		inputFileNameWithPath = std::string(fullFileName);

		cout << "Enter the full output file name with path:\n";
		cin >> fullFileName;
		outputFileNameWithPath = std::string(fullFileName);

		cout << "Enter the bit size:\n";
		cin >> bitSize;

		cout << "Write uncompressed data file (0 or 1)?\n";
		cin >> shouldWriteUncompressedData;
	}

	FileParser fileParser(inputFileNameWithPath);
	std::list<Entry> list = fileParser.Parse();

	FileWriter fileWriter(outputFileNameWithPath, shouldWriteUncompressedData);
	fileWriter.WriteToFile(list, bitSize);

}