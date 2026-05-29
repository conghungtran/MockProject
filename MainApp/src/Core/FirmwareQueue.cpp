#include"pch.h"
#include"FirmwareQueue.h"

namespace PrinterHub {
	namespace Core {
		void FirmwareQueue::enqueue(FirmwarePackage package) {
			fwQueue.push(std::move(package));
		}

		bool FirmwareQueue::dequeue() {
			if (fwQueue.empty()) { return false; }
			fwQueue.pop();
			return true;
		}

		const FirmwarePackage* FirmwareQueue::peek() const {
			if (fwQueue.empty()) { return nullptr; }
			return &fwQueue.front();
		}
	}
}