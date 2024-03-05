#include "Driver.hpp"
int main(int argc, char* argv[]) {
    std::string type = (argc == 5) ? argv[1] : "llvm";
    std::string input = (argc == 5) ? argv[2] : "/app/tests/3/in.sy";
    std::string output = (argc == 5) ? argv[3] : "/app/tests/3/out.md";
    std::string error = (argc == 5) ? argv[4] : "/app/tests/3/error.txt";
    driver.setSourceFileName(input);
    driver.setOutputFileName(output);
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
    return 0;
}
