//
// EEL_Test.cpp : Defines the entry point for the console application.
//


#include "../EasyEEL.h"
#include <iostream>

class App
{
public:
    App()  :  mVM(getThis(), {"@code", "@bling", "@blop", "@blam"}, "../../../script.eel"),
              mMemberValue(19.)
    {
        mVM.registerFunction("sayHello",       0, &App::sayHello);
        mVM.registerFunction("getMemberValue", 0, &App::getMemberValue);
        mVM.registerFunction("addTwoNumbers",  2, &App::addTwoNumbers);
        mVM.registerFunction("print",          1, &App::print);
        //mResults.SetLen(2048);
        mVM.compileFile(mResults);
        if (mResults.GetLength() > 0) std::cout << mResults.Get() << std::endl;
        double *d = mVM.registerVar("d");
        double *a = mVM.registerVar("a");
        std::cout << "Executing code handle..." << std::endl;
        mVM.executeHandle(0);
        std::cout << "d is now: " << *d << "\n";
        std::cout << "a is now: " << *a << "\n";
        int c;
        std::cin >> c;
    };

    ~App() {};

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
        App* _this = (App*)ctx;
        std::cout << "Object value is.. " << _this->mMemberValue << std::endl;
        return _this->mMemberValue;
    };

    App* getThis() { return this; }

private:
    EELVM mVM;
    WDL_FastString mResults;
    double mMemberValue;
};

int main(int arg, char** argv)
{
    App myApp;
    return 0;
};

// need these for EEL to compile
void NSEEL_HOSTSTUB_EnterMutex() { /* TODO: implement */ }

void NSEEL_HOSTSTUB_LeaveMutex() { /* TODO: implement */ }