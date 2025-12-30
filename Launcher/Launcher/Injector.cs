using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Launcher
{
    public enum InjectionResult
    {
        Success,
        Failed
    }

    public static class Injector
    {
        public static InjectionResult LaunchSteam(string injectedLibrary, string commandLine)
        {
            Process.Start($"steam://run/21780//{commandLine}");
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
