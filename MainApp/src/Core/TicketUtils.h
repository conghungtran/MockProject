#pragma once
#include"MaintenanceTicket.h"
#include<vector>
#include<string>

namespace PrinterHub {
	namespace Core {
		class TicketUtils {
			static void SortTicketByPriority(std::vector<MaintenanceTicket>& tickets);
			static const MaintenanceTicket* FindTicketByKeyword(const std::vector<MaintenanceTicket>& tickets, std::string keyword);
		};
	}
}