#include "stdafx.h"
#include "PlatformDetectionCore.h"

int64_t rva_to_offset(const IMAGE_SECTION_HEADER* sections, int num_sections, DWORD rva)
{
    for (int i = 0; i < num_sections; i++) {
        if (sections[i].VirtualAddress <= rva && sections[i].VirtualAddress + sections[i].SizeOfRawData > rva) {
            return sections[i].PointerToRawData + rva - sections[i].VirtualAddress;
        }
    }
    return 0;
}

ComponentInfo GetArchitectureInfo(LPSTR pszFilePath) {

    std::ifstream input(pszFilePath, std::ios::binary);
    if (!input)
        return ComponentInfo::UNKNOWN;

    IMAGE_DOS_HEADER image_dos_header = {};

    input.seekg(0, std::ios_base::beg);
    if (!input)
        return ComponentInfo::UNKNOWN;

    input.read(reinterpret_cast<char*>(&image_dos_header), sizeof(IMAGE_DOS_HEADER));
    if (!input)
        return ComponentInfo::UNKNOWN;

    if (image_dos_header.e_magic != IMAGE_DOS_SIGNATURE)
        return ComponentInfo::UNKNOWN;

    IMAGE_NT_HEADERS32 image_nt_headers32;
    input.seekg(image_dos_header.e_lfanew, std::ios_base::beg);
    if (!input)
        return ComponentInfo::UNKNOWN;

    input.read(reinterpret_cast<char*>(&image_nt_headers32), sizeof(image_nt_headers32));
    if (!input)
        return ComponentInfo::UNKNOWN;
    if (image_nt_headers32.Signature != IMAGE_NT_SIGNATURE)
        return ComponentInfo::UNKNOWN;

    auto isRom = image_nt_headers32.OptionalHeader.Magic == IMAGE_ROM_OPTIONAL_HDR_MAGIC;
    if (isRom)
        return ComponentInfo::rom;

    auto isX64 = image_nt_headers32.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC;
    if (isX64)
        return ComponentInfo::X64;

    auto isX86 = image_nt_headers32.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC;
    if (!isX86)
        return ComponentInfo::UNKNOWN;

    if (image_nt_headers32.FileHeader.NumberOfSections && image_nt_headers32.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size) {

        std::vector<IMAGE_SECTION_HEADER> hdr(image_nt_headers32.FileHeader.NumberOfSections);

        input.seekg(static_cast<int64_t>(image_dos_header.e_lfanew + sizeof(image_nt_headers32)), std::ios_base::beg);
        if (!input)
            return ComponentInfo::UNKNOWN;

        input.read(reinterpret_cast<char*>(hdr.data()), static_cast<int64_t>(sizeof(IMAGE_SECTION_HEADER) * hdr.size()));
        if (!input)
            return ComponentInfo::UNKNOWN;

        auto numberOfSections = image_nt_headers32.FileHeader.NumberOfSections;
        auto virtualAddress = image_nt_headers32.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress;
        auto offset = rva_to_offset(hdr.data(), numberOfSections, virtualAddress);

        input.seekg(offset, std::ios_base::beg);
        if (!input)
            return ComponentInfo::UNKNOWN;

        IMAGE_COR20_HEADER image_cor20_header = {};

        input.read(reinterpret_cast<char*>(&image_cor20_header), sizeof(image_cor20_header));
        if (!input)
            return ComponentInfo::UNKNOWN;

        if ((image_cor20_header.Flags & COMIMAGE_FLAGS_32BITREQUIRED) == 0)
            return ComponentInfo::ANYCPU;
    }

    return ComponentInfo::X86;
}
