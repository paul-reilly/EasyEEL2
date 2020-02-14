//
// EEL_Test.cpp : Defines the entry point for the console application.
//


#include "../src/EasyEEL.h"
#include <iostream> 

#include "WDL/wdltypes.h"
#include "WDL/ptrlist.h"

class App 
{
public: 
    App() 
        : _VM(getThis(), {"@code", "@bling", "@blop", "@blam"}, "script.eel")
        , _member_value(19.)
    {
        _VM.registerFunction("sayHello",       0, &App::sayHello);
        _VM.registerFunction("getMemberValue", 0, &App::getMemberValue);
        _VM.registerFunction("addTwoNumbers",  2, &App::addTwoNumbers);
        _VM.registerFunction("print",          1, &App::print);
        d = _VM.registerVar("d");
        a = _VM.registerVar("a");
        compileAndExecute();
    };

    ~App() {};

    void getUserInput(WDL_FastString& results)
    {
        char input;
        std::cout << "Enter (Q) to quit, any other key to recompile\n";
        std::cin >> input;
        if ((char)tolower(input) != 'q') { 
            results.Set(""); 
            compileAndExecute(); 
        }
    }

    void compileAndExecute()
    {
        _VM.compileFile(_results);
        if (_results.GetLength() == 0) {
            std::cout << "Executing code handle..." << std::endl;
            _VM.executeHandle(0);
            std::cout << "d is now: " << *d << "\n";
            std::cout << "a is now: " << *a << "\n";
        } else {
            std::cout << _results.Get() << std::endl; 
        }
        getUserInput(_results);
     }

    static EEL_F NSEEL_CGEN_CALL print(void *ctx, INT_PTR nop, EEL_F **params)
    {
        std::cout << params[0][0] << std::endl;
        return 1.;
    }

    static EEL_F NSEEL_CGEN_CALL addTwoNumbers(void *ctx, INT_PTR nop, EEL_F **params)
    {
        return params[0][0] + params[1][0];
    }

    static EEL_F NSEEL_CGEN_CALL sayHello(void *ctx, INT_PTR nop, EEL_F **params)
    {
        std::cout << "Hello from EEL calling C++ static method" << std::endl;
        return 5.0;
    }

    static EEL_F NSEEL_CGEN_CALL getMemberValue(void *ctx, INT_PTR nop, EEL_F **params)
    {
        App* this_ = (App*)ctx;
        std::cout << "Object value is.. " << this_->_member_value << std::endl;
        return this_->_member_value;
    };

    App* getThis() { return this; }

private:
    EELVM _VM;
    WDL_FastString _results;
    double _member_value;
    double* d, *a;
};


int main(int arg, char** argv)
{
    App my_app;
    return 0;
};

// need these for EEL to compile
void NSEEL_HOSTSTUB_EnterMutex() { /* TODO: implement */ }

void NSEEL_HOSTSTUB_LeaveMutex() { /* TODO: implement */ }
