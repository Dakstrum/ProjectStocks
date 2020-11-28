using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApp2
{
    public class CreateNewFunctions
    {

        public static void CreateNewCompany(Database database_object)
        {

            database_object.OpenConnection();

            string query = "INSERT INTO System_Company ( CategoryId, Ipo, `CompanyName`, `CompanyDescription`, `CompanyAbbreviation`) VALUES (@CategoryId, @Ipo, @CompanyName, @CompanyDescription, @CompanyAbbreviation)";
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@CategoryId", 0);
            myInsertCommand.Parameters.AddWithValue("@Ipo", 0);
            myInsertCommand.Parameters.AddWithValue("@CompanyName", "BLANK");
            myInsertCommand.Parameters.AddWithValue("@CompanyDescription", "BLANK Description");
            myInsertCommand.Parameters.AddWithValue("@CompanyAbbreviation", "---");

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

        }

        public static void CreateNewProduct(int company_id, Database database_object)
        {

            database_object.OpenConnection();

            string query = "INSERT INTO System_CompanyProducts ( CompanyId, `ProductName` ) VALUES (@companyid, @productname)";
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@companyid",    company_id);
            myInsertCommand.Parameters.AddWithValue("@productname", "Blank Product");

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

        }

        public static void CreateNewEvent(int company_id, Database database_object)
        {

            database_object.OpenConnection();

            string query = "INSERT INTO System_CompanyEvents ( CompanyId, `Event`, PriceModifier, ModifierLength ) VALUES (@companyid, @event, @pricemodifier, @modifierlength)";
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@companyid", company_id);
            myInsertCommand.Parameters.AddWithValue("@event", "Blank Event");
            myInsertCommand.Parameters.AddWithValue("@pricemodifier", 0);
            myInsertCommand.Parameters.AddWithValue("@modifierlength", 0);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

        }

        public static void CreateNewIcon(int company_id, Database database_object)
        {
            
            database_object.OpenConnection();

            string query = "INSERT INTO System_CompanyIcons (CompanyId, `IconPath`) VALUES (@companyid, @iconpath)";
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@companyid", company_id);
            myInsertCommand.Parameters.AddWithValue("@iconpath", "");

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

        }

        public static void CreateNewGlobalEvent(Database database_object)
        {

            database_object.OpenConnection();

            string query = "INSERT INTO System_GlobalEvents (`Event`, PriceModifier, ModifierLength) VALUES (@event, @pricemodifier, @modifierlength)";
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@event", "BLANK");
            myInsertCommand.Parameters.AddWithValue("@pricemodifier", 0);
            myInsertCommand.Parameters.AddWithValue("@modifierlength", 0);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

        }

        public static void CreateNewCategoryEvent(Database database_object, int current_category_viewing)
        {

            database_object.OpenConnection();

            string query = "INSERT INTO System_CategoryEvents (CategoryId, `Event`, PriceModifier, ModifierLength) VALUES (@categoryid, @event, @pricemodifier, @modifierlength)";
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@categoryid", current_category_viewing);
            myInsertCommand.Parameters.AddWithValue("@event", "BLANK");
            myInsertCommand.Parameters.AddWithValue("@pricemodifier", 0);
            myInsertCommand.Parameters.AddWithValue("@modifierlength", 0);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

        }

        public static void CreateNewCard(Database database_object)
        {

            database_object.OpenConnection();

            string query = "INSERT INTO System_Cards (CardName, `CardDesc`, `CardPath`, PriceModifier, ModifierLength) VALUES (@cardname, @carddesc, @cardpath, @pricemodifier, @modifierlength)";
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@cardname", "BLANK");
            myInsertCommand.Parameters.AddWithValue("@carddesc", "BLANK DESC");
            myInsertCommand.Parameters.AddWithValue("@cardpath", "assets/images/cards/Temp.png");
            myInsertCommand.Parameters.AddWithValue("@pricemodifier", 0);
            myInsertCommand.Parameters.AddWithValue("@modifierlength", 0);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

        }

        public static void CreateNewFirstName(Database database_object)
        {

            database_object.OpenConnection();

            string query = "INSERT INTO System_AIFirstNames (FirstName) VALUES (@firstname)";
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@firstname", "BLANK");

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

        }

        public static void CreateNewLastName(Database database_object)
        {

            database_object.OpenConnection();

            string query = "INSERT INTO System_AILastNames (LastName) VALUES (@lastname)";
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@lastname", "BLANK");

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

        }
    }
}
