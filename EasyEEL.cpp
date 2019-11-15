#include "EasyEEL.h"
#include <iostream>
#include <algorithm>
#include <cstring>



EELVM::EELVM(std::vector<const char*> sections) : m_code_names(sections)
{
    VM = NSEEL_VM_alloc(); // create virtual machine
}

EELVM::EELVM(void* this_ptr, std::vector<const char*> sections, const char* filename) : m_code_names(sections)
{
    VM = NSEEL_VM_alloc();
    NSEEL_VM_SetCustomFuncThis(VM, this_ptr);
    m_filename.Set(filename);	
}

EELVM::EELVM(std::vector<const char*> sections, const char* filename) : m_code_names(sections)
{
    VM = NSEEL_VM_alloc(); 
    m_filename.Set(filename);
}

bool EELVM::executeHandle(int h)
{
    if (m_codehandles.Exists(h))
    {
        NSEEL_code_execute(m_codehandles.Get(h));
        return true;
    } else {
        return false;
    }
}


void EELVM::registerFunction(const char* name, int min_arguments, EEL_F(NSEEL_CGEN_CALL *fptr)(void *, INT_PTR, EEL_F **))
{
    // number of arguments is exact, except with 1 which is used for 1 or 0 arguments
    min_arguments = min_arguments < 1 ? 1 : min_arguments;
    NSEEL_addfunc_varparm(name, min_arguments, NSEEL_PProc_THIS, fptr);
}

double* EELVM::registerVar(const char* name)
{
    return NSEEL_VM_regvar(VM, name);
}

void EELVM::setThis(void* t)
{
    NSEEL_VM_SetCustomFuncThis(VM, t);
}

WDL_PtrKeyedArray<NSEEL_CODEHANDLE>* EELVM::getCodeHandles()
{
    return &m_codehandles;
}

EELVM::~EELVM()
{
    // destroy all codehandles
    for (int i = 0; i < m_codehandles.GetSize(); ++i)
    {
        NSEEL_code_free(m_codehandles.Enumerate(i));
    }
    m_codehandles.DeleteAll();

    NSEEL_VM_free(VM);
}

void EELVM::SetCodeSection(const char* tok, int parsestate, const WDL_FastString &curblock, WDL_FastString &results, int lineoffs)
{
    if (parsestate < 0) return;
    NSEEL_CODEHANDLE ch = NSEEL_code_compile_ex(VM, curblock.Get(), lineoffs, NSEEL_CODE_COMPILE_FLAG_COMMONFUNCS);
    if (!ch)
    {
        char *err = NSEEL_code_getcodeerror(VM);
        if (err)
        {
            results.AppendFormatted(1024, "\nError: in %s: %s\r\n", m_code_names[parsestate], err);
        }
        return;
    }

    m_codehandles.Insert(parsestate, ch);
}

void EELVM::compileFile(WDL_FastString &results)
{
    // this bit totamostly snagged from Cockos OsciiBot's EEL code
    const char* mode = "r";
    
    FILE *fp = fopen(m_filename.Get(), mode);
    if (!fp)
    {
        results.AppendFormatted(1024, "\tError: fopen() - Failed opening file: %s\r\n", m_filename.Get());
        return;
    }

    int size_of_mcn = m_code_names.size();
    
    bool comment_state = false;
    int parsestate = -1, cursec_lineoffs = 0, lineoffs = 0;
    WDL_FastString curblock;
    for (;;)
    {
        char linebuf[8192];
        linebuf[0] = 0;
        fgets(linebuf, sizeof(linebuf), fp);
        lineoffs++;

        if (!linebuf[0]) break;

        char *p = linebuf;
        while (*p) p++;
        p--;
        while (p >= linebuf && (*p == '\r' || *p == '\n')) { *p = 0; p--; }
        
        LineParser lp(comment_state);
        if (linebuf[0] && !lp.parse(linebuf) && lp.getnumtokens() > 0 && lp.gettoken_str(0)[0] == '@')
        {
            const char *tok = lp.gettoken_str(0);
            int x;
            for (x = 0; x < size_of_mcn && strcmp(tok, m_code_names[x]); x++);

            if (x < size_of_mcn)
            {
                SetCodeSection(tok, parsestate, curblock, results, cursec_lineoffs);
                parsestate = x;
                cursec_lineoffs = lineoffs;
                curblock.Set("");
            } else {
                results.AppendFormatted(1024, "\tWarning: Unknown directive: %s\r\n", tok);
            }
        } else {
            const char *p = linebuf;
            if (parsestate == -3)
            {
                while (*p)
                {
                    if (p[0] == '*' && p[1] == '/')
                    {
                        parsestate = -1; // end of comment!
                        p += 2;
                        break;
                    }
                }
            }
            if (parsestate == -1 && p[0])
            {
                while (*p == ' ' || *p == '\t') p++;
                if (!*p || (p[0] == '/' && p[1] == '/'))
                {
                } else {
                    if (*p == '/' && p[1] == '*')
                    {
                        parsestate = -3;
                    } else {
                        results.AppendFormatted(1024, "\tWarning: line '%.100s' (and possibly more)' are not in valid section and may be ignored\r\n", linebuf);
                        parsestate = -2;
                    }
                }
            }
            if (parsestate >= 0)
            {
                curblock.Append(linebuf);
                curblock.Append("\n");
            }
        }
    }
    const char* tok = "end";
    SetCodeSection(tok, parsestate, curblock, results, cursec_lineoffs);
    fclose(fp);
}