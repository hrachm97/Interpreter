#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <fstream>
#include <iostream>

void tokenize(std::string my_code, std::vector<std::string>& tokens) {
    bool new_token = false;
    bool string1 = false; // ' string ' both quote types are valid
    bool string2 = false; // " string "
    int token_index{};
    for (int i = 0; i <= my_code.size(); ++i) {
        if (my_code[i] == '"' && !string1) {//when the loop founds the symbol '"' it includes everything in the new string type token until the next '"'
            string2 = !string2;
            if(new_token) tokens.push_back(my_code.substr(token_index, i + 1 - token_index));
            token_index = i;
            new_token = !new_token;
        }
        if (my_code[i] == '\'' && !string2) {//when the loop founds the symbol ' it includes everything in the new string type token until the next '
            string1 = !string1;
            if(new_token) tokens.push_back(my_code.substr(token_index, i + 1 - token_index));
            token_index = i;
            new_token = !new_token;
        }
        if (string1 || string2) continue;
        if ((my_code[i] == ' ' || my_code[i] == '\r' || my_code[i] == '\n' || my_code[i] == '\0')) {
            if(my_code[i - 1] == '"' || my_code[i - 1] == '\'') new_token = false;
            if(new_token) tokens.push_back(my_code.substr(token_index, i - token_index));
            token_index = i + 1;
            new_token = false;
        } else {
            new_token = true;
        }
    }
}

// void print_tokens(std::vector<std::string> tokens) {
//     int i{};
//     std::cin >> i;
//     while (i >= 0 && i < tokens.size()) {
//         std::cout << '"' << tokens[i] << '"' << std::endl;
//         std::cin >> i;
//     }
// }

std::string to_string(double n, int max = 6, int base = 10) {
    int fraction_counter{};
    auto local_n = n;
    while (local_n - (long long)local_n > 0.000001 && fraction_counter < max) {
        local_n *= base;
        fraction_counter++;
    }
    if (fraction_counter == 0) max++;
    std::string tmp = std::to_string(n).substr(0, std::to_string(n).size() - max + fraction_counter);
    for (int i = tmp.size() - 1; i > 0 ; --i) {
        if(tmp[i] != '0') {
            tmp = (tmp[0] == '-') ? tmp.substr(0, i + 2) :  tmp.substr(0, i + 1);
            break;
        }
    }
    return tmp;
}

struct Identifier {
    char type = 'u'; // 'u' for undefined, 's' for string, 'n' for number
    std::string str;
    double num;
    void operator=(const Identifier& value) {
        this->type = value.type;
        this->str = value.str;
        this->num = value.num;
    }
    void operator=(const double& value) {
        this->type = 'n';
        this->num = value;
    }
    void operator=(const std::string& value) {
        this->type = 's';
        this->str = value;
    }
    Identifier operator+(const Identifier& value) { // 9 possible combinations: 3*3
        Identifier tmp; 
        if (this->type == value.type) { // 3 covered
            tmp.type = value.type;
            tmp.str = this->str + value.str;
            tmp.num = this->num + value.num;
        } else if (value.type == 'u' || this->type == 'u')  tmp.type = 'u'; // 4 covered
        //if string is added to number or vice versa the number operand is converted to string
        else if (this->type == 's') { //1 covered
            tmp.type = 's'; 
            tmp.str = this->str + to_string(value.num);
        } else { //last 1 covered
            tmp.type = 's';
            tmp.str = to_string(this->num) + value.str;
        }
        return tmp;
    }
    Identifier operator-(const Identifier& value) { // 9 possible combinations: 3*3
        Identifier tmp; 
        if (this->type == value.type) { // 3 covered
            tmp.type = value.type;
            tmp.str = this->str.substr(0, this->str.size() - value.str.size());
            tmp.num = this->num - value.num;
        } else if (value.type == 'u' || this->type == 'u')  tmp.type = 'u'; // 4 covered
        //if string is subtracted by N the operation returnes a string removed N symbols from the end
        else if (this->type == 's') { //1 covered
            tmp.type = 's'; 
            tmp.str = this->str.substr(0, this->str.size() - value.num);
        } else { //last 1 covered
            tmp.type = 's';
            tmp.str = value.str.substr(this->num);
        }
        return tmp;
    }
    Identifier operator*(const Identifier& value) { // 9 possible combinations: 3*3
        Identifier tmp; 
        if (value.type == 'u' || this->type == 'u')  tmp.type = 'u'; // 5 covered
        //if string is multiplied by string the returned value is the first string repeated N times where N is the second string's size
        else if (this->type == value.type && value.type == 's') { // 1 covered
            tmp.type = value.type;
            tmp.str = this->str;
            int ttmp = value.str.size() - 1;
            while(ttmp > 0) {ttmp--; tmp.str += this->str;}
            return tmp;
        }
        else if (this->type == value.type && value.type == 'n') { // 1 covered
            tmp.type = this->type;
            tmp.num = this->num * value.num;
            return tmp;
        }
        else if (this->type == 's') { //1 covered
            tmp.type = 's'; 
            tmp.str = this->str;
            int ttmp = value.num;
            while(ttmp - 1) {ttmp--; tmp.str += this->str;}
            tmp.num = this->num * value.num;
            return tmp;
        } else { //last 1 covered
            tmp.type = 's'; 
            tmp.str = value.str;
            int ttmp = this->num;
            while(ttmp - 1) {ttmp--; tmp.str += value.str;}
            tmp.num = value.num * this->num;
            return tmp;
        }
        return tmp;
    }
    Identifier operator/(const Identifier& value) { // 9 possible combinations: 3*3
        Identifier tmp; 
        if (value.type == 'u' || this->type == 'u')  tmp.type = 'u'; // 5 covered
        else if (this->type == value.type && value.type == 's') { // 1 covered
            tmp.type = value.type;
            tmp.str = this->str.substr(0, this->str.size() / value.str.size());
        }
        else if (this->type == value.type && value.type == 'n') { // 1 covered
            tmp.type = value.type;
            tmp.num = this->num / value.num;
        }
        else if (this->type == 's') { //1 covered
            tmp.type = 's'; 
            tmp.str = this->str.substr(0, this->str.size() / value.num);
        } else { //last 1 covered
            tmp.type = 'n';
            tmp.num = this->num / value.str.size();
        }
        return tmp;
    }
    Identifier operator>(const Identifier& value) { // 9 possible combinations: 3*3
        Identifier tmp;
        tmp.type = 'n';
        if (value.type == 'u' || this->type == 'u') tmp.type = 'u'; // 5 covered
        else if (this->type == value.type && value.type == 's') { // 1 covered
            tmp.num = this->str.size() > value.str.size();
            return tmp;
        }
        else if (this->type == value.type && value.type == 'n') { // 1 covered
            tmp.num = this->num > value.num;
            return tmp;
        }
        else if (this->type == 's') { //1 covered
            tmp.num = this->str.size() > value.num;
            return tmp;
        } else {
            tmp.num = this->num > value.str.size();
            return tmp;
        }
        return tmp;
    }
    Identifier operator<(const Identifier& value) { // 9 possible combinations: 3*3
        Identifier tmp;
        tmp.type = 'n';
        if (value.type == 'u' || this->type == 'u') tmp.type = 'u'; // 5 covered
        else if (this->type == value.type && value.type == 's') { // 1 covered
            tmp.num = this->str.size() < value.str.size();
            return tmp;
        }
        else if (this->type == value.type && value.type == 'n') { // 1 covered
            tmp.num = this->num < value.num;
            return tmp;
        }
        else if (this->type == 's') { //1 covered
            tmp.num = this->str.size() < value.num;
            return tmp;
        } else {
            tmp.num = this->num < value.str.size();
            return tmp;
        }
        return tmp;
    }
    Identifier operator>=(const Identifier& value) { 
        Identifier tmp;
        tmp.type = 'n';
        tmp = operator<(value);
        tmp.num = !(bool)tmp.num;
        return tmp;
    }
    Identifier operator<=(const Identifier& value) { 
        Identifier tmp;
        tmp.type = 'n';
        tmp = operator>(value);
        tmp.num = !(bool)tmp.num;
        return tmp;
    }
    Identifier operator==(const Identifier& value) { // 9 possible combinations: 3*3
        Identifier tmp;
        tmp.type = 'n';
        if (value.type == 'u' || this->type == 'u') {
            tmp.num = (value.type == this->type) ? 1 : 0;
        } // 5 covered
        else if (this->type == value.type && value.type == 's') { // 1 covered
            tmp.num = this->str == value.str;
            return tmp;
        }
        else if (this->type == value.type && value.type == 'n') { // 1 covered
            tmp.num = this->num == value.num;
            return tmp;
        }
        else if (this->type == 's') { //1 covered
            tmp.num = this->str == to_string(value.num);
            return tmp;
        } else if (this->type == 'n') { //1 covered
            tmp.num = to_string(this->num) == value.str;
            return tmp;
        }
        return tmp;
    }
    Identifier operator!=(const Identifier& value) { 
        Identifier tmp;
        tmp.type = 'n';
        tmp = operator==(value);
        tmp.num = !(bool)tmp.num;
        return tmp;
    }
    Identifier operator&&(const Identifier& value) { // 9 possible combinations: 3*3
        Identifier tmp;
        tmp.type = 'n';
        if (value.type == 'u' || this->type == 'u') tmp.type = 'u'; // 5 covered
        else if (this->type == value.type && value.type == 's') { // 1 covered
            tmp.num = this->str.size() && value.str.size();
            return tmp;
        }
        else if (this->type == value.type && value.type == 'n') { // 1 covered
            tmp.num = this->num && value.num;
            return tmp;
        }
        else if (this->type == 's') { //1 covered
            tmp.num = this->str.size() && value.num;
            return tmp;
        } else if (this->type == 'n') { //1 covered
            tmp.num = this->num && value.str.size();
            return tmp;
        }
        return tmp;
    }
    Identifier operator||(const Identifier& value) { // 9 possible combinations: 3*3
        Identifier tmp;
        tmp.type = 'n';
        if (value.type == 'u' || this->type == 'u') tmp.type = 'u'; // 5 covered
        else if (this->type == value.type && value.type == 's') { // 1 covered
            tmp.num = this->str.size() || value.str.size();
            return tmp;
        }
        else if (this->type == value.type && value.type == 'n') { // 1 covered
            tmp.num = this->num || value.num;
            return tmp;
        }
        else if (this->type == 's') { //1 covered
            tmp.num = this->str.size() || value.num;
            return tmp;
        } else if (this->type == 'n') { //1 covered
            tmp.num = this->num || value.str.size();
            return tmp;
        }
        return tmp;
    }
    Identifier strict_equal(const Identifier& value) { // 9 possible combinations: 3*3
        Identifier tmp;
        tmp.type = 'n';
        if (value.type == 'u' || this->type == 'u') {
            tmp.num = 0;
            if (value.type == this->type) tmp.num = 1;
        }
        else if (this->type == value.type && value.type == 's') { 
            tmp.num = this->str == value.str;
            return tmp;
        }
        else if (this->type == value.type && value.type == 'n') {
            tmp.num = this->num == value.num;
            return tmp;
        } else {
            tmp.num = this->type == value.type;
        }
        return tmp;
    }
    Identifier not_strict_equal(const Identifier& value) {
        Identifier tmp = strict_equal(value);
        tmp.type = 'n';
        tmp.num = !(bool)tmp.num;
        return tmp;
    }
};

void print_identifier(Identifier i) {
    if(i.type == 'u') std::cout << "undefined";
    if(i.type == 's') std::cout << i.str;
    if(i.type == 'n') std::cout << i.num;
    if(i.type == 'b') std::cout << (i.num == 0) ? "false" : "true";
}

// void print_identifiers(std::map<std::string, std::vector<std::string>> token_types, std::map<std::string, Identifier>& identifiers) {
//     int i{};
//     std::cin >> i;
//     while (i >= 0 && i < token_types["identifier"].size()) {
//         std::cout << token_types["identifier"][i] << std::endl;
//         print_identifier(identifiers[token_types["identifier"][i]]);
//         std::cin >> i;
//     }
// }

void set_types( // checks the type of the tokens 
    std::vector<std::string> tokens, // reads and checks the type matches
    std::map<std::string, std::vector<std::string>>& token_types, // possible types tokens can have
    std::map<std::string, std::string>& code_tokens_types, //token types must be passed by reference to be filled by this function
    std::vector<int>& eol
) {
    for (int i = 0; i < tokens.size(); ++i) {
        bool found = true;
        //checking if the token is oparator
        for(int j = 0; j <= token_types["operator"].size(); ++j){
            if (j == token_types["operator"].size()) {
                found = false;
                break;
            }
            if (tokens[i] == token_types["operator"][j]) {
                code_tokens_types[tokens[i]] = "operator";
                break;
            }
        }
        if (found) continue;
        found = true;
        //checking if the token is identifier
        for(int j = 0; j <= tokens[i].size(); ++j){
            if (j == tokens[i].size()) {
                found = false;
                break;
            }
            if ((tokens[i][j] < '0' || tokens[i][j] > '9') && tokens[i][j] != '.' && tokens[i][j] != ';' && tokens[i][0] != '-') {
                if(tokens[i][j] == '\'' || tokens[i][j] == '"') {
                    found = false;
                    break;
                }
                code_tokens_types[tokens[i]] = "identifier";
                token_types["identifier"].push_back(tokens[i]);
                break;
            }
        }
        if (found) continue;
        //checking if the token is string
        if (
            (tokens[i][0] == '\'' && tokens[i][tokens[i].size() - 1] == '\'') 
            || 
            (tokens[i][0] == '"' && tokens[i][tokens[i].size() - 1] == '"')
        ) {
            code_tokens_types[tokens[i]] = "string";
            found = true;
        }
        if (found) continue;
        if (tokens[i] == ";"){
            eol.push_back(i);
            continue;
        } // semicolon does not have a token type
        // if token is neither operator nor identifier then it is a number
        code_tokens_types[tokens[i]] = "number";
    }
}

std::string rmv_quotes(const std::string& str) {
    return str.substr(1, str.size() - 2);
}

struct Statement {
    int start = 0;
    int end = 0;
};

Identifier add(Identifier& opnd1, Identifier& opnd2) {
    return opnd1 + opnd2;
}

Identifier oprt(Identifier opnd1, Identifier opnd2, Identifier(*fp)(Identifier& value1, Identifier& value2) = add) {
    return fp(opnd1, opnd2);
}

void set_values(
    std::vector<std::string> tokens, // looks for identifiers in tokens to assign values
    std::map<std::string, std::string> code_tokens_types, // checks the token type to be assigned to identifier 
    //in order to organize the assignment with particular approach
    std::map<std::string, Identifier>& identifiers, //Identifiers must be passed by reference to be filled by this function
    Statement statement
) {
    Identifier tmp; // tmp is used in asignments with expression and chain assignments or expressions.
    //If we have an expression in the right side of the '=' the expression is resolved and the value is assigned to tmp

    for (int i = statement.end - 1; i >= statement.start; --i) { // the loop starts from the end in case the chain assignment works
        std::string r_value_type = code_tokens_types[tokens[i]];
        std::string l_value_type;
        if(i > statement.start + 1) l_value_type = code_tokens_types[tokens[i - 2]];
        //setting tmp to the value of the first appearance of a token in an expression
        if((i < statement.end - 1 && code_tokens_types[tokens[i + 1]] != "operator") || i == statement.end - 1) {
            if (r_value_type == "number") {
                tmp = std::stod(tokens[i]);  // convert double to identifier so we can apply + operator (oop disknowledge)
            } else if (r_value_type == "string") {
                tmp = rmv_quotes(tokens[i]); // convert double to identifier so we can apply + operator (oop disknowledge)
            } else if (r_value_type == "identifier") {
                tmp = identifiers[tokens[i]];
            }
        }
        //assignment
        if(i > statement.start + 1 && tokens[i - 1] == "=") {// the case when the token before '=' can be a constant will be prevented by error checker
            identifiers[tokens[i - 2]] = tmp;
        }
        //addition
        if(i > statement.start + 1 && tokens[i - 1] == "+") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if(i < statement.end - 1 && code_tokens_types[tokens[i + 1]] == "operator") { // chain operation case
                if (l_value_type == "identifier") {
                    tmp = identifiers[tokens[i - 2]] + tmp; 
                } else if (l_value_type == "number") {
                    Identifier rvalue;
                    rvalue = std::stod(tokens[i - 2]);
                    tmp = rvalue + tmp;
                } else if (l_value_type == "string") {
                    Identifier rvalue;
                    rvalue = rmv_quotes(tokens[i - 2]);
                    tmp = rvalue + tmp;
                }
            } else { // NOT chain operation case or the first pair of chain operation
                if (l_value_type == "identifier") {
                    tmp = identifiers[tokens[i - 2]] + tmp;
                } else if (l_value_type == "number") {
                    Identifier rvalue;
                    rvalue = std::stod(tokens[i - 2]);
                    tmp = rvalue + tmp;
                } else if (l_value_type == "string") {
                    Identifier rvalue;
                    rvalue = rmv_quotes(tokens[i - 2]);
                    tmp = rvalue + tmp;
                }
            }
        }
        //subtraction
        if(i > statement.start + 1 && tokens[i - 1] == "-") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if(i < statement.end - 1 && code_tokens_types[tokens[i + 1]] == "operator") { // chain operation case
                if (l_value_type == "identifier") {
                    tmp = identifiers[tokens[i - 2]] - tmp; 
                } else if (l_value_type == "number") {
                    Identifier rvalue;
                    rvalue = std::stod(tokens[i - 2]);
                    tmp = rvalue - tmp;
                } else if (l_value_type == "string") {
                    Identifier rvalue;
                    rvalue = rmv_quotes(tokens[i - 2]);
                    tmp = rvalue - tmp;
                }
            } else { // NOT chain operation case or the first pair of chain operation
                if (l_value_type == "identifier") {
                    tmp = identifiers[tokens[i - 2]] - tmp;
                } else if (l_value_type == "number") {
                    Identifier rvalue;
                    rvalue = std::stod(tokens[i - 2]);
                    tmp = rvalue - tmp;
                } else if (l_value_type == "string") {
                    Identifier rvalue;
                    rvalue = rmv_quotes(tokens[i - 2]);
                    tmp = rvalue - tmp;
                }
            }
        }
        //multiplication
        if(i > statement.start + 1 && tokens[i - 1] == "*") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if(i < statement.end - 1 && code_tokens_types[tokens[i + 1]] == "operator") { // chain operation case
                if (l_value_type == "identifier") {
                    tmp = identifiers[tokens[i - 2]] * tmp; 
                } else if (l_value_type == "number") {
                    Identifier rvalue;
                    rvalue = std::stod(tokens[i - 2]);
                    tmp = rvalue * tmp;
                } else if (l_value_type == "string") {
                    Identifier rvalue;
                    rvalue = rmv_quotes(tokens[i - 2]);
                    tmp = rvalue * tmp;
                }
            } else { // NOT chain operation case or the first pair of chain operation
                if (l_value_type == "identifier") {
                    tmp = identifiers[tokens[i - 2]] * tmp;
                } else if (l_value_type == "number") {
                    Identifier rvalue;
                    rvalue = std::stod(tokens[i - 2]);
                    tmp = rvalue * tmp;
                } else if (l_value_type == "string") {
                    Identifier rvalue;
                    rvalue = rmv_quotes(tokens[i - 2]);
                    tmp = rvalue * tmp;
                }
            }
        }
        //division
        if(i > statement.start + 1 && tokens[i - 1] == "/") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if(i < statement.end - 1 && code_tokens_types[tokens[i + 1]] == "operator") { // chain operation case
                if (l_value_type == "identifier") {
                    tmp = identifiers[tokens[i - 2]] / tmp; 
                } else if (l_value_type == "number") {
                    Identifier rvalue;
                    rvalue = std::stod(tokens[i - 2]);
                    tmp = rvalue / tmp;
                } else if (l_value_type == "string") {
                    Identifier rvalue;
                    rvalue = rmv_quotes(tokens[i - 2]);
                    tmp = rvalue / tmp;
                }
            } else { // NOT chain operation case or the first pair of chain operation
                if (l_value_type == "identifier") {
                    tmp = identifiers[tokens[i - 2]] / tmp;
                } else if (l_value_type == "number") {
                    Identifier rvalue;
                    rvalue = std::stod(tokens[i - 2]);
                    tmp = rvalue / tmp;
                } else if (l_value_type == "string") {
                    Identifier rvalue;
                    rvalue = rmv_quotes(tokens[i - 2]);
                    tmp = rvalue / tmp;
                }
            }
        }
        // operation >
        if(i > statement.start + 1 && tokens[i - 1] == ">") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if (l_value_type == "identifier") {
                tmp = identifiers[tokens[i - 2]] > tmp;
            } else if (l_value_type == "number") {
                Identifier rvalue;
                rvalue = std::stod(tokens[i - 2]);
                tmp = rvalue > tmp;
            } else if (l_value_type == "string") {
                Identifier rvalue;
                rvalue = rmv_quotes(tokens[i - 2]);
                tmp = rvalue > tmp;
            }
        }
        // operation <
        if(i > statement.start + 1 && tokens[i - 1] == "<") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if (l_value_type == "identifier") {
                tmp = identifiers[tokens[i - 2]] < tmp;
            } else if (l_value_type == "number") {
                Identifier rvalue;
                rvalue = std::stod(tokens[i - 2]);
                tmp = rvalue < tmp;
            } else if (l_value_type == "string") {
                Identifier rvalue;
                rvalue = rmv_quotes(tokens[i - 2]);
                tmp = rvalue < tmp;
            }
        }
        // operation ==
        if(i > statement.start + 1 && tokens[i - 1] == "==") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if (l_value_type == "identifier") {
                tmp = identifiers[tokens[i - 2]] == tmp;
            } else if (l_value_type == "number") {
                Identifier rvalue;
                rvalue = std::stod(tokens[i - 2]);
                tmp = rvalue == tmp;
            } else if (l_value_type == "string") {
                Identifier rvalue;
                rvalue = rmv_quotes(tokens[i - 2]);
                tmp = rvalue == tmp;
            }
        }
        // operation !=
        if(i > statement.start + 1 && tokens[i - 1] == "!=") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if (l_value_type == "identifier") {
                tmp = identifiers[tokens[i - 2]] != tmp;
            } else if (l_value_type == "number") {
                Identifier rvalue;
                rvalue = std::stod(tokens[i - 2]);
                tmp = rvalue != tmp;
            } else if (l_value_type == "string") {
                Identifier rvalue;
                rvalue = rmv_quotes(tokens[i - 2]);
                tmp = rvalue != tmp;
            }
        }
        // operation ===
        if(i > statement.start + 1 && tokens[i - 1] == "===") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if (l_value_type == "identifier") {
                tmp = identifiers[tokens[i - 2]].strict_equal(tmp);
            } else if (l_value_type == "number") {
                Identifier rvalue;
                rvalue = std::stod(tokens[i - 2]);
                tmp = rvalue.strict_equal(tmp);
            } else if (l_value_type == "string") {
                Identifier rvalue;
                rvalue = rmv_quotes(tokens[i - 2]);
                tmp = rvalue.strict_equal(tmp);
            }
        }
        // operation !==
        if(i > statement.start + 1 && tokens[i - 1] == "!==") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if (l_value_type == "identifier") {
                tmp = identifiers[tokens[i - 2]].not_strict_equal(tmp);
            } else if (l_value_type == "number") {
                Identifier rvalue;
                rvalue = std::stod(tokens[i - 2]);
                tmp = rvalue.not_strict_equal(tmp);
            } else if (l_value_type == "string") {
                Identifier rvalue;
                rvalue = rmv_quotes(tokens[i - 2]);
                tmp = rvalue.not_strict_equal(tmp);
            }
        }
        // operation >=
        if(i > statement.start + 1 && tokens[i - 1] == ">=") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if (l_value_type == "identifier") {
                tmp = identifiers[tokens[i - 2]] >= tmp;
            } else if (l_value_type == "number") {
                Identifier rvalue;
                rvalue = std::stod(tokens[i - 2]);
                tmp = rvalue >= tmp;
            } else if (l_value_type == "string") {
                Identifier rvalue;
                rvalue = rmv_quotes(tokens[i - 2]);
                tmp = rvalue >= tmp;
            }
        }
        // operation <=
        if(i > statement.start + 1 && tokens[i - 1] == "<=") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if (l_value_type == "identifier") {
                tmp = identifiers[tokens[i - 2]] <= tmp;
            } else if (l_value_type == "number") {
                Identifier rvalue;
                rvalue = std::stod(tokens[i - 2]);
                tmp = rvalue <= tmp;
            } else if (l_value_type == "string") {
                Identifier rvalue;
                rvalue = rmv_quotes(tokens[i - 2]);
                tmp = rvalue <= tmp;
            }
        }
        // operation &&
        if(i > statement.start + 1 && tokens[i - 1] == "&&") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if (l_value_type == "identifier") {
                tmp = identifiers[tokens[i - 2]] && tmp;
            } else if (l_value_type == "number") {
                Identifier rvalue;
                rvalue = std::stod(tokens[i - 2]);
                tmp = rvalue && tmp;
            } else if (l_value_type == "string") {
                Identifier rvalue;
                rvalue = rmv_quotes(tokens[i - 2]);
                tmp = rvalue && tmp;
            }
        }
        // operation ||
        if(i > statement.start + 1 && tokens[i - 1] == "||") { // the case when the token before '=' can be a constant will be prevented by error checker   
            if (l_value_type == "identifier") {
                tmp = identifiers[tokens[i - 2]] || tmp;
            } else if (l_value_type == "number") {
                Identifier rvalue;
                rvalue = std::stod(tokens[i - 2]);
                tmp = rvalue || tmp;
            } else if (l_value_type == "string") {
                Identifier rvalue;
                rvalue = rmv_quotes(tokens[i - 2]);
                tmp = rvalue || tmp;
            }
        }
        //print
        if(
            ((i > statement.start && code_tokens_types[tokens[i - 1]] != "operator") || i == statement.start)
            && ((i < statement.end - 1 && tokens[i + 1] != "=") || i == statement.end - 1)
            && code_tokens_types[tokens[i]] != "operator"
        ) {
            print_identifier(tmp);
        }
    }
}

void print_types(std::vector<std::string> tokens, std::map<std::string, std::string> code_tokens_types) {
    int i{};
    std::cin >> i;
    while (i >= 0 && i < tokens.size()) {
        std::cout << code_tokens_types[tokens[i]] << std::endl;
        std::cin >> i;
    }
}

bool check_error(
    std::string my_code, 
    std::vector<std::string>& tokens, 
    std::map<std::string, std::string>& code_tokens_types, // symetric with tokens vector
    std::map<std::string, std::vector<std::string>>& token_types, // possible types tokens can have
    std::map<std::string, Identifier>& identifiers
    ) {
    
    std::vector<int> eol;
    eol.push_back(-1);

    tokenize(my_code, tokens);
    set_types(tokens, token_types, code_tokens_types, eol);
    eol.push_back(tokens.size());

    for (int i = 0; i < eol.size() - 1; ++i)
    {   
        Statement statement;
        statement.start = eol[i] + 1;
        statement.end = eol[i + 1];

        if (code_tokens_types[tokens[statement.start]] == "operator" || code_tokens_types[tokens[statement.end - 1]] == "operator") {
            std::cout << "Error: Can't start or end statement with operator";
            std::cout << std::endl;
            return 0;
        }

        for (int j = statement.start + 1; j < statement.end - 1; ++j) {
            auto pre = code_tokens_types[tokens[j - 1]];
            auto post = code_tokens_types[tokens[j + 1]];
            auto This = code_tokens_types[tokens[j]];
            if(
                This == "operator" 
                && 
                (
                    (pre != "identifier" && pre != "number" && pre != "string") 
                    || 
                    (post != "identifier" && post != "number" && post != "string")
                )
            ) {
                std::cout << "Error: Operator must have pre and post operands";
                return 0;
            }
            if (tokens[j] == "=" && pre != "identifier") {
                std::cout << "Error: Can't assign value to a constant literal";
                return 0;
            }
        }
    }
    for (int i = 0; i < eol.size() - 1; ++i)
    {   
        Statement statement;
        statement.start = eol[i] + 1;
        statement.end = eol[i + 1];

        set_values(tokens, code_tokens_types, identifiers, statement);
    }
    return 1;
}

void run(std::string my_code) {
    std::vector<std::string> tokens;
    std::map<std::string, std::string> code_tokens_types;
    std::map<std::string, std::vector<std::string>> token_types;
    std::map<std::string, Identifier> identifiers;
    token_types["operator"] = {"+", "-", "*", "/", "=", ">", "<", "==", "!=", "===", "!==", ">=", "<=", "&&", "||"};
    if( check_error(my_code, tokens, code_tokens_types, token_types, identifiers) ) {
    }
}

int main() {
    std::string my_code;
    std::cout << "Please enter the path of the file you want to compile." << std::endl;
    std::string path = "my_code.easy";
    std::cin >> path;
    std::fstream strm;
    strm.open(path);
        if (strm.is_open()) {
            char c;
            while (!strm.eof()) {
                strm.get(c);
                if(!strm.eof()) my_code.push_back(c);
            }
            strm.close();
            if (strm.is_open())
                std::cout << "Stream could not close!" << std::endl;
        }
    run(my_code);
    return 0;
}
