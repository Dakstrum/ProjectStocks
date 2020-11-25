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
            this.EventEditorButton.Location = new System.Drawing.Point(12, 125);
            this.EventEditorButton.Name = "EventEditorButton";
            this.EventEditorButton.Size = new System.Drawing.Size(278, 62);
            this.EventEditorButton.TabIndex = 1;
            this.EventEditorButton.Text = "EventEditor";
            this.EventEditorButton.UseVisualStyleBackColor = true;
            this.EventEditorButton.Click += new System.EventHandler(this.EventEditorButton_Click);
            // 
            // CardEditorButton
            // 
            this.CardEditorButton.Location = new System.Drawing.Point(12, 208);
            this.CardEditorButton.Name = "CardEditorButton";
            this.CardEditorButton.Size = new System.Drawing.Size(278, 62);
            this.CardEditorButton.TabIndex = 2;
            this.CardEditorButton.Text = "CardEditor";
            this.CardEditorButton.UseVisualStyleBackColor = true;
            // 
            // MainMenu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(302, 308);
            this.Controls.Add(this.CardEditorButton);
            this.Controls.Add(this.EventEditorButton);
            this.Controls.Add(this.CompanyEditorButton);
            this.Name = "MainMenu";
            this.Text = "MainMenu";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button CompanyEditorButton;
        private System.Windows.Forms.Button EventEditorButton;
        private System.Windows.Forms.Button CardEditorButton;
    }
}