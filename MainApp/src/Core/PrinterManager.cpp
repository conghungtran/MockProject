#include"pch.h"
#include"PrinterManager.h"

namespace PrinterHub {
	namespace Core {
		PrinterManager::PrinterManager(){ }
		PrinterManager::~PrinterManager(){ }
		void PrinterManager::addPrinter(const Printer& p) {
			repository.Add(p);
			lookUpMap[p.getId()] = repository.findById(p.getId());
			availableBrand.insert(p.getBrand());

		}
		bool PrinterManager::removePrinter(const std::string& id) {
			bool isremoved = repository.Remove(id);
			if (isremoved) {
				lookUpMap.erase(id);
			}
			return isremoved;
		}

		Printer* PrinterManager::getPrinterById(const std::string& id) {
			auto it = lookUpMap.find(id);
			if (it != lookUpMap.end()) {
				return it->second;
			}
			return nullptr;
		}

		const std::vector<Printer> PrinterManager::getAllPrinter() const {
			return repository.getAll();
		}

		std::vector<Brand> PrinterManager::GetAvailableBrand() const{
			std::vector<Brand> store;
			for (auto& a : availableBrand) {
				store.push_back(a);
			}
			return store;
		}

		std::size_t PrinterManager::getPrinterCount() const {
			return repository.Count();
		}
	}
}