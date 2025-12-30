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
        public MainWindow()
        {
            new Settings();
            InitializeComponent();
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
            Injector.LaunchAuto(gamePath, "");
        }

        private void GameExePathBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            Settings.Instance.GameExecutablePath = GameExePathBox.Text;
        }

        private void PlayerNameBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            Settings.Instance.PlayerName = PlayerNameBox.Text;
        }
    }
}
