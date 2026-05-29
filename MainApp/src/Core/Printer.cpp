#include"pch.h"
#include"Printer.h"


namespace PrinterHub {
	namespace Core {
		Printer::Printer(std::string ID,std::string model, Brand brand, PrinterStatus status, std::string purchaseDate, int warrantyMonth):
			ID(ID), model(model), brand(brand), status(status),warrantyMonth(warrantyMonth), purchaseDate(purchaseDate) {
		}
		Printer::~Printer() {

		}

		Printer::Printer(const Printer& other) :ID(other.ID), model(other.model), brand(other.brand), status(other.status), purchaseDate(other.purchaseDate), warrantyMonth(other.warrantyMonth) {

		}

		Printer& Printer::operator=(const Printer& other) {
			if (this != &other) {
				ID = other.ID;
				model = other.model;
				brand = other.brand;
				status = other.status;
				warrantyMonth = other.warrantyMonth;
				purchaseDate = other.purchaseDate;
			}
			return *this;
		}

		bool Printer::operator==(const Printer& other) const {
			return this->ID == other.ID;
		}

		bool Printer::operator>(const Printer& other) const {
			return this->model > other.model;
		}

		std::ostream& operator<<(std::ostream& os, const Printer& p) {
			os << "Printer [ID:" <<p.ID<<",model:"<<p.model<<"warranty:"<<p.warrantyMonth<<" months]";
			return os;
		}
	}
}

