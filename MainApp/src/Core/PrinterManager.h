#pragma once
#include"DeviceRepository.h"
#include"Printer.h"
#include<vector>
#include<string>
#include<map>
#include<set>
#include"IPrinterObserver.h"

namespace PrinterHub {
	namespace Core {
		class PrinterManager {
		private:
			std::vector<IPrinterObserver*> observers;

			DeviceRepository<Printer> repository;

			std::map<std::string, Printer*> lookUpMap;

			std::set<Brand> availableBrand;
		public:
			PrinterManager();
			~PrinterManager();
			void addPrinter(const Printer& p);
			bool removePrinter(const std::string& p);
			Printer* getPrinterById(const std::string& p);
			const std::vector<Printer> getAllPrinter() const;
			std::size_t getPrinterCount() const;
			std::vector<Brand> GetAvailableBrand() const;
			void Subcribe(IPrinterObserver* observer);
			void UnSubcribe(IPrinterObserver* observer);
			void notify();
		};
	}
}