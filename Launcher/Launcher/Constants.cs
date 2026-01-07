using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Launcher
{
    public static class Constants
    {
        public const string GameExecutableName = "DriverParallelLines";
        public const string GameSteamID = "21780";
        public const string HookLibraryName = "DPLMPClient.dll";
        public const string CommandLineFileName = "cmd_cache.bin";
        public const uint InjectionTimeout = 10000;
        public const string SettingsFile = "Settings.ini";
        public static byte[] SteamExecutableHash = { 0xA6, 0x48, 0x7A, 0x1D, 0xF2, 0x25, 0x83, 0x89, 0xFB, 0x60, 0x42, 0xF3, 0x27, 0xD8, 0x19, 0xB9 };
        public static TimeSpan SteamLaunchTimeout = TimeSpan.FromSeconds(30D);
    }
}
