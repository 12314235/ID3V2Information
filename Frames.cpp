#include "Frames.h"

#include <fstream>
#include <iostream>

void Frame::ParseFrameHeader(std::ifstream& file)
{
	char size;

	for (int i = 0; i < 4; i++) {
		file.get(size);
		this->header.size <<= 7;
		this->header.size |= size & 0x7F;
	}

	file.get(this->header.flag1);
	file.get(this->header.flag2);
}

void Frame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	for (int i = 0; i < this->header.size; i++) {
		file.ignore();
	}
}

void Frame::Show()
{
}

void TextFrame::ParseFrame(std::ifstream& file, const std::string& id) {
	this->header.ID = id;

	this->ParseFrameHeader(file);

	file.get(this->text_encoding);

	char buff;
	std::string string_buff;

	for (int i = 0; i < this->header.size - 1;) {
		file.get(buff);
		i += 1;
		string_buff = "";

		while (i < this->header.size - 1 && buff != '\0') {
			string_buff += buff;
			file.get(buff);
			i++;
		}

		if (i == this->header.size - 1) {
			string_buff += buff;
		}

		this->information.push_back(string_buff);
	}
}

void TextFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << encoding.at(this->text_encoding) << '\n';
	for (std::string& i : this->information) {
		std::cout << i << '\n';
	}
}

void URLLinkFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	char buff;

	for (uint16_t i = 0; i < this->header.size; i++) {
		file.get(buff);
		this->url += buff;
	}
}

void URLLinkFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << this->url << '\n';
}

void WXXXFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	char buff;

	file.get(this->text_encoding);

	file.get(buff);

	while (buff != '\0') {
		this->description += buff;
		file.get(buff);
	}

	for (int i = 0; i < this->header.size - 2 - this->description.size(); i++) {
		file.get(buff);
		this->url += buff;
	}
}

void WXXXFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << this->description << '\n';
	std::cout << this->url << '\n';
}

void TXXXFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	char buff;

	file.get(this->text_encoding);

	file.get(buff);

	while (buff != '\0') {
		this->description += buff;
		file.get(buff);
	}

	std::string string_buff;

	for (int i = 0; i < this->header.size - 2 - this->description.size();) {
		file.get(buff);
		i += 1;
		string_buff = "";

		while (i < this->header.size - 2 - this->description.size() && buff != '\0') {
			string_buff += buff;
			file.get(buff);
			i++;
		}

		this->information.push_back(string_buff);
	}
}

void TXXXFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << encoding.at(this->text_encoding) << '\n';
	std::cout << this->description << '\n';
	for (std::string& i : this->information) {
		std::cout << i << '\n';
	}
}

void MCDIFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	char buff;

	for (uint16_t i = 0; i < this->header.size; i++) {
		file.get(buff);
		this->cd_toc.push_back(buff);
	}
}

void MCDIFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	for (char i : this->cd_toc) {
		std::cout << i << ' ';
	}
	std::cout << '\n';
}

void ETCOFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	file.get(this->time_stamp_format);
	
	char buff;

	for (int i = 0; i < this->header.size - 1; i += 5) {
		TimeStamp* new_st = new TimeStamp();
		file.get(new_st->type_of_event);
		for (int j = 0; j < 4; j++) {
			file.get(buff);
			new_st->time_stamp <<= 8;
			new_st->time_stamp += buff;
		}
		this->time_stamp.push_back(new_st);
	}
}

void ETCOFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << ((int)this->time_stamp_format == 1 ? "Absolute time, 32 bit sized, using MPEG [MPEG] frames as unit" : "Absolute time, 32 bit sized, using milliseconds as unit") << '\n';
	for (TimeStamp* i : this->time_stamp) {
		std::cout << event.at(i->type_of_event) << '\n';
		std::cout << i->time_stamp << '\n';
	}
}

void COMM_USLTFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	file.get(this->text_encoding);
	for (int i = 0; i < 3; i++) {
		file.get(this->language[i]);
	}

	char buff;

	file.get(buff);

	while (buff != '\0') {
		this->content_descriptor += buff;
		file.get(buff);
	}

	for (int i = 0; i < this->header.size - 5 - this->content_descriptor.size(); i++) {
		file.get(buff);
		this->text += buff;
	}
}

void COMM_USLTFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	for (char i : this->language) {
		std::cout << i;
	}
	std::cout << '\n';
	std::cout << this->content_descriptor << '\n';
	std::cout << this->text << '\n';
}

void SYLTFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	file.get(this->text_encoding);
	for (int i = 0; i < 3; i++) {
		file.get(this->language[i]);
	}
	file.get(this->time_stamp_format);
	file.get(this->content_type);

	char buff;

	for (uint16_t i = 0; i < this->header.size - 6; i++) {
		file.get(buff);
		this->content_descriptor += buff;
	}
}

void SYLTFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	for (char i : this->language) {
		std::cout << i;
	}
	std::cout << '\n';
	std::cout << ((int)this->time_stamp_format == 1 ? "Absolute time, 32 bit sized, using MPEG [MPEG] frames as unit" : "Absolute time, 32 bit sized, using milliseconds as unit") << '\n';
	std::cout << content.at(this->content_type) << '\n';
	std::cout << this->content_descriptor << '\n';
}

void EQU2Frame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	file.get(this->interpolation_method);

	char buff;

	file.get(buff);

	while (buff != '\0') {
		this->identification += buff;
		file.get(buff);
	}

	for (int i = 0; i < this->header.size - 3 - this->identification.size(); i += 5) {
		situation new_situation;
		
		file.get(buff);
		new_situation.frequency += buff;
		new_situation.frequency <<= 8;
		file.get(buff);
		new_situation.frequency += buff;

		file.get(buff);
		new_situation.volume_adjustment += buff;
		new_situation.volume_adjustment <<= 8;
		file.get(buff);
		new_situation.volume_adjustment += buff;

		this->situations.push_back(new_situation);
	}

}

void EQU2Frame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << (this->interpolation_method == 0 ? "Band" : "Linear") << '\n';
	std::cout << identification << '\n';
	for (int i = 0; i < this->situations.size(); i++) {
		std::cout << situations[i].frequency << '\n';
		std::cout << situations[i].volume_adjustment << '\n';
	}
}

void SEEKFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	char buff;

	for (uint16_t i = 0; i < this->header.size; i++) {
		file.get(buff);
		this->counter <<= 8;
		this->counter += buff;
	}
}

void SEEKFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << "Play counter: " << this->counter << '\n';
	std::cout << this->counter << '\n';
}

void PCNTFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	char buff;

	for (uint16_t i = 0; i < this->header.size; i++) {
		file.get(buff);
		this->counter <<= 8;
		this->counter += buff;
	}
}

void PCNTFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << "Play counter: " << this->counter << '\n';
}

void POPMFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	char buff;

	file.get(buff);

	while (buff != '\0') {
		this->email += buff;
		file.get(buff);
	}

	file.get(buff);

	this->rating = buff;

	for (int i = 0; i < this->header.size - 2 - this->email.size(); i++) {
		file.get(buff);
		this->counter <<= 8;
		this->counter += buff;
	}

}

void POPMFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << this->email << '\n';
	std::cout << this->rating << '\n';
	std::cout << this->counter << '\n';
}

void RBUFFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	for (int i = 0; i < 3; i++) {
		file.get(this->buff_size[i]);
	}

	file.get(this->emb_into_flag);

	char buff;

	for (int i = 0; i < 4; i++) {
		file.get(buff);
		this->offset <<= 8;
		this->offset += buff;
	}

}

void RBUFFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << this->emb_into_flag << '\n';
	std::cout << this->offset << '\n';
}

void LINKFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	char buff;

	for (int i = 0; i < 4; i++) {
		file.get(buff);
		this->frame_id += buff;
	}

	file.get(buff);

	while (buff != '\0') {
		this->url += buff;
		file.get(buff);
	}

	for (int i = 0; i < this->header.size - 5 - this->url.size(); i++) {
		file.get(buff);
		this->add_data += buff;
	}

}

void LINKFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << "Linked Frame ID: " << this->frame_id << '\n';
	std::cout << this->url << '\n';
	std::cout << this->add_data << '\n';
}

void USERFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	file.get(this->text_encoding);

	for (int i = 0; i < 3; i++) {
		file.get(this->language[i]);
	}

	char buff;

	for (uint16_t i = 0; i < this->header.size - 4; i++) {
		file.get(buff);
		this->text += buff;
	}
}

void USERFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	for (int i = 0; i < 3; i++) {
		std::cout << this->language[i];
	}
	std::cout << '\n';
	std::cout << this->text << '\n';
}

void ENCR_GRIDFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	char buff;

	file.get(buff);

	while (buff != '\0') {
		this->owner_id += buff;
		file.get(buff);
	}

	file.get(this->symb);

	for (int i = 0; i < this->header.size - 2 - owner_id.size(); i++) {
		file.get(buff);
		this->binary_data.push_back(buff);
	}
}

void ENCR_GRIDFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << this->owner_id << '\n';
	std::cout << this->symb << '\n';
	for (auto i : this->binary_data) {
		std::cout << i;
	}
	std::cout << '\n';
}

void PRIV_UFIDFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	char buff;

	file.get(buff);

	while (buff != '\0') {
		this->owner_id += buff;
		file.get(buff);
	}

	for (int i = 0; i < this->header.size - 1 - this->owner_id.size(); i++) {
		file.get(buff);
		this->binary_data.push_back(buff);
	}
}

void PRIV_UFIDFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << this->owner_id << '\n';
	for (auto i : this->binary_data) {
		std::cout << i;
	}
	std::cout << '\n';
}

void SIGNFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	file.get(this->symb);

	char buff;

	for (uint16_t i = 0; i < this->header.size - 1; i++) {
		file.get(buff);
		this->binary_data.push_back(buff);
	}
}

void SIGNFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << this->symb << '\n';
	for (auto i : this->binary_data) {
		std::cout << i;
	}
	std::cout << '\n';
}

void OWNEFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	file.get(this->text_encoding);
	
	char buff;

	file.get(buff);

	while (buff != '\0') {
		this->price += buff;
		file.get(buff);
	}

	for (int i = 0; i < 8; i++) {
		file.get(buff);
		this->date_of_purch += buff;
	}

	for (int i = 0; i < this->header.size - 9 - this->price.size(); i++) {
		file.get(buff);
		this->seller += buff;
	}
}

void OWNEFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << this->price << '\n';
	std::cout << this->date_of_purch << '\n';
	std::cout << this->seller << '\n';
}

void RVA2Frame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	char buff;

	file.get(buff);

	while (buff != '\0') {
		this->identification += buff;
		file.get(buff);
	}

	for (size_t i = 0; i < this->header.size - 1 - this->identification.size();) {
		RVA2_channel new_channel;
		file.get(new_channel.type_of_channel);
		for (int j = 0; j < 2; j++) {
			file.get(buff);
			new_channel.volume_adjustment <<= 8;
			new_channel.volume_adjustment += buff;
		}
		file.get(new_channel.bits_representing_peak);
		for (int j = 0; j < new_channel.bits_representing_peak; j++) {
			file.get(buff);
			new_channel.peak_volume += buff;
		}

		i += 4 + new_channel.peak_volume.size();

		this->channels.push_back(new_channel);
	}
}

void RVA2Frame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << this->identification << '\n';
	for (RVA2_channel i : this->channels) {
		std::cout << '\n';
		std::cout << channel.at(i.type_of_channel) << '\n';
		std::cout << i.volume_adjustment << '\n';
		std::cout << i.bits_representing_peak << '\n';
		std::cout << i.peak_volume << '\n';
	}

}

void POSSFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	file.get(this->time_stamp_format);

	char buff;

	for (uint16_t i = 0; i < this->header.size - 1; i++) {
		file.get(buff);
		this->position.push_back(buff);
	}
}

void POSSFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << ((int)this->time_stamp_format == 1 ? "Absolute time, 32 bit sized, using MPEG [MPEG] frames as unit" : "Absolute time, 32 bit sized, using milliseconds as unit") << '\n';
	for (char i : this->position) {
		std::cout << i;
	}
	std::cout << '\n';
}

void COMRFrame::ParseFrame(std::ifstream& file, const std::string& id)
{
	this->header.ID = id;

	this->ParseFrameHeader(file);

	file.get(this->text_encoding);

	char buff;

	file.get(buff);

	while (buff != '\0') {
		this->price += buff;
		file.get(buff);
	}

	for (int i = 0; i < 8; i++) {
		file.get(buff);
		this->valid_until += buff;
	}

	while (buff != '\0') {
		this->contact_url += buff;
		file.get(buff);
	}

	file.get(this->received_as);

	while (buff != '\0') {
		this->name_of_seller += buff;
		file.get(buff);
	}

	while (buff != '\0') {
		this->description += buff;
		file.get(buff);
	}

	while (buff != '\0') {
		this->picture_MIME_type += buff;
		file.get(buff);
	}

	size_t s = picture_MIME_type.size() + description.size() + name_of_seller.size() + contact_url.size() + valid_until.size() + price.size();

	for (uint16_t i = 0; i < this->header.size - 7 - s; i++) {
		file.get(buff);
		this->logo.push_back(buff);
	}
}

void COMRFrame::Show()
{
	std::cout << "----------------------\n";
	std::cout << this->header.ID << " Frame" << '\n';
	std::cout << this->price << '\n';
	std::cout << this->valid_until << '\n';
	std::cout << this->contact_url << '\n';
	std::cout << this->received_as << '\n';
	std::cout << this->name_of_seller << '\n';
	std::cout << this->description << '\n';
	std::cout << this->picture_MIME_type << '\n';
	for (char i : this->logo) {
		std::cout << i;
	}
	std::cout << '\n';
}


