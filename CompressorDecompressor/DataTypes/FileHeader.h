#pragma once

#include "Entry.h"

struct FileHeader
{
	unsigned int numItems;
	unsigned char bitSize;
	Entry min;
	Entry bucketSize;
};
