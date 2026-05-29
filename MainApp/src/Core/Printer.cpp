#include "pch.h"
#include "Printer.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

// ========== PRINTER CLASS IMPLEMENTATION ==========

// Constructor mặc định
Printer::Printer()
    : m_id("")
    , m_model("")
    , m_brand(Brand::Unknown)
    , m_status(PrinterStatus::Idle)
    , m_purchaseDate("")
    , m_warrantyMonths(12)
    , m_pState(nullptr)
{
    // Khởi tạo state mặc định là Idle
    SetState(new IdleState());
}

// Constructor với tham số
Printer::Printer(const std::string& id, const std::string& model, Brand brand)
    : m_id(id)
    , m_model(model)
    , m_brand(brand)
    , m_status(PrinterStatus::Idle)
    , m_purchaseDate("")
    , m_warrantyMonths(12)
    , m_pState(nullptr)
{
    SetState(new IdleState());
}

// Destructor
Printer::~Printer()
{
    delete m_pState;
    m_pState = nullptr;
}

// ========== GETTERS ==========

std::string Printer::GetId() const
{
    return m_id;
}

std::string Printer::GetModel() const
{
    return m_model;
}

Brand Printer::GetBrand() const
{
    return m_brand;
}

PrinterStatus Printer::GetStatus() const
{
    return m_status;
}

std::string Printer::GetPurchaseDate() const
{
    return m_purchaseDate;
}

int Printer::GetWarrantyMonths() const
{
    return m_warrantyMonths;
}

const std::vector<std::string>& Printer::GetFirmwareHistory() const
{
    return m_firmwareHistory;
}

IPrinterState* Printer::GetState() const
{
    return m_pState;
}

// ========== SETTERS ==========

void Printer::SetId(const std::string& id)
{
    m_id = id;
}

void Printer::SetModel(const std::string& model)
{
    m_model = model;
}

void Printer::SetBrand(Brand brand)
{
    m_brand = brand;
}

void Printer::SetPurchaseDate(const std::string& date)
{
    m_purchaseDate = date;
}

void Printer::SetWarrantyMonths(int months)
{
    m_warrantyMonths = months;
}

// ========== PHƯƠNG THỨC QUẢN LÝ ==========

void Printer::AddFirmwareVersion(const std::string& version)
{
    m_firmwareHistory.push_back(version);
}

bool Printer::IsUnderWarranty() const
{
    if (m_purchaseDate.empty() || m_warrantyMonths <= 0)
        return false;

    // TODO: Parse date và tính toán
    // Giả sử hiện tại đang trong thời gian bảo hành
    return true;
}

std::string Printer::BrandToString(Brand brand)
{
    switch (brand)
    {
    case Brand::HP:       return "HP";
    case Brand::Canon:    return "Canon";
    case Brand::Epson:    return "Epson";
    case Brand::Brother:  return "Brother";
    case Brand::Xerox:    return "Xerox";
    case Brand::Samsung:  return "Samsung";
    case Brand::Lexmark:  return "Lexmark";
    default:              return "Unknown";
    }
}

Brand Printer::StringToBrand(const std::string& brandStr)
{
    std::string str = brandStr;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if (str == "hp") return Brand::HP;
    if (str == "canon") return Brand::Canon;
    if (str == "epson") return Brand::Epson;
    if (str == "brother") return Brand::Brother;
    if (str == "xerox") return Brand::Xerox;
    if (str == "samsung") return Brand::Samsung;
    if (str == "lexmark") return Brand::Lexmark;
    return Brand::Unknown;
}

std::string Printer::StatusToString(PrinterStatus status)
{
    switch (status)
    {
    case PrinterStatus::Idle:        return "Idle";
    case PrinterStatus::Printing:    return "Printing";
    case PrinterStatus::Paused:      return "Paused";
    case PrinterStatus::Error:       return "Error";
    case PrinterStatus::Offline:     return "Offline";
    case PrinterStatus::Maintenance: return "Maintenance";
    default:                         return "Unknown";
    }
}

PrinterStatus Printer::StringToStatus(const std::string& statusStr)
{
    std::string str = statusStr;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if (str == "idle") return PrinterStatus::Idle;
    if (str == "printing") return PrinterStatus::Printing;
    if (str == "paused") return PrinterStatus::Paused;
    if (str == "error") return PrinterStatus::Error;
    if (str == "offline") return PrinterStatus::Offline;
    if (str == "maintenance") return PrinterStatus::Maintenance;
    return PrinterStatus::Idle;
}

// ========== STATE PATTERN METHODS ==========

void Printer::SetState(IPrinterState* pNewState)
{
    if (m_pState)
        delete m_pState;

    m_pState = pNewState;

    // Cập nhật status enum cho tiện
    std::string stateName = pNewState->GetStateName();
    if (stateName == "Idle") m_status = PrinterStatus::Idle;
    else if (stateName == "Printing") m_status = PrinterStatus::Printing;
    else if (stateName == "Paused") m_status = PrinterStatus::Paused;
    else if (stateName == "Error") m_status = PrinterStatus::Error;
    else if (stateName == "Offline") m_status = PrinterStatus::Offline;
    else if (stateName == "Maintenance") m_status = PrinterStatus::Maintenance;
}

void Printer::Print(const std::string& document)
{
    if (m_pState)
        m_pState->Print(this, document);
    else
        TRACE(_T("Error: No state set!\n"));
}

void Printer::Pause()
{
    if (m_pState)
        m_pState->Pause(this);
}

void Printer::Resume()
{
    if (m_pState)
        m_pState->Resume(this);
}

void Printer::Cancel()
{
    if (m_pState)
        m_pState->Cancel(this);
}

void Printer::Maintenance()
{
    if (m_pState)
        m_pState->Maintenance(this);
}

void Printer::HandleError()
{
    if (m_pState)
        m_pState->HandleError(this);
}

std::string Printer::GetCurrentStateName() const
{
    if (m_pState)
        return m_pState->GetStateName();
    return "No State";
}

// ========== IDLE STATE ==========

void IdleState::Print(Printer* pPrinter, const std::string& document)
{
    TRACE(_T("Printer is now printing: %hs\n"), document.c_str());
    pPrinter->SetState(new PrintingState());
}

void IdleState::Pause(Printer* pPrinter)
{
    TRACE(_T("Cannot pause: Printer is idle\n"));
}

void IdleState::Resume(Printer* pPrinter)
{
    TRACE(_T("Cannot resume: Printer is idle\n"));
}

void IdleState::Cancel(Printer* pPrinter)
{
    TRACE(_T("Nothing to cancel: Printer is idle\n"));
}

void IdleState::Maintenance(Printer* pPrinter)
{
    TRACE(_T("Printer entering maintenance mode\n"));
    pPrinter->SetState(new MaintenanceState());
}

void IdleState::HandleError(Printer* pPrinter)
{
    TRACE(_T("Printer encountered an error\n"));
    pPrinter->SetState(new ErrorState());
}

std::string IdleState::GetStateName() const
{
    return "Idle";
}

// ========== PRINTING STATE ==========

void PrintingState::Print(Printer* pPrinter, const std::string& document)
{
    TRACE(_T("Already printing: %hs\n"), document.c_str());
}

void PrintingState::Pause(Printer* pPrinter)
{
    TRACE(_T("Printing paused\n"));
    pPrinter->SetState(new PausedState());
}

void PrintingState::Resume(Printer* pPrinter)
{
    TRACE(_T("Already printing\n"));
}

void PrintingState::Cancel(Printer* pPrinter)
{
    TRACE(_T("Printing cancelled\n"));
    pPrinter->SetState(new IdleState());
}

void PrintingState::Maintenance(Printer* pPrinter)
{
    TRACE(_T("Cannot enter maintenance while printing\n"));
}

void PrintingState::HandleError(Printer* pPrinter)
{
    TRACE(_T("Error occurred during printing\n"));
    pPrinter->SetState(new ErrorState());
}

std::string PrintingState::GetStateName() const
{
    return "Printing";
}

// ========== PAUSED STATE ==========

void PausedState::Print(Printer* pPrinter, const std::string& document)
{
    TRACE(_T("Resume first before printing\n"));
}

void PausedState::Pause(Printer* pPrinter)
{
    TRACE(_T("Already paused\n"));
}

void PausedState::Resume(Printer* pPrinter)
{
    TRACE(_T("Resuming printing\n"));
    pPrinter->SetState(new PrintingState());
}

void PausedState::Cancel(Printer* pPrinter)
{
    TRACE(_T("Printing cancelled\n"));
    pPrinter->SetState(new IdleState());
}

void PausedState::Maintenance(Printer* pPrinter)
{
    TRACE(_T("Cannot enter maintenance while paused\n"));
}

void PausedState::HandleError(Printer* pPrinter)
{
    TRACE(_T("Error occurred\n"));
    pPrinter->SetState(new ErrorState());
}

std::string PausedState::GetStateName() const
{
    return "Paused";
}

// ========== ERROR STATE ==========

void ErrorState::Print(Printer* pPrinter, const std::string& document)
{
    TRACE(_T("Cannot print: Printer has error\n"));
}

void ErrorState::Pause(Printer* pPrinter)
{
    TRACE(_T("Cannot pause: Printer has error\n"));
}

void ErrorState::Resume(Printer* pPrinter)
{
    TRACE(_T("Cannot resume: Printer has error\n"));
}

void ErrorState::Cancel(Printer* pPrinter)
{
    TRACE(_T("Error cleared, back to idle\n"));
    pPrinter->SetState(new IdleState());
}

void ErrorState::Maintenance(Printer* pPrinter)
{
    TRACE(_T("Cannot enter maintenance: Printer has error\n"));
}

void ErrorState::HandleError(Printer* pPrinter)
{
    TRACE(_T("Already in error state\n"));
}

std::string ErrorState::GetStateName() const
{
    return "Error";
}

// ========== MAINTENANCE STATE ==========

void MaintenanceState::Print(Printer* pPrinter, const std::string& document)
{
    TRACE(_T("Cannot print: Under maintenance\n"));
}

void MaintenanceState::Pause(Printer* pPrinter)
{
    TRACE(_T("Cannot pause: Under maintenance\n"));
}

void MaintenanceState::Resume(Printer* pPrinter)
{
    TRACE(_T("Maintenance complete, back to idle\n"));
    pPrinter->SetState(new IdleState());
}

void MaintenanceState::Cancel(Printer* pPrinter)
{
    TRACE(_T("Maintenance cancelled\n"));
    pPrinter->SetState(new IdleState());
}

void MaintenanceState::Maintenance(Printer* pPrinter)
{
    TRACE(_T("Already in maintenance\n"));
}

void MaintenanceState::HandleError(Printer* pPrinter)
{
    TRACE(_T("Error during maintenance\n"));
    pPrinter->SetState(new ErrorState());
}

std::string MaintenanceState::GetStateName() const
{
    return "Maintenance";
}

// ========== OFFLINE STATE ==========

void OfflineState::Print(Printer* pPrinter, const std::string& document)
{
    TRACE(_T("Cannot print: Printer is offline\n"));
}

void OfflineState::Pause(Printer* pPrinter)
{
    TRACE(_T("Cannot pause: Printer is offline\n"));
}

void OfflineState::Resume(Printer* pPrinter)
{
    TRACE(_T("Printer back online\n"));
    pPrinter->SetState(new IdleState());
}

void OfflineState::Cancel(Printer* pPrinter)
{
    TRACE(_T("Nothing to cancel\n"));
}

void OfflineState::Maintenance(Printer* pPrinter)
{
    TRACE(_T("Cannot enter maintenance: Printer is offline\n"));
}

void OfflineState::HandleError(Printer* pPrinter)
{
    TRACE(_T("Already offline\n"));
}

std::string OfflineState::GetStateName() const
{
    return "Offline";
}