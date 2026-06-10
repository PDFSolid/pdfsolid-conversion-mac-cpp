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
std::string resource_path = "";

const char* license = "";

auto progress_lambda = [](int current_page_count, int total_page_count) {
    std::cout << "lambda convert rate: " << current_page_count << "/" << total_page_count << std::endl;
};

int main()
{
    std::cout << "pdfsolid conversion sdk license verify: " << (int)LibraryManager::LicenseVerify(license, "", "com.pdfsolid.conversion") << std::endl;
    LibraryManager::Initialize(resource_path.c_str());
    LibraryManager::SetLogger(false, false);


    CConvertCallback callback = {nullptr, nullptr, progress_lambda};
    LibraryManager::SetDocumentAIModel("path/model");
    ConvertOptions opt;

    // pdf to word
    CPDFConversion::StartPDFToWord((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("word.docx").c_str(), opt, &callback);

    // pdf to excel
    CPDFConversion::StartPDFToExcel((input_file + "excel.pdf").c_str(), password.c_str(), std::string(output_file).append("excel.xlsx").c_str(), opt, &callback);

    // pdf to ppt
    CPDFConversion::StartPDFToPpt((input_file + "powerpoint.pdf").c_str(), password.c_str(), std::string(output_file).append("powerpoint.pptx").c_str(), opt, &callback);

    // pdf to csv
    opt.excel_csv_format = true;
    CPDFConversion::StartPDFToExcel((input_file + "excel.pdf").c_str(), password.c_str(), std::string(output_file).append("csv.csv").c_str(), opt, &callback);

    // pdf to html
    CPDFConversion::StartPDFToHtml((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("html.html").c_str(), opt, &callback);

    // pdf to rtf
    CPDFConversion::StartPDFToRtf((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("rtf.rtf").c_str(), opt, &callback);

    // pdf to image
    CPDFConversion::StartPDFToImage((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).c_str(), opt, &callback);

    // pdf to txt
    CPDFConversion::StartPDFToTxt((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("txt.txt").c_str(), opt, &callback);

    // pdf to json
    CPDFConversion::StartPDFToJson((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("json.json").c_str(), opt, &callback);

    // pdf to searchable pdf
    opt.enable_ocr = true;
    opt.transparent_text = true;
	opt.languages[0] = OCRLanguage::e_English;
    opt.language_count = 1;
    CPDFConversion::StartPDFToSearchablePdf((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("pdf.pdf").c_str(), opt, &callback);

    // pdf to ofd
    CPDFConversion::StartPDFToOfd((input_file + "word.pdf").c_str(), password.c_str(), std::string(output_file).append("pdf.pdf").c_str(), opt, &callback);


    char version[30];
    LibraryManager::GetVersion(version);
    std::cout << "pdfsolid conversion sdk version: " << version << std::endl;
    LibraryManager::Release();
    return 0;
}
