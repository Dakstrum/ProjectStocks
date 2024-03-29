﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SQLite;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp2
{
    public partial class AINameEditor : Form
    {
        Database database_object = new Database();

        public AINameEditor()
        {
            InitializeComponent();
        }

        private void AINameEditor_Load(object sender, EventArgs e)
        {
            NameComboBox.SelectedIndex = 0;
        }

        private void NameComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (NameComboBox.SelectedIndex == 1)
            {
                NameListBox.Items.Clear();
                PopulateListBox.PopulateLastNameListBox(NameListBox, database_object);
            }
            else
            {
                NameListBox.Items.Clear();
                PopulateListBox.PopulateFirstNameListBox(NameListBox, database_object);
            }


        }

        private void NameListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            {

                try
                {
                    NameEditTextBox.Text = NameListBox.SelectedItem.ToString();

                }
                catch { }

            }
        }

        private void SaveNameButton_Click(object sender, EventArgs e)
        {
            database_object.OpenConnection();

            if (NameComboBox.SelectedIndex == 1)
            {
                
                int category_id = GetFunctions.GetLastNameIdFromLastName(NameListBox.SelectedItem.ToString(), database_object);
                string query = "Update System_AILastNames Set LastName = @lastname Where LastNameId = " + category_id;

                SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

                myInsertCommand.Parameters.AddWithValue("@lastname", NameEditTextBox.Text);

                var result = myInsertCommand.ExecuteNonQuery();


                NameListBox.Items.Clear();
                PopulateListBox.PopulateLastNameListBox(NameListBox, database_object);

            }
            else
            {
               
                int category_id = GetFunctions.GetFirstNameIdFromFirstName(NameListBox.SelectedItem.ToString(), database_object);
                string query = "Update System_AIFirstNames Set FirstName = @firstname Where FirstNameId = " + category_id;

                SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

                myInsertCommand.Parameters.AddWithValue("@firstname", NameEditTextBox.Text);

                var result = myInsertCommand.ExecuteNonQuery();
                

                NameListBox.Items.Clear();
                PopulateListBox.PopulateFirstNameListBox(NameListBox, database_object);
            }

            database_object.CloseConnection();
        }

        private void AddNameButton_Click(object sender, EventArgs e)
        {

            if (NameComboBox.SelectedIndex == 1)
            {
                if (CreateNewFunctions.DoesThisAlreadyExist("System_AILastNames", "LastName", "BLANK", database_object))
                {
                    MessageBox.Show("There is already a BLANK item", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                CreateNewFunctions.CreateNewLastName(database_object);

                NameListBox.Items.Clear();
                PopulateListBox.PopulateLastNameListBox(NameListBox, database_object);

                PrimaryKey.UpdatePrimaryKey("System_AILastNames", "LastNameId", "LastName", database_object);
            }
            else
            {
                if (CreateNewFunctions.DoesThisAlreadyExist("System_AIFirstNames", "FirstName", "BLANK", database_object))
                {
                    MessageBox.Show("There is already a BLANK item", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                CreateNewFunctions.CreateNewFirstName(database_object);
                

                NameListBox.Items.Clear();
                PopulateListBox.PopulateFirstNameListBox(NameListBox, database_object);

                PrimaryKey.UpdatePrimaryKey("System_AIFirstNames", "FirstNameId", "FirstName", database_object);
            }


        }

        private void DeleteNameButton_Click(object sender, EventArgs e)
        {
            if (NameComboBox.SelectedIndex == 1)
            {
                DeleteFunctions.DeleteRow("System_AILastNames", "LastName", NameListBox.SelectedItem.ToString(), database_object);

                NameListBox.Items.Clear();
                PopulateListBox.PopulateLastNameListBox(NameListBox, database_object);

                PrimaryKey.UpdatePrimaryKey("System_AILastNames", "LastNameId", "LastName", database_object);

            }
            else
            {
                DeleteFunctions.DeleteRow("System_AIFirstNames", "FirstName", NameListBox.SelectedItem.ToString(), database_object);

                NameListBox.Items.Clear();
                PopulateListBox.PopulateFirstNameListBox(NameListBox, database_object);

                PrimaryKey.UpdatePrimaryKey("System_AIFirstNames", "FirstNameId", "FirstName", database_object);
            }
        }
    }
}
