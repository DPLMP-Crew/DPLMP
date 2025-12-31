using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Launcher.Ini;

namespace Launcher
{
    public class Settings
    {
        public static Settings Instance { get; private set; }
        private IniFile _ini = new();
        public string PlayerName
        {
            get
            {
                return _ini.GetProperty("General", "PlayerName", "Unnamed");
            }
            set
            {
                _ini.SetProperty("General", "PlayerName", value);
            }
        }

        public string GameExecutablePath
        {
            get
            {
                return _ini.GetProperty("Paths", "GameExecutable", "");
            }
            set
            {
                _ini.SetProperty("Paths", "GameExecutable", value);
            }
        }

        public Settings()
        {
            Instance = this;
        }

        public void Load()
        {
            var path = Path.Combine(AppContext.BaseDirectory, Constants.SettingsFile);
            if (!File.Exists(path)) return;
            _ini.Read(path);
        }

        public void Save()
        {
            var path = Path.Combine(AppContext.BaseDirectory, Constants.SettingsFile);
            _ini.Write(path);
        }
    }
}
