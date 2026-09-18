#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include "pdfsolid_conversion.h"

int main(int argc, char **argv) {
    using pdfsolid::common::LibraryManager;
    using pdfsolid::conversion::CPDFConversion;
    char version[64] = {};
    LibraryManager::GetVersion(version);
    std::printf("sdk_version=%s\n", version);
    if (std::strcmp(version, "1.2.0") != 0) return 1;
    if (argc == 2 && std::strcmp(argv[1], "--version") == 0) return 0;
    if (argc != 5) {
        std::fprintf(stderr, "Usage: demo <license.xml> <resource-dir> <input.pdf> <new-output.docx>\n");
        return 2;
    }
    const std::string model = std::string(argv[2]) + "/models/documentai.model";
    if (!std::filesystem::exists(argv[1]) || !std::filesystem::exists(model) ||
        !std::filesystem::exists(argv[3]) || std::filesystem::exists(argv[4])) return 2;
    int code = static_cast<int>(LibraryManager::LicenseVerify(argv[1], "", ""));
    std::printf("license_return_code=%d\n", code);
    if (code != 0) { LibraryManager::Release(); return 3; }
    LibraryManager::Initialize(argv[2]);
    code = static_cast<int>(LibraryManager::SetDocumentAIModel(model.c_str(), -1));
    std::printf("model_return_code=%d\n", code);
    if (code != 0) { LibraryManager::Release(); return 4; }
    pdfsolid::base::ConvertOptions options;
    code = static_cast<int>(CPDFConversion::StartPDFToWord(argv[3], "", argv[4], options, nullptr));
    std::printf("conversion_return_code=%d\n", code);
    LibraryManager::Release();
    if (code != 0) return 5;
    if (!std::filesystem::exists(argv[4])) return 6;
    const auto size = std::filesystem::file_size(argv[4]);
    std::printf("output_bytes=%llu\n", static_cast<unsigned long long>(size));
    return size > 0 ? 0 : 6;
}
