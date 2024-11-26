#include "util.h"
#include "globals.h"

TreeNode* newStmtNode(StmtKind kind) {
    //相关的实现已经在TreeNode类的构造函数中实现完毕
    TreeNode* t = new TreeNode();
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    return t;
}

TreeNode* newExpNode(ExpKind kind) {
    //和newStmtNode一样
    TreeNode* t = new TreeNode();
    t->nodekind = ExpK;
    t->kind.exp = kind;
    return t;
}

static int indent_no = 0;

//打印缩进
static void printSpaces() {
    for (int i = 0; i < indent_no; i++)
        std::cout << " ";
}

//输出语法树
void printTree(TreeNode* tree) {
    indent_no += 2;
    while (tree != nullptr) {
        printSpaces();
        if (tree->nodekind == StmtK) {
            switch (tree->kind.stmt) {
            case IfK:
                std::cout << "If\n";
                break;
            case RepeatK:
                std::cout << "Repeat\n";
                break;
            case AssignK:
                std::cout << "Assign to: " << *tree->attr.name << "\n";
                break;
            case ReadK:
                std::cout << "Read: " << *tree->attr.name << "\n";
                break;
            case WriteK:
                std::cout << "Write\n";
                break;
            case ForToK:
                std::cout << "For: " << *tree->attr.name
                    << "\n";
                break;
            case ADD_ASSIGNK:
                std::cout << "ADD-Assign to: " << *tree->attr.name << "\n";
                break;
            case SUB_ASSIGNK:
                std::cout << "SUB-Assign to: " << *tree->attr.name << "\n";
                break;
            default:
                std::cout << "Unknown ExpNode kind\n";
            }

        }
        else if (tree->nodekind == ExpK) {
            switch (tree->kind.exp) {
            case OpK:
                std::cout << "Op: ";
                switch (tree->attr.op) {
                case EQUAL: std::cout << "=\n"; break;
                case LESS: std::cout << "<\n"; break;
                case ADD: std::cout << "+\n"; break;
                case SUB: std::cout << "-\n"; break;
                case MUL: std::cout << "*\n"; break;
                case DIV: std::cout << "/\n"; break;
                case MOD: std::cout << "%\n"; break;
                case POWER: std::cout << "^\n"; break;
                case GREATER: std::cout << ">\n"; break;
                case LESS_EQUAL: std::cout << "<=\n"; break;
                case GREATER_EQUAL: std::cout << ">=\n"; break;
                case NOT_EQUAL: std::cout << "<>\n"; break;
                default: std::cout << "Unknown operator\n";
                }
                break;
            case ConstK:
                std::cout << "Const: " << tree->attr.val << "\n";
                break;
            case IdK:
                std::cout << "Id: " << *tree->attr.name << "\n";
                break;
            }
        }

        for (int i = 0; i < 3; i++)
            if (tree->child[i] != nullptr)
                printTree(tree->child[i]);

        tree = tree->sibling;
    }
    indent_no -= 2;
}