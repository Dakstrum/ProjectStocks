using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp2
{
    public class PopulateComboBox
    {

        public static void Category(ComboBox CategoryComboBox, Database database_object)
        {

            database_object.OpenConnection();

            string Tool_System_CompanyEvents_query = "SELECT * FROM System_Category";
            SQLiteCommand EventSelectCommand = new SQLiteCommand(Tool_System_CompanyEvents_query, database_object.myConnection);
            SQLiteDataReader EventSelectresult = EventSelectCommand.ExecuteReader();

            if (EventSelectresult.HasRows)
                while (EventSelectresult.Read())
                    CategoryComboBox.Items.Add(EventSelectresult["CategoryName"]);

            database_object.CloseConnection();

        }

    }
}
