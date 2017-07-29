#pragma once

#include <string>
#include <fstream>
#include <list>

struct Entry;

class FileParser
{

public:
	FileParser(std::string filePath);
	~FileParser();

	std::list<Entry> Parse();

private:
	std::ifstream mFileHandle;

};

