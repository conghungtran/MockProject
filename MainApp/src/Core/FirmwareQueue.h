#pragma once
#include<queue>
#include"FirmwarePackage.h"

namespace PrinterHub {
	namespace Core {
		class FirmwareQueue {
		private:
			std::queue<FirmwarePackage> fwQueue;
		public:
			void enqueue(FirmwarePackage package);
			bool dequeue();
			const FirmwarePackage* peek() const;
			bool is_empty() const;
			std::size_t GetSize() const;
		};
	}
}