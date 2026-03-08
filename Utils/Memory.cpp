#include <Windows.h>
#include <vector>
#include <Psapi.h>

uintptr_t FindSignature(const char* szSignature) {
    static auto patternToByte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + strlen(pattern);
        for (auto curr = start; curr < end; ++curr) {
            if (*curr == '?') {
                ++curr;
                if (*curr == '?') ++curr;
                bytes.push_back(-1);
            } else {
                bytes.push_back(strtoul(curr, &curr, 16));
            }
        }
        return bytes;
    };

    auto dosHeader = (PIMAGE_DOS_HEADER)GetModuleHandle(NULL);
    auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)dosHeader + dosHeader->e_lfanew);
    auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto patternBytes = patternToByte(szSignature);
    auto scanBytes = (std::uint8_t*)dosHeader;
    auto s = patternBytes.size();
    auto d = patternBytes.data();

    for (auto i = 0ul; i < sizeOfImage - s; ++i) {
        bool found = true;
        for (auto j = 0ul; j < s; ++j) {
            if (scanBytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found) return (uintptr_t)&scanBytes[i];
    }
    return 0;
}
