////
//// Created by 风唤长河 on 2024/3/1.
////

#include "ConstantFold.hpp"

namespace Compiler::SemanticAnalyzer {
    ConstantFold::ConstantFold(Driver &driver) : driver(driver) {}

    void ConstantFold::run() {
        driver.program->optimize();
    }
}