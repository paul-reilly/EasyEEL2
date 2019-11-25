#pragma once
/*--------------------------------------------------------------------------
EELVM.h

    Handy class for instances of EEL VMs
    EEL2 is part of the WDL framework by Cockos Inc

    Example use:

    EELVM evm(this, {"@init", "@process"}, "filename.ext");
    evm.compileFile();

    ... where @init and @process are sections of code within the file which
    will be compiled and which can then be called independently.

    Now if the sections, @init and @process compiled correctly, they
    will be stored in a zero based index. So to execute @process...

    evm.executeHandle(1);


---------------------------------------------------------------------------*/
#ifndef _WIN32
 #include "WDL/SWELL/swell-types.h"
#endif
//#include "WDL/wdltypes.h"
#include "WDL/ptrlist.h"
#include "WDL/eel2/ns-eel.h"
//#include "WDL/eel2/ns-eel-addfuncs.h"
#include "WDL/wdlcstring.h"
#include "WDL/dirscan.h"
#include "WDL/queue.h"
#include "WDL/assocarray.h"
#include <string>
#include <istream>
#include <vector>
#include <map>

#define EELVM_MAX_ALG_LENGTH 65536
#define EEL_VALIDATE_FSTUBS

class eel_string_context_state;

class EELVM
{
public:
    EELVM(std::vector<const char*> sections);
    EELVM(void* this_ptr, std::vector<const char*> sections, const char* filename);
    EELVM(std::vector<const char*> sections, const char* filename);
    EELVM(EELVM &other) = delete;
    EELVM(EELVM &&other) = delete;
    EELVM& operator=(EELVM& rhs) = delete;
    EELVM& operator=(EELVM&& rhs) = delete;
    ~EELVM();

    // fptr is pointer to function that must have these exact number and types of arguments:   v        v         v
    void registerFunction(const char* name, int min_arguments, EEL_F(NSEEL_CGEN_CALL *fptr)(void *, INT_PTR, EEL_F **));
    double* registerVar(const char* name); // returns double* that can be manipulated from inside and outside the VM
    void setThis(void* t); // for setting object reference for calling methods from EEL
    WDL_PtrKeyedArray<NSEEL_CODEHANDLE>* getCodeHandles();
    NSEEL_VMCTX& getVM() { return VM; };
    bool compileStream(std::istream &stream, WDL_FastString &results);
    bool compileFile(const std::string &filename, WDL_FastString &results);
    bool compileFile(WDL_FastString &results);
    bool executeHandle(int h);
    bool executeHandle(std::string section_name);
    eel_string_context_state* _eel_str_ctx;
    int getCodeHandlesSize() { return _codehandles.GetSize(); }

    
private:
    bool SetCodeSection(std::string tok, int parsestate, const WDL_FastString &curblock, WDL_FastString &results, int lineoffs);
    NSEEL_VMCTX VM;
    WDL_PtrKeyedArray<NSEEL_CODEHANDLE> _codehandles;
    WDL_FastString _filename;
    std::vector<const char*> _section_names;
    std::map<std::string, NSEEL_CODEHANDLE> _handle_map;
};
