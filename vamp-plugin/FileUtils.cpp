#include "FileUtils.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <shlobj.h>
#include <windows.h>
#elif __APPLE__
#include <cstdlib>
#else
#include <unistd.h>
#endif

std::filesystem::path getAppDataPath()
{
   std::filesystem::path appDataPath;

#ifdef _WIN32
   // Windows: Get the path to the AppData\Roaming folder
   char appData[MAX_PATH];
   if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appData)))
   {
      appDataPath = appData;
   }
   else
   {
      throw std::runtime_error("Failed to retrieve AppData folder path.");
   }
#elif __APPLE__
   // macOS: Use ~/Library/Application Support
   if (const char* home = std::getenv("HOME"))
   {
      appDataPath = std::filesystem::path(home) / "Library/Application Support";
   }
   else
   {
      throw std::runtime_error("Failed to retrieve HOME directory.");
   }
#else
   // Linux: Use ~/.local/share
   if (const char* home = std::getenv("HOME"))
   {
      appDataPath = std::filesystem::path(home) / ".local/share";
   }
   else
   {
      throw std::runtime_error("Failed to retrieve HOME directory.");
   }
#endif

   return appDataPath / "LoopTempoEstimator";
}

void LTE::FileUtils::WriteToAppDataFile(int value)
{
   auto appDataPath = getAppDataPath();

   if (!std::filesystem::exists(appDataPath))
   {
      std::filesystem::create_directories(appDataPath);
   }

   std::ofstream file(appDataPath / "sample_count.txt");
   if (!file)
   {
      throw std::runtime_error("Failed to open file for writing.");
   }

   file << value;
}

int LTE::FileUtils::ReadFromAppDataFile()
{
   auto appDataPath = getAppDataPath();

   std::ifstream file(appDataPath / "sample_count.txt");
   if (!file)
      return 0;

   int value;
   file >> value;
   return value;
}