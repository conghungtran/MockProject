#pragma once
#include<string>
#include"PrinterConfig.h"

namespace PrinterHub {
	namespace Core {
		class FirmwarePackage {
			private:
				std::string version;
				Brand targetBrand;
				size_t fileSize;
				char* rawData;
			public:
				FirmwarePackage(std::string version = "1.0", Brand brand = Brand::Other, size_t size = 0);
				~FirmwarePackage();
				FirmwarePackage(const FirmwarePackage& other);
				FirmwarePackage& operator =(const FirmwarePackage& other);
				FirmwarePackage(FirmwarePackage&& other) noexcept;
				FirmwarePackage& operator =(FirmwarePackage&& other) noexcept;

				std::string getVersion() const { return version; }
				Brand getTargetBrand() const { return targetBrand; }
				size_t getFileSize() const { return fileSize; }
		};
	}
}