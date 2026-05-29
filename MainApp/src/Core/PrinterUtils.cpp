#include"pch.h"
#include"PrinterUtils.h"
#include<iostream>

namespace PrinterHub{
	namespace Core {
		void PrinterUtils::displayModelByValue(Printer p){
			std::cout << "[Value] Model: " << p.getModel() << "\n";
		}

		void PrinterUtils::displayModelByRef(const Printer& p) {
			std::cout << "[Reference] Model: " << p.getModel() << "\n";
		}

		void PrinterUtils::displayModelByPointer(const Printer* p) {
			std::cout << "[Pointer] Model: " << p->getModel()<< "\n"; 
		}

		std::vector<Printer> PrinterFilters::Filter(const std::vector<Printer> printers, std::function<bool(const Printer&)> predicate) {
			std::vector<Printer> result;
			for (const auto& p : printers) {
				if (predicate(p)) {
					result.push_back(p);
				}
			}
			return result;
		}

	}
}