namespace WindowsFormsApp2
{
    partial class CompanyEditor
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
            this.EventListBox = new System.Windows.Forms.ListBox();
            this.ProductsListBox = new System.Windows.Forms.ListBox();
            this.ProductsEditTextBox = new System.Windows.Forms.TextBox();
            this.Companies = new System.Windows.Forms.Label();
            this.News = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.ProductsSaveButton = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.CompanyNameTextBox = new System.Windows.Forms.TextBox();
            this.DescriptionTextBox = new System.Windows.Forms.TextBox();
            this.AbbreviationTexBox = new System.Windows.Forms.TextBox();
            this.IPOTextBox = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.CompaniesListBox = new System.Windows.Forms.ListBox();
            this.AddProductButton = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.AddEventButton = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.AddCompanyButton = new System.Windows.Forms.Button();
            this.CompanyIdText = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.EventSaveButton = new System.Windows.Forms.Button();
            this.EventsEditTextBox = new System.Windows.Forms.TextBox();
            this.CompanySaveButton = new System.Windows.Forms.Button();
            this.PriceModifierTextBox = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.ModifierLengthTextBox = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.CompanyProductIdText = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.CompanyEventIdText = new System.Windows.Forms.Label();
            this.CategoryIdComboBox = new System.Windows.Forms.ComboBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.label12 = new System.Windows.Forms.Label();
            this.IconPathTextBox = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.CompanyIconId = new System.Windows.Forms.Label();
            this.IconPathSaveButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            this.SuspendLayout();
            // 
            // EventListBox
            // 
            this.EventListBox.FormattingEnabled = true;
            this.EventListBox.Location = new System.Drawing.Point(919, 49);
            this.EventListBox.Name = "EventListBox";
            this.EventListBox.Size = new System.Drawing.Size(573, 212);
            this.EventListBox.TabIndex = 2;
            this.EventListBox.SelectedIndexChanged += new System.EventHandler(this.EventListBox_SelectedIndexChanged);
            // 
            // ProductsListBox
            // 
            this.ProductsListBox.FormattingEnabled = true;
            this.ProductsListBox.Location = new System.Drawing.Point(512, 49);
            this.ProductsListBox.Name = "ProductsListBox";
            this.ProductsListBox.Size = new System.Drawing.Size(265, 212);
            this.ProductsListBox.TabIndex = 3;
            this.ProductsListBox.SelectedIndexChanged += new System.EventHandler(this.ProductsListBox_SelectedIndexChanged);
            // 
            // ProductsEditTextBox
            // 
            this.ProductsEditTextBox.Location = new System.Drawing.Point(512, 293);
            this.ProductsEditTextBox.Name = "ProductsEditTextBox";
            this.ProductsEditTextBox.Size = new System.Drawing.Size(265, 20);
            this.ProductsEditTextBox.TabIndex = 4;
            // 
            // Companies
            // 
            this.Companies.AutoSize = true;
            this.Companies.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Companies.Location = new System.Drawing.Point(12, 15);
            this.Companies.Name = "Companies";
            this.Companies.Size = new System.Drawing.Size(151, 31);
            this.Companies.TabIndex = 5;
            this.Companies.Text = "Companies";
            // 
            // News
            // 
            this.News.AutoSize = true;
            this.News.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.News.Location = new System.Drawing.Point(506, 15);
            this.News.Name = "News";
            this.News.Size = new System.Drawing.Size(122, 31);
            this.News.TabIndex = 6;
            this.News.Text = "Products";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(913, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 31);
            this.label1.TabIndex = 7;
            this.label1.Text = "Event";
            // 
            // ProductsSaveButton
            // 
            this.ProductsSaveButton.Location = new System.Drawing.Point(702, 319);
            this.ProductsSaveButton.Name = "ProductsSaveButton";
            this.ProductsSaveButton.Size = new System.Drawing.Size(75, 23);
            this.ProductsSaveButton.TabIndex = 8;
            this.ProductsSaveButton.Text = "Save";
            this.ProductsSaveButton.UseVisualStyleBackColor = true;
            this.ProductsSaveButton.Click += new System.EventHandler(this.ProductsSaveButton_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 379);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "CompanyName";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 427);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(104, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "CompanyDescription";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 514);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(110, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "CompanyAbbreviation";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(13, 563);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(49, 13);
            this.label5.TabIndex = 12;
            this.label5.Text = "Category";
            // 
            // CompanyNameTextBox
            // 
            this.CompanyNameTextBox.Location = new System.Drawing.Point(15, 395);
            this.CompanyNameTextBox.Name = "CompanyNameTextBox";
            this.CompanyNameTextBox.Size = new System.Drawing.Size(180, 20);
            this.CompanyNameTextBox.TabIndex = 13;
            // 
            // DescriptionTextBox
            // 
            this.DescriptionTextBox.Location = new System.Drawing.Point(15, 443);
            this.DescriptionTextBox.Multiline = true;
            this.DescriptionTextBox.Name = "DescriptionTextBox";
            this.DescriptionTextBox.Size = new System.Drawing.Size(346, 68);
            this.DescriptionTextBox.TabIndex = 14;
            // 
            // AbbreviationTexBox
            // 
            this.AbbreviationTexBox.Location = new System.Drawing.Point(15, 530);
            this.AbbreviationTexBox.Name = "AbbreviationTexBox";
            this.AbbreviationTexBox.Size = new System.Drawing.Size(101, 20);
            this.AbbreviationTexBox.TabIndex = 15;
            // 
            // IPOTextBox
            // 
            this.IPOTextBox.Location = new System.Drawing.Point(16, 357);
            this.IPOTextBox.Name = "IPOTextBox";
            this.IPOTextBox.Size = new System.Drawing.Size(45, 20);
            this.IPOTextBox.TabIndex = 18;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(13, 341);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(25, 13);
            this.label6.TabIndex = 17;
            this.label6.Text = "IPO";
            // 
            // CompaniesListBox
            // 
            this.CompaniesListBox.FormattingEnabled = true;
            this.CompaniesListBox.Location = new System.Drawing.Point(12, 49);
            this.CompaniesListBox.Name = "CompaniesListBox";
            this.CompaniesListBox.Size = new System.Drawing.Size(349, 212);
            this.CompaniesListBox.TabIndex = 19;
            this.CompaniesListBox.SelectedIndexChanged += new System.EventHandler(this.CompaniesListBox_SelectedIndexChanged);
            // 
            // AddProductButton
            // 
            this.AddProductButton.Location = new System.Drawing.Point(702, 267);
            this.AddProductButton.Name = "AddProductButton";
            this.AddProductButton.Size = new System.Drawing.Size(75, 20);
            this.AddProductButton.TabIndex = 20;
            this.AddProductButton.Text = "Add Product";
            this.AddProductButton.UseVisualStyleBackColor = true;
            this.AddProductButton.Click += new System.EventHandler(this.AddProductButton_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(596, 267);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(102, 20);
            this.button2.TabIndex = 21;
            this.button2.Text = "Delete Product";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // AddEventButton
            // 
            this.AddEventButton.Location = new System.Drawing.Point(1394, 267);
            this.AddEventButton.Name = "AddEventButton";
            this.AddEventButton.Size = new System.Drawing.Size(98, 20);
            this.AddEventButton.TabIndex = 22;
            this.AddEventButton.Text = "Add Event";
            this.AddEventButton.UseVisualStyleBackColor = true;
            this.AddEventButton.Click += new System.EventHandler(this.AddEventButton_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(1256, 267);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(132, 20);
            this.button4.TabIndex = 23;
            this.button4.Text = "Delete Event";
            this.button4.UseVisualStyleBackColor = true;
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(167, 267);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(93, 20);
            this.button5.TabIndex = 24;
            this.button5.Text = "Delete Company";
            this.button5.UseVisualStyleBackColor = true;
            // 
            // AddCompanyButton
            // 
            this.AddCompanyButton.Location = new System.Drawing.Point(266, 267);
            this.AddCompanyButton.Name = "AddCompanyButton";
            this.AddCompanyButton.Size = new System.Drawing.Size(92, 20);
            this.AddCompanyButton.TabIndex = 25;
            this.AddCompanyButton.Text = "Add Company";
            this.AddCompanyButton.UseVisualStyleBackColor = true;
            this.AddCompanyButton.Click += new System.EventHandler(this.AddCompanyButton_Click);
            // 
            // CompanyIdText
            // 
            this.CompanyIdText.AutoSize = true;
            this.CompanyIdText.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.CompanyIdText.Location = new System.Drawing.Point(67, 313);
            this.CompanyIdText.Name = "CompanyIdText";
            this.CompanyIdText.Size = new System.Drawing.Size(35, 13);
            this.CompanyIdText.TabIndex = 27;
            this.CompanyIdText.Text = "label7";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(7, 313);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(63, 13);
            this.label7.TabIndex = 28;
            this.label7.Text = "CompanyId:";
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.pictureBox1.Location = new System.Drawing.Point(428, -4);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(10, 769);
            this.pictureBox1.TabIndex = 29;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.pictureBox2.Location = new System.Drawing.Point(847, -34);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(10, 769);
            this.pictureBox2.TabIndex = 30;
            this.pictureBox2.TabStop = false;
            // 
            // EventSaveButton
            // 
            this.EventSaveButton.Location = new System.Drawing.Point(1417, 443);
            this.EventSaveButton.Name = "EventSaveButton";
            this.EventSaveButton.Size = new System.Drawing.Size(75, 23);
            this.EventSaveButton.TabIndex = 32;
            this.EventSaveButton.Text = "Save";
            this.EventSaveButton.UseVisualStyleBackColor = true;
            this.EventSaveButton.Click += new System.EventHandler(this.EventSaveButton_Click);
            // 
            // EventsEditTextBox
            // 
            this.EventsEditTextBox.Location = new System.Drawing.Point(919, 293);
            this.EventsEditTextBox.Name = "EventsEditTextBox";
            this.EventsEditTextBox.Size = new System.Drawing.Size(569, 20);
            this.EventsEditTextBox.TabIndex = 31;
            // 
            // CompanySaveButton
            // 
            this.CompanySaveButton.Location = new System.Drawing.Point(336, 660);
            this.CompanySaveButton.Name = "CompanySaveButton";
            this.CompanySaveButton.Size = new System.Drawing.Size(75, 23);
            this.CompanySaveButton.TabIndex = 33;
            this.CompanySaveButton.Text = "Save";
            this.CompanySaveButton.UseVisualStyleBackColor = true;
            this.CompanySaveButton.Click += new System.EventHandler(this.CompanySaveButton_Click);
            // 
            // PriceModifierTextBox
            // 
            this.PriceModifierTextBox.Location = new System.Drawing.Point(919, 372);
            this.PriceModifierTextBox.Name = "PriceModifierTextBox";
            this.PriceModifierTextBox.Size = new System.Drawing.Size(65, 20);
            this.PriceModifierTextBox.TabIndex = 36;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(916, 356);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(68, 13);
            this.label8.TabIndex = 35;
            this.label8.Text = "PriceModifier";
            // 
            // ModifierLengthTextBox
            // 
            this.ModifierLengthTextBox.Location = new System.Drawing.Point(919, 410);
            this.ModifierLengthTextBox.Name = "ModifierLengthTextBox";
            this.ModifierLengthTextBox.Size = new System.Drawing.Size(74, 20);
            this.ModifierLengthTextBox.TabIndex = 38;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(916, 394);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(77, 13);
            this.label9.TabIndex = 37;
            this.label9.Text = "ModifierLength";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(444, 324);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(100, 13);
            this.label10.TabIndex = 40;
            this.label10.Text = "CompanyProductId:";
            // 
            // CompanyProductIdText
            // 
            this.CompanyProductIdText.AutoSize = true;
            this.CompanyProductIdText.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.CompanyProductIdText.Location = new System.Drawing.Point(541, 324);
            this.CompanyProductIdText.Name = "CompanyProductIdText";
            this.CompanyProductIdText.Size = new System.Drawing.Size(35, 13);
            this.CompanyProductIdText.TabIndex = 39;
            this.CompanyProductIdText.Text = "label7";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(919, 324);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(91, 13);
            this.label11.TabIndex = 42;
            this.label11.Text = "CompanyEventId:";
            // 
            // CompanyEventIdText
            // 
            this.CompanyEventIdText.AutoSize = true;
            this.CompanyEventIdText.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.CompanyEventIdText.Location = new System.Drawing.Point(1007, 324);
            this.CompanyEventIdText.Name = "CompanyEventIdText";
            this.CompanyEventIdText.Size = new System.Drawing.Size(35, 13);
            this.CompanyEventIdText.TabIndex = 41;
            this.CompanyEventIdText.Text = "label7";
            // 
            // CategoryIdComboBox
            // 
            this.CategoryIdComboBox.FormattingEnabled = true;
            this.CategoryIdComboBox.Location = new System.Drawing.Point(15, 579);
            this.CategoryIdComboBox.Name = "CategoryIdComboBox";
            this.CategoryIdComboBox.Size = new System.Drawing.Size(121, 21);
            this.CategoryIdComboBox.TabIndex = 43;
            // 
            // pictureBox3
            // 
            this.pictureBox3.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.pictureBox3.Location = new System.Drawing.Point(428, 556);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(429, 10);
            this.pictureBox3.TabIndex = 44;
            this.pictureBox3.TabStop = false;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.Location = new System.Drawing.Point(441, 569);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(66, 31);
            this.label12.TabIndex = 45;
            this.label12.Text = "Icon";
            // 
            // IconPathTextBox
            // 
            this.IconPathTextBox.Location = new System.Drawing.Point(469, 633);
            this.IconPathTextBox.Name = "IconPathTextBox";
            this.IconPathTextBox.Size = new System.Drawing.Size(265, 20);
            this.IconPathTextBox.TabIndex = 46;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(466, 617);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(29, 13);
            this.label13.TabIndex = 47;
            this.label13.Text = "Path";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(444, 669);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(84, 13);
            this.label14.TabIndex = 49;
            this.label14.Text = "CompanyIconId:";
            // 
            // CompanyIconId
            // 
            this.CompanyIconId.AutoSize = true;
            this.CompanyIconId.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.CompanyIconId.Location = new System.Drawing.Point(525, 670);
            this.CompanyIconId.Name = "CompanyIconId";
            this.CompanyIconId.Size = new System.Drawing.Size(35, 13);
            this.CompanyIconId.TabIndex = 48;
            this.CompanyIconId.Text = "label7";
            // 
            // IconPathSaveButton
            // 
            this.IconPathSaveButton.Location = new System.Drawing.Point(702, 664);
            this.IconPathSaveButton.Name = "IconPathSaveButton";
            this.IconPathSaveButton.Size = new System.Drawing.Size(75, 23);
            this.IconPathSaveButton.TabIndex = 50;
            this.IconPathSaveButton.Text = "Save";
            this.IconPathSaveButton.UseVisualStyleBackColor = true;
            this.IconPathSaveButton.Click += new System.EventHandler(this.IconPathSaveButton_Click);
            // 
            // CompanyEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.ClientSize = new System.Drawing.Size(1497, 691);
            this.Controls.Add(this.IconPathSaveButton);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.CompanyIconId);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.IconPathTextBox);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.CategoryIdComboBox);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.CompanyEventIdText);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.CompanyProductIdText);
            this.Controls.Add(this.ModifierLengthTextBox);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.PriceModifierTextBox);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.CompanySaveButton);
            this.Controls.Add(this.EventSaveButton);
            this.Controls.Add(this.EventsEditTextBox);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.CompanyIdText);
            this.Controls.Add(this.AddCompanyButton);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.AddEventButton);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.AddProductButton);
            this.Controls.Add(this.CompaniesListBox);
            this.Controls.Add(this.IPOTextBox);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.AbbreviationTexBox);
            this.Controls.Add(this.DescriptionTextBox);
            this.Controls.Add(this.CompanyNameTextBox);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.ProductsSaveButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.News);
            this.Controls.Add(this.Companies);
            this.Controls.Add(this.ProductsEditTextBox);
            this.Controls.Add(this.ProductsListBox);
            this.Controls.Add(this.EventListBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "CompanyEditor";
            this.Text = "ProjectStocksCompanyEditor";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.ListBox EventListBox;
        private System.Windows.Forms.ListBox ProductsListBox;
        private System.Windows.Forms.TextBox ProductsEditTextBox;
        private System.Windows.Forms.Label Companies;
        private System.Windows.Forms.Label News;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button ProductsSaveButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox CompanyNameTextBox;
        private System.Windows.Forms.TextBox DescriptionTextBox;
        private System.Windows.Forms.TextBox AbbreviationTexBox;
        private System.Windows.Forms.TextBox IPOTextBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ListBox CompaniesListBox;
        private System.Windows.Forms.Button AddProductButton;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button AddEventButton;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button AddCompanyButton;
        private System.Windows.Forms.Label CompanyIdText;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Button EventSaveButton;
        private System.Windows.Forms.TextBox EventsEditTextBox;
        private System.Windows.Forms.Button CompanySaveButton;
        private System.Windows.Forms.TextBox PriceModifierTextBox;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox ModifierLengthTextBox;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label CompanyProductIdText;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label CompanyEventIdText;
        private System.Windows.Forms.ComboBox CategoryIdComboBox;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox IconPathTextBox;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label CompanyIconId;
        private System.Windows.Forms.Button IconPathSaveButton;
    }
}

