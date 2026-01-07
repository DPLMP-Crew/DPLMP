using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;

namespace Launcher
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Settings _settings;

        public MainWindow()
        {
            InitializeComponent();
            _settings = new Settings();
            _settings.Load();
            ApplySettings();
        }

        private void ApplySettings()
        {
            GameExePathBox.Text = _settings.GameExecutablePath;
            PlayerNameBox.Text = _settings.PlayerName;
        }

        private void PlayButton_Click(object sender, RoutedEventArgs e)
        {
            var gamePath = Settings.Instance.GameExecutablePath;
            if (string.IsNullOrWhiteSpace(gamePath))
            {
                MessageBox.Show("Please locate your game executable path.");
                return;
            }
            if (!File.Exists(gamePath))
            {
                MessageBox.Show("Can't find game executable.");
                return;
            }
            Injector.LaunchAuto(gamePath, []);
        }

        private void GameExePathBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (Settings.Instance == null) return;
            Settings.Instance.GameExecutablePath = GameExePathBox.Text;
            Settings.Instance.Save();
        }

        private void PlayerNameBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (Settings.Instance == null) return;
            Settings.Instance.PlayerName = PlayerNameBox.Text;
            Settings.Instance.Save();
        }

        private void DevPlayButton_Click(object sender, RoutedEventArgs e)
        {
            var cmd = new List<string>();

            cmd.Add("-dev");

            if (DevCops.IsChecked == true)
            {
                cmd.Add("-cops");
            }

            if (DevTraffic.IsChecked == true)
            {
                cmd.Add("-traffic");
            }

            if (DevSPLoop.IsChecked == true)
            {
                cmd.Add("-sp");
            }

            if (NowRadioButton.IsChecked == true)
            {
                cmd.Add("-now");
            }

            var gamePath = Settings.Instance.GameExecutablePath;
            if (string.IsNullOrWhiteSpace(gamePath))
            {
                MessageBox.Show("Please locate your game executable path.");
                return;
            }
            if (!File.Exists(gamePath))
            {
                MessageBox.Show("Can't find game executable.");
                return;
            }
            Injector.LaunchAuto(gamePath, cmd);
        }
    }
}
