# Keyboard Layout Unloader

## History and problem

In Microsoft Feedback Hub there are already THREE COLLECTIONS (not 3 individual issues) of various problems related to the keyboard layouts appearing in the system uncontrollably. For example,
*  "Extra Keyboard is repeatedly added after removal"
*  "Various ghost keyboard issues..."
*  "When I upgrade my ... install, EN-US keyboard is added"

Basically, you use your computer and then bang - you have suddenly another KB layout in the system.

From the user experience, this has several negative consequences:
* you get an unnecessary inconvenient additional entry in the Win-space layout switcher
* the name of the language may become confusing double-lined (by adding another English KB from different language)
* you have to do a number of clicks to remove it - that is if you can because...
* in some cases Windows (got it on 10) is able to add a layout that will confuse itself so much that it would not even be able to name it in any meaningful way, not mentioning being able to use or even remove it (e.q. "QAA-LATN-1" layout - yeah that's what you can get instead of "ENG" :) )

## Solution and usage

So I tired of fighting MS, and (as it usually happens) wrote a small tool to unload the ones I do not need automatically on the system load.
This version is improved version of that original code, and can be used in 2 ways:
1. Launch the application without a command line to list the system-wide loaded layouts.
2. Launch the application again specifying those layouts (IDs, hexadecimal, with 0x or without it, space separated) in the command line that you *want to preserve* (white-list). The tool will remove everything else.

## Notes

Windows 11 seems to have some improvements in the sense that despite there seems to always be "ghost" keyboard(s) loaded, they (normally) do not appear in the language switcher list. If they do, you can still use the tool. Also when they do, they then suddenly self-dissolve 🤯

The tool tries to do it's best to detect the locales to present the information but sometimes it won't be able to detect a layout, and in this case will just list it's ID. All blame Microsoft for not following their own documented rules (I am getting an ID for a KB layout with is literally impossible).

## Compilation

Be sure to build in 32-bit (x86)! The HKL's high 16 bits seem to be affected by something in the system that looks like a bug to me where there is a negative top 8 bits. This screws x64 version massively, so I did not bother to fix the math. x64 is compiled but may or may not work. It may unload all your keyboards FWIW. Or work flawlessly. So in short - do not compile as x64.
