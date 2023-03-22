#include "Parser.h"
#include "Fabric.h"
#include <bitset>
#include <iostream>

ID3Information* Parser::parse(const std::string& fylepath)
{
	file.open(fylepath, std::ios::binary);

	ParseHeader();

	if (storage->GetHeader()->flags.extended_header == 1) {
		ParseExtendedHeader();
	}

	std::string ID;
	char buff;

	size_t i = file.tellg();

	Fabric fabric;
	Frame* buff_frame;

	while (i <= storage->GetHeader()->size - 10) {
		for (int i = 0; i < 4; i++) {
			file.get(buff);
			ID += buff;
		}

		buff_frame = fabric.CreateFrame(ID);

		if (buff_frame == nullptr) {
			break;
		}

		buff_frame->ParseFrame(file, ID);

		storage->AddFrame(buff_frame);		

		ID = "";
		i += 14 + buff_frame->header.size;
	}
		
	return storage;
}

void Parser::ParseHeader()
{
	char buff[4];

	for (int i = 0; i < 3; i++) {
		file.get(buff[i]);
	}

	storage->SetIdentifier(buff[0], buff[1], buff[2]);

	file.get(buff[0]);

	storage->SetVersion(buff[0]);

	file.get(buff[0]);

	storage->SetRevisionNumber(buff[0]);

	file.get(buff[0]);

	std::bitset<8> flags = buff[0];

	storage->SetFlags(flags[7] == 1, flags[6] == 1, flags[5] == 1, flags[4] == 1);

	uint32_t size = 0;

	for (int i = 0; i < 4; i++) {
		file.get(buff[0]);
		size <<= 7;
		size |= buff[0] & 0x7F;
	}

	storage->SetSize(size);
}

void Parser::ParseExtendedHeader()
{
	storage->CreateExtendedHeader();

	uint32_t size = 0;

	char buff;

	for (int i = 0; i < 4; i++) {
		file.get(buff);
		size <<= 7;
		size |= buff & 0x7F;
	}

	storage->SetExtendedHeaderSize(size);

	file.get(buff);

	char* flags = new char[buff];

	for (uint16_t i = 0; i < size - 5; i++) {
		file.get(flags[i]);
	}

	storage->SetExtendedFlags(flags);
}
