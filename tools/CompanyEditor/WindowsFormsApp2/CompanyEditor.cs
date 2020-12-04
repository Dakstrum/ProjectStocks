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
    public partial class CompanyEditor : Form
    {
        Database database_object = new Database();

        public CompanyEditor()
        {

            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

            PopulateComboBox.Category(CategoryIdComboBox, database_object);
            PopulateCompanyList();
            PopulateWithCompany(1);
            CompaniesListBox.SelectedIndex = CompaniesListBox.Items.Count - 1;
            


        }

        void PopulateCompanyList()
        {

            database_object.OpenConnection();

            string Tool_System_Company_query = "SELECT * FROM System_Company";
            SQLiteCommand mySelectCommand    = new SQLiteCommand(Tool_System_Company_query, database_object.myConnection);
            SQLiteDataReader Selectresult    = mySelectCommand.ExecuteReader();

            if (Selectresult.HasRows)
                while (Selectresult.Read())
                    CompaniesListBox.Items.Add(Selectresult["CompanyName"].ToString());

            database_object.CloseConnection();

        }

        void PopulateWithCompany(int company_id)
        {

            PopulateCompanyInfo(company_id);
            PopulateCompanyProductsList(company_id);
            PopulateCompanyEvents(company_id);
            PopulateCompanyIcon(company_id);

        }

        void PopulateCompanyInfo(int company_id)
        {
            
            database_object.OpenConnection();

            string Tool_System_Company_query = "SELECT * FROM System_Company";
            SQLiteCommand mySelectCommand    = new SQLiteCommand(Tool_System_Company_query, database_object.myConnection);
            SQLiteDataReader Selectresult    = mySelectCommand.ExecuteReader();

            if (Selectresult.HasRows)
            {
                while (Selectresult.Read())
                {
                    if (long.Parse(Selectresult["CompanyId"].ToString()) == company_id)
                    {
                        CompanyIdText.Text      = Selectresult["CompanyId"].ToString();
                        IPOTextBox.Text         = Selectresult["Ipo"].ToString();
                        CompanyNameTextBox.Text = Selectresult["CompanyName"].ToString();
                        DescriptionTextBox.Text = Selectresult["CompanyDescription"].ToString();
                        AbbreviationTexBox.Text = Selectresult["CompanyAbbreviation"].ToString();

                        CategoryIdComboBox.SelectedIndex = int.Parse(Selectresult["CategoryId"].ToString()) - 1;

                    }
                }
            }
            database_object.CloseConnection();
        }

        void PopulateCompanyProductsList(int company_id)
        {

            database_object.OpenConnection();

            string query                   = "SELECT * FROM System_CompanyProducts";
            SQLiteCommand select_command   = new SQLiteCommand(query, database_object.myConnection);
            SQLiteDataReader select_result = select_command.ExecuteReader();

            if (select_result.HasRows)
            {
                while (select_result.Read())
                {
                    if ((int)select_result["CompanyId"] == company_id)
                    {
             
                        ProductsEditTextBox.Text = select_result["ProductName"].ToString();
                        CompanyProductIdText.Text  = GetFunctions.GetCompanyProductIdFromProductName(select_result["ProductName"].ToString(), database_object).ToString();

                        ProductsListBox.Items.Add(select_result["ProductName"]);

                    }                 
                }
            }

            ProductsListBox.SelectedIndex = ProductsListBox.Items.Count - 1;
            database_object.CloseConnection();

        }

        void PopulateCompanyEvents(int company_id)
        {

            database_object.OpenConnection();

            string Tool_System_CompanyEvents_query = "SELECT * FROM System_CompanyEvents";
            SQLiteCommand EventSelectCommand       = new SQLiteCommand(Tool_System_CompanyEvents_query, database_object.myConnection);
            SQLiteDataReader EventSelectresult     = EventSelectCommand.ExecuteReader();

            if (EventSelectresult.HasRows)
                while (EventSelectresult.Read())
                    if ((int)EventSelectresult["CompanyId"] == company_id)
                        EventListBox.Items.Add(EventSelectresult["Event"]);

            EventListBox.SelectedIndex = EventListBox.Items.Count - 1;
            database_object.CloseConnection();

        }

        void PopulateCompanyIcon(int company_id)
        {

            database_object.OpenConnection();

            string Tool_System_Company_query = "SELECT * FROM System_CompanyIcons";

            SQLiteCommand mySelectCommand = new SQLiteCommand(Tool_System_Company_query, database_object.myConnection);
            SQLiteDataReader Selectresult = mySelectCommand.ExecuteReader();

            if (Selectresult.HasRows)
            {
                while (Selectresult.Read())
                {
                    if (long.Parse(Selectresult["CompanyId"].ToString()) == company_id)
                    {
                        IconPathTextBox.Text = Selectresult["IconPath"].ToString();
                        CompanyIconId.Text   = Selectresult["CompanyIcon"].ToString();


                    }
                }
            }
            database_object.CloseConnection();

        }

        void PopulateCompanyEventsModifiers(int company_event_id)
        {

            database_object.OpenConnection();

            string Tool_System_CompanyEvents_query = "SELECT * FROM System_CompanyEvents";
            SQLiteCommand EventSelectCommand = new SQLiteCommand(Tool_System_CompanyEvents_query, database_object.myConnection);
            SQLiteDataReader EventSelectresult = EventSelectCommand.ExecuteReader();
            if (EventSelectresult.HasRows)
            {
                while (EventSelectresult.Read())
                {
                    if (EventSelectresult["CompanyEventId"].ToString() == company_event_id.ToString())
                    {

                        PriceModifierTextBox.Text  = EventSelectresult["PriceModifier"].ToString();
                        ModifierLengthTextBox.Text = EventSelectresult["ModifierLength"].ToString();
                        CompanyEventIdText.Text    = EventSelectresult["CompanyEventId"].ToString();
                        

                    }
                }
            }     
            database_object.CloseConnection();

        }

        void ClearProductAndEventDisplays()
        {

            ProductsListBox.Items.Clear();
            EventListBox.Items.Clear();

            ProductsEditTextBox.Clear();
            EventsEditTextBox.Clear();
            PriceModifierTextBox.Clear();
            ModifierLengthTextBox.Clear();
            IconPathTextBox.Clear();

            CompanyProductIdText.Text = "";
            CompanyEventIdText.Text   = "";
            CompanyIconId.Text        = "";

        }

        void ClearCompaniesList()
        {
            CompaniesListBox.Items.Clear();
        }

        void ClearCompanyProductDisplay()
        {
            ProductsListBox.Items.Clear();
        }

        void ClearCompanyEventsDisplay()
        {
            EventListBox.Items.Clear();
        }

        void UpdateCompanyInfo(int company_id)
        {

            database_object.OpenConnection();

            string query = "Update System_Company Set CategoryId = @categoryid, Ipo = @ipo, CompanyName = @companyname, CompanyDescription = @companydescription, CompanyAbbreviation = @companyabbreviation Where CompanyId = " + company_id;
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@categoryid", CategoryIdComboBox.SelectedIndex + 1);
            myInsertCommand.Parameters.AddWithValue("@ipo", IPOTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@companyname", CompanyNameTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@companydescription", DescriptionTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@companyabbreviation", AbbreviationTexBox.Text);
            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

            ClearCompaniesList();
            PopulateCompanyList();

            CompaniesListBox.SelectedIndex = CompaniesListBox.Items.Count - 1;

        }

        void UpdateCompanyProducts(int company_product_id)
        {

            database_object.OpenConnection();

            string query = "Update System_CompanyProducts Set ProductName = @productname Where CompanyProductId = " + company_product_id;
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@productname", ProductsEditTextBox.Text);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

            ClearCompanyProductDisplay();
            PopulateCompanyProductsList(int.Parse(CompanyIdText.Text));

        }

        void UpdateCompanyEvents(int company_event_id)
        {

            database_object.OpenConnection();
            string query = "Update System_CompanyEvents Set Event = @event, PriceModifier = @pricemodifier, ModifierLength = @modifierlength Where CompanyEventId = " + company_event_id;
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@event",          EventsEditTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@pricemodifier",  PriceModifierTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@modifierlength", ModifierLengthTextBox.Text);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

            ClearCompanyEventsDisplay();
            PopulateCompanyEvents(int.Parse(CompanyIdText.Text));

        }

        void UpdateCompanyIcon(int company_icon_id)
        {

            database_object.OpenConnection();
            string query = "Update System_CompanyIcons Set CompanyId = @companyid, IconPath = @iconpath Where CompanyIcon = " + company_icon_id;
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@companyid", int.Parse(CompanyIdText.Text));
            myInsertCommand.Parameters.AddWithValue("@iconpath", IconPathTextBox.Text);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

        }

        List<int> add_company_list_box = new List<int>();

        void CompaniesListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            ClearProductAndEventDisplays();
            try
            {   

                PopulateWithCompany(GetFunctions.GetCompanyIdFromName(CompaniesListBox.SelectedItem.ToString(), database_object));
                
            }
            catch { }
            
            
        }

        private void ProductsListBox_SelectedIndexChanged(object sender, EventArgs e)
        {

            try
            {
                ProductsEditTextBox.Text = ProductsListBox.SelectedItem.ToString();
                CompanyProductIdText.Text = GetFunctions.GetCompanyProductIdFromProductName(ProductsListBox.SelectedItem.ToString(), database_object).ToString();
            }
            catch {}

        }


        private void EventListBox_SelectedIndexChanged(object sender, EventArgs e)
        {

            try
            {
                EventsEditTextBox.Text = EventListBox.SelectedItem.ToString();
                PopulateCompanyEventsModifiers(GetFunctions.GetCompanyEventIdFromEvent(EventListBox.SelectedItem.ToString(), database_object));
                
            }
            catch { }

        }

        void CompanySaveButton_Click(object sender, EventArgs e)
        {
            UpdateCompanyInfo(GetFunctions.GetCompanyIdFromName(CompaniesListBox.SelectedItem.ToString(), database_object));
        }

        private void ProductsSaveButton_Click(object sender, EventArgs e)
        {
            UpdateCompanyProducts(GetFunctions.GetCompanyProductIdFromProductName(ProductsListBox.SelectedItem.ToString(), database_object));
        }

        private void EventSaveButton_Click(object sender, EventArgs e)
        {
            UpdateCompanyEvents(GetFunctions.GetCompanyEventIdFromEvent(EventListBox.SelectedItem.ToString(), database_object));
        }

        private void IconPathSaveButton_Click(object sender, EventArgs e)
        {
            Console.WriteLine(GetFunctions.GetCompanyIconIdFromCompanyId(int.Parse(CompanyIdText.Text), database_object));
            UpdateCompanyIcon(GetFunctions.GetCompanyIconIdFromCompanyId(int.Parse(CompanyIdText.Text), database_object));

        }

        

        private void AddCompanyButton_Click(object sender, EventArgs e)
        {

            if (CreateNewFunctions.DoesThisAlreadyExist("System_Company", "CompanyName", "BLANK", database_object))
            {
                MessageBox.Show("There is already a BLANK item", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            CreateNewFunctions.CreateNewCompany(database_object);
            CreateNewFunctions.CreateNewIcon(int.Parse(CompanyIdText.Text) + 1, database_object);

            ClearCompaniesList();
            PopulateCompanyList();

            PrimaryKey.UpdatePrimaryKey("System_Company", "CompanyId", "CompanyName", database_object);

            CompaniesListBox.SelectedIndex = CompaniesListBox.Items.Count - 1;


        }

        private void AddProductButton_Click(object sender, EventArgs e)
        {
            if (CreateNewFunctions.DoesThisAlreadyExist("System_CompanyProducts", "ProductName", "Blank Product", database_object))
            {
                MessageBox.Show("There is already a BLANK item", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            CreateNewFunctions.CreateNewProduct(int.Parse(CompanyIdText.Text), database_object);
            PrimaryKey.UpdatePrimaryKey("System_CompanyProducts", "CompanyProductId", "ProductName", database_object);

            ClearCompanyProductDisplay();
            PopulateCompanyProductsList(int.Parse(CompanyIdText.Text));
        }

        private void AddEventButton_Click(object sender, EventArgs e)
        {
            if (CreateNewFunctions.DoesThisAlreadyExist("System_CompanyEvents", "Event", "Blank", database_object))
            {
                MessageBox.Show("There is already a BLANK item", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            CreateNewFunctions.CreateNewEvent(int.Parse(CompanyIdText.Text), database_object);
            PrimaryKey.UpdatePrimaryKey("System_CompanyEvents", "CompanyEventId", "Event", database_object);

            ClearCompanyEventsDisplay();
            PopulateCompanyEvents(int.Parse(CompanyIdText.Text));
        }

        private void DeleteCompanyButton_Click(object sender, EventArgs e)
        {

            DeleteFunctions.DeleteRow("System_CompanyIcons",    "CompanyId", GetFunctions.GetCompanyIdFromName(CompaniesListBox.SelectedItem.ToString(), database_object).ToString(), database_object);
            DeleteFunctions.DeleteRow("System_CompanyEvents",   "CompanyId", GetFunctions.GetCompanyIdFromName(CompaniesListBox.SelectedItem.ToString(), database_object).ToString(), database_object);
            DeleteFunctions.DeleteRow("System_CompanyProducts", "CompanyId", GetFunctions.GetCompanyIdFromName(CompaniesListBox.SelectedItem.ToString(), database_object).ToString(), database_object);
            DeleteFunctions.DeleteRow("System_Company",         "CompanyName", CompaniesListBox.SelectedItem.ToString(), database_object);

            CompaniesListBox.Items.Clear();
            PopulateCompanyList();

            PrimaryKey.UpdatePrimaryKey("System_CompanyIcons", "CompanyId",   "CompanyId", database_object);
            PrimaryKey.UpdatePrimaryKey("System_CompanyIcons", "CompanyIcon", "CompanyId", database_object);

            PrimaryKey.UpdatePrimaryKey("System_CompanyEvents",  "CompanyEventId", "Event",     database_object);
            PrimaryKey.SubstractOneAfter("System_CompanyEvents", "CompanyEventId", "CompanyId", int.Parse(CompanyIdText.Text), database_object);

            PrimaryKey.UpdatePrimaryKey("System_CompanyProducts",  "CompanyProductId", "ProductName", database_object);
            PrimaryKey.SubstractOneAfter("System_CompanyProducts", "CompanyProductId", "CompanyId",      int.Parse(CompanyIdText.Text), database_object);

            PrimaryKey.UpdatePrimaryKey("System_Company", "CompanyId", "CompanyName", database_object);

            CompaniesListBox.Items.Clear();
            PopulateCompanyList();

            CompaniesListBox.SelectedIndex = CompaniesListBox.Items.Count - 1;
        }

        private void DeleteProductButton_Click(object sender, EventArgs e)
        {
            DeleteFunctions.DeleteRow("System_CompanyProducts", "ProductName", ProductsListBox.SelectedItem.ToString(), database_object);

            ProductsListBox.Items.Clear();
            PopulateCompanyProductsList(int.Parse(CompanyIdText.Text));

            PrimaryKey.UpdatePrimaryKey("System_CompanyProducts", "CompanyProductId", "ProductName", database_object);

        }

        private void DeleteEventButton_Click(object sender, EventArgs e)
        {
            DeleteFunctions.DeleteRow("System_CompanyEvents", "Event", EventListBox.SelectedItem.ToString(), database_object);

            EventListBox.Items.Clear();
            PopulateCompanyEvents(int.Parse(CompanyIdText.Text));

            PrimaryKey.UpdatePrimaryKey("System_CompanyEvents", "CompanyEventId", "Event", database_object);
        }
    }
}
