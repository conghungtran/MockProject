#pragma once

namespace PrinterHub {
	namespace Core {
		enum class PrinterStatus {
			Active,
			Inservice,
			Retired,
		};

		enum class Brand {
			HP,
			Canon,
			Epson,
			Other,
		};

		enum class Priority {
			Low,
			Medium,
			High,
			Critical
		};
	}
}