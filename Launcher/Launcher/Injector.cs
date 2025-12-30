using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.IO;
using System.Windows;

namespace Launcher
{
    public enum InjectionResult
    {
        Success,
        Failed
    }

    public static class Injector
    {
        public static Process WaitForDPLSteam()
        {
            var time = Stopwatch.StartNew();
            while(time.Elapsed < Constants.SteamLaunchTimeout)
            {
                var procs = Process.GetProcessesByName(Constants.GameExecutableName);

                foreach(var proc in procs)
                {
                    try
                    {
                        var mainMod = proc.MainModule;
                        if (mainMod != null)
                            return proc;
                    }
                    catch
                    {

                    }
                }

                Thread.Sleep(10);
            }
            return null;
        }

        public static InjectionResult LaunchSteam(string injectedLibrary, string commandLine)
        {
            Process.Start(new ProcessStartInfo {
                FileName = $"steam://run/{Constants.GameSteamID}//{commandLine}",
                UseShellExecute = true
            });
            var dplProc = WaitForDPLSteam();
            if (dplProc != null)
                MessageBox.Show("GAME BOOTED!");
            return InjectionResult.Success;
        }

        public static InjectionResult Launch(string executable, string injectedLibrary, string commandLine)
        {
            var si = new STARTUPINFO();
            si.cb = Marshal.SizeOf<STARTUPINFO>();

            var saProc = new SECURITY_ATTRIBUTES();
            saProc.nLength = Marshal.SizeOf<SECURITY_ATTRIBUTES>();

            var saThread = new SECURITY_ATTRIBUTES();
            saThread.nLength = Marshal.SizeOf<SECURITY_ATTRIBUTES>();

            var result = Kernel.CreateProcess(executable, commandLine, ref saProc, ref saThread, false, 0, IntPtr.Zero, null, ref si, out var pi);
            
            if (!result)
                return InjectionResult.Failed;

            return InjectionResult.Success;
        }
    }
}
