#pragma once

#include <string>
#include <vector>

// ========== ENUMS ==========

// Enum cho hãng máy in
enum class Brand
{
    Unknown = 0,
    HP,
    Canon,
    Epson,
    Brother,
    Xerox,
    Samsung,
    Lexmark
};

// Enum cho trạng thái máy in
enum class PrinterStatus
{
    Idle = 0,           // Sẵn sàng
    Printing,           // Đang in
    Paused,             // Tạm dừng
    Error,              // Lỗi
    Offline,            // Ngoại tuyến
    Maintenance         // Đang bảo trì
};

// ========== FORWARD DECLARATION ==========
class IPrinterState;

// ========== PRINTER CLASS ==========
class Printer
{
private:
    // Thuộc tính
    std::string m_id;
    std::string m_model;
    Brand m_brand;
    PrinterStatus m_status;
    std::string m_purchaseDate;      // Format: YYYY-MM-DD
    int m_warrantyMonths;
    std::vector<std::string> m_firmwareHistory;
    IPrinterState* m_pState;          // State pattern

public:
    // Constructor & Destructor
    Printer();
    Printer(const std::string& id, const std::string& model, Brand brand);
    ~Printer();

    // Copy constructor & Assignment operator (disabled)
    Printer(const Printer&) = delete;
    Printer& operator=(const Printer&) = delete;

    // ========== GETTERS ==========
    std::string GetId() const;
    std::string GetModel() const;
    Brand GetBrand() const;
    PrinterStatus GetStatus() const;
    std::string GetPurchaseDate() const;
    int GetWarrantyMonths() const;
    const std::vector<std::string>& GetFirmwareHistory() const;
    IPrinterState* GetState() const;

    // ========== SETTERS ==========
    void SetId(const std::string& id);
    void SetModel(const std::string& model);
    void SetBrand(Brand brand);
    void SetPurchaseDate(const std::string& date);
    void SetWarrantyMonths(int months);

    // ========== PHƯƠNG THỨC QUẢN LÝ ==========

    // Thêm phiên bản firmware mới
    void AddFirmwareVersion(const std::string& version);

    // Kiểm tra còn bảo hành không
    bool IsUnderWarranty() const;

    // Chuyển đổi Brand sang string
    static std::string BrandToString(Brand brand);

    // Chuyển đổi string sang Brand
    static Brand StringToBrand(const std::string& brandStr);

    // Chuyển đổi PrinterStatus sang string
    static std::string StatusToString(PrinterStatus status);

    // Chuyển đổi string sang PrinterStatus
    static PrinterStatus StringToStatus(const std::string& statusStr);

    // ========== STATE PATTERN METHODS ==========

    // Thay đổi trạng thái
    void SetState(IPrinterState* pNewState);

    // Các hành động của máy in (delegate cho state)
    void Print(const std::string& document);
    void Pause();
    void Resume();
    void Cancel();
    void Maintenance();
    void HandleError();

    // Lấy tên trạng thái hiện tại
    std::string GetCurrentStateName() const;
};

// ========== STATE INTERFACE ==========
class IPrinterState
{
public:
    virtual ~IPrinterState() = default;

    // Các hành động có thể thực hiện
    virtual void Print(Printer* pPrinter, const std::string& document) = 0;
    virtual void Pause(Printer* pPrinter) = 0;
    virtual void Resume(Printer* pPrinter) = 0;
    virtual void Cancel(Printer* pPrinter) = 0;
    virtual void Maintenance(Printer* pPrinter) = 0;
    virtual void HandleError(Printer* pPrinter) = 0;

    // Lấy tên trạng thái
    virtual std::string GetStateName() const = 0;
};

// ========== CONCRETE STATES ==========

// Trạng thái: Sẵn sàng
class IdleState : public IPrinterState
{
public:
    void Print(Printer* pPrinter, const std::string& document) override;
    void Pause(Printer* pPrinter) override;
    void Resume(Printer* pPrinter) override;
    void Cancel(Printer* pPrinter) override;
    void Maintenance(Printer* pPrinter) override;
    void HandleError(Printer* pPrinter) override;
    std::string GetStateName() const override;
};

// Trạng thái: Đang in
class PrintingState : public IPrinterState
{
public:
    void Print(Printer* pPrinter, const std::string& document) override;
    void Pause(Printer* pPrinter) override;
    void Resume(Printer* pPrinter) override;
    void Cancel(Printer* pPrinter) override;
    void Maintenance(Printer* pPrinter) override;
    void HandleError(Printer* pPrinter) override;
    std::string GetStateName() const override;
};

// Trạng thái: Tạm dừng
class PausedState : public IPrinterState
{
public:
    void Print(Printer* pPrinter, const std::string& document) override;
    void Pause(Printer* pPrinter) override;
    void Resume(Printer* pPrinter) override;
    void Cancel(Printer* pPrinter) override;
    void Maintenance(Printer* pPrinter) override;
    void HandleError(Printer* pPrinter) override;
    std::string GetStateName() const override;
};

// Trạng thái: Lỗi
class ErrorState : public IPrinterState
{
public:
    void Print(Printer* pPrinter, const std::string& document) override;
    void Pause(Printer* pPrinter) override;
    void Resume(Printer* pPrinter) override;
    void Cancel(Printer* pPrinter) override;
    void Maintenance(Printer* pPrinter) override;
    void HandleError(Printer* pPrinter) override;
    std::string GetStateName() const override;
};

// Trạng thái: Bảo trì
class MaintenanceState : public IPrinterState
{
public:
    void Print(Printer* pPrinter, const std::string& document) override;
    void Pause(Printer* pPrinter) override;
    void Resume(Printer* pPrinter) override;
    void Cancel(Printer* pPrinter) override;
    void Maintenance(Printer* pPrinter) override;
    void HandleError(Printer* pPrinter) override;
    std::string GetStateName() const override;
};

// Trạng thái: Ngoại tuyến
class OfflineState : public IPrinterState
{
public:
    void Print(Printer* pPrinter, const std::string& document) override;
    void Pause(Printer* pPrinter) override;
    void Resume(Printer* pPrinter) override;
    void Cancel(Printer* pPrinter) override;
    void Maintenance(Printer* pPrinter) override;
    void HandleError(Printer* pPrinter) override;
    std::string GetStateName() const override;
};