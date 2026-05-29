#pragma once
#include"Printer.h"
#include<vector>
#include<functional>

namespace PrinterHub {
	namespace Core {
		class PrinterUtils {
		public:
			static void displayModelByValue(Printer p);
			static void displayModelByRef(const Printer& p);
			static void displayModelByPointer(const Printer* p);
		};

		class PrinterFilters {
		public:
			static std::vector<Printer> Filter(const std::vector<Printer> printers, std::function<bool(const Printer&)> predicate);
		};
	}
}
