#include "Driver.hpp"
#include "Header.hpp"
#include <any>

int main(int argc, char* argv[]) {
    std::string type = (argc == 5) ? argv[1] : "llvm";
    std::string input = (argc == 5) ? argv[2] : "/app/tests/2/in.sy";
    std::string output = (argc == 5) ? argv[3] : "/app/tests/2/out.ll";
    std::string error = (argc == 5) ? argv[4] : "/app/tests/2/error.txt";
    if (type == std::string("mermaid")) {
        Compiler::AbstractSyntaxTree::dumpType =
        Compiler::AbstractSyntaxTree::DumpType::AST_MERMAID;
    }
    else if (type == std::string("llvm"))
    {
        Compiler::AbstractSyntaxTree::dumpType = Compiler::AbstractSyntaxTree::DumpType::IR_LLVM;
    }
    Compiler::Driver driver = Compiler::Driver(input, output, error, false, false);
    freopen( output.c_str(), "w", stdout );
    freopen( error.c_str(), "w", stderr );
    driver.parse();
    driver.result->dump();
    // std::int32_t a = 12444;
    // auto int12 = std::make_unique<Compiler::AbstractSyntaxTree::Number>(std::move(a));
    // std::string c = "c123";
    // auto idC = std::make_unique<Compiler::AbstractSyntaxTree::Ident>(std::move(c));
    // Compiler::AbstractSyntaxTree::ExpVarient exp= std::move(int12);
    // Compiler::AbstractSyntaxTree::ExpVarient exp2= std::move(idC);
    // Compiler::AbstractSyntaxTree::ExpVarient exp3= std::move(exp);
    // Compiler::AbstractSyntaxTree::ExpVarient exp4 = std::make_unique<Compiler::AbstractSyntaxTree::BinaryExp>(
    //     Compiler::AbstractSyntaxTree::Operator::ADD,
    //     std::move(exp3),
    //     std::move(exp2)
    // );
    // std::cout<<"```mermaid\n";
    // std::cout<<"graph TD\n";
    // std::visit([](auto&& arg) { arg->dump(); }, exp4);
    // std::cout<<"```\n";

    return 0;
}
