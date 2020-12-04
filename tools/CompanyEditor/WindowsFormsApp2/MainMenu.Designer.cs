namespace WindowsFormsApp2
{
    partial class MainMenu
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
            this.CompanyEditorButton = new System.Windows.Forms.Button();
            this.EventEditorButton = new System.Windows.Forms.Button();
            this.CardEditorButton = new System.Windows.Forms.Button();
            this.CategoryEditorButton = new System.Windows.Forms.Button();
            this.ExportButton = new System.Windows.Forms.Button();
            this.AINameEditorButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // CompanyEditorButton
            // 
            this.CompanyEditorButton.Location = new System.Drawing.Point(12, 39);
            this.CompanyEditorButton.Name = "CompanyEditorButton";
            this.CompanyEditorButton.Size = new System.Drawing.Size(278, 62);
            this.CompanyEditorButton.TabIndex = 0;
            this.CompanyEditorButton.Text = "CompanyEditor";
            this.CompanyEditorButton.UseVisualStyleBackColor = true;
            this.CompanyEditorButton.Click += new System.EventHandler(this.CompanyEditorButton_Click);
            // 
            // EventEditorButton
            // 
            this.EventEditorButton.Location = new System.Drawing.Point(12, 107);
            this.EventEditorButton.Name = "EventEditorButton";
            this.EventEditorButton.Size = new System.Drawing.Size(278, 62);
            this.EventEditorButton.TabIndex = 1;
            this.EventEditorButton.Text = "EventEditor";
            this.EventEditorButton.UseVisualStyleBackColor = true;
            this.EventEditorButton.Click += new System.EventHandler(this.EventEditorButton_Click);
            // 
            // CardEditorButton
            // 
            this.CardEditorButton.Location = new System.Drawing.Point(12, 243);
            this.CardEditorButton.Name = "CardEditorButton";
            this.CardEditorButton.Size = new System.Drawing.Size(278, 62);
            this.CardEditorButton.TabIndex = 2;
            this.CardEditorButton.Text = "CardEditor";
            this.CardEditorButton.UseVisualStyleBackColor = true;
            this.CardEditorButton.Click += new System.EventHandler(this.CardEditorButton_Click);
            // 
            // CategoryEditorButton
            // 
            this.CategoryEditorButton.Location = new System.Drawing.Point(12, 175);
            this.CategoryEditorButton.Name = "CategoryEditorButton";
            this.CategoryEditorButton.Size = new System.Drawing.Size(278, 62);
            this.CategoryEditorButton.TabIndex = 3;
            this.CategoryEditorButton.Text = "Category Editor";
            this.CategoryEditorButton.UseVisualStyleBackColor = true;
            this.CategoryEditorButton.Click += new System.EventHandler(this.CategoryEditorButton_Click);
            // 
            // ExportButton
            // 
            this.ExportButton.BackColor = System.Drawing.Color.MediumSeaGreen;
            this.ExportButton.Location = new System.Drawing.Point(46, 404);
            this.ExportButton.Name = "ExportButton";
            this.ExportButton.Size = new System.Drawing.Size(205, 50);
            this.ExportButton.TabIndex = 27;
            this.ExportButton.Text = "Export To DB file";
            this.ExportButton.UseVisualStyleBackColor = false;
            this.ExportButton.Click += new System.EventHandler(this.ExportButton_Click);
            // 
            // AINameEditorButton
            // 
            this.AINameEditorButton.Location = new System.Drawing.Point(12, 311);
            this.AINameEditorButton.Name = "AINameEditorButton";
            this.AINameEditorButton.Size = new System.Drawing.Size(278, 62);
            this.AINameEditorButton.TabIndex = 28;
            this.AINameEditorButton.Text = "AINameEditor";
            this.AINameEditorButton.UseVisualStyleBackColor = true;
            this.AINameEditorButton.Click += new System.EventHandler(this.AINameEditorButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(5, 444);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(22, 13);
            this.label1.TabIndex = 29;
            this.label1.Text = "1.0";
            // 
            // MainMenu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.ClientSize = new System.Drawing.Size(305, 466);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.AINameEditorButton);
            this.Controls.Add(this.ExportButton);
            this.Controls.Add(this.CategoryEditorButton);
            this.Controls.Add(this.CardEditorButton);
            this.Controls.Add(this.EventEditorButton);
            this.Controls.Add(this.CompanyEditorButton);
            this.Name = "MainMenu";
            this.Text = "MainMenu";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button CompanyEditorButton;
        private System.Windows.Forms.Button EventEditorButton;
        private System.Windows.Forms.Button CardEditorButton;
        private System.Windows.Forms.Button CategoryEditorButton;
        private System.Windows.Forms.Button ExportButton;
        private System.Windows.Forms.Button AINameEditorButton;
        private System.Windows.Forms.Label label1;
    }
}