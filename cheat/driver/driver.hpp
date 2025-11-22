#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <winioctl.h>

uintptr_t virtualaddy;
uintptr_t cr3;

#define CODE_RW                  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x47536, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_BA                  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x36236, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_GET_GUARDED_REGION  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x13437, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_GET_DIR_BASE        CTL_CODE(FILE_DEVICE_UNKNOWN, 0x13438, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_SECURITY            0x457c1d6

typedef struct _RW {
    INT32 security;
    INT32 process_id;
    ULONGLONG address;
    ULONGLONG buffer;
    ULONGLONG size;
    BOOLEAN write;
} RW, * PRW;

typedef struct _BA {
    INT32 security;
    INT32 process_id;
    ULONGLONG* address;
} BA, * PBA;

typedef struct _GA {
    INT32 security;
    ULONGLONG* address;
} GA, * PGA;

typedef struct _MEMORY_OPERATION_DATA {
    uint32_t pid;
    ULONGLONG* cr3;          
} MEMORY_OPERATION_DATA, * PMEMORY_OPERATION_DATA;

namespace mem {
    HANDLE driver_handle = INVALID_HANDLE_VALUE;
    INT32 process_id = 0;

    bool find_driver() {
        driver_handle = CreateFileW(
            L"\\\\.\\{d6579ab0-c95b-4463-9135-41gbcf16e4eg}",
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (driver_handle == INVALID_HANDLE_VALUE) {
            driver_handle = CreateFileW(
                L"\\\\.\\Global\\{d6579ab0-c95b-4463-9135-41gbcf16e4eg}",
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
        }

        return (driver_handle != INVALID_HANDLE_VALUE);
    }

    void read_physical(PVOID address, PVOID buffer, DWORD size) {
        RW args = { 0 };
        args.security = CODE_SECURITY;
        args.process_id = process_id;
        args.address = (ULONGLONG)address;
        args.buffer = (ULONGLONG)buffer;
        args.size = size;
        args.write = FALSE;

        DWORD returned;
        DeviceIoControl(driver_handle, CODE_RW, &args, sizeof(args), nullptr, 0, &returned, nullptr);
    }

    void write_physical(PVOID address, PVOID buffer, DWORD size) {
        RW args = { 0 };
        args.security = CODE_SECURITY;
        args.process_id = process_id;
        args.address = (ULONGLONG)address;
        args.buffer = (ULONGLONG)buffer;
        args.size = size;
        args.write = TRUE;

        DWORD returned;
        DeviceIoControl(driver_handle, CODE_RW, &args, sizeof(args), nullptr, 0, &returned, nullptr);
    }

    uintptr_t fetch_cr3() {
        uintptr_t cr3_value = 0;
        MEMORY_OPERATION_DATA args = { 0 };
        args.pid = process_id;
        args.cr3 = (ULONGLONG*)&cr3_value;

        DWORD returned;
        DeviceIoControl(driver_handle, CODE_GET_DIR_BASE, &args, sizeof(args), nullptr, 0, &returned, nullptr);
        return cr3_value;
    }

    uintptr_t find_image() {
        uintptr_t image_address = 0;
        BA args = { 0 };
        args.security = CODE_SECURITY;
        args.process_id = process_id;
        args.address = (ULONGLONG*)&image_address;

        DWORD returned;
        DeviceIoControl(driver_handle, CODE_BA, &args, sizeof(args), nullptr, 0, &returned, nullptr);
        return image_address;
    }

    uintptr_t get_guarded_region() {
        uintptr_t guarded_address = 0;
        GA args = { 0 };
        args.security = CODE_SECURITY;
        args.address = (ULONGLONG*)&guarded_address;

        DWORD returned;
        DeviceIoControl(driver_handle, CODE_GET_GUARDED_REGION, &args, sizeof(args), nullptr, 0, &returned, nullptr);
        return guarded_address;
    }

    INT32 find_process(LPCTSTR process_name) {
        PROCESSENTRY32 pe32 = { sizeof(pe32) };
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (hSnap == INVALID_HANDLE_VALUE)
            return 0;

        if (Process32First(hSnap, &pe32)) {
            do {
                if (_wcsicmp(pe32.szExeFile, process_name) == 0) {
                    process_id = pe32.th32ProcessID;
                    CloseHandle(hSnap);
                    return process_id;
                }
            } while (Process32Next(hSnap, &pe32));
        }

        CloseHandle(hSnap);
        return 0;
    }

    void close_driver() {
        if (driver_handle != INVALID_HANDLE_VALUE) {
            CloseHandle(driver_handle);
            driver_handle = INVALID_HANDLE_VALUE;
        }
    }
}

template <typename T>
T read(uint64_t address) {
    T buffer = { };
    mem::read_physical((PVOID)address, &buffer, sizeof(T));
    return buffer;
}

template <typename T>
bool write(uint64_t address, T value) {
    mem::write_physical((PVOID)address, &value, sizeof(T));
    return true;
}