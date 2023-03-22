#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "Frames.h"

class ID3Information {
private:

	struct HeaderFlags {
		bool unsynchronisation = false;
		bool extended_header = false;
		bool experimental_indicator = false;
		bool footer = false;

		HeaderFlags() {
			
		}

		HeaderFlags(bool un, bool ext, bool exp, bool foot) {
			this->unsynchronisation = un;
			this->extended_header = ext;
			this->experimental_indicator = experimental_indicator;
			this->footer = foot;
		}
	};

	struct Header {

		char identifier[3] = "";
		char version = ' ';
		char revision_number = ' ';

		HeaderFlags flags;

		uint32_t size = 0;

		Header() {

		}

		Header(const Header& other) {
			for (int i = 0; i < 3; i++) {
				this->identifier[i] = other.identifier[i];
			}
			this->version = other.version;
			this->revision_number = other.revision_number;
			this->size = other.size;
		}

		~Header() {
			delete &flags;
		}
	};

	struct ExtendedHeader {
		uint32_t size = 0;
		char number_of_flag_bytes = ' ';

		char* extended_flags = nullptr;

		ExtendedHeader() {

		}

		ExtendedHeader(uint32_t size, char number_of_flags) {
			this->size = size;
			this->number_of_flag_bytes = number_of_flags;

			extended_flags = new char[number_of_flags];
		}

		~ExtendedHeader() {
			delete extended_flags;
		}

		ExtendedHeader(const ExtendedHeader& other) {
			this->size = other.size;
			this->number_of_flag_bytes = other.number_of_flag_bytes;
			this->extended_flags = new char[other.number_of_flag_bytes];

			for (int i = 0; i < other.number_of_flag_bytes; i++) {
				this->extended_flags[i] = other.extended_flags[i];
			}
		}
	};

	Header header;

	int counter = 0;

	ExtendedHeader* extended_header;

	std::vector<Frame*> storage;

public:

	ID3Information() {

	}

	~ID3Information() {
		delete &header;
		delete extended_header;
		for (auto i : storage) {
			delete i;
		}
	}

	ID3Information(const ID3Information& other) {
		this->header = other.header;
		this->extended_header = other.extended_header;
		this->storage.clear();
		for (Frame* i : other.storage) {
			this->storage.push_back(i);
		}
	}

	void CreateExtendedHeader() {
		this->extended_header = new ExtendedHeader();
	}

	void SetIdentifier(char first, char second, char third) {
		header.identifier[0] = first;
		header.identifier[1] = second;
		header.identifier[2] = third;
	}

	void SetVersion(char version) {
		header.version = version;
	}

	void SetRevisionNumber(char number) {
		header.revision_number = number;
	}

	void SetFlags(bool unsynchronisation, bool extended_header, bool experimental_indicator, bool footer) {
		header.flags = HeaderFlags(unsynchronisation, extended_header, experimental_indicator, footer);;
	}

	void SetSize(uint32_t size) {
		header.size = size;
	}

	void SetExtendedHeaderSize(uint32_t size) {
		this->extended_header->size = size;
	}

	void SetNumberOfFlags(char number) {
		this->extended_header->number_of_flag_bytes = number;
	}

	void SetExtendedFlags(char* flags) {
		this->extended_header->extended_flags = flags;
	}

	void AddFrame(Frame* fr) {

		this->storage.push_back(fr);
		counter++;
	}

	const std::vector<Frame*>& GetStorage() {
		return this->storage;
	}

	const Header* GetHeader() {
		return &this->header;
	}

	void ShowFrames() {
		std::cout << "'''''''''''''''''''''''''''''\n";
		for (Frame* i : this->storage) {
			i->Show();
		}
		std::cout << "'''''''''''''''''''''''''''''\n";
	}
};