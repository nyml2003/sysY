#include "Driver.hpp"
int main(int argc, char* argv[]) {

    std::string folderName = "/app/tests/Assign-1/";
    std::string input = (argc == 5) ? argv[1] : folderName + "in.sy";
    std::string md = (argc == 5) ? argv[2] : folderName + "out.md";
    std::string error = (argc == 5) ? argv[3] : folderName + "out.err";
    std::string ir = (argc == 5) ? argv[4] : folderName + "out.ll";
    driver.setSourceFileName(input);
    driver.setOutputFileName(md);
    driver.setErrorFileName(error);
    driver.setTraceParsing(false);
    driver.setTraceScanning(false);
    driver.parse();
    freopen(error.c_str(), "w", stderr);
    driver.result->analyze();
    std::cout << "```mermaid" << std::endl;
    std::cout << "graph TD" << std::endl;
    driver.result->toMermaid();
    std::cout << "```" << std::endl;
    driver.setOutputFileName(ir);
    driver.result->toLLVM();
    return 0;
}
