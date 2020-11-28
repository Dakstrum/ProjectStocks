using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp2
{
    public class PopulateListBox
    {

        public static void PopulateCategoriesListBox(ListBox CategoryListBox, Database database_object)
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

        public static void PopulateFirstNameListBox(ListBox NameListBox, Database database_object)
        {

            database_object.OpenConnection();

            string Tool_System_CompanyEvents_query = "SELECT * FROM System_AIFirstNames";
            SQLiteCommand EventSelectCommand = new SQLiteCommand(Tool_System_CompanyEvents_query, database_object.myConnection);
            SQLiteDataReader EventSelectresult = EventSelectCommand.ExecuteReader();

            if (EventSelectresult.HasRows)
                while (EventSelectresult.Read())
                    NameListBox.Items.Add(EventSelectresult["FirstName"]);

            NameListBox.SelectedIndex = NameListBox.Items.Count - 1;
            database_object.CloseConnection();

        }

        public static void PopulateLastNameListBox(ListBox NameListBox, Database database_object)
        {

            database_object.OpenConnection();

            string Tool_System_CompanyEvents_query = "SELECT * FROM System_AILastNames";
            SQLiteCommand EventSelectCommand = new SQLiteCommand(Tool_System_CompanyEvents_query, database_object.myConnection);
            SQLiteDataReader EventSelectresult = EventSelectCommand.ExecuteReader();

            if (EventSelectresult.HasRows)
                while (EventSelectresult.Read())
                    NameListBox.Items.Add(EventSelectresult["LastName"]);

            NameListBox.SelectedIndex = NameListBox.Items.Count - 1;
            database_object.CloseConnection();

        }

        public static void PopulateCardListBox(ListBox CardListBox, Database database_object)
        {

            database_object.OpenConnection();

            string Tool_System_CompanyEvents_query = "SELECT * FROM System_Cards";
            SQLiteCommand EventSelectCommand = new SQLiteCommand(Tool_System_CompanyEvents_query, database_object.myConnection);
            SQLiteDataReader EventSelectresult = EventSelectCommand.ExecuteReader();

            if (EventSelectresult.HasRows)
                while (EventSelectresult.Read())
                    CardListBox.Items.Add(EventSelectresult["CardName"]);

            CardListBox.SelectedIndex = CardListBox.Items.Count - 1;
            database_object.CloseConnection();

        }

    }
}
