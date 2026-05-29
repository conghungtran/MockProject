#include"pch.h"
#include"FirmwareCatalog.h"
#include<mutex>
namespace PrinterHub {
	namespace Core {
		static FirmwareCatalog* instanceptr = nullptr;
		static std::once_flag initflag;

		FirmwareCatalog& FirmwareCatalog::GetInstance() {
			std::call_once(initflag, []() {
				instanceptr = new FirmwareCatalog();
				});
			return *instanceptr;

		}
	}
}