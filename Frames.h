#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct FrameHeader {
	std::string ID;
	uint32_t size = 0;
	char flag1;
	char flag2;
};

const static std::unordered_map<char, std::string> encoding = {
	{0, "ISO-8859-1 [ISO-8859-1]"},
	{1, "UTF-16 [UTF-16]"},
	{2, "UTF-16BE"},
	{3, "UTF-8"}
};

class Frame {
public:
	void ParseFrameHeader(std::ifstream& file);
	virtual void ParseFrame(std::ifstream& file, const std::string& id);
	virtual void Show();

	virtual ~Frame() = default;

	FrameHeader header;
};

class TextFrame : public Frame { //T000 - TZZZ
public:
	char text_encoding;
	std::vector<std::string> information;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class TXXXFrame : public TextFrame {
public:
	std::string description;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class URLLinkFrame : public Frame { //W000 - WZZZ 
public:
	std::string url;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class WXXXFrame : public URLLinkFrame {
public:
	char text_encoding;
	std::string description;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class MCDIFrame : public Frame {
public:
	std::vector<char> cd_toc;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

struct TimeStamp {
	char type_of_event;
	uint32_t time_stamp;
};

const static std::unordered_map<char, std::string> event = {
		{0, "padding (has no meaning)"},
		{1, "end of initial silence"},
		{2, "intro start"},
		{3, "main part start"},
		{4, "outro start"},
		{5, "outro end"},
		{6, "verse start"},
		{7, "refrain start"},
		{8, "interlude start"},
		{9, "theme start"},
		{10, "variation start"},
		{11, "key change"},
		{12, "time change"},
		{13, "momentary unwanted noise"},
		{14, "sustained noise"},
		{15, "sustained noise end"},
		{16, "intro end"},
		{17, "main part end"},
		{18, "verse end"},
		{19, "refrain end"},
		{20, "theme end"},
		{21, "profanity"},
		{22, "profanity end"}
};

class ETCOFrame : public Frame {
public:
	char time_stamp_format;
	std::vector<TimeStamp*> time_stamp;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

const static std::unordered_map<char, std::string> content = {
		{0, "other"},
		{1, "lyrics"},
		{2, "text transcription"},
		{3, "movement/part name"},
		{4, "events"},
		{5, "chord"},
		{6, "trivia/'pop up'"},
		{7, "URLs to webpages"},
		{8, "URLs to images"}
};

class SYLTFrame : public Frame { //SYLT
public:
	char text_encoding;
	char language[3];
	char time_stamp_format;
	char content_type;
	std::string content_descriptor;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class COMM_USLTFrame : public Frame { //USLT
public:
	char text_encoding;
	char language[3];
	std::string content_descriptor;
	std::string text;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

struct RVA2_channel {
	char type_of_channel = '\0';
	uint16_t volume_adjustment = 0;
	char bits_representing_peak = '\0';
	std::string peak_volume = "";
};

const static std::unordered_map<char, std::string> channel = {
		{0, "other"},
		{1, "Master volume"},
		{2, "Front right"},
		{3, "Front left"},
		{4, "Back right"},
		{5, "Back left"},
		{6, "Front centre"},
		{7, "Back centre"},
		{8, "Subwoofer"}
};

class RVA2Frame : public Frame { //RVA2
public:
	std::string identification;
	std::vector<RVA2_channel> channels;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

struct situation {
	uint16_t frequency = 0;
	uint16_t volume_adjustment = 0;
};

class EQU2Frame : public Frame { //EQU2
public:
	char interpolation_method;
	std::string identification;
	std::vector<situation> situations;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class SEEKFrame : public Frame { //PCNT
public:
	size_t counter;

	void ParseFrame(std::ifstream& file, const std::string& id); 
	void Show();
};

class PCNTFrame : public SEEKFrame {
public:
public:
	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class POPMFrame : public PCNTFrame { //POPM
public:
	std::string email;
	uint8_t rating;

	void ParseFrame(std::ifstream& file, const std::string& id); 
	void Show();
};

class RBUFFrame : public Frame { //RBUFF
public:
	char buff_size[3];
	char emb_into_flag;
	uint32_t offset;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class LINKFrame : public Frame { //LINK
public:
	std::string frame_id;
	std::string url;
	std::string add_data;

	void ParseFrame(std::ifstream& file, const std::string& id); 
	void Show();
};

class POSSFrame : public Frame { //POSS
public:
	char time_stamp_format;
	std::vector<char> position;
	void ParseFrame(std::ifstream& file, const std::string& id); 
	void Show();
};

class USERFrame : public Frame { //USER
public:
	char text_encoding;
	char language[3];
	std::string text;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class OWNEFrame : public Frame { //OWNE
public:
	char text_encoding;
	std::string price;
	std::string date_of_purch;
	std::string seller;

	void ParseFrame(std::ifstream& file, const std::string& id); 
	void Show();
};

class COMRFrame : public Frame { //COMR
public:
	char text_encoding;
	std::string price;
	std::string valid_until;
	std::string contact_url;
	char received_as;
	std::string name_of_seller;
	std::string description;
	std::string picture_MIME_type;
	std::vector<char> logo;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class SIGNFrame : public Frame { //SIGN
public:
	char symb;
	std::vector<char> binary_data;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class PRIV_UFIDFrame : public SIGNFrame { //PRIV
public:
	std::string owner_id;

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};

class ENCR_GRIDFrame : public PRIV_UFIDFrame { //ENCR
public:

	void ParseFrame(std::ifstream& file, const std::string& id);
	void Show();
};