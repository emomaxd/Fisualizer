#pragma once

#include <functional>
#include <string>



class Parser {
public:
    Parser() = delete;
    static std::function<float(float)> Parse(const std::string& function) {
        auto expression = [function](float x) -> float {

            std::string str = "";

            if (function.at(0) != '+' && function.at(0) != '-')
                str += '+';


            for (char c : function) {
                if (!std::isspace(c)) {
                    str += c;
                }
            }

            for (size_t i = 0; i < str.length(); ++i) {
                if (str[i] == '+' || str[i] == '-') {
                    if (i + 1 == str.length() || !std::isdigit(str[i + 1])) {
                        str.insert(i + 1, "1");
                    }
                }
            }
            float result = 0.0f;
            size_t pos = 0;

            

            while (pos < str.size()) {
                
                char op = '+';
                
                

                str = str.substr(pos);
                size_t next_op = str.find_first_of("+-", 1);
                

                std::string term = str.substr(0, next_op);
                op = term[0];

                size_t x_pos = term.find('x');

                if (x_pos != std::string::npos) {
                    float coefficient = (x_pos == 0) ? 1.0f : str[x_pos - 1] - 48;

                    int exponent = 1;

                    if(x_pos+1 < term.size())
                        if (term.at(x_pos + 1) == '^') {
                            exponent = term.at(x_pos + 2) - 48;
                        }

                    float term_result = coefficient * std::pow(x, exponent);

                    if (op == '-') {
                        term_result = -term_result;
                    }

                    result += term_result;

                }
                else {
                    float constant = term[1] - 48;

                    if (op == '-') {
                        constant = -constant;
                    }

                    result += constant;
                    break;
                }

                pos = next_op;

                

            }

            return result;
        };

        return expression;
    }
    
};