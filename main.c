#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <comdef.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dlfcn.h>
typedef void *FARPROC;
typedef void *HINSTANCE;
typedef void *HWND;
typedef void *VARIANT;
#define __stdcall
#endif

#ifndef _WIN32
static void *GetProcAddress(HINSTANCE TestDLL, const char *method)
{
    return dlsym(TestDLL, method);
}
#endif

#define tstfn

// Names:
// V = void
// B = bool
// b = bool *
// I = int
// i = int *
// S = char *
// C = const char *
// H = Handle

typedef void(__stdcall *TestMethod_V)();
typedef void(__stdcall *TestMethod_V_I)(int);

static HINSTANCE TestDLL;

tstfn void TestPrint()
{
    static TestMethod_V method = NULL;
    if (method == NULL)
        method = (TestMethod_V)GetProcAddress(TestDLL, "Print");
    if (method == NULL)
    {
        printf("Method %s not found", "Print");
    }
    method();
}

tstfn void TestPrintNum(int x)
{
    static TestMethod_V_I method = NULL;
    if (method == NULL)
        method = (TestMethod_V_I)GetProcAddress(TestDLL, "PrintNum");
    if (method == NULL)
    {
        printf("Method %s not found", "PrintNum");
    }
    method(x);
}

// Try to load the Test DLL.
// Returns true if loaded ok.
bool tstfn TestLoadDLL(const char *fileName)
{
#if _WIN32
    TestDLL = LoadLibraryA(fileName);
#else
    TestDLL = dlopen(fileName, RTLD_NOW);
#endif

    if (TestDLL == NULL)
    {
        char *error = dlerror();
        fprintf(stderr, "Error: %s\r\n", error);
        return false;
    }

    return true;
}

// Unload the Test DLL.
void tstfn TestUnloadDLL()
{
    if (TestDLL != NULL)
    {
#if _WIN32
        FreeLibrary(TestDLL);
#else
        dlclose(TestDLL);
#endif
    }

    TestDLL = NULL;
}

int main()
{
    bool res = TestLoadDLL("./libprint.so");
    printf("Load: %d\n\n", res);
    TestPrint();
    TestPrintNum(20);
    TestUnloadDLL();
    printf("Change library, compile, press any key to continue: ");
    getchar();
    res = TestLoadDLL("./libprint.so");
    printf("Load: %d\n\n", res);
    TestPrint();
    TestPrintNum(20);
    TestUnloadDLL();
    return 0;
}