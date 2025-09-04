#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <set>
#include <tchar.h>

void PrintLocaleId(unsigned value, TCHAR* buf);

int _tmain(int argc, TCHAR** argv)
{
    std::wcout << "--== Keyboard Layout Unloader ==--\n";
    std::set<unsigned> allowed;
    for (int i = 1; i < argc; i++) 
    {
        allowed.insert(_tcstoul(argv[i], nullptr, 16));
        std::wcout << "Added allowed: " << argv[i] << "\n";
    }

    auto localesCount = GetKeyboardLayoutList(0, nullptr);
    std::wcout << "Locales count = " << localesCount << "\n";
    if (localesCount == 0) exit(0);
    HKL* layouts = (HKL*)malloc(localesCount * sizeof(HKL));
    localesCount = GetKeyboardLayoutList(localesCount, layouts);
    std::wcout << "Locales loaded = " << localesCount << "\n";
    if (localesCount == 0) exit(0);

    TCHAR buffer[LOCALE_NAME_MAX_LENGTH * 2];
    auto layout = &buffer[0], locale = layout + LOCALE_NAME_MAX_LENGTH;
    for (int i = 0; i < localesCount; i++) 
    {
        auto l = (unsigned)(unsigned long long)layouts[i];
        PrintLocaleId(l >> 16, layout);
        PrintLocaleId(l & 0xFFFF, locale);
        std::wcout << L"0x" << std::setfill(L'0') << std::setw(8) << std::uppercase << std::hex << l << std::nouppercase << L" - KB layout " << layout << L" in language " << locale;
        if (allowed.size() > 0) 
        {
            std::wcout << (allowed.find(l) != allowed.end() ? L", " : L", not ") << L"allowed.";
        }
        std::wcout << "\n";
    }

    if (allowed.size() > 0)
    {
        std::wcout << L"Unloading all except those allowed in the command line...\n";

        for (int i = localesCount - 1; i >= 0; i--)
        {
            auto l = layouts[i];
            auto lx86 = (unsigned)(unsigned long long)l;
            if (allowed.find(lx86) == allowed.end())
            {
                auto x = UnloadKeyboardLayout(l);
                std::wcout << L"0x" << std::uppercase << lx86 << std::nouppercase << L" unloaded with result: " << (x ? L"OK" : L"FAILED") << L"\n";
            }
        }
    }
    else
    {
        std::wcout << L"Tool launched in read-only mode. Supply hexadecimal IDs (space-separated) in command line to whitelist (with 0x or without).\n";
    }

    std::wcout << L"--== Done ==--\n";
}

void PrintLocaleId(unsigned value, TCHAR* buf) 
{
    if (!LCIDToLocaleName(value, buf, LOCALE_NAME_MAX_LENGTH, LOCALE_ALLOW_NEUTRAL_NAMES)) wsprintf(buf, L"0x%04X", value);
}