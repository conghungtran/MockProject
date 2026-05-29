#pragma once
#include<exception>
#include<string>

namespace PrinterHub {
	namespace Core {
		class PrinterHubException : public std::exception {
		private:
			std::string message;
		public:
			explicit PrinterHubException(const std::string& msg): message(msg){}

			const char* what() const noexcept override {
				return message.c_str();
			}
		};

		class FileNotFoundException : public PrinterHubException {
		public:
			explicit FileNotFoundException(const std::string& filenames) : PrinterHubException("Loi: Khong tim thay file: "+filenames){}
		};

		class InvalidDataException : public PrinterHubException {
		public:
			explicit InvalidDataException(const std::string& details): PrinterHubException("Loi du lieu: "+details){}
		};
	}
}