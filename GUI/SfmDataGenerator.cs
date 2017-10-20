using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace GUI
{
    class SfmDataGenerator
    {
        public SfmDataGenerator(Action<string> stdOutHandler = null, Action<string> stdErrHandler = null)
        {
            OnStdErr = stdErrHandler;
            OnStdOut = stdOutHandler;
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.CreateNoWindow = true;
            startInfo.UseShellExecute = false;
            startInfo.RedirectStandardOutput = true;
            startInfo.RedirectStandardInput = true;
            startInfo.RedirectStandardError = true;
            startInfo.FileName = "SfmDataGenerator.exe";
            startInfo.Arguments = "r";
            process = new Process();
            process.StartInfo = startInfo;
            process.OutputDataReceived += new DataReceivedEventHandler(HandleStdOut);
            process.ErrorDataReceived += new DataReceivedEventHandler(HandleStdErr);
            process.Start();
            process.BeginErrorReadLine();
            process.BeginOutputReadLine();
        }

        public void Exit()
        {
            Command("e");
            if (!process.HasExited)
            {
                process.Kill();
            }
        }

        public void LoadDataset(string path) => Command("l", path);
        public void SaveDataset(string path) => Command("s", path);
        public void GenerateDataset(string typeSuffix, string inputModel, string outputFolder) =>
            Command("g" + typeSuffix, inputModel, outputFolder);
        public void AddNoise(double stddev) => Command("n", stddev.ToString());
        public void AddOutliers(int count) => Command("o", count.ToString());
        public void View() => Command("v");
        public void ViewSingle(int idx) => Command("vo", idx.ToString());
        public void ViewPair(int idx1, int idx2) => Command("vm", idx1.ToString(), idx2.ToString());
        private Process process;
        private void Command(params string[] args) => process.StandardInput.WriteLine(string.Join(" ", args));
        private Action<string> OnStdErr, OnStdOut;

        private void HandleStdOut(object sender, DataReceivedEventArgs data)
        {
            if (!String.IsNullOrEmpty(data.Data) && OnStdOut != null)
            {
                OnStdOut(data.Data);
            }
        }

        private void HandleStdErr(object sender, DataReceivedEventArgs data)
        {
            if (!String.IsNullOrEmpty(data.Data) && OnStdErr != null)
            {
                OnStdErr(data.Data);
            }
        }
    }
}
