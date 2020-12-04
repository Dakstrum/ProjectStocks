using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp2
{
    public class DeleteFunctions
    {

        public static void DeleteRow(string SQL_Table, string SQL_Condition, string SQL_SearchCondition, Database database_object)
        {

            database_object.OpenConnection();

            string query = "DELETE FROM "+ SQL_Table + " WHERE " + SQL_Condition + " = " + "'" + SQL_SearchCondition+ "'";
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            Console.WriteLine(query);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

        }

    }
}
