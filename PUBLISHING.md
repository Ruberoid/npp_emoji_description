# Publishing Emoji Description to Notepad++ Plugin Admin

This guide will help you publish the Emoji Description plugin to the official Notepad++ Plugin Admin repository.

## Prerequisites

- GitHub account
- Git installed
- Plugin binaries built for both x86 and x64 architectures
- Repository: https://github.com/Ruberoid/npp_emoji_description

## Step 1: Create GitHub Repository

1. Go to https://github.com/new
2. Create repository: `npp_emoji_description`
3. Set visibility to **Public**
4. Don't initialize with README (we already have one)

## Step 2: Push Code to GitHub

```bash
cd c:\work\github\npp_showchar\EmojiCodePlugin

# Initialize git (if not already done)
git init

# Add all files
git add .

# Commit
git commit -m "Initial release v0.1.0

- Display character encoding information in status bar
- Support for all Unicode characters including emoji
- Show Unicode code point, decimal, hex, HTML entity, UTF-8 bytes"

# Add remote
git remote add origin https://github.com/Ruberoid/npp_emoji_description.git

# Push to GitHub
git branch -M master
git push -u origin master
```

## Step 3: Create Release with Binaries

1. Go to your repository: https://github.com/Ruberoid/npp_emoji_description

2. Click on "Releases" → "Create a new release"

3. Set tag: `v0.1.0`

4. Set title: `Emoji Description v0.1.0`

5. Description:
```markdown
## Emoji Description v0.1.0

First release of Emoji Description plugin for Notepad++.

### Features
- Displays detailed character encoding information in the status bar
- Shows Unicode code point (U+XXXX)
- Shows decimal and hexadecimal values
- Shows HTML entity (&#XXXX;)
- Shows UTF-8 byte sequence
- Supports all Unicode characters including emoji

### Installation
Download the appropriate ZIP file for your Notepad++ version:
- `EmojiDescription_x64_v0.1.0.zip` for 64-bit Notepad++
- `EmojiDescription_x86_v0.1.0.zip` for 32-bit Notepad++

Extract the DLL to `%APPDATA%\Notepad++\plugins\EmojiDescription\` and restart Notepad++.
```

6. Attach binaries:
   - Create `EmojiDescription_x64_v0.1.0.zip` containing `bin64\EmojiDescription.dll`
   - Create `EmojiDescription_x86_v0.1.0.zip` containing `bin\EmojiDescription.dll`

## Step 4: Prepare Plugin List Entry

Create a JSON file with plugin information for submission:

```json
{
    "folder-name": "EmojiDescription",
    "display-name": "Emoji Description",
    "version": "0.1.0",
    "id": "UNIQUE_GUID_HERE",
    "repository": "https://github.com/Ruberoid/npp_emoji_description",
    "description": "Displays detailed character encoding information in the status bar. Shows Unicode code point, decimal/hexadecimal values, HTML entity, and UTF-8 byte sequence for any character including emoji.",
    "author": "Ruberoid",
    "homepage": "https://github.com/Ruberoid/npp_emoji_description",
    "x64-download": "https://github.com/Ruberoid/npp_emoji_description/releases/download/v0.1.0/EmojiDescription_x64_v0.1.0.zip",
    "x86-download": "https://github.com/Ruberoid/npp_emoji_description/releases/download/v0.1.0/EmojiDescription_x86_v0.1.0.zip"
}
```

**Note:** Generate a GUID using PowerShell:
```powershell
[guid]::NewGuid()
```

## Step 5: Submit to Plugin List

1. Fork the official plugin list repository:
   https://github.com/notepad-plus-plus/nppPluginList

2. Clone your fork:
```bash
git clone https://github.com/YOUR_USERNAME/nppPluginList.git
cd nppPluginList
```

3. Create a new branch:
```bash
git checkout -b add-emoji-description
```

4. Add your plugin entry to `src/pl.x64.json` (for 64-bit):
   - Add your JSON entry to the array
   - Keep alphabetical order by `folder-name`

5. Add your plugin entry to `src/pl.x86.json` (for 32-bit):
   - Same JSON structure
   - Update `x86-download` URL

6. Validate JSON:
```bash
# Install Node.js if needed
npm install
npm test
```

7. Commit and push:
```bash
git add src/pl.x64.json src/pl.x86.json
git commit -m "Add Emoji Description plugin v0.1.0"
git push origin add-emoji-description
```

8. Create Pull Request:
   - Go to your fork on GitHub
   - Click "Pull Request"
   - Title: `Add Emoji Description plugin v0.1.0`
   - Description:
```markdown
## Plugin Information
- **Name:** Emoji Description
- **Version:** 0.1.0
- **Author:** Ruberoid
- **Repository:** https://github.com/Ruberoid/npp_emoji_description

## Description
Displays detailed character encoding information in the status bar for any character under the cursor, including full emoji support.

## Checklist
- [x] Plugin binaries are hosted on GitHub Releases
- [x] Both x86 and x64 versions provided
- [x] JSON entries added to both pl.x64.json and pl.x86.json
- [x] Plugin follows Notepad++ plugin guidelines
- [x] Source code is publicly available
- [x] GPL v2 compatible license
```

9. Wait for review from Notepad++ maintainers

## Step 6: Update for Future Releases

For version updates:

1. Build new binaries
2. Create new GitHub release (e.g., v0.2.0)
3. Update JSON entries in nppPluginList:
   - Update `version` field
   - Update download URLs
4. Create PR with title: `Update Emoji Description to v0.2.0`

## Requirements for Acceptance

- ✓ Plugin must be GPL v2 compatible
- ✓ Source code must be publicly available
- ✓ Binaries hosted on GitHub Releases
- ✓ Both x86 and x64 builds (if applicable)
- ✓ Plugin follows Notepad++ plugin interface
- ✓ No malware or suspicious code
- ✓ Clear description and documentation

## Useful Links

- Plugin List Repository: https://github.com/notepad-plus-plus/nppPluginList
- Plugin Template: https://github.com/npp-plugins/plugintemplate
- Plugin Development Guide: https://npp-user-manual.org/docs/plugin-communication/
