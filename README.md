# PrinterHub — Thiết kế Class Model

> **Khóa học:** C/C++ Foundation for MFC  
> **Ngôn ngữ:** C++ (MFC Dialog Application)  
> **Namespace gốc:** `PrinterHub`  
> **Sub-namespace:** `PrinterHub::Core` | `PrinterHub::IO` | `PrinterHub::UI` | `PrinterHub::Tests`

---

## Mục lục

1. [Enums & Config](#1-enums--config)
2. [Core Entity Classes](#2-core-entity-classes)
3. [Repository Template](#3-repository-template)
4. [Singleton — FirmwareCatalog](#4-singleton--firmwarecatalog)
5. [Observer Pattern — PrinterManager](#5-observer-pattern--printermanager)
6. [State Pattern — Vòng đời Printer](#6-state-pattern--vòng-đời-printer)
7. [Strategy Pattern — Sắp xếp](#7-strategy-pattern--sắp-xếp)
8. [Command Pattern — Undo/Redo](#8-command-pattern--undoredo)
9. [File I/O & Exception](#9-file-io--exception)
10. [ViewModel & Worker Thread](#10-viewmodel--worker-thread)
11. [MFC Dialog Layer](#11-mfc-dialog-layer)
12. [Sơ đồ quan hệ tổng thể](#12-sơ-đồ-quan-hệ-tổng-thể)

---

## 1. Enums & Config

### PrinterConfig.h — Hằng số toàn cục

```cpp
// PrinterConfig.h
#pragma once   // hoặc include guard #ifndef PRINTER_CONFIG_H

namespace PrinterHub {

    // ── Ch.1: enum class thay thế magic int ──────────────────────────
    enum class PrinterStatus {
        Active,       // Máy đang hoạt động bình thường
        InService,    // Đang bảo trì / sửa chữa
        Retired       // Đã ngừng sử dụng (terminal state)
    };

    enum class Brand {
        HP,
        Canon,
        Epson,
        Other
    };

    enum class Priority {
        Low      = 0,
        Medium   = 1,
        High     = 2,
        Critical = 3
    };

    enum class TicketStatus {
        Open,
        InProgress,
        Closed
    };

    // ── Hằng số toàn cục (không dùng magic number) ───────────────────
    constexpr int    MAX_WARRANTY_MONTHS = 120;
    constexpr int    MAX_FIRMWARE_QUEUE  = 50;
    constexpr size_t BATCH_RESERVE_SIZE  = 100;   // dùng với vector::reserve()

} // namespace PrinterHub
```

### Sơ đồ Enums

```
┌──────────────────────┐  ┌──────────────────────┐  ┌──────────────────────┐
│   «enum class»       │  │   «enum class»        │  │   «enum class»       │
│   PrinterStatus      │  │   Priority            │  │   Brand              │
├──────────────────────┤  ├──────────────────────┤  ├──────────────────────┤
│ Active               │  │ Low      = 0          │  │ HP                   │
│ InService            │  │ Medium   = 1          │  │ Canon                │
│ Retired              │  │ High     = 2          │  │ Epson                │
│                      │  │ Critical = 3          │  │ Other                │
│ Transition hợp lệ:   │  │                       │  │                      │
│ Active→InService     │  │ Dùng cho Ticket       │  │ Dùng trong           │
│ InService→Retired    │  │ sort & filter         │  │ set<Brand> unique    │
└──────────────────────┘  └──────────────────────┘  └──────────────────────┘
```

---

## 2. Core Entity Classes

### 2.1 Class `Printer`

```
┌─────────────────────────────────────────────────────────┐
│                     «class» Printer                     │
│               namespace PrinterHub::Core                │
├──────────────────────────── Fields ─────────────────────┤
│ - id             : std::string                          │
│ - model          : std::string                          │
│ - brand          : Brand                                │
│ - status         : PrinterStatus                        │
│ - purchaseDate   : std::string   // YYYY-MM-DD          │
│ - warrantyMonths : int                                  │
│ - firmwareHistory: std::vector<std::string>             │
│ - state          : IPrinterState*  // State pattern     │
├──────────────────────────── Methods ────────────────────┤
│ + Printer()                         // default ctor     │
│ + Printer(id, model, brand, ...)    // param ctor       │
│ + Printer(const Printer&)           // copy ctor ★Rule3 │
│ + operator=(const Printer&): Printer& // copy assign ★  │
│ + ~Printer()                        // destructor ★     │
│ + operator==(const Printer&): bool                      │
│ + operator<(const Printer&): bool   // dùng cho sort    │
│ + Transition(next: IPrinterState*): void                │
│ + IsUnderWarranty() const: bool                         │
│ + GetId() const: std::string                            │
│ + GetStatus() const: PrinterStatus                      │
└─────────────────────────────────────────────────────────┘
```

**Ghi chú kỹ thuật:**
- Implement **Rule of 3**: copy constructor + copy assignment + destructor
- `operator<` dùng để `std::sort` không cần lambda (sort theo `id` mặc định)
- `state` dùng raw pointer `IPrinterState*` — con trỏ mượn, không sở hữu; lifetime quản lý bởi State objects

### 2.2 Class `FirmwarePackage`

```
┌─────────────────────────────────────────────────────────┐
│               «class» FirmwarePackage                   │
│               namespace PrinterHub::Core                │
├──────────────────────────── Fields ─────────────────────┤
│ - name            : std::string                         │
│ - version         : std::string                         │
│ - sizeBytes       : size_t                              │
│ - compatibleBrand : Brand                               │
│ - releaseDate     : std::string                         │
│ - changelog       : std::string                         │
│ - targetPrinterId : std::string                         │
├──────────────────────────── Methods ────────────────────┤
│ + FirmwarePackage()                      // default ctor │
│ + FirmwarePackage(name, ver, size, ...)  // param ctor  │
│ + FirmwarePackage(const FirmwarePackage&) // copy ctor  │
│ + operator=(const FirmwarePackage&)       // copy assign │
│ + FirmwarePackage(FirmwarePackage&&)  ★ // move ctor    │
│ + operator=(FirmwarePackage&&)        ★ // move assign   │
│ + ~FirmwarePackage()                     // destructor  │
│ + GetVersion() const: std::string                       │
│ + IsCompatibleWith(brand: Brand): bool                  │
└─────────────────────────────────────────────────────────┘
```

**Ghi chú kỹ thuật:**
- Implement **Rule of 5** (Rule of 3 + move constructor + move assignment)
- Move constructor tối ưu khi `std::move(pkg)` push vào `std::queue`
- `changelog` có thể lớn → move tránh copy không cần thiết

### 2.3 Class `MaintenanceTicket`

```
┌─────────────────────────────────────────────────────────┐
│              «class» MaintenanceTicket                  │
│               namespace PrinterHub::Core                │
├──────────────────────────── Fields ─────────────────────┤
│ - ticketId        : std::string                         │
│ - printerId       : std::string                         │
│ - description     : std::string                         │
│ - priority        : Priority                            │
│ - status          : TicketStatus                        │
│ - createdAt       : std::string                         │
│ - closedAt        : std::string                         │
│ - technicianName  : std::string                         │
├──────────────────────────── Methods ────────────────────┤
│ + MaintenanceTicket(printerId, desc, priority)          │
│ + Close(techName: std::string): void                    │
│ + GetPriority() const: Priority                         │
│ + GetStatus() const: TicketStatus                       │
│ + IsOpen() const: bool                                  │
│ + MatchKeyword(kw: std::string) const: bool             │
│ + operator<(const MaintenanceTicket&): bool             │
│ + operator>(const MaintenanceTicket&): bool             │
└─────────────────────────────────────────────────────────┘
```

**Sử dụng STL:**
```cpp
// Sort theo priority (Critical trước)
std::sort(tickets.begin(), tickets.end(),
    [](const MaintenanceTicket& a, const MaintenanceTicket& b) {
        return a.GetPriority() > b.GetPriority();
    });

// Filter theo keyword
std::function<bool(const MaintenanceTicket&)> pred =
    [&kw](const MaintenanceTicket& t) { return t.MatchKeyword(kw); };
auto it = std::find_if(tickets.begin(), tickets.end(), pred);
```

---

## 3. Repository Template

```
┌─────────────────────────────────────────────────────────┐
│           «template class» DeviceRepository<T>          │
│         namespace PrinterHub::Core  (header-only)       │
├──────────────────────────── Fields ─────────────────────┤
│ - items : std::vector<T>                                │
│ - index : std::map<std::string, T>   // lookup O(1)     │
├──────────────────────────── Methods ────────────────────┤
│ + Add(item: T): void                                    │
│ + Remove(id: std::string): bool                         │
│ + FindById(id: std::string): T*        // raw ptr mượn  │
│ + GetAll(): const std::vector<T>&                       │
│ + Count(): size_t                                       │
│ + Reserve(n: size_t): void            // gọi vector::reserve() │
└─────────────────────────────────────────────────────────┘
```

**Dùng chung cho nhiều kiểu:**
```cpp
DeviceRepository<Printer>         printerRepo;
DeviceRepository<FirmwarePackage> firmwareRepo;
```

---

## 4. Singleton — FirmwareCatalog

```
┌─────────────────────────────────────────────────────────┐
│             «Singleton» FirmwareCatalog                 │
│               namespace PrinterHub::Core                │
├──────────────────────────── Fields ─────────────────────┤
│ - instance  : shared_ptr<FirmwareCatalog>  [static]     │
│ - onceFlag  : std::once_flag               [static]     │
│ - catalog   : DeviceRepository<FirmwarePackage>         │
├──────────────────────────── Methods ────────────────────┤
│ + GetInstance(): shared_ptr<FirmwareCatalog>  [static]  │
│ + Register(pkg: FirmwarePackage): void                  │
│ + FindByVersion(ver: string): FirmwarePackage*          │
│ + GetAll(): const vector<FirmwarePackage>&              │
│ - FirmwareCatalog()   // private constructor            │
└─────────────────────────────────────────────────────────┘
```

**Thread-safe lazy init:**
```cpp
// FirmwareCatalog.cpp
static std::once_flag s_flag;
static std::shared_ptr<FirmwareCatalog> s_instance;

std::shared_ptr<FirmwareCatalog> FirmwareCatalog::GetInstance() {
    std::call_once(s_flag, []() {
        s_instance = std::shared_ptr<FirmwareCatalog>(new FirmwareCatalog());
    });
    return s_instance;
}
```

---

## 5. Observer Pattern — PrinterManager

### Interface `IPrinterObserver`

```
┌─────────────────────────────────────────────────────────┐
│        «interface (pure virtual)» IPrinterObserver      │
├──────────────────────────── Methods ────────────────────┤
│ + OnPrinterListChanged(): void = 0                      │
│ + OnPrinterStatusChanged(id: string): void = 0          │
│ + virtual ~IPrinterObserver() = default   // bắt buộc  │
└─────────────────────────────────────────────────────────┘
```

### Class `PrinterManager` (Subject)

```
┌─────────────────────────────────────────────────────────┐
│            «class — Subject» PrinterManager             │
│               namespace PrinterHub::Core                │
├──────────────────────────── Fields ─────────────────────┤
│ - printers      : DeviceRepository<Printer>             │
│ - observers     : std::vector<IPrinterObserver*>        │
│ - brandSet      : std::set<Brand>                       │
│ - sortStrategy  : ISortStrategy*                        │
│ - firmwareQueue : std::queue<FirmwarePackage>           │
│ - undoRedo      : UndoRedoManager                       │
├──────────────────────────── Methods ────────────────────┤
│ + Subscribe(obs: IPrinterObserver*): void               │
│ + Unsubscribe(obs: IPrinterObserver*): void             │
│ + Notify(): void                                        │
│ + AddPrinter(p: Printer): void                          │
│ + UpdatePrinter(id, p: Printer): void                   │
│ + SoftDelete(id: string): void        // set Retired    │
│ + GetSorted(): std::vector<Printer>                     │
│ + FilterBy(pred: std::function<bool(const Printer&)>)   │
│             : std::vector<Printer>                      │
│ + SetSortStrategy(s: ISortStrategy*): void              │
│ + EnqueueFirmware(pkg: FirmwarePackage): void           │
│ + ProcessFirmwareQueue(): void        // FIFO           │
└─────────────────────────────────────────────────────────┘
```

### Sơ đồ Observer

```
        PrinterManager (Subject)
               │
               │  Notify()
               ▼
   ┌───────────────────────┐
   │   IPrinterObserver    │ ◄──── interface thuần ảo
   └───────────┬───────────┘
               │ «implements»
     ┌─────────┴──────────┐
     ▼                    ▼
  MainDlg           FirmwareDlg
  (Dialog 1)        (Dialog 2)
```

---

## 6. State Pattern — Vòng đời Printer

### Interface `IPrinterState`

```
┌─────────────────────────────────────────────────────────┐
│          «interface» IPrinterState                      │
├──────────────────────────── Methods ────────────────────┤
│ + HandleTransition(p: Printer*,                         │
│       next: IPrinterState*): void = 0                   │
│   // Throw InvalidTransitionException nếu không hợp lệ  │
│ + GetStatus() const: PrinterStatus = 0                  │
│ + virtual ~IPrinterState() = default                    │
└─────────────────────────────────────────────────────────┘
```

### Concrete States

```
┌────────────────┐   ┌────────────────┐   ┌────────────────┐
│  ActiveState   │   │ InServiceState │   │  RetiredState  │
├────────────────┤   ├────────────────┤   ├────────────────┤
│ GetStatus():   │   │ GetStatus():   │   │ GetStatus():   │
│  → Active      │   │  → InService   │   │  → Retired     │
│                │   │                │   │                │
│ Cho phép:      │   │ Cho phép:      │   │ Terminal:      │
│  → InService ✓ │   │  → Retired  ✓  │   │ Mọi transition │
│ Từ chối:       │   │ Từ chối:       │   │ đều bị từ chối │
│  → Retired  ✗  │   │  → Active   ✗  │   │ (throw)        │
└────────────────┘   └────────────────┘   └────────────────┘
        │                   │                     │
        └───────────────────┴─────────────────────┘
                       «extends»
                    IPrinterState
```

### Sơ đồ State Machine

```
    ┌──────────┐  transition   ┌────────────┐  transition  ┌──────────┐
    │  Active  │──────────────►│ InService  │─────────────►│ Retired  │
    └──────────┘               └────────────┘              └──────────┘
         │                                                       ▲
         │            (các transition khác bị throw)            │
         └───────────────────────────────── ✗ ──────────────────┘
```

**Kiểm tra state bằng dynamic_cast:**
```cpp
if (auto* active = dynamic_cast<ActiveState*>(printer.state)) {
    // printer đang Active
}
// Luôn null-check sau dynamic_cast
```

---

## 7. Strategy Pattern — Sắp xếp

### Interface `ISortStrategy`

```
┌─────────────────────────────────────────────────────────┐
│               «interface» ISortStrategy                 │
├──────────────────────────── Methods ────────────────────┤
│ + Sort(v: std::vector<Printer>): std::vector<Printer> = 0│
│ + virtual ~ISortStrategy() = default                    │
└─────────────────────────────────────────────────────────┘
```

### Concrete Strategies

```
┌────────────────────┐  ┌──────────────────────┐  ┌────────────────────────┐
│   SortByName       │  │   SortByStatus       │  │   SortByPurchaseDate   │
├────────────────────┤  ├──────────────────────┤  ├────────────────────────┤
│ Sort theo model    │  │ Sort theo enum order │  │ Sort theo ngày mua     │
│ (alphabetical)     │  │ Active < InService   │  │ (string YYYY-MM-DD     │
│                    │  │ < Retired            │  │  sort lexicographic)   │
└────────────────────┘  └──────────────────────┘  └────────────────────────┘
           │                      │                          │
           └──────────────────────┴──────────────────────────┘
                                «extends»
                             ISortStrategy
```

**Đổi strategy tại runtime (không sửa PrinterManager):**
```cpp
manager.SetSortStrategy(new SortByName());
auto sorted = manager.GetSorted();

manager.SetSortStrategy(new SortByPurchaseDate());
sorted = manager.GetSorted();  // đổi cách sort, code Manager không đổi
```

---

## 8. Command Pattern — Undo/Redo

### Interface `ICommand`

```
┌─────────────────────────────────────────────────────────┐
│                  «interface» ICommand                   │
├──────────────────────────── Methods ────────────────────┤
│ + Execute(): void = 0                                   │
│ + Undo(): void = 0                                      │
│ + virtual ~ICommand() = default                         │
└─────────────────────────────────────────────────────────┘
```

### Concrete Commands

```
┌──────────────────────────┐  Lưu context đủ để Undo:
│  AddPrinterCommand       │  - manager ref + Printer bị thêm
├──────────────────────────┤
│  DeletePrinterCommand    │  - manager ref + Printer đã xóa
├──────────────────────────┤
│  UpdateFirmwareCommand   │  - manager ref + printerId + FirmwarePackage
└──────────────────────────┘
            │
         «extends» ICommand
```

### Class `UndoRedoManager`

```
┌─────────────────────────────────────────────────────────┐
│                  «class» UndoRedoManager                │
├──────────────────────────── Fields ─────────────────────┤
│ - undoStack : std::stack<ICommand*>                     │
│ - redoStack : std::stack<ICommand*>                     │
├──────────────────────────── Methods ────────────────────┤
│ + Execute(cmd: ICommand*): void                         │
│   // Execute → push undoStack, clear redoStack          │
│ + Undo(): void                                          │
│   // pop undoStack → Undo() → push redoStack            │
│ + Redo(): void                                          │
│   // pop redoStack → Execute() → push undoStack         │
│ + CanUndo() const: bool                                 │
│ + CanRedo() const: bool                                 │
└─────────────────────────────────────────────────────────┘
```

### Luồng Undo/Redo

```
 Execute(cmd)          Undo()                  Redo()
     │                   │                       │
     ▼                   ▼                       ▼
 cmd.Execute()    undoStack.pop()→cmd     redoStack.pop()→cmd
 undoStack.push   cmd.Undo()              cmd.Execute()
 redoStack.clear  redoStack.push          undoStack.push
```

---

## 9. File I/O & Exception

### RAII — Class `FileGuard`

```
┌─────────────────────────────────────────────────────────┐
│               «RAII wrapper» FileGuard                  │
├──────────────────────────── Fields ─────────────────────┤
│ - fp: FILE*                                             │
├──────────────────────────── Methods ────────────────────┤
│ + FileGuard(path, mode: const char*): void // fopen     │
│ + ~FileGuard(): void                       // fclose ★  │
│ + Get(): FILE*                                          │
│ + IsOpen() const: bool                                  │
│ FileGuard(const FileGuard&) = delete   // non-copyable  │
└─────────────────────────────────────────────────────────┘
```

### Class `CsvReader`

```
┌─────────────────────────────────────────────────────────┐
│                   «class» CsvReader                     │
│                  namespace PrinterHub::IO               │
├──────────────────────────── Fields ─────────────────────┤
│ - filePath : std::string                                │
├──────────────────────────── Methods ────────────────────┤
│ + CsvReader(path: std::string)                          │
│ + ReadPrinters(): std::vector<Printer>                  │
│   // throw FileNotFoundException nếu không có file      │
│   // throw InvalidDataException nếu sai format          │
│ - ParseLine(line: string): Printer    // private        │
│ - ValidateFields(fields): void        // private        │
└─────────────────────────────────────────────────────────┘
```

### Class `CsvWriter`

```
┌─────────────────────────────────────────────────────────┐
│                   «class» CsvWriter                     │
│                  namespace PrinterHub::IO               │
├──────────────────────────── Fields ─────────────────────┤
│ - filePath : std::string                                │
├──────────────────────────── Methods ────────────────────┤
│ + CsvWriter(path: std::string)                          │
│ + Write(printers: vector<Printer>): void                │
│ + Append(printer: Printer): void                        │
│ // Dùng FileGuard để đảm bảo fclose kể cả khi exception│
└─────────────────────────────────────────────────────────┘
```

### Class `FirmwareLog`

```
┌─────────────────────────────────────────────────────────┐
│                   «class» FirmwareLog                   │
│                  namespace PrinterHub::IO               │
├──────────────────────────── Fields ─────────────────────┤
│ - logPath : std::string                                 │
├──────────────────────────── Methods ────────────────────┤
│ + FirmwareLog(path: std::string)                        │
│ + Append(printerId: string,                             │
│           pkg: FirmwarePackage): void                   │
│ + ReadHistory(printerId: string): std::vector<string>   │
│ // fstream append mode, định dạng: [timestamp] id ver  │
└─────────────────────────────────────────────────────────┘
```

### Exception Hierarchy

```
        std::exception
               │
               │ «extends»
               ▼
    PrinterHubException
    (what() override, message field)
               │
       ┌───────┴──────────────────┐
       ▼                          ▼                         ▼
FileNotFoundException    InvalidDataException    InvalidTransitionException
 (file CSV/log           (thiếu field,           (State transition
  không tồn tại)          sai format)             không hợp lệ)
```

```cpp
// Ví dụ sử dụng
try {
    CsvReader reader("printers.csv");
    auto printers = reader.ReadPrinters();
} catch (const FileNotFoundException& e) {
    // xử lý file không tồn tại
} catch (const InvalidDataException& e) {
    // xử lý dữ liệu sai format
} catch (const PrinterHubException& e) {
    // catch-all cho custom exception
}
```

---

## 10. ViewModel & Worker Thread

### Class `PrinterFormVM` (ViewModel — không phụ thuộc MFC)

```
┌─────────────────────────────────────────────────────────┐
│          «ViewModel» PrinterFormVM                      │
│     namespace PrinterHub::Core  (no MFC dependency)     │
├──────────────────────────── Fields ─────────────────────┤
│ - model          : std::string                          │
│ - brand          : Brand                                │
│ - warrantyMonths : int                                  │
│ - purchaseDate   : std::string                          │
│ - errors         : std::vector<std::string>             │
├──────────────────────────── Methods ────────────────────┤
│ + Validate(): bool                                      │
│   // model != empty, warranty > 0, date hợp lệ         │
│ + ToPrinter(): Printer                                  │
│ + GetErrors(): const std::vector<std::string>&          │
│ + SetModel(s: string): void                             │
│ + SetBrand(b: Brand): void                              │
│ + SetWarranty(months: int): void                        │
│ + SetPurchaseDate(d: string): void                      │
└─────────────────────────────────────────────────────────┘
```

**Mục đích:** Validate logic được test bằng GoogleTest mà không cần khởi động MFC/Dialog.

```cpp
// ViewModelTests.cpp
TEST(PrinterFormVMTest, EmptyModelShouldFail) {
    PrinterFormVM vm;
    vm.SetModel("");
    vm.SetWarranty(12);
    EXPECT_FALSE(vm.Validate());
}

TEST(PrinterFormVMTest, ValidDataShouldPass) {
    PrinterFormVM vm;
    vm.SetModel("HP LaserJet Pro");
    vm.SetBrand(Brand::HP);
    vm.SetWarranty(24);
    vm.SetPurchaseDate("2024-01-15");
    EXPECT_TRUE(vm.Validate());
}
```

### Class `LoadWorker` (Background Thread)

```
┌─────────────────────────────────────────────────────────┐
│          «class» LoadWorker                             │
│          namespace PrinterHub::Worker                   │
├──────────────────────────── Fields ─────────────────────┤
│ - thread   : std::thread                                │
│ - mutex    : std::mutex                                 │
│ - result   : std::vector<Printer>                       │
│ - hwnd     : HWND            // target window           │
│ - msgId    : UINT            // WM_USER message ID      │
├──────────────────────────── Methods ────────────────────┤
│ + LoadWorker(hwnd: HWND, msgId: UINT)                   │
│ + Start(csvPath: std::string): void                     │
│ + GetResult(): std::vector<Printer>    // thread-safe   │
│ + ~LoadWorker()                        // join thread   │
│ - WorkerThread(path: string): void  // private          │
│   // load xong → PostMessage(hwnd, msgId, ...)         │
│   // KHÔNG gọi MFC UI trực tiếp từ thread              │
└─────────────────────────────────────────────────────────┘
```

**Luồng hoạt động:**
```
Main Thread                          Worker Thread
     │                                    │
     │── Start("printers.csv") ─────────►│
     │                                    │── CsvReader::ReadPrinters()
     │  (UI không bị block)               │── mutex.lock()
     │                                    │── result = printers
     │                                    │── mutex.unlock()
     │◄─ PostMessage(WM_LOAD_DONE) ───────│
     │
     │── GetResult() → render CListCtrl
```

---

## 11. MFC Dialog Layer

### Tổng quan Dialog Classes

```
┌──────────────────────────────────────────────────────────────┐
│              namespace PrinterHub::UI                        │
│                   MFC Dialog Layer                           │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌───────────────────┐      ┌────────────────────────────┐  │
│  │     MainDlg       │      │    AddEditPrinterDlg        │  │
│  │  : CDialogEx      │      │    : CDialogEx              │  │
│  │  : IPrinterObserver│     ├────────────────────────────┤  │
│  ├───────────────────┤      │ DDX: model, brand,         │  │
│  │ CListCtrl listCtrl│      │      warranty, date         │  │
│  │ Nút: Thêm/Sửa/Xóa│      │ DDV: model != empty        │  │
│  │ Nút: Undo / Redo  │      │      warranty > 0           │  │
│  │                   │      │ Uses: PrinterFormVM         │  │
│  │ OnPrinterList     │      └────────────────────────────┘  │
│  │  Changed() override│                                      │
│  └───────────────────┘                                       │
│                                                              │
│  ┌───────────────────┐      ┌────────────────────────────┐  │
│  │   FirmwareDlg     │      │       TicketDlg             │  │
│  │   : CDialogEx     │      │       : CDialogEx           │  │
│  ├───────────────────┤      ├────────────────────────────┤  │
│  │ Hiển thị queue    │      │ Tạo ticket mới             │  │
│  │ firmware          │      │ Sort theo Priority          │  │
│  │ Lịch sử firmware  │      │ Search theo keyword         │  │
│  │ của printer chọn  │      │ Đóng ticket                │  │
│  └───────────────────┘      └────────────────────────────┘  │
└──────────────────────────────────────────────────────────────┘
```

### Message Map quan trọng

```cpp
BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_ADD,    &MainDlg::OnBnClickedAdd)
    ON_BN_CLICKED(IDC_BTN_EDIT,   &MainDlg::OnBnClickedEdit)
    ON_BN_CLICKED(IDC_BTN_DELETE, &MainDlg::OnBnClickedDelete)
    ON_BN_CLICKED(IDC_BTN_UNDO,   &MainDlg::OnBnClickedUndo)
    ON_BN_CLICKED(IDC_BTN_REDO,   &MainDlg::OnBnClickedRedo)
    ON_LBN_SELCHANGE(IDC_LIST_PRINTERS, &MainDlg::OnSelChangePrinters)
    ON_MESSAGE(WM_LOAD_DONE,      &MainDlg::OnLoadDone)
END_MESSAGE_MAP()
```

### CPrinterDoc / CPrinterView (Doc/View)

```
┌──────────────────┐      ┌──────────────────────────┐
│  CPrinterDoc     │      │  CPrinterView             │
│  : CDocument     │      │  : CView                  │
├──────────────────┤      ├──────────────────────────┤
│ - manager:       │      │ + OnUpdate(): void        │
│   PrinterManager │◄─────│   // gọi khi doc thay đổi│
│                  │      │                           │
│ + Serialize(     │      │ UpdateAllViews() ─────────┤
│   CArchive&)     │      │   khi data thay đổi       │
└──────────────────┘      └──────────────────────────┘
```

### Hàm chuyển đổi CString ↔ std::string

```cpp
// PrinterUtils.h
namespace PrinterHub::UI {
    std::string ToStdString(const CString& cs);
    CString     ToCString(const std::string& s);
    // Dùng _T() macro, TCHAR cho Unicode support
}
```

---

## 12. Sơ đồ quan hệ tổng thể

```
                    PrinterHub — Class Relationship Overview
┌──────────────────────────────────────────────────────────────────────────────┐
│ namespace PrinterHub::Core                                                   │
│                                                                              │
│  ┌──────────────────┐      ┌─────────────────────────────────────────────┐  │
│  │ FirmwareCatalog  │      │              PrinterManager                 │  │
│  │ (Singleton)      │      │  (Subject — Observer Pattern)               │  │
│  └────────┬─────────┘      ├─────────────────────────────────────────────┤  │
│           │ uses           │ - DeviceRepository<Printer>                 │  │
│           ▼                │ - set<Brand>                                │  │
│  DeviceRepository<         │ - queue<FirmwarePackage>  (FIFO)           │  │
│    FirmwarePackage>        │ - ISortStrategy*           (Strategy)       │  │
│                            │ - UndoRedoManager          (Command)        │  │
│  ┌──────────────────┐      │ - vector<IPrinterObserver*> (Observer)      │  │
│  │   Printer        │◄─────┤                                             │  │
│  │  (Rule of 3)     │      └────────────────────────┬────────────────────┘  │
│  │  IPrinterState*  │                               │ Notify()               │
│  └────────┬─────────┘                               ▼                        │
│           │                              IPrinterObserver                    │
│  ┌────────┴──────────┐                   (interface)                         │
│  │   IPrinterState   │                       │                               │
│  │   (interface)     │                       │ «implements»                  │
│  └──┬─────┬─────┬───┘                        │                               │
│     │     │     │                            │                               │
│  Active InSvc Retired                        │                               │
│  State  State  State                         │                               │
│                                              │                               │
│  ┌─────────────────────────────────────────  │  ──────────────────────────┐  │
│  │ namespace PrinterHub::IO                 │                            │  │
│  │                                          │                            │  │
│  │  CsvReader → CsvWriter → FirmwareLog     │                            │  │
│  │  FileGuard (RAII)                        │                            │  │
│  │  Exception hierarchy                     │                            │  │
│  └──────────────────────────────────────────┼────────────────────────────┘  │
│                                             │                               │
│  ┌──────────────────────────────────────────┼────────────────────────────┐  │
│  │ namespace PrinterHub::UI                 │                            │  │
│  │                                          ▼                            │  │
│  │  MainDlg ──────────────────────► IPrinterObserver                    │  │
│  │     │                             (OnPrinterListChanged)              │  │
│  │     ├── AddEditPrinterDlg ──────► PrinterFormVM (testable)           │  │
│  │     ├── FirmwareDlg                                                   │  │
│  │     └── TicketDlg                                                     │  │
│  │                                                                        │  │
│  │  CPrinterDoc ◄──── CPrinterView                                       │  │
│  │  LoadWorker (std::thread + std::mutex + PostMessage)                  │  │
│  └────────────────────────────────────────────────────────────────────────┘  │
└──────────────────────────────────────────────────────────────────────────────┘

namespace PrinterHub::Tests
   └── GoogleTest: PrinterTests, TicketTests, UndoRedoTests, ViewModelTests
```

---

## Cấu trúc thư mục

```
PrinterHub/
├── src/
│   ├── core/
│   │   ├── PrinterConfig.h           # enums, constants (#pragma once)
│   │   ├── Printer.h / .cpp          # Rule of 3, operator==, operator<
│   │   ├── FirmwarePackage.h / .cpp  # Rule of 5, move semantics
│   │   ├── MaintenanceTicket.h / .cpp
│   │   ├── DeviceRepository.h        # template, header-only
│   │   ├── FirmwareCatalog.h / .cpp  # Singleton (call_once)
│   │   ├── PrinterManager.h / .cpp   # Subject (Observer)
│   │   ├── Commands.h / .cpp         # ICommand + concrete commands
│   │   ├── States.h / .cpp           # IPrinterState + Active/InSvc/Retired
│   │   ├── Strategies.h / .cpp       # ISortStrategy + concrete
│   │   ├── UndoRedoManager.h / .cpp  # stack<ICommand*>
│   │   └── PrinterFormVM.h / .cpp    # ViewModel (no MFC)
│   ├── io/
│   │   ├── CsvReader.h / .cpp
│   │   ├── CsvWriter.h / .cpp
│   │   ├── FirmwareLog.h / .cpp
│   │   └── FileGuard.h               # RAII FILE* wrapper
│   ├── ui/
│   │   ├── MainDlg.h / .cpp          # : CDialogEx, IPrinterObserver
│   │   ├── AddEditPrinterDlg.h / .cpp # DDX/DDV
│   │   ├── FirmwareDlg.h / .cpp
│   │   ├── TicketDlg.h / .cpp
│   │   ├── PrinterDoc.h / .cpp       # CPrinterDoc : CDocument
│   │   └── PrinterView.h / .cpp      # CPrinterView : CView
│   └── worker/
│       └── LoadWorker.h / .cpp       # std::thread + mutex + PostMessage
├── tests/
│   ├── PrinterTests.cpp              # TEST_F fixture lifecycle
│   ├── TicketTests.cpp               # sort, filter, close
│   ├── UndoRedoTests.cpp             # regression test
│   └── ViewModelTests.cpp            # validate không cần MFC
├── data/
│   ├── printers.csv
│   └── firmware_log.txt
├── PrinterHub.sln
└── report/
    ├── design_report.md
    └── defect_log.md
```

---

## Tóm tắt kỹ thuật C++ theo chương

| Chương | Class áp dụng | Kỹ thuật |
|--------|--------------|----------|
| Ch.1 | PrinterConfig.h | `enum class` PrinterStatus, Brand, Priority |
| Ch.2 | Tất cả .h/.cpp | `#pragma once`, tách header/source |
| Ch.3 | PrinterManager, States | raw pointer + reference, null-check |
| Ch.4 | PrinterUtils.h | pass-by-value, by-ref, by-pointer |
| Ch.5 | CsvReader/Writer | `fstream`, validate, debugger trace |
| Ch.6 | Printer | Rule of 3, `operator==`, `operator<` |
| Ch.7 | IPrinterObserver, IPrinterState | interface thuần ảo, `dynamic_cast` |
| Ch.8 | DeviceRepository | namespace, template class |
| Ch.9 | PrinterManager | `vector`, `map`, `queue`, `stack`, `set`, lambda, `std::function` |
| Ch.10 | FileGuard, exceptions | RAII, custom exception, `unique_ptr`/`shared_ptr` |
| Ch.11 | FirmwarePackage | move ctor, `std::move`, `reserve()` |
| Ch.12 | tests/ | GoogleTest, `TEST_F`, regression |
| Ch.13 | PrinterFormVM | ViewModel, coverage analysis |
| Ch.14 | LoadWorker | `std::thread`, `std::mutex`, `PostMessage` |
| Ch.15 | FirmwareCatalog, Observer | Singleton `call_once`, Observer pattern |
| Ch.16 | States, Strategies, Commands | State, Strategy, Command pattern |
| Ch.17 | AddEditPrinterDlg | DDX/DDV, Message Map |
| Ch.18 | CPrinterDoc/View | Doc/View, `UpdateAllViews()` |
| Ch.20 | UI layer | `CString`, `_T()`, `TCHAR` |
