//
// Created by 风唤长河 on 2024/2/23.
//

#include <iostream>
#include "Driver.hpp"
#include <fstream>
namespace Compiler::Core {

    Driver::Driver(bool traceScanning, bool traceParsing) : traceScanning(traceScanning), traceParsing(traceParsing) {}

    void Driver::setTraceParsing(bool traceParsing) {
        this->traceParsing = traceParsing;
    }

    void Driver::setTraceScanning(bool traceScanning) {
        this->traceScanning = traceScanning;
    }

    void Driver::setSourceFileName(const std::string &sourceFileName) {
        this->sourceFileName = sourceFileName;
        freopen(sourceFileName.c_str(), "r", stdin);
    }

    void Driver::setOutputFileName(const std::string &outputFileName) {
        this->outputFileName = outputFileName;
        freopen(outputFileName.c_str(), "w", stdout);
    }

    void Driver::setErrorFileName(const std::string &errorFileName) {
        this->errorFileName = errorFileName;
        freopen(errorFileName.c_str(), "w", stderr);
    }

    void Driver::parse() {
        if (this->sourceFileName.empty()) {
            std::cerr << "No input file" << std::endl;
            return;
        }
        location.initialize(&sourceFileName);
        yy::parser parse(*this);
        parse.set_debug_level(traceParsing);
        int res = parse();
        if (res != 0) {
            std::cerr << "Parse failed" << std::endl;
        }
    }

    void Driver::error(const yy::location &l, const std::string &m) {
        std::cerr << l << ": " << m << std::endl;
    }
}