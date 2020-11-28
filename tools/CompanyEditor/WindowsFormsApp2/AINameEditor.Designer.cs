namespace WindowsFormsApp2
{
    partial class AINameEditor
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
            this.SaveNameButton = new System.Windows.Forms.Button();
            this.NameEditTextBox = new System.Windows.Forms.TextBox();
            this.DeleteNameButton = new System.Windows.Forms.Button();
            this.AddNameButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.NameListBox = new System.Windows.Forms.ListBox();
            this.NameComboBox = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // SaveNameButton
            // 
            this.SaveNameButton.Location = new System.Drawing.Point(608, 423);
            this.SaveNameButton.Name = "SaveNameButton";
            this.SaveNameButton.Size = new System.Drawing.Size(120, 20);
            this.SaveNameButton.TabIndex = 43;
            this.SaveNameButton.Text = "Save Name";
            this.SaveNameButton.UseVisualStyleBackColor = true;
            this.SaveNameButton.Click += new System.EventHandler(this.SaveNameButton_Click);
            // 
            // NameEditTextBox
            // 
            this.NameEditTextBox.Location = new System.Drawing.Point(76, 387);
            this.NameEditTextBox.Name = "NameEditTextBox";
            this.NameEditTextBox.Size = new System.Drawing.Size(652, 20);
            this.NameEditTextBox.TabIndex = 42;
            // 
            // DeleteNameButton
            // 
            this.DeleteNameButton.Location = new System.Drawing.Point(474, 361);
            this.DeleteNameButton.Name = "DeleteNameButton";
            this.DeleteNameButton.Size = new System.Drawing.Size(132, 20);
            this.DeleteNameButton.TabIndex = 41;
            this.DeleteNameButton.Text = "Delete Name";
            this.DeleteNameButton.UseVisualStyleBackColor = true;
            // 
            // AddNameButton
            // 
            this.AddNameButton.Location = new System.Drawing.Point(608, 361);
            this.AddNameButton.Name = "AddNameButton";
            this.AddNameButton.Size = new System.Drawing.Size(120, 20);
            this.AddNameButton.TabIndex = 40;
            this.AddNameButton.Text = "AddName";
            this.AddNameButton.UseVisualStyleBackColor = true;
            this.AddNameButton.Click += new System.EventHandler(this.AddNameButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(67, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 31);
            this.label1.TabIndex = 39;
            this.label1.Text = "Names";
            // 
            // NameListBox
            // 
            this.NameListBox.FormattingEnabled = true;
            this.NameListBox.Location = new System.Drawing.Point(73, 55);
            this.NameListBox.Name = "NameListBox";
            this.NameListBox.Size = new System.Drawing.Size(655, 290);
            this.NameListBox.TabIndex = 38;
            this.NameListBox.SelectedIndexChanged += new System.EventHandler(this.NameListBox_SelectedIndexChanged);
            // 
            // NameComboBox
            // 
            this.NameComboBox.FormattingEnabled = true;
            this.NameComboBox.Items.AddRange(new object[] {
            "First",
            "Last"});
            this.NameComboBox.Location = new System.Drawing.Point(173, 21);
            this.NameComboBox.Name = "NameComboBox";
            this.NameComboBox.Size = new System.Drawing.Size(121, 21);
            this.NameComboBox.TabIndex = 44;
            this.NameComboBox.SelectedIndexChanged += new System.EventHandler(this.NameComboBox_SelectedIndexChanged);
            // 
            // AINameEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.NameComboBox);
            this.Controls.Add(this.SaveNameButton);
            this.Controls.Add(this.NameEditTextBox);
            this.Controls.Add(this.DeleteNameButton);
            this.Controls.Add(this.AddNameButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.NameListBox);
            this.Name = "AINameEditor";
            this.Text = "AINameEditor";
            this.Load += new System.EventHandler(this.AINameEditor_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button SaveNameButton;
        private System.Windows.Forms.TextBox NameEditTextBox;
        private System.Windows.Forms.Button DeleteNameButton;
        private System.Windows.Forms.Button AddNameButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListBox NameListBox;
        private System.Windows.Forms.ComboBox NameComboBox;
    }
}