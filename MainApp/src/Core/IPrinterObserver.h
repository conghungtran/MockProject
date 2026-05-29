#pragma once
#include<vector>

namespace PrinterHub {
	namespace Core {
		class IPrinterObserver {
		public:
			virtual ~IPrinterObserver() = default;
			virtual void OnPrinterListChanged() = 0;
		};
	}
}