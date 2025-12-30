using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Launcher
{
    public class Settings
    {
        public static Settings Instance { get; private set; }
        public string GameExecutablePath = "";
        public string PlayerName = "Unnamed";

        public Settings()
        {
            Instance = this;
        }
    }
}
