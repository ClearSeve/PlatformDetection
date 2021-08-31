#pragma once

#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <io.h>
#include <fcntl.h>
#include <algorithm>

enum class ComponentInfo {
    UNKNOWN,
    ANYCPU,
    X86,
    X64,
    rom
};

int64_t rva_to_offset(const IMAGE_SECTION_HEADER* sections, int num_sections, DWORD rva);
ComponentInfo GetArchitectureInfo(LPSTR pszFilePath);

// by: https://github.com/chipmunk-sm/ArchitectureInfo/blob/main/main.cpp