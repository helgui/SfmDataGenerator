using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

namespace GUI
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
            sfmDataGen = new SfmDataGenerator(AddStdOutLine, AddStdErrLine);
            state = new State();
            panels = new List<Panel>();
            panels.Add(newDatasetPanel);
            panels.Add(viewImagePanel);
            panels.Add(viewMatchesPanel);
            panels.Add(addNoisePanel);
            panels.Add(addOutliersPanel);
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            sfmDataGen.Exit();
        }

        private void sfmItem_Click(object sender, EventArgs e)
        {
            state.Suffix = "";
            CreateDatasetDialog();
            
        }

        private void CreateDatasetDialog()
        {
            newDatasetPanel.Show();
            if (inputFileBox.Text == "")
                ChooseInputFile();
        }

        private void ChooseInputFile()
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.CheckFileExists = true;
            dialog.CheckPathExists = true;
            dialog.Filter = "3d model (*.ply;*.obj)|*.ply;*.obj";
            if (String.IsNullOrEmpty(inputFileBox.Text))
            {
                dialog.InitialDirectory = Directory.GetCurrentDirectory();
            }
            else
            {
                dialog.InitialDirectory = Path.GetDirectoryName(inputFileBox.Text);
                dialog.FileName = Path.GetFileName(inputFileBox.Text);
            }
            DialogResult result = dialog.ShowDialog();
            inputFileBox.Text = dialog.FileName;
            if (result == DialogResult.OK && autoOutDir.Checked)
            {
                outFolderBox.Text = Path.Combine(
                    Path.GetDirectoryName(dialog.FileName),
                    Path.GetFileNameWithoutExtension(dialog.FileName)
                );
            }
        }

        private void changeFileBtn_Click(object sender, EventArgs e)
        {
            ChooseInputFile();    
        }

        private void newBtn_ButtonClick(object sender, EventArgs e)
        {
            newBtn.ShowDropDown();
        }

        private void HideAllPanels()
        {
            foreach (Panel p in panels)
            {
                p.Hide();
            }
        }

        private void cancelBtn_Click(object sender, EventArgs e)
        {
            HideAllPanels();
        }

        private void rgbdItem_Click(object sender, EventArgs e)
        {
            state.Suffix = "d";
            CreateDatasetDialog();
        }

        private void silItem_Click(object sender, EventArgs e)
        {
            state.Suffix = "s";
            CreateDatasetDialog();
        }

        private void changeFolderBtn_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.SelectedPath = Directory.GetCurrentDirectory();
            DialogResult result = dialog.ShowDialog();
            if(result == DialogResult.OK)
                outFolderBox.Text = dialog.SelectedPath;
        }

        private void generateBtn_Click(object sender, EventArgs e)
        {
            state.CurrentDataset = "";
            newDatasetPanel.Hide();
            saveBtn.Enabled = true;
            sfmDataGen.GenerateDataset(state.Suffix, Quote(inputFileBox.Text), Quote(outFolderBox.Text));
        }

        private string Quote(string s)
        {
            return "\"" + s + "\"";
        }

        private void openBtn_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "Dataset files (*.txt;*.yml;*.xml)|*.txt;*.yml;*.xml";
            dialog.CheckFileExists = true;
            dialog.CheckPathExists = true;
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                saveBtn.Enabled = false;
                sfmDataGen.LoadDataset(Quote(dialog.FileName));
                state.CurrentDataset = dialog.FileName;
                Text = "SfmDataGenerator [" + state.CurrentDataset + "]";
            }
        }

        private void saveBtn_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Filter = "TXT (*.txt)|*.txt|YAML (*.yml)|*.yml|XML (*.xml)|*.xml|All files (*.*)|*.*";
            dialog.CheckFileExists = false;
            dialog.CheckPathExists = false;
            dialog.OverwritePrompt = true;
            if (state.CurrentDataset != "")
            {
                dialog.InitialDirectory = Path.GetDirectoryName(state.CurrentDataset);
                dialog.FileName = Path.GetFileName(state.CurrentDataset);
                dialog.AddExtension = true;
            }
            else
            {
                dialog.FileName = "sfmData";
                dialog.FilterIndex = 0;
                if (outFolderBox.Text != "")
                    dialog.InitialDirectory = outFolderBox.Text;
            }
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                saveBtn.Enabled = false;
                state.CurrentDataset = dialog.FileName;
                Text = "SfmDataGenerator [" + state.CurrentDataset + "]"; 
                sfmDataGen.SaveDataset(Quote(dialog.FileName));
            }
        }

        private void addNoiseBtn_Click(object sender, EventArgs e)
        {
            HideAllPanels();
            addNoisePanel.Show();
        }

        private void AddStdOutLine(string line)
        {
            outputList.AppendText(line + "\n", Color.Black);
        }

        private void AddStdErrLine(string line)
        {
            outputList.AppendText(line + "\n", Color.Red);
        }

        private void addOutliersBtn_Click(object sender, EventArgs e)
        {
            HideAllPanels();
            addOutliersPanel.Show();
        }

        private void view3dItem_Click(object sender, EventArgs e)
        {
            sfmDataGen.View();
        }
        private List<Panel> panels;
        private SfmDataGenerator sfmDataGen;
        private State state;

        private void showImageBtn_Click(object sender, EventArgs e)
        {
            sfmDataGen.ViewSingle(Decimal.ToInt32(imageIdxCounter.Value));
        }

        private void viewImageItem_Click(object sender, EventArgs e)
        {
            HideAllPanels();
            viewImagePanel.Show();
        }

        private void showMatchesBtn_Click(object sender, EventArgs e)
        {
            sfmDataGen.ViewPair(Decimal.ToInt32(firstImageCounter.Value), Decimal.ToInt32(secondImageCounter.Value));
        }

        private void viewMatchesItem_Click(object sender, EventArgs e)
        {
            HideAllPanels();
            viewMatchesPanel.Show();
        }

        private void noiseBtn_Click(object sender, EventArgs e)
        {
            saveBtn.Enabled = true;
            sfmDataGen.AddNoise(Decimal.ToDouble(noiseCounter.Value));
            HideAllPanels();
        }

        private void addOutliersBtn_Click_1(object sender, EventArgs e)
        {
            saveBtn.Enabled = true;
            sfmDataGen.AddOutliers(Decimal.ToInt32(outliersCounter.Value));
            HideAllPanels();
        }
    }
}
