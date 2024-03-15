#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_map>

using namespace std;

enum TokenType {
    USING_KEYWORD,
    INT_KEYWORD,
    MAIN_KEYWORD,
    CIN_KEYWORD,
    COUT_KEYWORD,
    FOR_KEYWORD,
    IF_KEYWORD,
    RETURN_KEYWORD,

    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    LEFT_CURLY_BRACE,
    RIGHT_CURLY_BRACE,
    LEFT_SQUARE_BRACKET,
    RIGHT_SQUARE_BRACKET,

    SEMICOLON,

    OUTPUT_OPERATOR,
    INSERTION_OPERATOR,
    QUOTATION_MARK,
    
    PLUS_OPERATOR,
    ASSIGNMENT_OPERATOR,
    MULTIPLICATION_OPERATOR,
    DIVISION_OPERATOR,
    INC_OPERATOR,

    GREATER_THAN_OPERATOR,
    LESS_THAN_OPERATOR,

    IDENTIFIER,
    NUMBER,
    SYSTEM_HEADER_INCLUDE,
    PREPROCESSOR_DIRECTIVE,
    STRING_LITERAL,
    UNKNOWN
};

std::unordered_map<std::string, int> tokenMap = {
    {"using", USING_KEYWORD},
    {"int", INT_KEYWORD},
    {"main", MAIN_KEYWORD},
    {"cin", CIN_KEYWORD},
    {"cout", COUT_KEYWORD},
    {"for", FOR_KEYWORD},
    {"if", IF_KEYWORD},
    {"return", RETURN_KEYWORD},
    {"(", LEFT_PARENTHESIS},
    {")", RIGHT_PARENTHESIS},
    {"{", LEFT_CURLY_BRACE},
    {"}", RIGHT_CURLY_BRACE},
    {"[", LEFT_SQUARE_BRACKET},
    {"]", RIGHT_SQUARE_BRACKET},
    {";", SEMICOLON},
    {"<<", OUTPUT_OPERATOR},
    {">>", INSERTION_OPERATOR},
    {"\"", QUOTATION_MARK},
    {"+", PLUS_OPERATOR},
    {"/", DIVISION_OPERATOR},
    {"=", ASSIGNMENT_OPERATOR},
    {"*", MULTIPLICATION_OPERATOR},
    {"++", INC_OPERATOR},
    {">", GREATER_THAN_OPERATOR},
    {"<", LESS_THAN_OPERATOR}
};

string input;
int token;
string lexeme("");
int idx = 0;

string readFileContents(const string& filename);
char getChar();
void match(string expectedLexeme);
void lex();
bool getLexeme();
void reset();

void program();
void includeDirective();
void stringContent();
void getStringLiteral();
void quotedString();
void namespaceDeclaration();
void identifier();
void statement(bool notChecked = true, bool runOnce = false);
void variableDeclaration();
void outputStatement();
void inputStatement();
void forStatement();
void compareStatement();
void operationStatement();
void ifStatement();
void swapStatement();

int main()
{
    string filename = "in.cpp";
    input = readFileContents(filename);
    if (input.empty()) {
        return 1;
    }

    program();

    return 0;
}

void program()
{
    lex();
    if (token == PREPROCESSOR_DIRECTIVE)
    {
        includeDirective();
    }

    reset();
    lex();
    if (token == USING_KEYWORD)
    {
        namespaceDeclaration();
    }

    reset();
    lex();

    reset();
    lex();

    reset();
    lex();

    reset();
    lex();

    reset();
    lex();

    statement();
}

void includeDirective()
{
    reset();
    lex();

    reset();
    lex();

    stringContent();

    reset();
    lex();
}

void stringContent()
{
    reset();
    getStringLiteral();
}

void quotedString()
{
    reset();
    lex();

    stringContent();

    reset();
    lex();
}

void namespaceDeclaration()
{
    reset();
    lex();

    reset();
    lex();

    reset();
    lex();
}

void identifier()
{
    reset();
    lex();

    reset();
    if(getChar() == '[')
    {
        lex();

        reset();
        lex();

        reset();
        lex();
    }
    else
    {
        --idx;
    }

    return;
}

void statement(bool notChecked, bool runOnce)
{
    do
    {
        if(notChecked)
        {
            reset();
            lex();
        }
        if (token == INT_KEYWORD)
        {
            variableDeclaration();
        }
        else if (token == COUT_KEYWORD)
        {
            cout << "OUTPUT\n";
            outputStatement();
        }
        else if (token == CIN_KEYWORD)
        {
            inputStatement();
        }
        else if (token == FOR_KEYWORD)
        {
            forStatement();
        }
        else if (token == IF_KEYWORD)
        {
            ifStatement();
            break;
        }
        else if (
            token == RIGHT_CURLY_BRACE
        )
        {
            break;
        }
        else if (token == IDENTIFIER)
        {
            identifier();
        }
        else
        {
            break;
        }
    }while(!runOnce);
}

void variableDeclaration()
{
    reset();
    lex();

    reset();
    lex();

    if(token == LEFT_SQUARE_BRACKET)
    {
        reset();
        lex();

        reset();
        lex();

        reset();
        lex();
    }
    else if(token == ASSIGNMENT_OPERATOR)
    {
        statement();
    }
}

void outputStatement()
{
    reset();
    lex();

    quotedString();

    reset();
    lex();
}

void inputStatement()
{
    reset();
    lex();

    reset();
    lex();

    reset();
    lex();

    if(token == SEMICOLON)
    {
        return;
    }

    reset();
    lex();

    reset();
    lex();

    reset();
    lex();
}

void forStatement()
{
    reset();
    lex();

    reset();
    lex();

    if (token == INT_KEYWORD)
    {
        variableDeclaration();
    }

    reset();
    lex();

    compareStatement();

    reset();
    lex();

    operationStatement();

    reset();
    lex();

    reset();
    lex();

    if(token != LEFT_CURLY_BRACE)
    {
        statement(false, true);
    }
    else
    {
        statement(true, true);

        reset();
        lex();
    }
}

void compareStatement()
{
    identifier();

    reset();
    lex();

    identifier();
}

void operationStatement()
{
    reset();
    lex();

    reset();
    lex();
}

void ifStatement()
{
    reset();
    lex();

    compareStatement();

    reset();
    lex();

    reset();
    lex();

    statement(true, true);

    swapStatement();
    
    reset();
    lex();
}

void swapStatement()
{
    do
    {
        reset();
        lex();
    } while (getChar() != ';');

    do
    {
        reset();
        if (getChar() != '{')
        {
            lex();
        }
    } while (getChar() != ';');
}

void lex()
{
    char c;
    do {
        c = getChar();
        if (isspace(c))
        {
            ++idx;
        }
    } while (isspace(c));

    switch (c)
    {
        case '#':
            lexeme += c;
            token = PREPROCESSOR_DIRECTIVE;
            break;
        
        case '<':
            lexeme += c;
            if(input[idx + 1] == '<')
            {
                lexeme += c;
                token = OUTPUT_OPERATOR;
                idx++;
                break;
            }
            token = LESS_THAN_OPERATOR;
            break;
        
        case '>':
            lexeme += c;
            if(input[idx + 1] == '>')
            {
                lexeme += c;
                token = INSERTION_OPERATOR;
                idx++;
                break;
            }
            token = GREATER_THAN_OPERATOR;
            break;
        
        case ';':
            lexeme += c;
            token = SEMICOLON;
            break;

        case '(':
            lexeme += c;
            token = LEFT_PARENTHESIS;
            break;
        
        case ')':
            lexeme += c;
            token = RIGHT_PARENTHESIS;
            break;

        case '[':
            lexeme += c;
            token = LEFT_SQUARE_BRACKET;
            break;
        
        case ']':
            lexeme += c;
            token = RIGHT_SQUARE_BRACKET;
            break;
        
        case '{':
            lexeme += c;
            token = LEFT_CURLY_BRACE;
            break;
        
        case '}':
            lexeme += c;
            token = RIGHT_CURLY_BRACE;
            break;

        case '"':
            lexeme += c;
            token = QUOTATION_MARK;
            break;
        
        case '=':
            lexeme += c;
            token = ASSIGNMENT_OPERATOR;
            break;

        case '+':
            lexeme += c;
            if(input[idx + 1] == '+')
            {
                lexeme += c;
                token = INC_OPERATOR;
                idx++;
                break;
            }
            token = PLUS_OPERATOR;
            break;
        
        default:
            if(isdigit(c))
            {
                token = NUMBER;
                getLexeme();
                break;
            }

            token = UNKNOWN;
            getLexeme();

            if(lexeme == "using")
            {
                token = USING_KEYWORD;
            }
            else if(lexeme == "int")
            {
                token = INT_KEYWORD;
            }
            else if(lexeme == "cout")
            {
                token = COUT_KEYWORD;
            }
            else if(lexeme == "cin")
            {
                token = CIN_KEYWORD;
            }
            else if(lexeme == "for")
            {
                token = FOR_KEYWORD;
            }
            else if(lexeme == "if")
            {
                token = IF_KEYWORD;
            }
            else
            {
                token = IDENTIFIER;
            }
        }

    printf("The next Token is: %2d, the next Lexeme is: %s\n", token, lexeme.c_str());
}

void getStringLiteral()
{
    token = STRING_LITERAL;
    char c = getChar();
    while(
        c != '"' && c != '>'
    )
    {
        lexeme += c;
        ++idx;
        c = getChar();

        if (
            c == '"' || c == '>'
        )
        {
            --idx;
            printf("The next Token is: %2d, the next Lexeme is: %s\n", token, lexeme.c_str());
            return;
        }
    };
}

bool getLexeme()
{
    char c = getChar();
    while(
        !isspace(c)
    )
    {
        if (
            c == '<' || c == '>' ||
            c == ';' || c == '"' ||
            c == '(' || c == ')' ||
            c == '[' || c == ']' ||
            c == '=' || c == '+' ||
            c == '-'
        )
        {
            --idx;
            return false;
        }
        lexeme += c;
        ++idx;
        c = getChar();
    };

    return true;
}

void reset()
{
    char c;
    ++idx;
    do {
        c = getChar();
        if (isspace(c))
        {
            ++idx;
        }
    } while (isspace(c));
    lexeme = "";
    token = -1;
}

char getChar()
{
    return input[idx];
}

void match(string expectedLexeme)
{
    if (lexeme != expectedLexeme)
    {
        printf("Expected: %s, Got: %s\n", expectedLexeme.c_str(), lexeme.c_str());
        exit(EXIT_FAILURE);
    }
}

string readFileContents(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return "";
    }

    string contents;
    string line;
    while (getline(file, line)) {
        contents += line + '\n';
    }

    file.close();
    return contents;
}