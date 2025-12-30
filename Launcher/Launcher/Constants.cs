using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Launcher
{
    public static class Constants
    {
        public const string GameExecutableName = "DriverParallelLines.exe";
        public const string GameSteamID = "21780";
        public static TimeSpan SteamLaunchTimeout = TimeSpan.FromSeconds(30D);
    }
}
