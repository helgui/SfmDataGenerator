namespace GUI
{
    partial class MainForm
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.newBtn = new System.Windows.Forms.ToolStripSplitButton();
            this.sfmItem = new System.Windows.Forms.ToolStripMenuItem();
            this.rgbdItem = new System.Windows.Forms.ToolStripMenuItem();
            this.silItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openBtn = new System.Windows.Forms.ToolStripButton();
            this.saveBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.viewBtn = new System.Windows.Forms.ToolStripDropDownButton();
            this.view3dItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewImageItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewMatchesItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.addNoiseItem = new System.Windows.Forms.ToolStripButton();
            this.addOutliersItem = new System.Windows.Forms.ToolStripButton();
            this.newDatasetPanel = new System.Windows.Forms.Panel();
            this.generateBtn = new System.Windows.Forms.Button();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.autoOutDir = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.outFolderBox = new System.Windows.Forms.TextBox();
            this.inputFileBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.changeFileBtn = new System.Windows.Forms.Button();
            this.changeFolderBtn = new System.Windows.Forms.Button();
            this.cancelBtn = new System.Windows.Forms.Button();
            this.outputPanel = new System.Windows.Forms.Panel();
            this.outputList = new System.Windows.Forms.RichTextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.viewImagePanel = new System.Windows.Forms.Panel();
            this.cancelImageBtn = new System.Windows.Forms.Button();
            this.showImageBtn = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.imageIdxCounter = new System.Windows.Forms.NumericUpDown();
            this.viewMatchesPanel = new System.Windows.Forms.Panel();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.firstImageCounter = new System.Windows.Forms.NumericUpDown();
            this.secondImageCounter = new System.Windows.Forms.NumericUpDown();
            this.showMatchesBtn = new System.Windows.Forms.Button();
            this.cancelMatchesBtn = new System.Windows.Forms.Button();
            this.addNoisePanel = new System.Windows.Forms.Panel();
            this.noiseCounter = new System.Windows.Forms.NumericUpDown();
            this.noiseBtn = new System.Windows.Forms.Button();
            this.noiseCancelBtn = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.addOutliersPanel = new System.Windows.Forms.Panel();
            this.label8 = new System.Windows.Forms.Label();
            this.outliersCancelBtn = new System.Windows.Forms.Button();
            this.addOutliersBtn = new System.Windows.Forms.Button();
            this.outliersCounter = new System.Windows.Forms.NumericUpDown();
            this.toolStrip1.SuspendLayout();
            this.newDatasetPanel.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.outputPanel.SuspendLayout();
            this.viewImagePanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imageIdxCounter)).BeginInit();
            this.viewMatchesPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.firstImageCounter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.secondImageCounter)).BeginInit();
            this.addNoisePanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.noiseCounter)).BeginInit();
            this.addOutliersPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.outliersCounter)).BeginInit();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newBtn,
            this.openBtn,
            this.saveBtn,
            this.toolStripSeparator1,
            this.viewBtn,
            this.toolStripSeparator2,
            this.addNoiseItem,
            this.addOutliersItem});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(791, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // newBtn
            // 
            this.newBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.newBtn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sfmItem,
            this.rgbdItem,
            this.silItem});
            this.newBtn.Image = ((System.Drawing.Image)(resources.GetObject("newBtn.Image")));
            this.newBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.newBtn.Name = "newBtn";
            this.newBtn.Size = new System.Drawing.Size(32, 22);
            this.newBtn.Text = "Generate";
            this.newBtn.ButtonClick += new System.EventHandler(this.newBtn_ButtonClick);
            // 
            // sfmItem
            // 
            this.sfmItem.Name = "sfmItem";
            this.sfmItem.Size = new System.Drawing.Size(152, 22);
            this.sfmItem.Text = "SfM";
            this.sfmItem.Click += new System.EventHandler(this.sfmItem_Click);
            // 
            // rgbdItem
            // 
            this.rgbdItem.Name = "rgbdItem";
            this.rgbdItem.Size = new System.Drawing.Size(152, 22);
            this.rgbdItem.Text = "RGB-D";
            this.rgbdItem.Click += new System.EventHandler(this.rgbdItem_Click);
            // 
            // silItem
            // 
            this.silItem.Name = "silItem";
            this.silItem.Size = new System.Drawing.Size(152, 22);
            this.silItem.Text = "Silhouette";
            this.silItem.Click += new System.EventHandler(this.silItem_Click);
            // 
            // openBtn
            // 
            this.openBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.openBtn.Image = ((System.Drawing.Image)(resources.GetObject("openBtn.Image")));
            this.openBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.openBtn.Name = "openBtn";
            this.openBtn.Size = new System.Drawing.Size(23, 22);
            this.openBtn.Text = "Open";
            this.openBtn.Click += new System.EventHandler(this.openBtn_Click);
            // 
            // saveBtn
            // 
            this.saveBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.saveBtn.Enabled = false;
            this.saveBtn.Image = ((System.Drawing.Image)(resources.GetObject("saveBtn.Image")));
            this.saveBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveBtn.Name = "saveBtn";
            this.saveBtn.Size = new System.Drawing.Size(23, 22);
            this.saveBtn.Text = "Save";
            this.saveBtn.Click += new System.EventHandler(this.saveBtn_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // viewBtn
            // 
            this.viewBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.viewBtn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.view3dItem,
            this.viewImageItem,
            this.viewMatchesItem});
            this.viewBtn.Image = ((System.Drawing.Image)(resources.GetObject("viewBtn.Image")));
            this.viewBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.viewBtn.Name = "viewBtn";
            this.viewBtn.Size = new System.Drawing.Size(45, 22);
            this.viewBtn.Text = "View";
            // 
            // view3dItem
            // 
            this.view3dItem.Name = "view3dItem";
            this.view3dItem.Size = new System.Drawing.Size(152, 22);
            this.view3dItem.Text = "3D";
            this.view3dItem.Click += new System.EventHandler(this.view3dItem_Click);
            // 
            // viewImageItem
            // 
            this.viewImageItem.Name = "viewImageItem";
            this.viewImageItem.Size = new System.Drawing.Size(152, 22);
            this.viewImageItem.Text = "Image";
            this.viewImageItem.Click += new System.EventHandler(this.viewImageItem_Click);
            // 
            // viewMatchesItem
            // 
            this.viewMatchesItem.Name = "viewMatchesItem";
            this.viewMatchesItem.Size = new System.Drawing.Size(152, 22);
            this.viewMatchesItem.Text = "Matches";
            this.viewMatchesItem.Click += new System.EventHandler(this.viewMatchesItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // addNoiseItem
            // 
            this.addNoiseItem.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.addNoiseItem.Image = ((System.Drawing.Image)(resources.GetObject("addNoiseItem.Image")));
            this.addNoiseItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addNoiseItem.Name = "addNoiseItem";
            this.addNoiseItem.Size = new System.Drawing.Size(64, 22);
            this.addNoiseItem.Text = "Add noise";
            this.addNoiseItem.Click += new System.EventHandler(this.addNoiseBtn_Click);
            // 
            // addOutliersItem
            // 
            this.addOutliersItem.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.addOutliersItem.Image = ((System.Drawing.Image)(resources.GetObject("addOutliersItem.Image")));
            this.addOutliersItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addOutliersItem.Name = "addOutliersItem";
            this.addOutliersItem.Size = new System.Drawing.Size(75, 22);
            this.addOutliersItem.Text = "Add outliers";
            this.addOutliersItem.Click += new System.EventHandler(this.addOutliersBtn_Click);
            // 
            // newDatasetPanel
            // 
            this.newDatasetPanel.AutoSize = true;
            this.newDatasetPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.newDatasetPanel.Controls.Add(this.generateBtn);
            this.newDatasetPanel.Controls.Add(this.tableLayoutPanel1);
            this.newDatasetPanel.Controls.Add(this.cancelBtn);
            this.newDatasetPanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.newDatasetPanel.Location = new System.Drawing.Point(0, 25);
            this.newDatasetPanel.Name = "newDatasetPanel";
            this.newDatasetPanel.Size = new System.Drawing.Size(791, 137);
            this.newDatasetPanel.TabIndex = 1;
            this.newDatasetPanel.Visible = false;
            // 
            // generateBtn
            // 
            this.generateBtn.Location = new System.Drawing.Point(11, 107);
            this.generateBtn.Name = "generateBtn";
            this.generateBtn.Size = new System.Drawing.Size(113, 27);
            this.generateBtn.TabIndex = 8;
            this.generateBtn.Text = "Generate dataset";
            this.generateBtn.UseVisualStyleBackColor = true;
            this.generateBtn.Click += new System.EventHandler(this.generateBtn_Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.AutoSize = true;
            this.tableLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 80F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.autoOutDir, 2, 1);
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.outFolderBox, 2, 2);
            this.tableLayoutPanel1.Controls.Add(this.inputFileBox, 2, 0);
            this.tableLayoutPanel1.Controls.Add(this.label2, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.changeFileBtn, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.changeFolderBtn, 1, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(5);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.Padding = new System.Windows.Forms.Padding(5);
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(791, 99);
            this.tableLayoutPanel1.TabIndex = 2;
            // 
            // autoOutDir
            // 
            this.autoOutDir.AutoSize = true;
            this.autoOutDir.Checked = true;
            this.autoOutDir.CheckState = System.Windows.Forms.CheckState.Checked;
            this.autoOutDir.Location = new System.Drawing.Point(123, 41);
            this.autoOutDir.Name = "autoOutDir";
            this.autoOutDir.Size = new System.Drawing.Size(189, 17);
            this.autoOutDir.TabIndex = 8;
            this.autoOutDir.Text = "Automatically change output folder";
            this.autoOutDir.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Left;
            this.label1.Location = new System.Drawing.Point(8, 5);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 33);
            this.label1.TabIndex = 2;
            this.label1.Text = "Input file";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // outFolderBox
            // 
            this.outFolderBox.Dock = System.Windows.Forms.DockStyle.Top;
            this.outFolderBox.Location = new System.Drawing.Point(123, 64);
            this.outFolderBox.Name = "outFolderBox";
            this.outFolderBox.Size = new System.Drawing.Size(660, 20);
            this.outFolderBox.TabIndex = 4;
            // 
            // inputFileBox
            // 
            this.inputFileBox.Dock = System.Windows.Forms.DockStyle.Top;
            this.inputFileBox.Location = new System.Drawing.Point(123, 8);
            this.inputFileBox.Name = "inputFileBox";
            this.inputFileBox.Size = new System.Drawing.Size(660, 20);
            this.inputFileBox.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Left;
            this.label2.Location = new System.Drawing.Point(8, 61);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 33);
            this.label2.TabIndex = 3;
            this.label2.Text = "Out folder";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // changeFileBtn
            // 
            this.changeFileBtn.Location = new System.Drawing.Point(88, 8);
            this.changeFileBtn.Name = "changeFileBtn";
            this.changeFileBtn.Size = new System.Drawing.Size(29, 27);
            this.changeFileBtn.TabIndex = 0;
            this.changeFileBtn.Text = "...";
            this.changeFileBtn.UseVisualStyleBackColor = true;
            this.changeFileBtn.Click += new System.EventHandler(this.changeFileBtn_Click);
            // 
            // changeFolderBtn
            // 
            this.changeFolderBtn.Location = new System.Drawing.Point(88, 64);
            this.changeFolderBtn.Name = "changeFolderBtn";
            this.changeFolderBtn.Size = new System.Drawing.Size(29, 27);
            this.changeFolderBtn.TabIndex = 5;
            this.changeFolderBtn.Text = "...";
            this.changeFolderBtn.UseVisualStyleBackColor = true;
            this.changeFolderBtn.Click += new System.EventHandler(this.changeFolderBtn_Click);
            // 
            // cancelBtn
            // 
            this.cancelBtn.Location = new System.Drawing.Point(130, 107);
            this.cancelBtn.Name = "cancelBtn";
            this.cancelBtn.Size = new System.Drawing.Size(127, 27);
            this.cancelBtn.TabIndex = 7;
            this.cancelBtn.Text = "Cancel";
            this.cancelBtn.UseVisualStyleBackColor = true;
            this.cancelBtn.Click += new System.EventHandler(this.cancelBtn_Click);
            // 
            // outputPanel
            // 
            this.outputPanel.AutoSize = true;
            this.outputPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.outputPanel.Controls.Add(this.outputList);
            this.outputPanel.Controls.Add(this.label3);
            this.outputPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.outputPanel.Location = new System.Drawing.Point(0, 329);
            this.outputPanel.Name = "outputPanel";
            this.outputPanel.Size = new System.Drawing.Size(791, 116);
            this.outputPanel.TabIndex = 3;
            // 
            // outputList
            // 
            this.outputList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.outputList.Location = new System.Drawing.Point(0, 19);
            this.outputList.Name = "outputList";
            this.outputList.ReadOnly = true;
            this.outputList.Size = new System.Drawing.Size(791, 97);
            this.outputList.TabIndex = 1;
            this.outputList.Text = "";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Dock = System.Windows.Forms.DockStyle.Top;
            this.label3.Location = new System.Drawing.Point(0, 0);
            this.label3.Name = "label3";
            this.label3.Padding = new System.Windows.Forms.Padding(3);
            this.label3.Size = new System.Drawing.Size(134, 19);
            this.label3.TabIndex = 0;
            this.label3.Text = "SfmDataGenerator output";
            // 
            // viewImagePanel
            // 
            this.viewImagePanel.Controls.Add(this.cancelImageBtn);
            this.viewImagePanel.Controls.Add(this.showImageBtn);
            this.viewImagePanel.Controls.Add(this.label4);
            this.viewImagePanel.Controls.Add(this.imageIdxCounter);
            this.viewImagePanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.viewImagePanel.Location = new System.Drawing.Point(0, 162);
            this.viewImagePanel.Name = "viewImagePanel";
            this.viewImagePanel.Size = new System.Drawing.Size(791, 41);
            this.viewImagePanel.TabIndex = 4;
            this.viewImagePanel.Visible = false;
            // 
            // cancelImageBtn
            // 
            this.cancelImageBtn.Location = new System.Drawing.Point(273, 6);
            this.cancelImageBtn.Name = "cancelImageBtn";
            this.cancelImageBtn.Size = new System.Drawing.Size(95, 27);
            this.cancelImageBtn.TabIndex = 3;
            this.cancelImageBtn.Text = "Cancel";
            this.cancelImageBtn.UseVisualStyleBackColor = true;
            this.cancelImageBtn.Click += new System.EventHandler(this.cancelBtn_Click);
            // 
            // showImageBtn
            // 
            this.showImageBtn.Location = new System.Drawing.Point(171, 6);
            this.showImageBtn.Name = "showImageBtn";
            this.showImageBtn.Size = new System.Drawing.Size(96, 27);
            this.showImageBtn.TabIndex = 2;
            this.showImageBtn.Text = "Show";
            this.showImageBtn.UseVisualStyleBackColor = true;
            this.showImageBtn.Click += new System.EventHandler(this.showImageBtn_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(8, 13);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(46, 13);
            this.label4.TabIndex = 1;
            this.label4.Text = "Image #";
            // 
            // imageIdxCounter
            // 
            this.imageIdxCounter.Location = new System.Drawing.Point(60, 10);
            this.imageIdxCounter.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.imageIdxCounter.Name = "imageIdxCounter";
            this.imageIdxCounter.Size = new System.Drawing.Size(95, 20);
            this.imageIdxCounter.TabIndex = 0;
            // 
            // viewMatchesPanel
            // 
            this.viewMatchesPanel.Controls.Add(this.cancelMatchesBtn);
            this.viewMatchesPanel.Controls.Add(this.showMatchesBtn);
            this.viewMatchesPanel.Controls.Add(this.secondImageCounter);
            this.viewMatchesPanel.Controls.Add(this.firstImageCounter);
            this.viewMatchesPanel.Controls.Add(this.label6);
            this.viewMatchesPanel.Controls.Add(this.label5);
            this.viewMatchesPanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.viewMatchesPanel.Location = new System.Drawing.Point(0, 203);
            this.viewMatchesPanel.Name = "viewMatchesPanel";
            this.viewMatchesPanel.Size = new System.Drawing.Size(791, 46);
            this.viewMatchesPanel.TabIndex = 5;
            this.viewMatchesPanel.Visible = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 15);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(67, 13);
            this.label5.TabIndex = 2;
            this.label5.Text = "First image #";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(197, 15);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(85, 13);
            this.label6.TabIndex = 3;
            this.label6.Text = "Second image #";
            // 
            // firstImageCounter
            // 
            this.firstImageCounter.Location = new System.Drawing.Point(85, 13);
            this.firstImageCounter.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.firstImageCounter.Name = "firstImageCounter";
            this.firstImageCounter.Size = new System.Drawing.Size(95, 20);
            this.firstImageCounter.TabIndex = 4;
            // 
            // secondImageCounter
            // 
            this.secondImageCounter.Location = new System.Drawing.Point(288, 13);
            this.secondImageCounter.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.secondImageCounter.Name = "secondImageCounter";
            this.secondImageCounter.Size = new System.Drawing.Size(95, 20);
            this.secondImageCounter.TabIndex = 5;
            // 
            // showMatchesBtn
            // 
            this.showMatchesBtn.Location = new System.Drawing.Point(398, 8);
            this.showMatchesBtn.Name = "showMatchesBtn";
            this.showMatchesBtn.Size = new System.Drawing.Size(96, 27);
            this.showMatchesBtn.TabIndex = 6;
            this.showMatchesBtn.Text = "Show";
            this.showMatchesBtn.UseVisualStyleBackColor = true;
            this.showMatchesBtn.Click += new System.EventHandler(this.showMatchesBtn_Click);
            // 
            // cancelMatchesBtn
            // 
            this.cancelMatchesBtn.Location = new System.Drawing.Point(500, 8);
            this.cancelMatchesBtn.Name = "cancelMatchesBtn";
            this.cancelMatchesBtn.Size = new System.Drawing.Size(95, 27);
            this.cancelMatchesBtn.TabIndex = 7;
            this.cancelMatchesBtn.Text = "Cancel";
            this.cancelMatchesBtn.UseVisualStyleBackColor = true;
            this.cancelMatchesBtn.Click += new System.EventHandler(this.cancelBtn_Click);
            // 
            // addNoisePanel
            // 
            this.addNoisePanel.Controls.Add(this.label7);
            this.addNoisePanel.Controls.Add(this.noiseCancelBtn);
            this.addNoisePanel.Controls.Add(this.noiseBtn);
            this.addNoisePanel.Controls.Add(this.noiseCounter);
            this.addNoisePanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.addNoisePanel.Location = new System.Drawing.Point(0, 249);
            this.addNoisePanel.Name = "addNoisePanel";
            this.addNoisePanel.Size = new System.Drawing.Size(791, 40);
            this.addNoisePanel.TabIndex = 6;
            this.addNoisePanel.Visible = false;
            // 
            // noiseCounter
            // 
            this.noiseCounter.DecimalPlaces = 2;
            this.noiseCounter.Location = new System.Drawing.Point(88, 11);
            this.noiseCounter.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.noiseCounter.Name = "noiseCounter";
            this.noiseCounter.Size = new System.Drawing.Size(95, 20);
            this.noiseCounter.TabIndex = 5;
            this.noiseCounter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // noiseBtn
            // 
            this.noiseBtn.Location = new System.Drawing.Point(202, 6);
            this.noiseBtn.Name = "noiseBtn";
            this.noiseBtn.Size = new System.Drawing.Size(96, 27);
            this.noiseBtn.TabIndex = 7;
            this.noiseBtn.Text = "Noise";
            this.noiseBtn.UseVisualStyleBackColor = true;
            this.noiseBtn.Click += new System.EventHandler(this.noiseBtn_Click);
            // 
            // noiseCancelBtn
            // 
            this.noiseCancelBtn.Location = new System.Drawing.Point(304, 6);
            this.noiseCancelBtn.Name = "noiseCancelBtn";
            this.noiseCancelBtn.Size = new System.Drawing.Size(95, 27);
            this.noiseCancelBtn.TabIndex = 8;
            this.noiseCancelBtn.Text = "Cancel";
            this.noiseCancelBtn.UseVisualStyleBackColor = true;
            this.noiseCancelBtn.Click += new System.EventHandler(this.cancelBtn_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(15, 13);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(50, 13);
            this.label7.TabIndex = 9;
            this.label7.Text = "Std. dev.";
            // 
            // addOutliersPanel
            // 
            this.addOutliersPanel.Controls.Add(this.label8);
            this.addOutliersPanel.Controls.Add(this.outliersCancelBtn);
            this.addOutliersPanel.Controls.Add(this.addOutliersBtn);
            this.addOutliersPanel.Controls.Add(this.outliersCounter);
            this.addOutliersPanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.addOutliersPanel.Location = new System.Drawing.Point(0, 289);
            this.addOutliersPanel.Name = "addOutliersPanel";
            this.addOutliersPanel.Size = new System.Drawing.Size(791, 40);
            this.addOutliersPanel.TabIndex = 7;
            this.addOutliersPanel.Visible = false;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(15, 13);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(44, 13);
            this.label8.TabIndex = 9;
            this.label8.Text = "Number";
            // 
            // outliersCancelBtn
            // 
            this.outliersCancelBtn.Location = new System.Drawing.Point(304, 6);
            this.outliersCancelBtn.Name = "outliersCancelBtn";
            this.outliersCancelBtn.Size = new System.Drawing.Size(95, 27);
            this.outliersCancelBtn.TabIndex = 8;
            this.outliersCancelBtn.Text = "Cancel";
            this.outliersCancelBtn.UseVisualStyleBackColor = true;
            this.outliersCancelBtn.Click += new System.EventHandler(this.cancelBtn_Click);
            // 
            // addOutliersBtn
            // 
            this.addOutliersBtn.Location = new System.Drawing.Point(202, 6);
            this.addOutliersBtn.Name = "addOutliersBtn";
            this.addOutliersBtn.Size = new System.Drawing.Size(96, 27);
            this.addOutliersBtn.TabIndex = 7;
            this.addOutliersBtn.Text = "Add";
            this.addOutliersBtn.UseVisualStyleBackColor = true;
            this.addOutliersBtn.Click += new System.EventHandler(this.addOutliersBtn_Click_1);
            // 
            // outliersCounter
            // 
            this.outliersCounter.Location = new System.Drawing.Point(88, 11);
            this.outliersCounter.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.outliersCounter.Name = "outliersCounter";
            this.outliersCounter.Size = new System.Drawing.Size(95, 20);
            this.outliersCounter.TabIndex = 5;
            this.outliersCounter.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // MainForm
            // 
            this.ClientSize = new System.Drawing.Size(791, 445);
            this.Controls.Add(this.outputPanel);
            this.Controls.Add(this.addOutliersPanel);
            this.Controls.Add(this.addNoisePanel);
            this.Controls.Add(this.viewMatchesPanel);
            this.Controls.Add(this.viewImagePanel);
            this.Controls.Add(this.newDatasetPanel);
            this.Controls.Add(this.toolStrip1);
            this.Name = "MainForm";
            this.Text = "SfmDataGenerator";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.newDatasetPanel.ResumeLayout(false);
            this.newDatasetPanel.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.outputPanel.ResumeLayout(false);
            this.outputPanel.PerformLayout();
            this.viewImagePanel.ResumeLayout(false);
            this.viewImagePanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imageIdxCounter)).EndInit();
            this.viewMatchesPanel.ResumeLayout(false);
            this.viewMatchesPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.firstImageCounter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.secondImageCounter)).EndInit();
            this.addNoisePanel.ResumeLayout(false);
            this.addNoisePanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.noiseCounter)).EndInit();
            this.addOutliersPanel.ResumeLayout(false);
            this.addOutliersPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.outliersCounter)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton openBtn;
        private System.Windows.Forms.ToolStripButton saveBtn;
        private System.Windows.Forms.ToolStripSplitButton newBtn;
        private System.Windows.Forms.ToolStripMenuItem sfmItem;
        private System.Windows.Forms.ToolStripMenuItem rgbdItem;
        private System.Windows.Forms.ToolStripMenuItem silItem;
        private System.Windows.Forms.Panel newDatasetPanel;
        private System.Windows.Forms.Button changeFileBtn;
        private System.Windows.Forms.Button changeFolderBtn;
        private System.Windows.Forms.TextBox outFolderBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox inputFileBox;
        private System.Windows.Forms.Button cancelBtn;
        private System.Windows.Forms.CheckBox autoOutDir;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Button generateBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton addNoiseItem;
        private System.Windows.Forms.ToolStripButton addOutliersItem;
        private System.Windows.Forms.Panel outputPanel;
        private System.Windows.Forms.RichTextBox outputList;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ToolStripDropDownButton viewBtn;
        private System.Windows.Forms.ToolStripMenuItem view3dItem;
        private System.Windows.Forms.ToolStripMenuItem viewImageItem;
        private System.Windows.Forms.ToolStripMenuItem viewMatchesItem;
        private System.Windows.Forms.Panel viewImagePanel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown imageIdxCounter;
        private System.Windows.Forms.Button cancelImageBtn;
        private System.Windows.Forms.Button showImageBtn;
        private System.Windows.Forms.Panel viewMatchesPanel;
        private System.Windows.Forms.Button cancelMatchesBtn;
        private System.Windows.Forms.Button showMatchesBtn;
        private System.Windows.Forms.NumericUpDown secondImageCounter;
        private System.Windows.Forms.NumericUpDown firstImageCounter;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Panel addNoisePanel;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button noiseCancelBtn;
        private System.Windows.Forms.Button noiseBtn;
        private System.Windows.Forms.NumericUpDown noiseCounter;
        private System.Windows.Forms.Panel addOutliersPanel;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button outliersCancelBtn;
        private System.Windows.Forms.Button addOutliersBtn;
        private System.Windows.Forms.NumericUpDown outliersCounter;
    }
}

