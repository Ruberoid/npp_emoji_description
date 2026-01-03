# Установка компонентов C++ для Visual Studio 2022

## Автоматическая установка (рекомендуется)

Откройте PowerShell от имени администратора и выполните:

```powershell
& "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vs_installer.exe" modify --installPath "C:\Program Files\Microsoft Visual Studio\2022\Community" --add Microsoft.VisualStudio.Workload.NativeDesktop --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows11SDK.22000 --quiet --norestart
```

Эта команда установит:
- Рабочую нагрузку "Разработка классических приложений на C++"
- Инструменты сборки MSVC v143
- Windows 11 SDK

## Ручная установка через GUI

1. Закройте Visual Studio (если открыта)

2. Запустите Visual Studio Installer:
   - Нажмите Win + S
   - Введите "Visual Studio Installer"
   - Запустите приложение

3. В Visual Studio Installer:
   - Найдите "Visual Studio Community 2022"
   - Нажмите кнопку **"Изменить"** (Modify)

4. В списке рабочих нагрузок:
   - Отметьте галочкой **"Разработка классических приложений на C++"** (Desktop development with C++)

5. Справа в разделе "Сведения об установке" убедитесь, что выбраны:
   - ✓ MSVC v143 - средства сборки C++ для VS 2022 (последняя версия)
   - ✓ Windows SDK (любая версия 10.0.x или 11.0.x)
   - ✓ C++ CMake tools for Windows

6. Нажмите кнопку **"Изменить"** внизу справа

7. Дождитесь завершения установки (может занять 5-15 минут)

## Проверка установки

После установки откройте "Developer Command Prompt for VS 2022" и выполните:

```bash
cl
```

Должно вывести информацию о компиляторе Microsoft C/C++, например:
```
Microsoft (R) C/C++ Optimizing Compiler Version 19.xx.xxxxx for x64
```

## После установки компонентов

1. Вернитесь в папку проекта:
```bash
cd c:\work\github\npp_showchar\EmojiCodePlugin
```

2. Откройте проект в Visual Studio:
```bash
start vs.proj\NppPluginTemplate.vcxproj
```

3. Или соберите из командной строки:
   - Откройте "Developer Command Prompt for VS 2022"
   - Выполните:
   ```bash
   cd c:\work\github\npp_showchar\EmojiCodePlugin
   msbuild vs.proj\NppPluginTemplate.vcxproj /p:Configuration=Release /p:Platform=x64
   ```

## Альтернатива: Использование Build Tools

Если вы не хотите устанавливать полную Visual Studio, можете установить только Build Tools:

1. Скачайте: https://aka.ms/vs/17/release/vs_BuildTools.exe
2. Запустите установщик
3. Выберите "Средства сборки C++"
4. Установите

Затем используйте "Developer Command Prompt for VS 2022" для сборки проекта.
