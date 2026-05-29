#include"pch.h"
#include"MaintenanceTicket.h"

namespace PrinterHub {
	namespace Core {
		MaintenanceTicket::MaintenanceTicket(std::string IdTicket, std::string description, Priority priority):
			IdTicket(IdTicket), description(description), priority(priority),isclosed(false), technician(""),closedTime("") { }
		void MaintenanceTicket::CloseTicket(const std::string& techname, const std::string& time) {
			isclosed = true;
			technician = techname;
			closedTime = time;
		}
	}
}