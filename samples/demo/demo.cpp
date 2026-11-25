#include <cstdio>
#include <iostream>
#include <string>

#include "pdfsolid_conversion.h"

using namespace pdfsolid;
using namespace pdfsolid::base;
using namespace pdfsolid::common;
using namespace pdfsolid::conversion;

static const std::string input_file = "../../input_files/";
static const std::string output_file = "../../output_files/";
static const std::string password = "";
static const char* default_license = "../../../resource/license/license.xml";
static const int WORD_TO_PDF_DIAGNOSTICS_SIZE = 8192;

static void print_result(const char* name, ErrorCode error_code)
{
    std::cout << name << ": " << static_cast<int>(error_code) << std::endl;
}

int main(int argc, char** argv)
{
    const char* license = (argc > 1 && argv[1][0] != '\0') ? argv[1] : default_license;

    char version[64] = { 0 };
    LibraryManager::GetVersion(version);
    std::cout << "pdfsolid conversion sdk version: " << version << std::endl;

    ErrorCode lic_err = LibraryManager::LicenseVerify(license, "", "");
    print_result("pdfsolid conversion sdk license verify", lic_err);
    if (lic_err != ErrorCode::e_ErrSuccess) {
        return 3;
    }

    LibraryManager::Initialize("../../../resource");
    LibraryManager::SetLogger(false, false);

    ErrorCode model_err = LibraryManager::SetDocumentAIModel("../../../resource/models/documentai.model");
    print_result("set document ai model", model_err);
    if (model_err != ErrorCode::e_ErrSuccess) {
        LibraryManager::Release();
        return 4;
    }

    CConvertCallback callback = {};
    callback.progress = [](int current_page_count, int total_page_count) {
        std::cout << "convert rate: " << current_page_count << "/" << total_page_count << std::endl;
    };
    callback.cancel = []() -> bool { return false; };

    int failures = 0;
    auto check = [&](const char* label, ErrorCode err, bool allow_unsupported = false) {
        print_result(label, err);
        if (err != ErrorCode::e_ErrSuccess && !(allow_unsupported && err == ErrorCode::e_ErrUnsupportedFeature)) {
            failures++;
        }
    };

    ConvertOptions opt;
    check("pdf to word", CPDFConversion::StartPDFToWord((input_file + "word.pdf").c_str(), password.c_str(), (output_file + "word.docx").c_str(), opt, &callback));
    check("pdf to excel", CPDFConversion::StartPDFToExcel((input_file + "excel.pdf").c_str(), password.c_str(), (output_file + "excel.xlsx").c_str(), opt, &callback));
    check("pdf to ppt", CPDFConversion::StartPDFToPpt((input_file + "powerpoint.pdf").c_str(), password.c_str(), (output_file + "powerpoint.pptx").c_str(), opt, &callback));

    opt.excel_csv_format = true;
    check("pdf to csv", CPDFConversion::StartPDFToExcel((input_file + "excel.pdf").c_str(), password.c_str(), output_file.c_str(), opt, &callback));
    opt.excel_csv_format = false;

    opt.page_layout_mode = PageLayoutMode::e_Box;
    check("pdf to html", CPDFConversion::StartPDFToHtml((input_file + "word.pdf").c_str(), password.c_str(), (output_file + "html.html").c_str(), opt, &callback));
    check("pdf to rtf", CPDFConversion::StartPDFToRtf((input_file + "word.pdf").c_str(), password.c_str(), (output_file + "rtf.rtf").c_str(), opt, &callback));
    check("pdf to image", CPDFConversion::StartPDFToImage((input_file + "word.pdf").c_str(), password.c_str(), output_file.c_str(), opt, &callback));
    check("pdf to txt", CPDFConversion::StartPDFToTxt((input_file + "word.pdf").c_str(), password.c_str(), (output_file + "txt.txt").c_str(), opt, &callback));
    check("pdf to json", CPDFConversion::StartPDFToJson((input_file + "word.pdf").c_str(), password.c_str(), (output_file + "json.json").c_str(), opt, &callback));
    check("pdf to markdown", CPDFConversion::StartPDFToMarkdown((input_file + "word.pdf").c_str(), password.c_str(), (output_file + "markdown.md").c_str(), opt, &callback));

    opt.enable_ocr = true;
    opt.transparent_text = true;
    opt.language_count = 1;
    opt.languages[0] = OCRLanguage::e_English;
    check("pdf to searchable pdf", CPDFConversion::StartPDFToSearchablePdf((input_file + "word.pdf").c_str(), password.c_str(), (output_file + "pdf.pdf").c_str(), opt, &callback));
    check("pdf to ofd", CPDFConversion::StartPDFToOfd((input_file + "word.pdf").c_str(), password.c_str(), (output_file + "pdf.ofd").c_str(), opt, &callback));

    check("word to pdf", CPDFConversion::StartWordToPdf((input_file + "word.docx").c_str(), (output_file + "word_to_pdf.pdf").c_str(), &callback), true);

    ConvertOptions word_opt;
    word_opt.word_pdf_bookmark_mode = WordPdfBookmarkMode::e_Headings;
    std::snprintf(word_opt.page_ranges, sizeof(word_opt.page_ranges), "%s", "1-2");
    char diagnostics[WORD_TO_PDF_DIAGNOSTICS_SIZE] = { 0 };
    ErrorCode word_res = CPDFConversion::StartWordToPdf((input_file + "word.docx").c_str(), (output_file + "word_to_pdf_bookmarks.pdf").c_str(), word_opt, &callback, diagnostics);
    check("word to pdf with options", word_res, true);
    if (diagnostics[0] != '\0') {
        std::cout << "word to pdf diagnostics:\n" << diagnostics << std::endl;
    }

    LibraryManager::ReleaseDocumentAIModel();
    LibraryManager::Release();
    return failures == 0 ? 0 : 5;
}
