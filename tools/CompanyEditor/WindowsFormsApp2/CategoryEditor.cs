using System;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data.SQLite;
using System.Data.SqlClient;
using WindowsFormsApp2;
using System.Reflection;
using System.IO;
using System.Diagnostics;


namespace WindowsFormsApp2
{
    public partial class CategoryEditor : Form
    {

        Database database_object = new Database();

        public CategoryEditor()
        {
            InitializeComponent();
        }

        private void CategoryEditor_Load(object sender, EventArgs e)
        {
            PopulateListBox.PopulateCategoriesListBox(CategoryListBox, database_object);
        }

        void PopulateCategories()
        {

            database_object.OpenConnection();

            string Tool_System_CompanyEvents_query = "SELECT * FROM System_Category";
            SQLiteCommand EventSelectCommand = new SQLiteCommand(Tool_System_CompanyEvents_query, database_object.myConnection);
            SQLiteDataReader EventSelectresult = EventSelectCommand.ExecuteReader();

            if (EventSelectresult.HasRows)
                while (EventSelectresult.Read())
                    CategoryListBox.Items.Add(EventSelectresult["CategoryName"]);

            CategoryListBox.SelectedIndex = CategoryListBox.Items.Count - 1;
            database_object.CloseConnection();

        }

        private void CategoryListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            {

                try
                {
                    CategoryEditTextBox.Text = CategoryListBox.SelectedItem.ToString();
                    categoryidtext.Text = GetFunctions.GetCategoryIdFromCategoryName(CategoryListBox.SelectedItem.ToString(), database_object).ToString();

                }
                catch { }

            }
        }

        private void SaveCategoryButton_Click(object sender, EventArgs e)
        {

            database_object.OpenConnection();
            int category_id = GetFunctions.GetCategoryIdFromCategoryName(CategoryListBox.SelectedItem.ToString(), database_object);

            string query = "Update System_Category Set CategoryName = @categoryname Where Categoryid = " + category_id;
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@categoryname", CategoryEditTextBox.Text);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

            CategoryListBox.Items.Clear();
            PopulateListBox.PopulateCategoriesListBox(CategoryListBox, database_object);
        }
    }
}
