#include "Driver.hpp"
int main(int argc, char* argv[]) {
    std::string folderName = "/app/tests/Decl-1/";
    std::string type = (argc == 6) ? argv[1] : "llvm";
    std::string input = (argc == 6) ? argv[2] : folderName + "in.sy";
    std::string md = (argc == 6) ? argv[3] : folderName + "out.md";
    std::string error = (argc == 6) ? argv[4] : folderName + "out.err";
    std::string ir = (argc == 6) ? argv[5] : folderName + "out.ll";
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
    return 0;
}
