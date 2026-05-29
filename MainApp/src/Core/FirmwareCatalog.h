#pragma once
#include<memory>

namespace PrinterHub {
	namespace Core {
		class FirmwareCatalog {
		private: 
			static std::shared_ptr<FirmwareCatalog> instance;
			FirmwareCatalog()=default;

		public:
			FirmwareCatalog(const FirmwareCatalog& other) = delete;
			FirmwareCatalog& operator = (const FirmwareCatalog& other) = delete;
			static FirmwareCatalog& GetInstance();
		};
	}
}