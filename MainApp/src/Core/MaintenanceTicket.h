#pragma once
#include<string>
#include"PrinterConfig.h"

namespace PrinterHub {
	namespace Core {
		class MaintenanceTicket {
		private:
			std::string IdTicket;
			Priority priority;
			std::string description;

			bool isclosed;
			std::string technician;
			std::string closedTime;
		public:
			MaintenanceTicket(std::string IdTicket = "", std::string description = "", Priority priority = Priority::Medium);
			
			std::string getIdTicket() const { return IdTicket; }
			std::string getDescription() const { return description; }
			Priority getPriority() const { return priority; }
			bool getIsclosed() const { return isclosed; }
			std::string getTechnician() const { return technician; }
			std::string getClosedTime() const { return closedTime; }

			void CloseTicket(const std::string& techname, const std::string& time);
		};
	}
}