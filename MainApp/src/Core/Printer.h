#pragma once
#include<iostream>
#include<string>
#include"PrinterConfig.h"

namespace PrinterHub {
	namespace Core {
		class Printer {
			private:
				std::string ID;
				std::string model;
				Brand brand;
				PrinterStatus status;
				std::string purchaseDate;
				int warrantyMonth;
			public:
				Printer(std::string ID ="", std::string model = "", Brand brand = Brand::Other, PrinterStatus status = PrinterStatus::Active, std::string purchaseDate = "", int warrantyMonth = 12);
				~Printer();
				Printer(const Printer& other);
				Printer& operator=(const Printer& other);

				bool operator ==(const Printer& other) const;
				bool operator >(const Printer& other) const;
				friend std::ostream& operator <<(std::ostream& os, const Printer& p);

				std::string getId() const { return ID; }
				std::string getModel() const { return model; }
				Brand getBrand() const { return brand; }
				PrinterStatus getStatus() const { return status; }
				std::string getPurchaseDate() const { return purchaseDate; }
				int getWarantyMonth() const { return warrantyMonth; }
		};
	}
}