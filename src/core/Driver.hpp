//
// Created by 风唤长河 on 2024/2/23.
//

#ifndef SYSY_COMPILER_CORE_DRIVER_HPP
#define SYSY_COMPILER_CORE_DRIVER_HPP
#include "Parser.hpp"
#include "Program.hpp"
#define YY_DECL \
    yy::parser::symbol_type yylex(Driver &driver)

YY_DECL;
namespace Compiler::Core {
    class Driver {
    public:
        Driver(bool traceParsing,bool traceScanning);
        ~Driver()=default;
        void setTraceParsing(bool traceParsing);
        void setTraceScanning(bool traceScanning);
        void setSourceFileName(const std::string &sourceFileName);
        void setOutputFileName(const std::string &outputFileName);
        void setErrorFileName(const std::string &errorFileName);
        void parse();
        static void error(const yy::location &l, const std::string &m);
        void scan_begin(); // 初始化词法分析器, 实现在scanner.l中，主要将sourceFileName和yyin绑定
        void scan_end(); // 释放源代码所在的文件流, 实现在scanner.l中
        
        yy::location location;
        ProgramPtr program;
        bool traceScanning;
        bool traceParsing;
        std::string sourceFileName; // 源代码文件名
        std::string outputFileName; // 输出文件名, 会对标准输出流进行重定向
        std::string errorFileName; // 报错信息的输出，会对标准错误流进行重定向
    };
    
}
using Driver = Compiler::Core::Driver;
inline static Driver driver(false, false);

#endif // SYSY_COMPILER_CORE_DRIVER_HPP
