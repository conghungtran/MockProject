#include"pch.h"
#include"FirmwarePackage.h"

namespace PrinterHub {
	namespace Core {
		FirmwarePackage::FirmwarePackage(std::string version, Brand brand, size_t size): version(version), targetBrand(brand), fileSize(size),rawData(nullptr){
			if (fileSize > 0) {
				rawData = new char[fileSize];
			}
		}
		FirmwarePackage::~FirmwarePackage() {
			delete[] rawData;
		}

		FirmwarePackage::FirmwarePackage(const FirmwarePackage& other): version(other.version), targetBrand(other.targetBrand),fileSize(other.fileSize),rawData(nullptr){
			if (fileSize > 0) {
				rawData = new char[fileSize];
				std::copy(other.rawData, other.rawData + fileSize, rawData);
			}
		}
		FirmwarePackage& FirmwarePackage::operator=(const FirmwarePackage& other) {
			if (this != &other) {
				delete[] rawData;
				
				version = other.version;
				targetBrand = other.targetBrand;
				fileSize = other.fileSize;

				if (fileSize > 0) {
					rawData = new char[fileSize];
					std::copy(other.rawData, other.rawData + fileSize, rawData);
				}
				return *this;
			}
		}
		FirmwarePackage::FirmwarePackage(FirmwarePackage&& other) noexcept
		: version(std::move(other.version)), targetBrand(other.targetBrand), fileSize(std::exchange(other.fileSize,0)), rawData(std::exchange(other.rawData,nullptr)){ }
		FirmwarePackage& FirmwarePackage::operator=(FirmwarePackage&& other) noexcept{
			if (this != &other) {
				delete[] rawData;

				version = std::move(other.version);
				targetBrand = other.targetBrand;
				fileSize = std::exchange(other.fileSize, 0);
				rawData = std::exchange(other.rawData, nullptr);
			}
			return *this;
		}
	}
}