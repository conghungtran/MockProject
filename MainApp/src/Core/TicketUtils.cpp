#include"pch.h"
#include"TicketUtils.h"
#include<algorithm>

namespace PrinterHub{
	namespace Core {
		void TicketUtils::SortTicketByPriority(std::vector<MaintenanceTicket>& tickets) {
			std::sort(tickets.begin(), tickets.end(), [](const MaintenanceTicket& MT1, const MaintenanceTicket& MT2) {
				return MT1.getPriority() > MT2.getPriority();
				});
		}

		const MaintenanceTicket* TicketUtils::FindTicketByKeyword(const std::vector<MaintenanceTicket>& tickets, std::string keyword) {
			auto it = std::find_if(tickets.begin(), tickets.end(), [&](const MaintenanceTicket& MT) {
				return MT.getDescription().find(keyword) != std::string::npos;
				});
			if (it != tickets.end()) {
				return &(*it);
			}
			return nullptr;
		}
	}
}