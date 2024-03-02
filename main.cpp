#include "Driver.hpp"
extern Driver driver;
int main(int argc, char* argv[]) {
    std::string type = (argc == 5) ? argv[1] : "llvm";
    std::string input = (argc == 5) ? argv[2] : "/app/tests/2/in.sy";
    std::string output = (argc == 5) ? argv[3] : "/app/tests/2/out.md";
    std::string error = (argc == 5) ? argv[4] : "/app/tests/2/error.txt";
    freopen(error.c_str(), "w", stderr);
    driver.setSourceFileName(input);
    driver.setOutputFileName(output);
    driver.setErrorFileName(error);
    driver.setTraceParsing(false);
    driver.setTraceScanning(false);
    driver.parse();
    driver.program->toMermaid();
    driver.program->optimize();
    driver.program->toMermaid();
    return 0;
}
