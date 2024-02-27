//
// Created by venty on 2024/2/23.
//

#include "Driver.hpp"

#include <utility>
#include "FunctionDefinition.hpp"


namespace Compiler{
    Driver::Driver(std::string input, std::string output, std::string error,bool trace_parsing, bool trace_scanning)
    : trace_parsing(trace_parsing), trace_scanning(trace_scanning), inputFileName(std::move(input)), outputFileName(std::move(output)), errorFileName(std::move(error)) {}

    void Driver::parse() {
        location.initialize(&inputFileName);
        scan_begin();
        yy::parser parse(*this);
        parse.set_debug_level(trace_parsing);
        int res=parse();
        scan_end();
    }

    void Driver::error(const yy::location &l, const std::string &m) {
        std::cerr << l << ": " << m << std::endl;
    }

    Driver::~Driver() = default;


}