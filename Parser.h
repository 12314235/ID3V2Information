#include "Storage.h"
#include "Frames.h"

#include <fstream>

class Parser {
public:

	ID3Information* storage = new ID3Information();

	std::ifstream file;

	ID3Information* parse(const std::string& fylepath);

	void ParseHeader();
	void ParseExtendedHeader();
};