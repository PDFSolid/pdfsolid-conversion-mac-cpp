#include <iostream>
#include <string>

#include "pdfsolid_conversion.h"

using namespace pdfsolid;
using namespace pdfsolid::base;
using namespace pdfsolid::common;
using namespace pdfsolid::conversion;

std::string input_file = "../../input_files/";
std::string output_file = "../../output_files/";
std::string password = "";

const char* license = R"(../../../resource/license/license.xml)";

int failed_conversions = 0;
void print_result(const char* name, ErrorCode code) {
    std::cout << name << ": " << (int)code << std::endl;
    if ((int)code != 0) ++failed_conversions;
}

auto progress_lambda = [](int current_page_count, int total_page_count) {
    std::cout << "lambda convert rate: " << current_page_count << "/" << total_page_count << std::endl;
};

auto cancel_lambda = []() -> bool {
    // Return true to cancel the conversion, false to continue.
    return false;
	};

int main()
{
    int license_code = (int)LibraryManager::LicenseVerify(license, "", "");
    std::cout << "license_return_code=" << license_code << std::endl;
    if (license_code != 0) { LibraryManager::Release(); return 3; }
    LibraryManager::Initialize("../../../resource");
    LibraryManager::SetLogger(false, false);

    int model_code = (int)LibraryManager::SetDocumentAIModel("../../../resource/models/documentai.model");
    std::cout << "model_return_code=" << model_code << std::endl;
    if (model_code != 0) { LibraryManager::Release(); return 4; }
    ConvertOptions opt;
    CConvertCallback callback = {};
	callback.progress = progress_lambda;
	callback.cancel = cancel_lambda;
    // pdf to word
    print_result("StartPDFToWord", CPDFConversion::StartPDFToWord((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("word.docx").c_str(), opt, &callback));

    // pdf to excel
    print_result("StartPDFToExcel", CPDFConversion::StartPDFToExcel((input_file + "excel.pdf").c_str(), password.c_str(), std::string(output_file).append("excel.xlsx").c_str(), opt, &callback));

    // pdf to ppt
    print_result("StartPDFToPpt", CPDFConversion::StartPDFToPpt((input_file + "powerpoint.pdf").c_str(), password.c_str(), std::string(output_file).append("powerpoint.pptx").c_str(), opt, &callback));

    // pdf to csv
    opt.excel_csv_format = true;
    print_result("StartPDFToExcel", CPDFConversion::StartPDFToExcel((input_file + "excel.pdf").c_str(), password.c_str(), std::string(output_file).c_str(), opt, &callback));
    opt.excel_csv_format = false;

    // pdf to html
    opt.page_layout_mode = PageLayoutMode::e_Box;
    print_result("StartPDFToHtml", CPDFConversion::StartPDFToHtml((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("html.html").c_str(), opt, &callback));

    // pdf to rtf
    print_result("StartPDFToRtf", CPDFConversion::StartPDFToRtf((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("rtf.rtf").c_str(), opt, &callback));

    // pdf to image
    print_result("StartPDFToImage", CPDFConversion::StartPDFToImage((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).c_str(), opt, &callback));

    // pdf to txt
    print_result("StartPDFToTxt", CPDFConversion::StartPDFToTxt((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("txt.txt").c_str(), opt, &callback));

    // pdf to json
    print_result("StartPDFToJson", CPDFConversion::StartPDFToJson((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("json.json").c_str(), opt, &callback));

    // pdf to markdown
    print_result("StartPDFToMarkdown", CPDFConversion::StartPDFToMarkdown((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("markdown.md").c_str(), opt, &callback));


    // pdf to searchable pdf
    opt.enable_ocr = true;
	opt.transparent_text = true;
	opt.languages[0] = OCRLanguage::e_English;
    opt.language_count = 1;
    print_result("StartPDFToSearchablePdf", CPDFConversion::StartPDFToSearchablePdf((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("pdf.pdf").c_str(), opt, &callback));

    // pdf to ofd
    print_result("StartPDFToOfd", CPDFConversion::StartPDFToOfd((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("pdf.ofd").c_str(), opt, &callback));

    char version[30];
    LibraryManager::GetVersion(version);
    std::cout << "pdfsolid conversion sdk version: " << version << std::endl;
    
    LibraryManager::Release();
    return failed_conversions == 0 ? 0 : 5;
}
