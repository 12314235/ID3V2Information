#include "Fabric.h"

Frame* Fabric::CreateFrame(const std::string& ID)
{
	if (ID == "TXXX") {
		return new TXXXFrame();
	}
	else if (is_text_frame(ID)) {
		return new TextFrame();
	}
	else if (ID == "COMM" || ID == "USLT") {
		return new COMM_USLTFrame();
	}
	else if (ID == "WXXX") {
		return new WXXXFrame();
	}
	else if (is_url_frame(ID)) {
		return new URLLinkFrame();
	}
	else if (ID == "MCDI") {
		return new MCDIFrame();
	}
	else if (ID == "ETCO") {
		return new ETCOFrame();
	}
	else if (ID == "SYLT") {
		return new SYLTFrame();
	}
	else if (ID == "RVA2") {
		return new RVA2Frame();
	}
	else if (ID == "EQU2") {
		return new EQU2Frame();
	}
	else if (ID == "SEEK") {
		return new SEEKFrame();
	}
	else if (ID == "PCNT") {
		return new PCNTFrame();
	}
	else if (ID == "POPM") {
		return new POPMFrame();
	}
	else if (ID == "RBUF") {
		return new RBUFFrame();
	}
	else if (ID == "LINK") {
		return new LINKFrame();
	}
	else if (ID == "POSS") {
		return new POSSFrame();
	}
	else if (ID == "USER") {
		return new USERFrame();
	}
	else if (ID == "OWNE") {
		return new OWNEFrame();
	}
	else if (ID == "COMR") {
		return new COMRFrame();
	}
	else if (ID == "ENCR" || ID == "GRID") {
		return new ENCR_GRIDFrame();
	}
	else if (ID == "PRIV" || ID == "UFID") {
		return new PRIV_UFIDFrame();
	}
	else if (ID == "SIGN") {
		return new SIGNFrame();
	}
	else if (ID == "AENK" || ID == "APIC" || ID == "ASPI" || ID == "GEOB" ||
		ID == "MLLT" || ID == "RVRB" || ID == "SYTC") {
		return new Frame();
	}
	else {
		return nullptr;
	}
}

bool Fabric::is_text_frame(const std::string& id)
{
	bool f = id[0] == 'T';

	for (int i = 1; i < 4; i++) {
		f &= isdigit(id[i]) || isupper(id[i]);
	}

	return f;
}

bool Fabric::is_url_frame(const std::string& id)
{
	bool f = id[0] == 'W';

	for (int i = 1; i < 4; i++) {
		f &= isdigit(id[i]) || isupper(id[i]);
	}

	return f;
}
