//
// Created by 风唤长河 on 2024/2/23.
//

#include "Node.hpp"
#include "Driver.hpp"
#include <fstream>
namespace Compiler::AbstractSyntaxTree::Node {
    Node::Node(){
        this->id = ++maxId;
        this->begin = driver.location.begin;
        this->end = driver.location.end;
    }
    void Node::printLocation() {
        std::ifstream targetFile(driver.sourceFileName);
        std::vector<std::string> lines;
        std::string line;
        for (int i = 0; i < this->begin.line-1; i++){
            std::getline(targetFile, line);
        }
        for (int i = this->begin.line-1; i < this->end.line; i++){
            std::getline(targetFile, line);
            lines.push_back(line);
        }
        for (size_t i = 0; i < lines.size(); i++){
            std::cerr <<"|"<< lines[i] << std::endl;
            std::cerr << "|";
            if (i == 0){
                for (int j = 0; j < this->begin.column-1; j++){
                    std::cerr << " ";
                }
                if (this->begin.line == this->end.line){
                    for (int j = this->begin.column - 1; j < this->end.column - 1; j++)
                    {
                        std::cerr << "^";
                    }
                } else {
                    for (size_t j = this->begin.column-1; j < lines[i].size(); j++)
                    {
                        std::cerr << "^";
                    }
                }
            }else if (i == lines.size() - 1){
                for (int j = 0; j < this->end.column-1; j++)
                {
                    std::cerr << "^";
                }
            }else {
                for (size_t j = 0; j < lines[i].size(); j++){
                    std::cerr << " ";
                }
            }
            std::cerr << std::endl;
        }
    }
}