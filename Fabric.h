#pragma once
#include "Frames.h"

class Fabric {
public:
	Frame* CreateFrame(const std::string& ID);

	bool is_text_frame(const std::string& id);
	bool is_url_frame(const std::string& id);
}; 