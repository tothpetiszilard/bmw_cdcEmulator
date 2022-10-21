namespace IBUS_CDEmu
{
    partial class CDEmu
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.port_cb = new System.Windows.Forms.ComboBox();
            this.connect_b = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.timer3 = new System.Windows.Forms.Timer(this.components);
            this.diskSel = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.trackSel = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.status_l = new System.Windows.Forms.Label();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.diskSel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackSel)).BeginInit();
            this.SuspendLayout();
            // 
            // port_cb
            // 
            this.port_cb.FormattingEnabled = true;
            this.port_cb.Location = new System.Drawing.Point(12, 22);
            this.port_cb.Name = "port_cb";
            this.port_cb.Size = new System.Drawing.Size(121, 21);
            this.port_cb.TabIndex = 0;
            this.port_cb.DropDown += new System.EventHandler(this.port_cb_DropDown);
            // 
            // connect_b
            // 
            this.connect_b.Location = new System.Drawing.Point(154, 22);
            this.connect_b.Name = "connect_b";
            this.connect_b.Size = new System.Drawing.Size(75, 23);
            this.connect_b.TabIndex = 1;
            this.connect_b.Text = "Connect";
            this.connect_b.UseVisualStyleBackColor = true;
            this.connect_b.Click += new System.EventHandler(this.connect_b_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 239);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(284, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(118, 17);
            this.toolStripStatusLabel1.Text = "toolStripStatusLabel1";
            // 
            // richTextBox1
            // 
            this.richTextBox1.BackColor = System.Drawing.SystemColors.MenuText;
            this.richTextBox1.EnableAutoDragDrop = true;
            this.richTextBox1.ForeColor = System.Drawing.Color.Lime;
            this.richTextBox1.Location = new System.Drawing.Point(12, 123);
            this.richTextBox1.MaxLength = 1024;
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(260, 113);
            this.richTextBox1.TabIndex = 3;
            this.richTextBox1.Text = "";
            // 
            // timer3
            // 
            this.timer3.Interval = 30000;
            this.timer3.Tick += new System.EventHandler(this.timer3_Tick);
            // 
            // diskSel
            // 
            this.diskSel.Location = new System.Drawing.Point(49, 62);
            this.diskSel.Maximum = new decimal(new int[] {
            9,
            0,
            0,
            0});
            this.diskSel.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.diskSel.Name = "diskSel";
            this.diskSel.Size = new System.Drawing.Size(46, 20);
            this.diskSel.TabIndex = 4;
            this.diskSel.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.diskSel.ValueChanged += new System.EventHandler(this.diskSel_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 64);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Disk:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(116, 64);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Track: ";
            // 
            // trackSel
            // 
            this.trackSel.Location = new System.Drawing.Point(163, 62);
            this.trackSel.Maximum = new decimal(new int[] {
            99,
            0,
            0,
            0});
            this.trackSel.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackSel.Name = "trackSel";
            this.trackSel.Size = new System.Drawing.Size(46, 20);
            this.trackSel.TabIndex = 7;
            this.trackSel.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackSel.ValueChanged += new System.EventHandler(this.trackSel_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 97);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(43, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Status: ";
            // 
            // status_l
            // 
            this.status_l.AutoSize = true;
            this.status_l.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.status_l.ForeColor = System.Drawing.Color.DarkGreen;
            this.status_l.Location = new System.Drawing.Point(61, 97);
            this.status_l.Name = "status_l";
            this.status_l.Size = new System.Drawing.Size(30, 13);
            this.status_l.TabIndex = 9;
            this.status_l.Text = "N/A";
            // 
            // CDEmu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 261);
            this.Controls.Add(this.status_l);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.trackSel);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.diskSel);
            this.Controls.Add(this.richTextBox1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.connect_b);
            this.Controls.Add(this.port_cb);
            this.Name = "CDEmu";
            this.Text = "IBUS CD Emulator";
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.diskSel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackSel)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.ComboBox port_cb;
        private System.Windows.Forms.Button connect_b;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.Timer timer2;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Timer timer3;
        private System.Windows.Forms.NumericUpDown diskSel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown trackSel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label status_l;
    }
}

