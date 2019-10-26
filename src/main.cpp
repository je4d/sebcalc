#include <iostream>
#include <iomanip>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

double to_number(std::string&& str)
{
    boost::trim(str);
    return boost::lexical_cast<double>(str);
}

double eval_mult(std::string&& expr)
{
    while (true)
    {
        auto op_pos = expr.find_first_of("*/");
        if (op_pos == std::string::npos)
            break;
        auto next_op_pos = expr.find_first_of("*/", op_pos+1);
        if (next_op_pos == std::string::npos)
            next_op_pos = expr.size();
        auto lhs = to_number(expr.substr(0, op_pos));
        auto rhs = to_number(expr.substr(op_pos+1, next_op_pos-(op_pos+1)));
        double result;
        switch(expr[op_pos])
        {
            case '*':
                result = lhs * rhs;
                break;
            case '/':
                result = lhs / rhs;
                break;
            default:
                throw "unexpected multiplicative operator";
        }
        expr.replace(0, next_op_pos,
                     boost::lexical_cast<std::string>(result));
    }
    return to_number(std::move(expr));
}

double eval_arith(std::string expr)
{
    while (true)
    {
        auto op_pos = expr.find_first_of("+-");
        if (op_pos == std::string::npos)
            break;
        auto next_op_pos = expr.find_first_of("+-", op_pos+1);
        if (next_op_pos == std::string::npos)
            next_op_pos = expr.size();
        auto lhs = eval_mult(expr.substr(0, op_pos));
        auto rhs = eval_mult(expr.substr(op_pos+1, next_op_pos-(op_pos+1)));
        double result;
        switch(expr[op_pos])
        {
            case '+':
                result = lhs + rhs;
                break;
            case '-':
                result = lhs - rhs;
                break;
            default:
                throw "unexpected arithmetic operator";
        }
        expr.replace(0, next_op_pos, boost::lexical_cast<std::string>(result));
    }
    return eval_mult(std::move(expr));
}

double eval_expr(std::string&& expr)
{
    while (true)
    {
        std::string::size_type rparen_pos = expr.find(')');
        if (rparen_pos == std::string::npos)
            break;
        auto lparen_pos = expr.rfind('(', rparen_pos);
        if (lparen_pos == std::string::npos)
            throw "mismatched parens";
        auto paren_val = eval_arith(expr.substr(lparen_pos+1, rparen_pos-(lparen_pos+1)));
        expr.replace(lparen_pos, rparen_pos+1-lparen_pos, boost::lexical_cast<std::string>(paren_val));
    }
    return eval_arith(std::move(expr));
}

int main()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        try {
            std::cout << std::setw(7) << eval_expr(std::move(line)) << std::endl;
        }
        catch(const char* error)
        {
            std::cout << "error: " << error << std::endl;
        }
    }
}
