using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp2
{
    public class PrimaryKey
    {

        public static void UpdatePrimaryKey(string SQL_TableName, string SQL_PrimaryKeyName, string SQL_ListCompareVar, Database database_object)
        {
            database_object.OpenConnection();

            List<string> list = GetListOfStringColumnsInTable(SQL_TableName, SQL_ListCompareVar, database_object);

            for (int i = 0; i < list.Count; i++)
            {
                string query = "Update " + SQL_TableName + " Set " + SQL_PrimaryKeyName + " = @primarykeyname WHERE " + SQL_ListCompareVar + " = '" + list[i] + "'";
                SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

                myInsertCommand.Parameters.AddWithValue("@primarykeyname", i + 1);
                Console.WriteLine("query : " + query);

                var result = myInsertCommand.ExecuteNonQuery();
            }

            database_object.CloseConnection();

        }

        public static void SubstractOneAfter(string SQL_TableName, string SQL_PrimaryKeyName, string SQL_ListCompareVar, int num_to_sub_after, Database database_object)
        {
            database_object.OpenConnection();

            Console.WriteLine("NUMM<MM" + num_to_sub_after);

            List<int> list = GetListOfIntColumnsInTable(SQL_TableName, SQL_ListCompareVar, database_object);

            for (int i = 0; i < list.Count; i++)
            {
                if (list[i] > num_to_sub_after)
                {
                    list[i] = list[i] - 1; ;
                }
                
                
            }
            for (int i = 0; i < list.Count; i++)
            {
                Console.WriteLine(list[i]);


            }

            UpdateColumnWithList(list, SQL_TableName, SQL_ListCompareVar, SQL_PrimaryKeyName, database_object);

            database_object.CloseConnection();
        }

        public static void UpdateColumnWithList(List<int> list, string SQL_TableName, string SQL_PrimaryKeyName, string Increment, Database database_object)
        {

            database_object.OpenConnection();

            for (int i = 0; i < list.Count; i++)
            {
                int incrementint = i + 1;
                string query = "Update " + SQL_TableName + " Set " + SQL_PrimaryKeyName + " = @primarykeyname WHERE " + Increment + " = " + incrementint;
                SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

                myInsertCommand.Parameters.AddWithValue("@primarykeyname", list[i]);
                Console.WriteLine("query : " + query);

                var result = myInsertCommand.ExecuteNonQuery();
            }

            database_object.CloseConnection();
        }

        public static int GetNextPrimaryKey(string SQL_TableName, string SQL_ListCompareVar, Database database_object)
        {

            database_object.OpenConnection();

            List<string> Items_in_table = new List<string>();

            string select_query = "SELECT " + SQL_ListCompareVar + " FROM " + SQL_TableName;

            Console.WriteLine(select_query);

            using (SQLiteCommand cmd = new SQLiteCommand(select_query, database_object.myConnection))
            {
                using (SQLiteDataReader rdr = cmd.ExecuteReader())
                {
                    while (rdr.Read())
                    {
                        Items_in_table.Add(rdr[SQL_ListCompareVar].ToString());
                    }
                }
            }
            database_object.CloseConnection();
            return Items_in_table.Count + 1;
        }

        public static List<string> GetListOfStringColumnsInTable(string SQL_TableName, string SQL_ListCompareVar, Database database_object)
        {
            database_object.OpenConnection();

            List<string> Items_in_table = new List<string>();

            string select_query = "SELECT " + SQL_ListCompareVar + " FROM " + SQL_TableName;

            Console.WriteLine(select_query);

            using (SQLiteCommand cmd = new SQLiteCommand(select_query, database_object.myConnection))
            {
                using (SQLiteDataReader rdr = cmd.ExecuteReader())
                {
                    while (rdr.Read())
                    {
                        Items_in_table.Add(rdr[SQL_ListCompareVar].ToString());
                    }
                }
            }
            database_object.CloseConnection();
            return Items_in_table;
        }

        public static List<int> GetListOfIntColumnsInTable(string SQL_TableName, string SQL_ListCompareVar, Database database_object)
        {
            database_object.OpenConnection();

            List<int> Items_in_table = new List<int>();

            string select_query = "SELECT " + SQL_ListCompareVar + " FROM " + SQL_TableName;

            Console.WriteLine(select_query);

            using (SQLiteCommand cmd = new SQLiteCommand(select_query, database_object.myConnection))
            {
                using (SQLiteDataReader rdr = cmd.ExecuteReader())
                {
                    while (rdr.Read())
                    {
                        Items_in_table.Add(int.Parse(rdr[SQL_ListCompareVar].ToString()));
                    }
                }
            }
            database_object.CloseConnection();
            return Items_in_table;
        }

        public static int GetAmountOfItemsWithThatName(string SQL_TableName, string SQL_ListCompareVar, string name, Database database_object)
        {
            List<string> list = PrimaryKey.GetListOfStringColumnsInTable(SQL_TableName, SQL_ListCompareVar, database_object);

            int amount_of = 0;
            for (int i = 0; i < list.Count; i++)
            {
                Console.WriteLine(list[i] + " === " + name);
                if (list[i] == name)
                    amount_of++;

            }
            Console.WriteLine("AMOUNT OF: " + amount_of);

            return amount_of;
        }
    }
}
