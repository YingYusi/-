#include "parse.h"
#include "scan.h"
#include "util.h"
#include "globals.h"

static TokenType token; // 当前token

static void syntaxError(const std::string& message) {
    std::cerr << "Syntax error: " << message << std::endl;
}

static void match(TokenType expected) {
    if (token == expected) {
        token = getToken();
    }
    else {
        syntaxError("Unexpected token");
    }
}

TreeNode* stmt_sequence() {
    TreeNode* t = statement();
    TreeNode* p = t;
    //扩充enddo
    while ((token != ENDFILE) && (token != END) && (token != ELSE) && (token != UNTIL) && (token != ENDDO)) {
        match(SEMI);
        TreeNode* q = statement();
        if (q != nullptr) {
            if (t == nullptr) t = p = q;
            else {
                p->sibling = q;
                p = q;
            }
        }
    }
    return t;
}

TreeNode* statement() {
    TreeNode* t = nullptr;
    // 保存当前标识符
    std::string currentId = tokenString;
    switch (token) {
    case IF: t = if_stmt(); break;
    case REPEAT: t = repeat_stmt(); break;
    case READ: t = read_stmt(); break;
    case WRITE: t = write_stmt(); break;
    case WHILE: t = while_stmt(); break;
    case FOR: t = for_stmt(); break;
    case ID: {
        match(ID);
        // 检查下一个token是什么类型的赋值
        switch (token) {
        case ASSIGN:
            t = assign_stmt();
            t->attr.name = new std::string(currentId);
            break;
        case ADD_ASSIGN:
            t = newStmtNode(ADD_ASSIGNK);
            t->attr.name = new std::string(currentId);
            match(ADD_ASSIGN);
            t->child[0] = exp();
            break;
        case SUB_ASSIGN:
            t = newStmtNode(SUB_ASSIGNK);
            t->attr.name = new std::string(currentId);
            match(SUB_ASSIGN);
            t->child[0] = exp();
            break;
        }
        break;
        //扩充end,enddo,endfile,until，读到他们的时候，不做任何处理
    case END: break;
    case ENDDO: break;
    case ENDFILE: break;
    case UNTIL:break;
    }
    default:
        syntaxError("Unexpected token -> " + tokenString);
        token = getToken();
        break;
    }
    return t;
}

//语句函数，在注释里，我使用“表达式”来指代小括号内的代码部分，使用“循环体”来指代大括号内的代码部分
TreeNode* if_stmt() {
    //if语句语法：if(表达式) then 循环体 (else (循环体)) end
    TreeNode* t = newStmtNode(IfK);
    match(IF);
    if (t != nullptr) t->child[0] = exp();
    match(THEN);
    if (t != nullptr) t->child[1] = stmt_sequence();
    if (token == ELSE) {
        match(ELSE);
        if (t != nullptr) t->child[2] = stmt_sequence();
    }
    match(END);
    return t;
}

TreeNode* repeat_stmt() {
    //repeat语句语法：repeat 循环体 until 表达式
    TreeNode* t = newStmtNode(RepeatK);
    match(REPEAT);
    if (t != nullptr) t->child[0] = stmt_sequence();
    match(UNTIL);
    if (t != nullptr) t->child[1] = exp();
    return t;
}

TreeNode* assign_stmt() {
    //assign语句语法：ID := 表达式
    TreeNode* t = newStmtNode(AssignK);
    if (t != nullptr && token == ID) {
        t->attr.name = new std::string(tokenString);
        match(ID);
    }
    match(ASSIGN);
    if (t != nullptr) t->child[0] = exp();
    return t;
}

TreeNode* read_stmt() {
    //read语句语法：read ID
    TreeNode* t = newStmtNode(ReadK);
    match(READ);
    if (t != nullptr && (token == ID)) {
        t->attr.name = new std::string(tokenString);
        match(ID);
    }
    return t;

}

TreeNode* write_stmt() {
    //write语句语法：write exp
    TreeNode* t = newStmtNode(WriteK);
    match(WRITE);
    if (t != nullptr) t->child[0] = exp();
    return t;
}

TreeNode* while_stmt() {
    //while语句写法：while 表达式 do 循环体 enddo
    TreeNode* t = newStmtNode(WhileK);
    match(WHILE);
    if (t != nullptr) {
        t->child[0] = exp();
    }
    match(DO);
    if (t != nullptr) {
        t->child[1] = stmt_sequence();
    }
    match(ENDDO);
    return t;
}

TreeNode* for_stmt() {
    //for语句写法：for ID := 表达式 to 表达式 do 循环体 enddo
    TreeNode* t = nullptr;
    match(FOR);
    std::string id_name = tokenString;
    match(ID);
    match(ASSIGN);
    TreeNode* start_exp = simple_exp();

    if (token == TO) {
        t = newStmtNode(ForToK);
        match(TO);
    }
    if (t != nullptr) {
        t->attr.name = new std::string(id_name);
        //起始值
        t->child[0] = start_exp;
        //终止值
        t->child[1] = simple_exp();
        match(DO);
        //循环体
        t->child[2] = stmt_sequence();
        match(ENDDO);
    }
    return t;
}

//表达式函数
TreeNode* exp() {
    //第一级，最低级，计算优先度最低
    TreeNode* t = simple_exp();
    //扩充大于号，小于等于，大于等于，不等于三个计算符
    if (token == LESS || token == GREATER || token == EQUAL || token == LESS_EQUAL || token == GREATER_EQUAL || token == NOT_EQUAL) {
        TreeNode* p = newExpNode(OpK);
        if (p != nullptr) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
        }
        match(token);
        if (t != nullptr)
            t->child[1] = simple_exp();
    }
    return t;
}


TreeNode* simple_exp() {
    //第二级
    TreeNode* t = term();
    while (token == ADD || token == SUB) {
        TreeNode* p = newExpNode(OpK);
        if (p != nullptr) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = term();
        }
    }
    return t;
}

TreeNode* term() {
    //第三级
    TreeNode* t = power();
    while (token == MUL || token == DIV || token == MOD) {  // 第二级别，* and /
        TreeNode* p = newExpNode(OpK);
        if (p != nullptr) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            p->child[1] = power();
        }
    }
    return t;
}

TreeNode* power() {
    //第四级
    TreeNode* t = factor();
    while (token == POWER) {
        TreeNode* p = newExpNode(OpK);
        if (p != nullptr) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            p->child[1] = power();//因为指数运算是右递归，所以需要改
        }
    }
    return t;
}


TreeNode* factor() {
    //第五级，最高级，计算优先度最高
    TreeNode* t = nullptr;
    switch (token) {
    case NUM:
        t = newExpNode(ConstK);
        if (t != nullptr && token == NUM) {
            t->attr.val = std::stoi(tokenString);
            match(NUM);
        }

        break;
    case ID:
        t = newExpNode(IdK);
        if (t != nullptr && token == ID) {
            t->attr.name = new std::string(tokenString);
            match(ID);
        }
        break;
    case LPAREN:
        match(LPAREN);
        t = exp();
        match(RPAREN);
        break;
    default:
        syntaxError("Unexpected token -> " + tokenString);
        token = getToken();
        break;
    }
    return t;
}

TreeNode* parse(const std::string& program) {
    init(program);
    token = getToken();
    TreeNode* t = stmt_sequence();
    if (token != ENDFILE) {
        syntaxError("Code ends before file");
    }
    return t;
}

