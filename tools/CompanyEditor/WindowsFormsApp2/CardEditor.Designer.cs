namespace WindowsFormsApp2
{
    partial class CardEditor
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
            this.CardIdText = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.SaveCardButton = new System.Windows.Forms.Button();
            this.CardEditTextBox = new System.Windows.Forms.TextBox();
            this.CardListBox = new System.Windows.Forms.ListBox();
            this.CardDescriptionTextBox = new System.Windows.Forms.TextBox();
            this.CardPathTextBox = new System.Windows.Forms.TextBox();
            this.CardPriceModifierTextBox = new System.Windows.Forms.TextBox();
            this.CardModifierLengthTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.AddCardButton = new System.Windows.Forms.Button();
            this.DeleteCardButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // CardIdText
            // 
            this.CardIdText.AutoSize = true;
            this.CardIdText.Location = new System.Drawing.Point(49, 311);
            this.CardIdText.Name = "CardIdText";
            this.CardIdText.Size = new System.Drawing.Size(13, 13);
            this.CardIdText.TabIndex = 10;
            this.CardIdText.Text = "--";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 311);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "CardId:";
            // 
            // SaveCardButton
            // 
            this.SaveCardButton.Location = new System.Drawing.Point(364, 575);
            this.SaveCardButton.Name = "SaveCardButton";
            this.SaveCardButton.Size = new System.Drawing.Size(92, 23);
            this.SaveCardButton.TabIndex = 8;
            this.SaveCardButton.Text = "Save Card";
            this.SaveCardButton.UseVisualStyleBackColor = true;
            this.SaveCardButton.Click += new System.EventHandler(this.SaveCardButton_Click);
            // 
            // CardEditTextBox
            // 
            this.CardEditTextBox.Location = new System.Drawing.Point(12, 279);
            this.CardEditTextBox.Name = "CardEditTextBox";
            this.CardEditTextBox.Size = new System.Drawing.Size(444, 20);
            this.CardEditTextBox.TabIndex = 7;
            // 
            // CardListBox
            // 
            this.CardListBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CardListBox.FormattingEnabled = true;
            this.CardListBox.ItemHeight = 20;
            this.CardListBox.Location = new System.Drawing.Point(12, 12);
            this.CardListBox.Name = "CardListBox";
            this.CardListBox.Size = new System.Drawing.Size(444, 224);
            this.CardListBox.TabIndex = 6;
            this.CardListBox.SelectedIndexChanged += new System.EventHandler(this.CardListBox_SelectedIndexChanged);
            // 
            // CardDescriptionTextBox
            // 
            this.CardDescriptionTextBox.Location = new System.Drawing.Point(12, 351);
            this.CardDescriptionTextBox.Multiline = true;
            this.CardDescriptionTextBox.Name = "CardDescriptionTextBox";
            this.CardDescriptionTextBox.Size = new System.Drawing.Size(444, 60);
            this.CardDescriptionTextBox.TabIndex = 11;
            // 
            // CardPathTextBox
            // 
            this.CardPathTextBox.Location = new System.Drawing.Point(12, 444);
            this.CardPathTextBox.Name = "CardPathTextBox";
            this.CardPathTextBox.Size = new System.Drawing.Size(444, 20);
            this.CardPathTextBox.TabIndex = 12;
            // 
            // CardPriceModifierTextBox
            // 
            this.CardPriceModifierTextBox.Location = new System.Drawing.Point(12, 499);
            this.CardPriceModifierTextBox.Name = "CardPriceModifierTextBox";
            this.CardPriceModifierTextBox.Size = new System.Drawing.Size(50, 20);
            this.CardPriceModifierTextBox.TabIndex = 13;
            // 
            // CardModifierLengthTextBox
            // 
            this.CardModifierLengthTextBox.Location = new System.Drawing.Point(12, 541);
            this.CardModifierLengthTextBox.Name = "CardModifierLengthTextBox";
            this.CardModifierLengthTextBox.Size = new System.Drawing.Size(50, 20);
            this.CardModifierLengthTextBox.TabIndex = 14;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 335);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(60, 13);
            this.label2.TabIndex = 15;
            this.label2.Text = "Description";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 428);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(54, 13);
            this.label3.TabIndex = 16;
            this.label3.Text = "Card Path";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 483);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(71, 13);
            this.label4.TabIndex = 17;
            this.label4.Text = "Price Modifier";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(9, 525);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(80, 13);
            this.label5.TabIndex = 18;
            this.label5.Text = "Modifier Length";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(12, 263);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(60, 13);
            this.label6.TabIndex = 19;
            this.label6.Text = "Card Name";
            // 
            // AddCardButton
            // 
            this.AddCardButton.Location = new System.Drawing.Point(364, 242);
            this.AddCardButton.Name = "AddCardButton";
            this.AddCardButton.Size = new System.Drawing.Size(92, 23);
            this.AddCardButton.TabIndex = 20;
            this.AddCardButton.Text = "Add Card";
            this.AddCardButton.UseVisualStyleBackColor = true;
            this.AddCardButton.Click += new System.EventHandler(this.AddCardButton_Click);
            // 
            // DeleteCardButton
            // 
            this.DeleteCardButton.Location = new System.Drawing.Point(266, 242);
            this.DeleteCardButton.Name = "DeleteCardButton";
            this.DeleteCardButton.Size = new System.Drawing.Size(92, 23);
            this.DeleteCardButton.TabIndex = 21;
            this.DeleteCardButton.Text = "Delete Card";
            this.DeleteCardButton.UseVisualStyleBackColor = true;
            this.DeleteCardButton.Click += new System.EventHandler(this.DeleteCardButton_Click);
            // 
            // CardEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.ClientSize = new System.Drawing.Size(473, 610);
            this.Controls.Add(this.DeleteCardButton);
            this.Controls.Add(this.AddCardButton);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.CardModifierLengthTextBox);
            this.Controls.Add(this.CardPriceModifierTextBox);
            this.Controls.Add(this.CardPathTextBox);
            this.Controls.Add(this.CardDescriptionTextBox);
            this.Controls.Add(this.CardIdText);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.SaveCardButton);
            this.Controls.Add(this.CardEditTextBox);
            this.Controls.Add(this.CardListBox);
            this.Name = "CardEditor";
            this.Text = "CardEditor";
            this.Load += new System.EventHandler(this.CardEditor_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label CardIdText;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button SaveCardButton;
        private System.Windows.Forms.TextBox CardEditTextBox;
        private System.Windows.Forms.ListBox CardListBox;
        private System.Windows.Forms.TextBox CardDescriptionTextBox;
        private System.Windows.Forms.TextBox CardPathTextBox;
        private System.Windows.Forms.TextBox CardPriceModifierTextBox;
        private System.Windows.Forms.TextBox CardModifierLengthTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button AddCardButton;
        private System.Windows.Forms.Button DeleteCardButton;
    }
}