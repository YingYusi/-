#include "parse.h"
#include "scan.h"
#include "util.h"
#include "globals.h"

static TokenType token; // ��ǰtoken

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
    //����enddo
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
    // ���浱ǰ��ʶ��
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
        // �����һ��token��ʲô���͵ĸ�ֵ
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
        //����end,enddo,endfile,until���������ǵ�ʱ�򣬲����κδ���
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

//��亯������ע�����ʹ�á����ʽ����ָ��С�����ڵĴ��벿�֣�ʹ�á�ѭ���塱��ָ���������ڵĴ��벿��
TreeNode* if_stmt() {
    //if����﷨��if(���ʽ) then ѭ���� (else (ѭ����)) end
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
    //repeat����﷨��repeat ѭ���� until ���ʽ
    TreeNode* t = newStmtNode(RepeatK);
    match(REPEAT);
    if (t != nullptr) t->child[0] = stmt_sequence();
    match(UNTIL);
    if (t != nullptr) t->child[1] = exp();
    return t;
}

TreeNode* assign_stmt() {
    //assign����﷨��ID := ���ʽ
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
    //read����﷨��read ID
    TreeNode* t = newStmtNode(ReadK);
    match(READ);
    if (t != nullptr && (token == ID)) {
        t->attr.name = new std::string(tokenString);
        match(ID);
    }
    return t;

}

TreeNode* write_stmt() {
    //write����﷨��write exp
    TreeNode* t = newStmtNode(WriteK);
    match(WRITE);
    if (t != nullptr) t->child[0] = exp();
    return t;
}

TreeNode* while_stmt() {
    //while���д����while ���ʽ do ѭ���� enddo
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
    //for���д����for ID := ���ʽ to ���ʽ do ѭ���� enddo
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
        //��ʼֵ
        t->child[0] = start_exp;
        //��ֵֹ
        t->child[1] = simple_exp();
        match(DO);
        //ѭ����
        t->child[2] = stmt_sequence();
        match(ENDDO);
    }
    return t;
}

//���ʽ����
TreeNode* exp() {
    //��һ������ͼ����������ȶ����
    TreeNode* t = simple_exp();
    //������ںţ�С�ڵ��ڣ����ڵ��ڣ����������������
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
    //�ڶ���
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
    //������
    TreeNode* t = power();
    while (token == MUL || token == DIV || token == MOD) {  // �ڶ�����* and /
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
    //���ļ�
    TreeNode* t = factor();
    while (token == POWER) {
        TreeNode* p = newExpNode(OpK);
        if (p != nullptr) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            p->child[1] = power();//��Ϊָ���������ҵݹ飬������Ҫ��
        }
    }
    return t;
}


TreeNode* factor() {
    //���弶����߼����������ȶ����
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

