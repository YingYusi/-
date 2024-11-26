#include "globals.h"
#include "util.h"
#include "parse.h"
#include "scan.h"

std::string tokenString;
int main() {
    std::cout << "编译成功！\n" << std::endl;

    std::string input;
    std::string line;

    //读取输入
    std::cout << "请开始输入，使用ctrl+z来结束输入" << std::endl;
    while (std::getline(std::cin, line)) {
        input += line + "\n";
    }

    //生成语法树
    TreeNode* syntaxTree = parse(input);

    //输出语法树
    if (syntaxTree != nullptr) {
        std::cout << "语法树如下:" << std::endl;
        printTree(syntaxTree);
    }

    return 0;
}