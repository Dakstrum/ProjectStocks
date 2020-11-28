namespace WindowsFormsApp2
{
    partial class EventEditor
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
            this.GlobalEventEditTextBox = new System.Windows.Forms.TextBox();
            this.DeleteGloabalEventButton = new System.Windows.Forms.Button();
            this.AddGlobalEventButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.GlobalEventListBox = new System.Windows.Forms.ListBox();
            this.SaveGloabalEvent = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.GlobalModifierLengthTextBox = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.GlobalPriceModifierTextBox = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.globaleventidtext = new System.Windows.Forms.Label();
            this.CateogryEventIdText = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.CategoryModifierLenghtTextBox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.CategoryPriceModifierTextBox = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.SaveCategoryEventButton = new System.Windows.Forms.Button();
            this.CategoryEditTextBox = new System.Windows.Forms.TextBox();
            this.DeleteCategoryEventButton = new System.Windows.Forms.Button();
            this.AddCategoryEventButton = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.CategoryEventListBox = new System.Windows.Forms.ListBox();
            this.CategoryIdComboBox = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.CategoryEventsEditComboBox = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // GlobalEventEditTextBox
            // 
            this.GlobalEventEditTextBox.Location = new System.Drawing.Point(15, 289);
            this.GlobalEventEditTextBox.Name = "GlobalEventEditTextBox";
            this.GlobalEventEditTextBox.Size = new System.Drawing.Size(652, 20);
            this.GlobalEventEditTextBox.TabIndex = 36;
            // 
            // DeleteGloabalEventButton
            // 
            this.DeleteGloabalEventButton.Location = new System.Drawing.Point(413, 263);
            this.DeleteGloabalEventButton.Name = "DeleteGloabalEventButton";
            this.DeleteGloabalEventButton.Size = new System.Drawing.Size(132, 20);
            this.DeleteGloabalEventButton.TabIndex = 35;
            this.DeleteGloabalEventButton.Text = "Delete Global Event";
            this.DeleteGloabalEventButton.UseVisualStyleBackColor = true;
            // 
            // AddGlobalEventButton
            // 
            this.AddGlobalEventButton.Location = new System.Drawing.Point(547, 263);
            this.AddGlobalEventButton.Name = "AddGlobalEventButton";
            this.AddGlobalEventButton.Size = new System.Drawing.Size(120, 20);
            this.AddGlobalEventButton.TabIndex = 34;
            this.AddGlobalEventButton.Text = "Add Global Event";
            this.AddGlobalEventButton.UseVisualStyleBackColor = true;
            this.AddGlobalEventButton.Click += new System.EventHandler(this.AddGlobalEventButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(6, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(183, 31);
            this.label1.TabIndex = 33;
            this.label1.Text = "Global Events";
            // 
            // GlobalEventListBox
            // 
            this.GlobalEventListBox.FormattingEnabled = true;
            this.GlobalEventListBox.Location = new System.Drawing.Point(12, 45);
            this.GlobalEventListBox.Name = "GlobalEventListBox";
            this.GlobalEventListBox.Size = new System.Drawing.Size(655, 212);
            this.GlobalEventListBox.TabIndex = 32;
            this.GlobalEventListBox.SelectedIndexChanged += new System.EventHandler(this.GlobalEventListBox_SelectedIndexChanged);
            // 
            // SaveGloabalEvent
            // 
            this.SaveGloabalEvent.Location = new System.Drawing.Point(547, 456);
            this.SaveGloabalEvent.Name = "SaveGloabalEvent";
            this.SaveGloabalEvent.Size = new System.Drawing.Size(120, 20);
            this.SaveGloabalEvent.TabIndex = 37;
            this.SaveGloabalEvent.Text = "Save Global Event";
            this.SaveGloabalEvent.UseVisualStyleBackColor = true;
            this.SaveGloabalEvent.Click += new System.EventHandler(this.SaveGloabalEvent_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(12, 317);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(77, 13);
            this.label11.TabIndex = 48;
            this.label11.Text = "GlobalEventId:";
            // 
            // GlobalModifierLengthTextBox
            // 
            this.GlobalModifierLengthTextBox.Location = new System.Drawing.Point(12, 403);
            this.GlobalModifierLengthTextBox.Name = "GlobalModifierLengthTextBox";
            this.GlobalModifierLengthTextBox.Size = new System.Drawing.Size(74, 20);
            this.GlobalModifierLengthTextBox.TabIndex = 47;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(9, 387);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(77, 13);
            this.label9.TabIndex = 46;
            this.label9.Text = "ModifierLength";
            // 
            // GlobalPriceModifierTextBox
            // 
            this.GlobalPriceModifierTextBox.Location = new System.Drawing.Point(12, 365);
            this.GlobalPriceModifierTextBox.Name = "GlobalPriceModifierTextBox";
            this.GlobalPriceModifierTextBox.Size = new System.Drawing.Size(65, 20);
            this.GlobalPriceModifierTextBox.TabIndex = 45;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(9, 349);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(68, 13);
            this.label8.TabIndex = 44;
            this.label8.Text = "PriceModifier";
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.pictureBox1.Location = new System.Drawing.Point(673, -57);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(10, 769);
            this.pictureBox1.TabIndex = 49;
            this.pictureBox1.TabStop = false;
            // 
            // globaleventidtext
            // 
            this.globaleventidtext.AutoSize = true;
            this.globaleventidtext.Location = new System.Drawing.Point(84, 317);
            this.globaleventidtext.Name = "globaleventidtext";
            this.globaleventidtext.Size = new System.Drawing.Size(31, 13);
            this.globaleventidtext.TabIndex = 50;
            this.globaleventidtext.Text = "lablel";
            // 
            // CateogryEventIdText
            // 
            this.CateogryEventIdText.AutoSize = true;
            this.CateogryEventIdText.Location = new System.Drawing.Point(761, 317);
            this.CateogryEventIdText.Name = "CateogryEventIdText";
            this.CateogryEventIdText.Size = new System.Drawing.Size(31, 13);
            this.CateogryEventIdText.TabIndex = 63;
            this.CateogryEventIdText.Text = "lablel";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(689, 317);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(89, 13);
            this.label3.TabIndex = 62;
            this.label3.Text = "CategoryEventId:";
            // 
            // CategoryModifierLenghtTextBox
            // 
            this.CategoryModifierLenghtTextBox.Location = new System.Drawing.Point(689, 403);
            this.CategoryModifierLenghtTextBox.Name = "CategoryModifierLenghtTextBox";
            this.CategoryModifierLenghtTextBox.Size = new System.Drawing.Size(74, 20);
            this.CategoryModifierLenghtTextBox.TabIndex = 61;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(686, 387);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(77, 13);
            this.label4.TabIndex = 60;
            this.label4.Text = "ModifierLength";
            // 
            // CategoryPriceModifierTextBox
            // 
            this.CategoryPriceModifierTextBox.Location = new System.Drawing.Point(689, 365);
            this.CategoryPriceModifierTextBox.Name = "CategoryPriceModifierTextBox";
            this.CategoryPriceModifierTextBox.Size = new System.Drawing.Size(65, 20);
            this.CategoryPriceModifierTextBox.TabIndex = 59;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(686, 349);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(68, 13);
            this.label5.TabIndex = 58;
            this.label5.Text = "PriceModifier";
            // 
            // SaveCategoryEventButton
            // 
            this.SaveCategoryEventButton.Location = new System.Drawing.Point(1196, 456);
            this.SaveCategoryEventButton.Name = "SaveCategoryEventButton";
            this.SaveCategoryEventButton.Size = new System.Drawing.Size(120, 20);
            this.SaveCategoryEventButton.TabIndex = 56;
            this.SaveCategoryEventButton.Text = "Save Category Event";
            this.SaveCategoryEventButton.UseVisualStyleBackColor = true;
            this.SaveCategoryEventButton.Click += new System.EventHandler(this.SaveCategoryEventButton_Click);
            // 
            // CategoryEditTextBox
            // 
            this.CategoryEditTextBox.Location = new System.Drawing.Point(692, 289);
            this.CategoryEditTextBox.Name = "CategoryEditTextBox";
            this.CategoryEditTextBox.Size = new System.Drawing.Size(624, 20);
            this.CategoryEditTextBox.TabIndex = 55;
            // 
            // DeleteCategoryEventButton
            // 
            this.DeleteCategoryEventButton.Location = new System.Drawing.Point(1062, 263);
            this.DeleteCategoryEventButton.Name = "DeleteCategoryEventButton";
            this.DeleteCategoryEventButton.Size = new System.Drawing.Size(132, 20);
            this.DeleteCategoryEventButton.TabIndex = 54;
            this.DeleteCategoryEventButton.Text = "Delete Category Event";
            this.DeleteCategoryEventButton.UseVisualStyleBackColor = true;
            // 
            // AddCategoryEventButton
            // 
            this.AddCategoryEventButton.Location = new System.Drawing.Point(1196, 263);
            this.AddCategoryEventButton.Name = "AddCategoryEventButton";
            this.AddCategoryEventButton.Size = new System.Drawing.Size(120, 20);
            this.AddCategoryEventButton.TabIndex = 53;
            this.AddCategoryEventButton.Text = "Add Category Event";
            this.AddCategoryEventButton.UseVisualStyleBackColor = true;
            this.AddCategoryEventButton.Click += new System.EventHandler(this.AddCategoryEventButton_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(683, 11);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(216, 31);
            this.label6.TabIndex = 52;
            this.label6.Text = "Category Events";
            // 
            // CategoryEventListBox
            // 
            this.CategoryEventListBox.FormattingEnabled = true;
            this.CategoryEventListBox.Location = new System.Drawing.Point(689, 45);
            this.CategoryEventListBox.Name = "CategoryEventListBox";
            this.CategoryEventListBox.Size = new System.Drawing.Size(627, 212);
            this.CategoryEventListBox.TabIndex = 51;
            this.CategoryEventListBox.SelectedIndexChanged += new System.EventHandler(this.CategoryEventListBox_SelectedIndexChanged_1);
            // 
            // CategoryIdComboBox
            // 
            this.CategoryIdComboBox.FormattingEnabled = true;
            this.CategoryIdComboBox.Location = new System.Drawing.Point(905, 23);
            this.CategoryIdComboBox.Name = "CategoryIdComboBox";
            this.CategoryIdComboBox.Size = new System.Drawing.Size(121, 21);
            this.CategoryIdComboBox.TabIndex = 65;
            this.CategoryIdComboBox.SelectedIndexChanged += new System.EventHandler(this.CategoryIdComboBox_SelectedIndexChanged_1);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(903, 7);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(86, 13);
            this.label7.TabIndex = 64;
            this.label7.Text = "Category Search";
            // 
            // CategoryEventsEditComboBox
            // 
            this.CategoryEventsEditComboBox.FormattingEnabled = true;
            this.CategoryEventsEditComboBox.Location = new System.Drawing.Point(689, 444);
            this.CategoryEventsEditComboBox.Name = "CategoryEventsEditComboBox";
            this.CategoryEventsEditComboBox.Size = new System.Drawing.Size(121, 21);
            this.CategoryEventsEditComboBox.TabIndex = 67;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(687, 428);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(49, 13);
            this.label2.TabIndex = 66;
            this.label2.Text = "Category";
            // 
            // EventEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.ClientSize = new System.Drawing.Size(1328, 488);
            this.Controls.Add(this.CategoryEventsEditComboBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.CategoryIdComboBox);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.CateogryEventIdText);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.CategoryModifierLenghtTextBox);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.CategoryPriceModifierTextBox);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.SaveCategoryEventButton);
            this.Controls.Add(this.CategoryEditTextBox);
            this.Controls.Add(this.DeleteCategoryEventButton);
            this.Controls.Add(this.AddCategoryEventButton);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.CategoryEventListBox);
            this.Controls.Add(this.globaleventidtext);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.GlobalModifierLengthTextBox);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.GlobalPriceModifierTextBox);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.SaveGloabalEvent);
            this.Controls.Add(this.GlobalEventEditTextBox);
            this.Controls.Add(this.DeleteGloabalEventButton);
            this.Controls.Add(this.AddGlobalEventButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.GlobalEventListBox);
            this.Name = "EventEditor";
            this.Text = "EventEditor";
            this.Load += new System.EventHandler(this.EventEditor_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox GlobalEventEditTextBox;
        private System.Windows.Forms.Button DeleteGloabalEventButton;
        private System.Windows.Forms.Button AddGlobalEventButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListBox GlobalEventListBox;
        private System.Windows.Forms.Button SaveGloabalEvent;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox GlobalModifierLengthTextBox;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox GlobalPriceModifierTextBox;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label globaleventidtext;
        private System.Windows.Forms.Label CateogryEventIdText;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox CategoryModifierLenghtTextBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox CategoryPriceModifierTextBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button SaveCategoryEventButton;
        private System.Windows.Forms.TextBox CategoryEditTextBox;
        private System.Windows.Forms.Button DeleteCategoryEventButton;
        private System.Windows.Forms.Button AddCategoryEventButton;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ListBox CategoryEventListBox;
        private System.Windows.Forms.ComboBox CategoryIdComboBox;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox CategoryEventsEditComboBox;
        private System.Windows.Forms.Label label2;
    }
}