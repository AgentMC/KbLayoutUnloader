#include <iostream>
#include <Windows.h>
#include <set>
#include <tchar.h>

void PrintLocaleId(unsigned value, TCHAR* buf);

int _tmain(int argc, TCHAR** argv)
{
    std::wcout << "--== Keyboard Layout Unloader ==--\n";
    std::set<int> allowed;
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
        auto l = layouts[i];
        PrintLocaleId((unsigned)l >> 16, layout);
        PrintLocaleId((unsigned)l & 0xFFFF, locale);
        if (allowed.size() > 0) 
        {
            std::wcout << std::hex << "0x" << l << " - KB layout " << layout << " in language " << locale << (allowed.find((int)l) != allowed.end() ? ", " : ", not ") << "allowed.\n";
        }
        else
        {
            std::wcout << std::hex << "0x" << l << " - KB layout " << layout << " in language " << locale << "\n";
        }
    }

    if (allowed.size() > 0)
    {
        std::wcout << "Unloading not allowed...\n";

        for (int i = localesCount - 1; i >= 0; i--)
        {
            auto l = layouts[i];
            if (allowed.find((int)l) == allowed.end())
            {
                auto x = UnloadKeyboardLayout(l);
                std::wcout << std::hex << "0x" << l << " unloaded with result: " << (x ? "OK" : "FAILED") << "\n";
            }
        }
    }
    else
    {
        std::wcout << "Tool launched in read-only mode. Supply hexadecimal IDs in command line to whitelist (with 0x or without).\n";
    }

    std::wcout << "Done.";
}

void PrintLocaleId(unsigned value, TCHAR* buf) 
{
    if (!LCIDToLocaleName(value, buf, LOCALE_NAME_MAX_LENGTH, LOCALE_ALLOW_NEUTRAL_NAMES)) wsprintf(buf, L"0x%04X", value);
}

//bool Allowed(HKL layout)
//{
//    return (int)layout == 0x04091809 || (int)layout == 0xf0a80422;
//}