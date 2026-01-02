using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Xml;

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
                        {
                            return proc;
                        }
                    }
                    catch
                    {

                    }
                }

                Thread.Sleep(1);
            }
            return null;
        }

        private static void Inject(int procId)
        {
            var procHandle = Kernel.OpenProcess(ProcessAccessFlags.All, false, procId);
            Console.WriteLine(procHandle.ToInt32().ToString());
            var libraryPath = Path.Combine(AppContext.BaseDirectory, Constants.HookLibraryName);
            var libraryBytes = Encoding.Unicode.GetBytes(libraryPath + "\0");
            var libraryBytesSize = (IntPtr)libraryBytes.Length;
            var libraryMem = Kernel.VirtualAllocEx(procHandle, IntPtr.Zero, libraryBytesSize, AllocationType.Reserve | AllocationType.Commit, MemoryProtection.ReadWrite);
            Console.WriteLine($"Allocated {libraryMem.ToInt32()}");
            var libraryWrite = Kernel.WriteProcessMemory(procHandle, libraryMem, libraryBytes, libraryBytes.Length, out var bytesread);
            if (libraryWrite)
                Console.WriteLine("Wrote to allocation.");
            var kernel32Handle = Kernel.GetModuleHandle("Kernel32.dll");
            Console.WriteLine($"Found Kernel32 handle {kernel32Handle.ToInt32()}");
            var loadLibraryAAddress = Kernel.GetProcAddress(kernel32Handle, "LoadLibraryW");

            var remoteThreadHandle = Kernel.CreateRemoteThread(procHandle, IntPtr.Zero, 0, loadLibraryAAddress, libraryMem, 0, IntPtr.Zero);
            Console.WriteLine($"Created remote thread {remoteThreadHandle.ToInt32()}");

            var wait = Kernel.WaitForSingleObject(remoteThreadHandle, Constants.InjectionTimeout);

            if (wait == Kernel.WaitTimeout)
                Console.WriteLine("Failed to inject!");

            var freeLibraryMem = Kernel.VirtualFreeEx(procHandle, libraryMem, 0, AllocationType.Release);

            Kernel.CloseHandle(remoteThreadHandle);

            Kernel.CloseHandle(procHandle);
        }

        private static string ByteArrayToString(byte[] arr)
        {
            var str = "{ ";
            foreach(var by in arr)
            {
                str += by.ToString("X")+", ";
            }
            str += "}";
            return str;
        }

        public static InjectionResult LaunchAuto(string executable, string commandLine)
        {
            byte[] hash = { };
            using (var md5 = MD5.Create())
            {
                using (var stream = File.OpenRead(executable))
                {
                    hash = md5.ComputeHash(stream);
                }
            }
            if (hash.Length == 0)
                return InjectionResult.Failed;

            Console.WriteLine($"Our Steam Hash: {ByteArrayToString(Constants.SteamExecutableHash)} ({Constants.SteamExecutableHash.Length})");
            Console.WriteLine($"Exe Hash: {ByteArrayToString(hash)} ({hash.Length})");

            if (hash.SequenceEqual(Constants.SteamExecutableHash))
            {
                Console.WriteLine("Doing Steam Launch");
                return LaunchSteam(commandLine);
            }
            else
            {
                Console.WriteLine("Doing Direct Launch");
                return LaunchDirect(executable, commandLine);
            }
        }

        public static InjectionResult LaunchSteam(string commandLine)
        {
            Process.Start(new ProcessStartInfo {
                FileName = $"steam://run/{Constants.GameSteamID}//{commandLine}",
                UseShellExecute = true
            });
            var dplProc = WaitForDPLSteam();
            if (dplProc != null)
            {
                Inject(dplProc.Id);
            }
            return InjectionResult.Success;
        }

        public static InjectionResult LaunchDirect(string executable, string commandLine)
        {
            var si = new STARTUPINFO();
            si.cb = Marshal.SizeOf<STARTUPINFO>();

            var saProc = new SECURITY_ATTRIBUTES();
            saProc.nLength = Marshal.SizeOf<SECURITY_ATTRIBUTES>();

            var saThread = new SECURITY_ATTRIBUTES();
            saThread.nLength = Marshal.SizeOf<SECURITY_ATTRIBUTES>();

            var result = Kernel.CreateProcess(executable, commandLine, ref saProc, ref saThread, false, (uint)ProcessCreationFlags.CreateSuspended, IntPtr.Zero, Path.GetDirectoryName(executable), ref si, out var pi);
            Inject(pi.dwProcessId);
            Kernel.ResumeThread(pi.hThread);

            if (!result)
                return InjectionResult.Failed;

            return InjectionResult.Success;
        }
    }
}
