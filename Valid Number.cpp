// Validate if a given string is numeric.

// Some examples:
// "0" => true
// " 0.1 " => true
// "abc" => false
// "1 a" => false
// "2e10" => true
// Note: It is intended for the problem statement to be ambiguous. You should gather all requirements up front before implementing one.
// Author: Zihang Zou.

class Solution {
public:
    const int IS_ANY        = -100;
    const int IS_INVALID    = -1;
    const int IS_ZERO       = 0;
    const int IS_DIGIT      = 1;
    
    const int IS_EXPO       = 2;
    const int IS_DOT        = 3;
    const int IS_SPACE      = 4;
    const int IS_OPERATOR   = 5;
    
    const int IS_END        = 6;
    
    const int CHECK_BEGIN   = 0;
    const int CHECK_END     = 1;
    
    bool isNumber(const char *s) 
    {
        int index = 0;
        
        bool hasZero     = false;
        bool hasDigit    = false;
        bool hasDot      = false;
        bool hasExpo     = false;
        
        while(s[index] != '\0')
        {
            int current = isValidInput(s[index]);
            if(current == IS_INVALID)
            {
                return false;
            }
            else if(current ==  IS_ZERO)
            {
                hasZero = true;
            }
            else if(current ==  IS_DIGIT)
            {
                hasDigit = true;
            }
            else if(current ==  IS_EXPO)
            {                
                if(hasExpo)
                    return false;
                
                if(!hasDigit && !hasZero)
                    return false;
                
                hasExpo = true;
            }
            else if(current ==  IS_DOT)
            {
                if(hasDot || hasExpo)
                    return false;
                    
                hasDot = true;
            }
            else if(current == IS_SPACE)
            {
                int next = isValidInput(s[index+1]);
                if(next != IS_END && next != IS_SPACE)
                {
                    if(hasDigit || hasZero)
                        return false;
                }
            }
            
            if(adjacentRuleCheck(IS_EXPO,       IS_SPACE,       IS_ANY,         s, index)   // e_x
            || adjacentRuleCheck(IS_OPERATOR,   IS_SPACE,       IS_ANY,         s, index)   // +_x
            || adjacentRuleCheck(IS_ZERO,       IS_SPACE,       IS_ZERO,        s, index)   // 0_0
            || adjacentRuleCheck(IS_ZERO,       IS_SPACE,       IS_DIGIT,       s, index)   // 0_5
            || adjacentRuleCheck(IS_DIGIT,      IS_SPACE,       IS_ZERO,        s, index)   // 5_0
            || adjacentRuleCheck(IS_DIGIT,      IS_SPACE,       IS_DIGIT,       s, index)   // 5_5
            || adjacentRuleCheck(IS_DIGIT,      IS_SPACE,       IS_DOT,         s, index)   // 5_.
            || adjacentRuleCheck(IS_ZERO,       IS_SPACE,       IS_DOT,         s, index)   // 0_.
            || adjacentRuleCheck(IS_DOT,        IS_SPACE,       IS_DIGIT,       s, index)   // ._5
            || adjacentRuleCheck(IS_DOT,        IS_SPACE,       IS_ZERO,        s, index)   // ._0
            || adjacentRuleCheck(IS_DOT,        IS_SPACE,       IS_EXPO,        s, index)   // ._e
            || adjacentRuleCheck(IS_DOT,        IS_SPACE,       IS_OPERATOR,    s, index)   // ._+
            
            || adjacentRuleCheck(IS_ANY,        IS_EXPO,        IS_DOT,         s, index)   // xe.
            || adjacentRuleCheck(IS_ANY,        IS_EXPO,        IS_SPACE,       s, index)   // xe_
            
            || adjacentRuleCheck(IS_ZERO,       IS_OPERATOR,    IS_ANY,         s, index)   // 0+x
            || adjacentRuleCheck(IS_DIGIT,      IS_OPERATOR,    IS_ANY,         s, index)   // 9+x
            || adjacentRuleCheck(IS_DOT,        IS_OPERATOR,    IS_ZERO,        s, index)   // .+0
            || adjacentRuleCheck(IS_DOT,        IS_OPERATOR,    IS_DIGIT,       s, index)   // .+9
            || adjacentRuleCheck(IS_DOT,        IS_OPERATOR,    IS_DOT,         s, index)   // .+.
            || adjacentRuleCheck(IS_DOT,        IS_OPERATOR,    IS_SPACE,       s, index)   // .+_
            
            || adjacentRuleCheck(IS_EXPO,       IS_DOT,         IS_ANY,         s, index)   // e.x
            || adjacentRuleCheck(IS_SPACE,      IS_DOT,         IS_SPACE,       s, index)   // _._
            || adjacentRuleCheck(IS_SPACE,      IS_DOT,         IS_EXPO,        s, index)   // _.e
            || adjacentRuleCheck(IS_SPACE,      IS_DOT,         IS_OPERATOR,    s, index)   // _.+
            || adjacentRuleCheck(IS_DIGIT,      IS_DOT,         IS_OPERATOR,    s, index)   // 5.+
            || adjacentRuleCheck(IS_ZERO,       IS_DOT,         IS_OPERATOR,    s, index)   // 0.+
            
            || adjacentRuleCheck(IS_DOT,        IS_DIGIT,       IS_OPERATOR,    s, index)   // .8+
            || adjacentRuleCheck(IS_SPACE,      IS_DIGIT,       IS_OPERATOR,    s, index)   // _8+
            || adjacentRuleCheck(IS_OPERATOR,   IS_DIGIT,       IS_OPERATOR,    s, index)   // +8+
            
            || adjacentRuleCheck(IS_DOT,        IS_ZERO,        IS_OPERATOR,    s, index)   // .0+
            || adjacentRuleCheck(IS_SPACE,      IS_ZERO,        IS_OPERATOR,    s, index)   // _+
            || adjacentRuleCheck(IS_OPERATOR,   IS_ZERO,        IS_OPERATOR,    s, index)   // +0+
            
            
            || boundaryRuleCheck(IS_EXPO,       IS_ANY,         CHECK_BEGIN,    s, index)   // ex
            || boundaryRuleCheck(IS_ANY,        IS_EXPO,        CHECK_END,      s, index)   // xe
            || boundaryRuleCheck(IS_ANY,        IS_OPERATOR,    CHECK_END,      s, index)   // xe
            )
            {
                return false;
            }
            
            index++;
        } 
        
        if(!hasDigit && !hasZero)
        {
            return false;
        }
        
        return true;
    }
    
    bool adjacentRuleCheck(int previous, int current, int next, const char* s, int index)
    {
        if(index < 1 || isValidInput(s[index+1]) == IS_END)
        {
            return false;
        }
        
        int left  = isValidInput(s[index-1]);;
        int right = isValidInput(s[index+1]);
        int mid   = isValidInput(s[index]);
        
        if(previous != IS_ANY && next != IS_ANY)
        {
            if(left == previous && right == next && mid == current)
            {
                return true;
            }
        }
        else if(previous == IS_ANY && next != IS_ANY)
        {
            if(mid == current && right == next)
            {
                return true;
            }
        }
        else if(previous != IS_ANY && next == IS_ANY)
        {
            if(mid == current && left == previous)
            {
                return true;
            }
        }
        
        return false;
    }
    
    bool boundaryRuleCheck(int previous, int current, int checkBoundary, const char* s, int index)
    {
        if((checkBoundary == CHECK_BEGIN && index == 1) || (checkBoundary == CHECK_END && isValidInput(s[index+1]) == IS_END))
        {
            int left = isValidInput(s[index-1]);
            int mid  = isValidInput(s[index]);
        
            if(previous != IS_ANY && current != IS_ANY)
            {
                if(left == previous && mid == current)
                {
                    return true;
                }
            }
            else if(previous == IS_ANY)
            {
                if(mid == current)
                {
                    return true;
                }
            }
            else if(current == IS_ANY)
            {
                if(left == previous)
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    int isValidInput(const char & c)
    {
        switch(c)
            {
                case '0':
                    return IS_ZERO;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    return IS_DIGIT;
                case 'e':
                case 'E':
                    return IS_EXPO;
                case '.':
                    return IS_DOT;
                case ' ':
                    return IS_SPACE;
                case '-':
                case '+':
                    return IS_OPERATOR;
                case '\0':
                    return IS_END;
                default:
                    return IS_INVALID;
            }
    }
};