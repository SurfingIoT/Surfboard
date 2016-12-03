namespace RealSenseMqqt
{
    partial class IoT_RealSense_Surfing
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
            this.comboGive5Action = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.comboTarget = new System.Windows.Forms.ComboBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label3 = new System.Windows.Forms.Label();
            this.labelHandClosed = new System.Windows.Forms.Label();
            this.comboHandsClosed = new System.Windows.Forms.ComboBox();
            this.buttonStart = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.textQUEUE = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.textMQTTBroker = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.comboSmile = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.comboSurprise = new System.Windows.Forms.ComboBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // comboGive5Action
            // 
            this.comboGive5Action.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboGive5Action.FormattingEnabled = true;
            this.comboGive5Action.Items.AddRange(new object[] {
            "speaker?1",
            "speaker?0",
            "red?255",
            "red?0",
            "green?255",
            "green?0",
            "blue?255",
            "blue?0",
            "relay?1",
            "relay?0"});
            this.comboGive5Action.Location = new System.Drawing.Point(207, 514);
            this.comboGive5Action.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.comboGive5Action.Name = "comboGive5Action";
            this.comboGive5Action.Size = new System.Drawing.Size(153, 37);
            this.comboGive5Action.TabIndex = 0;
            this.comboGive5Action.Text = "relay?1";
            this.comboGive5Action.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(28, 514);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(121, 29);
            this.label1.TabIndex = 1;
            this.label1.Text = "Give me 5";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // comboTarget
            // 
            this.comboTarget.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboTarget.FormattingEnabled = true;
            this.comboTarget.Items.AddRange(new object[] {
            "*",
            "surfboard3g",
            "surfboard0",
            "surfboard1"});
            this.comboTarget.Location = new System.Drawing.Point(653, 336);
            this.comboTarget.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.comboTarget.Name = "comboTarget";
            this.comboTarget.Size = new System.Drawing.Size(263, 33);
            this.comboTarget.TabIndex = 2;
            this.comboTarget.Text = "*";
            this.comboTarget.SelectedIndexChanged += new System.EventHandler(this.comboBox2_SelectedIndexChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::RealSenseMqqt.Properties.Resources.surfboard;
            this.pictureBox1.Location = new System.Drawing.Point(21, 32);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(896, 459);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 4;
            this.pictureBox1.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Calibri", 14.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(349, 448);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(227, 29);
            this.label3.TabIndex = 5;
            this.label3.Text = "RealSense Surfing IoT";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // labelHandClosed
            // 
            this.labelHandClosed.AutoSize = true;
            this.labelHandClosed.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelHandClosed.Location = new System.Drawing.Point(28, 558);
            this.labelHandClosed.Name = "labelHandClosed";
            this.labelHandClosed.Size = new System.Drawing.Size(165, 29);
            this.labelHandClosed.TabIndex = 7;
            this.labelHandClosed.Text = "Hands Closed";
            // 
            // comboHandsClosed
            // 
            this.comboHandsClosed.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboHandsClosed.FormattingEnabled = true;
            this.comboHandsClosed.Items.AddRange(new object[] {
            "speaker?1",
            "speaker?0",
            "red?255",
            "red?0",
            "green?255",
            "green?0",
            "blue?255",
            "blue?0",
            "relay?1",
            "relay?0"});
            this.comboHandsClosed.Location = new System.Drawing.Point(207, 558);
            this.comboHandsClosed.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.comboHandsClosed.Name = "comboHandsClosed";
            this.comboHandsClosed.Size = new System.Drawing.Size(153, 37);
            this.comboHandsClosed.TabIndex = 6;
            this.comboHandsClosed.Text = "relay?0";
            // 
            // buttonStart
            // 
            this.buttonStart.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonStart.Location = new System.Drawing.Point(805, 506);
            this.buttonStart.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(113, 37);
            this.buttonStart.TabIndex = 8;
            this.buttonStart.Text = "Start";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(649, 305);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(96, 25);
            this.label2.TabIndex = 9;
            this.label2.Text = "Device(s)";
            // 
            // textQUEUE
            // 
            this.textQUEUE.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textQUEUE.Location = new System.Drawing.Point(337, 338);
            this.textQUEUE.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.textQUEUE.Name = "textQUEUE";
            this.textQUEUE.Size = new System.Drawing.Size(263, 30);
            this.textQUEUE.TabIndex = 12;
            this.textQUEUE.Text = "globalcode/things";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(332, 305);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(72, 25);
            this.label5.TabIndex = 13;
            this.label5.Text = "Queue";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(24, 308);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(133, 25);
            this.label4.TabIndex = 15;
            this.label4.Text = "MQTT Broker";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // textMQTTBroker
            // 
            this.textMQTTBroker.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textMQTTBroker.Location = new System.Drawing.Point(33, 338);
            this.textMQTTBroker.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.textMQTTBroker.Name = "textMQTTBroker";
            this.textMQTTBroker.Size = new System.Drawing.Size(263, 30);
            this.textMQTTBroker.TabIndex = 14;
            this.textMQTTBroker.Text = "iot.eclipse.org";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(400, 514);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(75, 29);
            this.label6.TabIndex = 17;
            this.label6.Text = "Smile";
            this.label6.Click += new System.EventHandler(this.label6_Click);
            // 
            // comboSmile
            // 
            this.comboSmile.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboSmile.FormattingEnabled = true;
            this.comboSmile.Items.AddRange(new object[] {
            "speaker?1",
            "speaker?0",
            "red?255",
            "red?0",
            "green?255",
            "green?0",
            "blue?255",
            "blue?0",
            "relay?1",
            "relay?0"});
            this.comboSmile.Location = new System.Drawing.Point(579, 514);
            this.comboSmile.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.comboSmile.Name = "comboSmile";
            this.comboSmile.Size = new System.Drawing.Size(153, 37);
            this.comboSmile.TabIndex = 16;
            this.comboSmile.Text = "speaker?1";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(400, 558);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(104, 29);
            this.label7.TabIndex = 19;
            this.label7.Text = "Surprise";
            // 
            // comboSurprise
            // 
            this.comboSurprise.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboSurprise.FormattingEnabled = true;
            this.comboSurprise.Items.AddRange(new object[] {
            "speaker?1",
            "speaker?0",
            "red?255",
            "red?0",
            "green?255",
            "green?0",
            "blue?255",
            "blue?0",
            "relay?1",
            "relay?0"});
            this.comboSurprise.Location = new System.Drawing.Point(579, 558);
            this.comboSurprise.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.comboSurprise.Name = "comboSurprise";
            this.comboSurprise.Size = new System.Drawing.Size(153, 37);
            this.comboSurprise.TabIndex = 18;
            this.comboSurprise.Text = "speaker?0";
            // 
            // IoT_RealSense_Surfing
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(947, 666);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.comboSurprise);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.comboSmile);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textMQTTBroker);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textQUEUE);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.buttonStart);
            this.Controls.Add(this.labelHandClosed);
            this.Controls.Add(this.comboHandsClosed);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.comboTarget);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.comboGive5Action);
            this.Controls.Add(this.pictureBox1);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "IoT_RealSense_Surfing";
            this.Text = "IoT RealSense Surfing";
            this.Load += new System.EventHandler(this.IoT_RealSense_Surfing_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label labelHandClosed;
        private System.Windows.Forms.Button buttonStart;
        public System.Windows.Forms.ComboBox comboGive5Action;
        public System.Windows.Forms.ComboBox comboTarget;
        public System.Windows.Forms.ComboBox comboHandsClosed;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label6;
        public System.Windows.Forms.ComboBox comboSmile;
        private System.Windows.Forms.Label label7;
        public System.Windows.Forms.ComboBox comboSurprise;
        public System.Windows.Forms.TextBox textQUEUE;
        public System.Windows.Forms.TextBox textMQTTBroker;
    }
}