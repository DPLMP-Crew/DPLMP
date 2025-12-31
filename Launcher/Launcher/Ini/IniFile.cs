using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Launcher.Ini
{
    /// <summary>
    /// Parses an .ini file.
    /// </summary>
    public class IniFile
    {
        public Dictionary<string, Section> Sections = new();

        public IniFile()
        {

        }

        public Section GetSection(string name)
        {
            if (Sections.TryGetValue(name, out var section))
                return section;
            return null;
        }

        public string GetProperty(string sectionName, string propertyName, string defaultValue = "")
        {
            var section = GetSection(sectionName);
            if (section == null)
                return defaultValue;
            if (section.KeyValues.TryGetValue(propertyName, out var val))
                return val;
            return defaultValue;
        }

        public void SetProperty(string sectionName, string propertyName, string value)
        {
            var section = GetSection(sectionName);
            if (section == null)
            {
                section = new Section();
                Sections[sectionName] = section;
            }
            section.KeyValues[propertyName] = value;
        }

        public void Read(string path)
        {
            using var ms = new FileStream(path, FileMode.Open);
            using var reader = new StreamReader(ms);
            var sections = new Dictionary<string, Section>();
            Section currentSection = null;
            string readLine;
            while ((readLine = reader.ReadLine()) != null)
            {
                var line = readLine.Trim();
                if (string.IsNullOrEmpty(line)) continue;
                if (line[0] == ';') continue;
                if (line[0] == '[' && line[line.Length - 1] == ']')
                {
                    currentSection = new Section();
                    sections[line.Substring(1, line.Length - 2)] = currentSection;
                    continue;
                }
                if (currentSection == null) continue;
                var split = line.Split('=');
                var key = split[0].Trim();
                var value = split[1].Trim();
                currentSection.KeyValues[key] = value;
            }
            Sections = sections;
        }

        public void Write(string path)
        {
            using var fs = new FileStream(path, FileMode.Create);
            using var writer = new StreamWriter(fs);
            foreach(var section in Sections)
            {
                writer.WriteLine($"[{section.Key}]");
                foreach(var keyValue in section.Value.KeyValues)
                {
                    writer.WriteLine($"{keyValue.Key} = {keyValue.Value}");
                }
                writer.WriteLine();
            }
        }
    }
}