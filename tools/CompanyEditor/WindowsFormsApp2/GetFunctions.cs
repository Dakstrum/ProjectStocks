using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp2
{
    public class GetFunctions
    {

        public static int GetCompanyIdFromName(string name, Database database_object)
        {

            database_object.OpenConnection();

            string query = "SELECT * FROM System_Company";
            SQLiteCommand select_command = new SQLiteCommand(query, database_object.myConnection);
            SQLiteDataReader select_result = select_command.ExecuteReader();


            if (select_result.HasRows)
                while (select_result.Read())
                    if (select_result["CompanyName"].ToString() == name)
                        return Int32.Parse(select_result["CompanyId"].ToString());


            database_object.CloseConnection();
            return 0;
        }

        public static int GetCompanyProductIdFromProductName(string product_name, Database database_object)
        {

            database_object.OpenConnection();

            string query = "SELECT * FROM System_CompanyProducts";
            SQLiteCommand select_command = new SQLiteCommand(query, database_object.myConnection);
            SQLiteDataReader select_result = select_command.ExecuteReader();


            if (select_result.HasRows)
                while (select_result.Read())
                    if (select_result["ProductName"].ToString() == product_name)
                        return Int32.Parse(select_result["CompanyProductId"].ToString());


            database_object.CloseConnection();
            return 0;
        }

        public static int GetCompanyEventIdFromEvent(string company_event, Database database_object)
        {

            database_object.OpenConnection();
           
            string query = "SELECT * FROM System_CompanyEvents";
            SQLiteCommand select_command = new SQLiteCommand(query, database_object.myConnection);
            SQLiteDataReader select_result = select_command.ExecuteReader();

            if (select_result.HasRows)
                while (select_result.Read())
                    if (select_result["Event"].ToString() == company_event)
                        return Int32.Parse(select_result["CompanyEventId"].ToString());

            database_object.CloseConnection();
            return 0;
        }

        public static int GetCompanyIconIdFromCompanyId(int company_id, Database database_object)
        {

            database_object.OpenConnection();

            string query = "SELECT * FROM System_CompanyIcons";
            SQLiteCommand select_command = new SQLiteCommand(query, database_object.myConnection);
            SQLiteDataReader select_result = select_command.ExecuteReader();

            if (select_result.HasRows)
                while (select_result.Read())
                    if (Int32.Parse(select_result["CompanyId"].ToString()) == company_id)
                        return Int32.Parse(select_result["CompanyIcon"].ToString());

            database_object.CloseConnection();
            return 0;
        }

        public static int GetGlobalEventIdFromEvent(string global_event, Database database_object)
        {

            database_object.OpenConnection();

            string query = "SELECT * FROM System_GlobalEvents";
            SQLiteCommand select_command = new SQLiteCommand(query, database_object.myConnection);
            SQLiteDataReader select_result = select_command.ExecuteReader();
            
            if (select_result.HasRows)
                while (select_result.Read())
                    if (select_result["Event"].ToString() == global_event)
                        return Int32.Parse(select_result["GlobalEventId"].ToString());

            database_object.CloseConnection();
            return 0;
        }

        public static int GetCategoryEventIdFromEvent(string category_event, Database database_object)
        {

            database_object.OpenConnection();

            string query = "SELECT * FROM System_CategoryEvents";
            SQLiteCommand select_command = new SQLiteCommand(query, database_object.myConnection);
            SQLiteDataReader select_result = select_command.ExecuteReader();

            if (select_result.HasRows)
                while (select_result.Read())
                    if (select_result["Event"].ToString() == category_event)
                        return Int32.Parse(select_result["CategoryEventId"].ToString());

            database_object.CloseConnection();
            return 0;
        }

    }
}
