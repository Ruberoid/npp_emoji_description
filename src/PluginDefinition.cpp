//this file is part of notepad++
//Copyright (C)2022 Don HO <don.h@free.fr>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"
#include <windows.h>
#include <tchar.h>
#include <string>
#include <sstream>
#include <iomanip>

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Plugin state
//
static bool g_showCharInfo = true;

//
// Initialize your plugin data here
// It will be called while plugin loading
void pluginInit(HANDLE /*hModule*/)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{
    setCommand(0, TEXT("Show Character Info"), toggleShowCharInfo, NULL, true);
    setCommand(1, TEXT("About"), aboutDialog, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
}

//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit)
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//
// Decode UTF-8 character at given position
//
uint32_t decodeUtf8Char(const unsigned char* text, int& bytesRead)
{
    bytesRead = 0;
    if (!text || !text[0])
        return 0;

    unsigned char firstByte = text[0];

    // 1-byte character (ASCII)
    if ((firstByte & 0x80) == 0) {
        bytesRead = 1;
        return firstByte;
    }

    // 2-byte character
    if ((firstByte & 0xE0) == 0xC0) {
        if ((text[1] & 0xC0) != 0x80)
            return 0;
        bytesRead = 2;
        return ((firstByte & 0x1F) << 6) | (text[1] & 0x3F);
    }

    // 3-byte character
    if ((firstByte & 0xF0) == 0xE0) {
        if ((text[1] & 0xC0) != 0x80 || (text[2] & 0xC0) != 0x80)
            return 0;
        bytesRead = 3;
        return ((firstByte & 0x0F) << 12) | ((text[1] & 0x3F) << 6) | (text[2] & 0x3F);
    }

    // 4-byte character (includes emoji)
    if ((firstByte & 0xF8) == 0xF0) {
        if ((text[1] & 0xC0) != 0x80 || (text[2] & 0xC0) != 0x80 || (text[3] & 0xC0) != 0x80)
            return 0;
        bytesRead = 4;
        return ((firstByte & 0x07) << 18) | ((text[1] & 0x3F) << 12) |
               ((text[2] & 0x3F) << 6) | (text[3] & 0x3F);
    }

    return 0;
}

//
// Format character codes for display
//
void formatCharacterCodes(uint32_t codepoint, TCHAR* output, size_t outputSize)
{
    if (!codepoint) {
        _sntprintf_s(output, outputSize, _TRUNCATE, TEXT("No character"));
        return;
    }

    std::wstringstream ss;

    // Unicode code point
    ss << L"U+" << std::uppercase << std::hex << std::setfill(L'0') << std::setw(4) << codepoint;

    // Decimal
    ss << L" | Dec: " << std::dec << codepoint;

    // Hexadecimal
    ss << L" | Hex: 0x" << std::uppercase << std::hex << codepoint;

    // HTML entity
    ss << L" | HTML: &#" << std::dec << codepoint << L";";

    // UTF-8 bytes
    ss << L" | UTF-8: ";
    if (codepoint < 0x80) {
        ss << L"0x" << std::uppercase << std::hex << std::setfill(L'0') << std::setw(2) << codepoint;
    }
    else if (codepoint < 0x800) {
        unsigned char b1 = static_cast<unsigned char>(0xC0 | (codepoint >> 6));
        unsigned char b2 = static_cast<unsigned char>(0x80 | (codepoint & 0x3F));
        ss << L"0x" << std::uppercase << std::hex << std::setfill(L'0') << std::setw(2) << (int)b1
           << L" 0x" << std::setw(2) << (int)b2;
    }
    else if (codepoint < 0x10000) {
        unsigned char b1 = static_cast<unsigned char>(0xE0 | (codepoint >> 12));
        unsigned char b2 = static_cast<unsigned char>(0x80 | ((codepoint >> 6) & 0x3F));
        unsigned char b3 = static_cast<unsigned char>(0x80 | (codepoint & 0x3F));
        ss << L"0x" << std::uppercase << std::hex << std::setfill(L'0') << std::setw(2) << (int)b1
           << L" 0x" << std::setw(2) << (int)b2
           << L" 0x" << std::setw(2) << (int)b3;
    }
    else {
        unsigned char b1 = static_cast<unsigned char>(0xF0 | (codepoint >> 18));
        unsigned char b2 = static_cast<unsigned char>(0x80 | ((codepoint >> 12) & 0x3F));
        unsigned char b3 = static_cast<unsigned char>(0x80 | ((codepoint >> 6) & 0x3F));
        unsigned char b4 = static_cast<unsigned char>(0x80 | (codepoint & 0x3F));
        ss << L"0x" << std::uppercase << std::hex << std::setfill(L'0') << std::setw(2) << (int)b1
           << L" 0x" << std::setw(2) << (int)b2
           << L" 0x" << std::setw(2) << (int)b3
           << L" 0x" << std::setw(2) << (int)b4;
    }

    std::wstring result = ss.str();
    _tcsncpy_s(output, outputSize, result.c_str(), _TRUNCATE);
}

//
// Update character information in status bar
//
void updateCharacterInfo()
{
    if (!g_showCharInfo)
        return;

    // Get current scintilla handle
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;

    HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

    // Get current cursor position
    Sci_Position pos = ::SendMessage(curScintilla, SCI_GETCURRENTPOS, 0, 0);

    // Get document length to check bounds
    Sci_Position docLength = ::SendMessage(curScintilla, SCI_GETLENGTH, 0, 0);
    if (pos >= docLength) {
        ::SendMessage(nppData._nppHandle, NPPM_SETSTATUSBAR, STATUSBAR_DOC_TYPE, (LPARAM)TEXT("End of document"));
        return;
    }

    // Get text range (read up to 4 bytes for UTF-8)
    unsigned char buffer[5] = {0};
    Sci_TextRangeFull tr;
    tr.chrg.cpMin = pos;
    tr.chrg.cpMax = pos + 4;
    if (tr.chrg.cpMax > docLength)
        tr.chrg.cpMax = docLength;
    tr.lpstrText = (char*)buffer;

    ::SendMessage(curScintilla, SCI_GETTEXTRANGEFULL, 0, (LPARAM)&tr);

    // Decode UTF-8 character
    int bytesRead = 0;
    uint32_t codepoint = decodeUtf8Char(buffer, bytesRead);

    // Format output
    TCHAR statusText[512];
    formatCharacterCodes(codepoint, statusText, 512);

    // Display in status bar
    ::SendMessage(nppData._nppHandle, NPPM_SETSTATUSBAR, STATUSBAR_DOC_TYPE, (LPARAM)statusText);
}

//
// Notification handler
//
void pluginBeNotified(SCNotification *notifyCode)
{
    switch (notifyCode->nmhdr.code)
    {
        case SCN_UPDATEUI:
            // Update when cursor moves or selection changes
            if (notifyCode->updated & (SC_UPDATE_SELECTION | SC_UPDATE_CONTENT))
            {
                updateCharacterInfo();
            }
            break;

        case NPPN_BUFFERACTIVATED:
            // Update when switching between documents
            updateCharacterInfo();
            break;
    }
}

//
// Message handler
//
LRESULT pluginMessageProc(UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return TRUE;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//

void toggleShowCharInfo()
{
    g_showCharInfo = !g_showCharInfo;

    // Update menu check state
    ::SendMessage(nppData._nppHandle, NPPM_SETMENUITEMCHECK, funcItem[0]._cmdID, g_showCharInfo);

    if (g_showCharInfo) {
        updateCharacterInfo();
    } else {
        ::SendMessage(nppData._nppHandle, NPPM_SETSTATUSBAR, STATUSBAR_DOC_TYPE, (LPARAM)TEXT(""));
    }
}

void aboutDialog()
{
    ::MessageBox(NULL,
        TEXT("Emoji Description v0.1.0\n\n")
        TEXT("Displays detailed character encoding information in the status bar.\n\n")
        TEXT("Shows for any character:\n")
        TEXT("- Unicode code point (U+XXXX)\n")
        TEXT("- Decimal and Hexadecimal values\n")
        TEXT("- HTML entity (&#XXXX;)\n")
        TEXT("- UTF-8 byte sequence\n\n")
        TEXT("Supports all Unicode characters including emoji!\n\n")
        TEXT("https://github.com/Ruberoid/npp_emoji_description"),
        TEXT("About Emoji Description"),
        MB_OK | MB_ICONINFORMATION);
}
