namespace WindowsFormsApp2
{
    partial class CategoryEditor
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
            this.CategoryListBox = new System.Windows.Forms.ListBox();
            this.CategoryEditTextBox = new System.Windows.Forms.TextBox();
            this.SaveCategoryButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.categoryid = new System.Windows.Forms.Label();
            this.categoryidtext = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // CategoryListBox
            // 
            this.CategoryListBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CategoryListBox.FormattingEnabled = true;
            this.CategoryListBox.ItemHeight = 20;
            this.CategoryListBox.Location = new System.Drawing.Point(12, 12);
            this.CategoryListBox.Name = "CategoryListBox";
            this.CategoryListBox.Size = new System.Drawing.Size(444, 224);
            this.CategoryListBox.TabIndex = 0;
            this.CategoryListBox.SelectedIndexChanged += new System.EventHandler(this.CategoryListBox_SelectedIndexChanged);
            // 
            // CategoryEditTextBox
            // 
            this.CategoryEditTextBox.Location = new System.Drawing.Point(12, 257);
            this.CategoryEditTextBox.Name = "CategoryEditTextBox";
            this.CategoryEditTextBox.Size = new System.Drawing.Size(444, 20);
            this.CategoryEditTextBox.TabIndex = 1;
            // 
            // SaveCategoryButton
            // 
            this.SaveCategoryButton.Location = new System.Drawing.Point(364, 293);
            this.SaveCategoryButton.Name = "SaveCategoryButton";
            this.SaveCategoryButton.Size = new System.Drawing.Size(92, 23);
            this.SaveCategoryButton.TabIndex = 2;
            this.SaveCategoryButton.Text = "Save Category";
            this.SaveCategoryButton.UseVisualStyleBackColor = true;
            this.SaveCategoryButton.Click += new System.EventHandler(this.SaveCategoryButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 298);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(61, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "CategoryId:";
            // 
            // categoryid
            // 
            this.categoryid.AutoSize = true;
            this.categoryid.Location = new System.Drawing.Point(68, 166);
            this.categoryid.Name = "categoryid";
            this.categoryid.Size = new System.Drawing.Size(0, 13);
            this.categoryid.TabIndex = 4;
            // 
            // categoryidtext
            // 
            this.categoryidtext.AutoSize = true;
            this.categoryidtext.Location = new System.Drawing.Point(68, 298);
            this.categoryidtext.Name = "categoryidtext";
            this.categoryidtext.Size = new System.Drawing.Size(13, 13);
            this.categoryidtext.TabIndex = 5;
            this.categoryidtext.Text = "--";
            // 
            // CategoryEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.ClientSize = new System.Drawing.Size(468, 421);
            this.Controls.Add(this.categoryidtext);
            this.Controls.Add(this.categoryid);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.SaveCategoryButton);
            this.Controls.Add(this.CategoryEditTextBox);
            this.Controls.Add(this.CategoryListBox);
            this.Name = "CategoryEditor";
            this.Text = "Category Editor";
            this.Load += new System.EventHandler(this.CategoryEditor_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox CategoryListBox;
        private System.Windows.Forms.TextBox CategoryEditTextBox;
        private System.Windows.Forms.Button SaveCategoryButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label categoryid;
        private System.Windows.Forms.Label categoryidtext;
    }
}