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
    public partial class EventEditor : Form
    {
        int current_category_viewing = 1;
        Database database_object = new Database();

        public EventEditor()
        {
            InitializeComponent();
        }

        private void EventEditor_Load(object sender, EventArgs e)
        {
            PopulateComboBox.Category(CategoryIdComboBox, database_object);
            PopulateComboBox.Category(CategoryEventsEditComboBox, database_object);

            PopulateGlobalEvents();
            PopulateCategoryEvents(1);
            CategoryIdComboBox.SelectedIndex = current_category_viewing - 1;
        }

        //GlobalEvents
        void PopulateGlobalEvents()
        {

            database_object.OpenConnection();

            string Tool_System_CompanyEvents_query = "SELECT * FROM System_GlobalEvents";
            SQLiteCommand EventSelectCommand = new SQLiteCommand(Tool_System_CompanyEvents_query, database_object.myConnection);
            SQLiteDataReader EventSelectresult = EventSelectCommand.ExecuteReader();

            if (EventSelectresult.HasRows)
                while (EventSelectresult.Read())
                    GlobalEventListBox.Items.Add(EventSelectresult["Event"]);
                        
            GlobalEventListBox.SelectedIndex = GlobalEventListBox.Items.Count - 1;
            database_object.CloseConnection();

        }

        void PopulateGlobalEventsModifiers(int global_event_id)
        {

            database_object.OpenConnection();

            string Tool_System_CompanyEvents_query = "SELECT * FROM System_GlobalEvents";
            SQLiteCommand EventSelectCommand = new SQLiteCommand(Tool_System_CompanyEvents_query, database_object.myConnection);
            SQLiteDataReader EventSelectresult = EventSelectCommand.ExecuteReader();
            if (EventSelectresult.HasRows)
            {
                while (EventSelectresult.Read())
                {
                    if (EventSelectresult["GlobalEventId"].ToString() == global_event_id.ToString())
                    {
                        GlobalPriceModifierTextBox.Text  = EventSelectresult["PriceModifier"].ToString();
                        GlobalModifierLengthTextBox.Text = EventSelectresult["ModifierLength"].ToString();
                        globaleventidtext.Text           = EventSelectresult["GlobalEventId"].ToString();
                    }
                }
            }
            database_object.CloseConnection();

        }

        private void GlobalEventListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            {

                try
                {
                    GlobalEventEditTextBox.Text = GlobalEventListBox.SelectedItem.ToString();
                    PopulateGlobalEventsModifiers(GetFunctions.GetGlobalEventIdFromEvent(GlobalEventListBox.SelectedItem.ToString(), database_object));

                }
                catch { }

            }
        }

        void UpdateGlobalEvents(int global_event_id)
        {

            database_object.OpenConnection();
            string query = "Update System_GlobalEvents Set Event = @event, PriceModifier = @pricemodifier, ModifierLength = @modifierlength Where GlobalEventId = " + global_event_id;
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);
            
            myInsertCommand.Parameters.AddWithValue("@event",          GlobalEventEditTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@pricemodifier",  GlobalPriceModifierTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@modifierlength", GlobalModifierLengthTextBox.Text);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

            ClearGlobalEventsListBox();
            PopulateGlobalEvents();

        }

        void ClearGlobalEventsListBox()
        {
            GlobalEventListBox.Items.Clear();
        }

        private void SaveGloabalEvent_Click(object sender, EventArgs e)
        {
            UpdateGlobalEvents(GetFunctions.GetGlobalEventIdFromEvent(GlobalEventListBox.SelectedItem.ToString(), database_object));
        }

        private void AddGlobalEventButton_Click(object sender, EventArgs e)
        {

            if (CreateNewFunctions.DoesThisAlreadyExist("System_GlobalEvents", "Event", "BLANK", database_object))
            {
                MessageBox.Show("There is already a BLANK item", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            CreateNewFunctions.CreateNewGlobalEvent(database_object);

            ClearGlobalEventsListBox();
            PopulateGlobalEvents();

            PrimaryKey.UpdatePrimaryKey("System_GlobalEvents", "GlobalEventId", "Event", database_object);

            PopulateGlobalEventsModifiers(GlobalEventListBox.SelectedIndex + 1); //STUFF ISNT UPDATING CORRECT, IDK, YOU FIX IT FUTURE JACK!!!

        }

        //Category

        void PopulateCategoryEvents(int category_id)
        {

            database_object.OpenConnection();

            string Tool_System_CompanyEvents_query = "SELECT * FROM System_CategoryEvents";
            SQLiteCommand EventSelectCommand = new SQLiteCommand(Tool_System_CompanyEvents_query, database_object.myConnection);
            SQLiteDataReader EventSelectresult = EventSelectCommand.ExecuteReader();

            if (EventSelectresult.HasRows)
                while (EventSelectresult.Read())
                    if (EventSelectresult["CategoryId"].ToString() == category_id.ToString())
                        CategoryEventListBox.Items.Add(EventSelectresult["Event"]);

            CategoryEventListBox.SelectedIndex = CategoryEventListBox.Items.Count - 1;
            database_object.CloseConnection();

        }

        void PopulateCategoryEventsModifiers(int category_event_id)
        {

            database_object.OpenConnection();

            string query = "SELECT * FROM System_CategoryEvents";
            SQLiteCommand EventSelectCommand = new SQLiteCommand(query, database_object.myConnection);
            SQLiteDataReader EventSelectresult = EventSelectCommand.ExecuteReader();
            if (EventSelectresult.HasRows)
            {
                while (EventSelectresult.Read())
                {
                    if (EventSelectresult["CategoryEventId"].ToString() == category_event_id.ToString())
                    {
                        CategoryPriceModifierTextBox.Text  = EventSelectresult["PriceModifier"].ToString();
                        CategoryModifierLenghtTextBox.Text = EventSelectresult["ModifierLength"].ToString();
                        CateogryEventIdText.Text           = EventSelectresult["CategoryEventId"].ToString();
                        CategoryEventsEditComboBox.SelectedIndex = int.Parse(EventSelectresult["CategoryId"].ToString()) - 1;
                    }
                }
            }
            database_object.CloseConnection();

        }
        
        private void CategoryEventListBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            {

                try
                {
                    CategoryEditTextBox.Text = CategoryEventListBox.SelectedItem.ToString();
                    PopulateCategoryEventsModifiers(GetFunctions.GetCategoryEventIdFromEvent(CategoryEventListBox.SelectedItem.ToString(), database_object));

                }
                catch { }

            }
        }

        void UpdateCategoryEvents(int global_event_id)
        {

            database_object.OpenConnection();
            string query = "Update System_CategoryEvents Set CategoryId = @categoryid, Event = @event, PriceModifier = @pricemodifier, ModifierLength = @modifierlength Where CategoryEventId = " + global_event_id;
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@categoryid",     CategoryEventsEditComboBox.SelectedIndex + 1);
            myInsertCommand.Parameters.AddWithValue("@event",          CategoryEditTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@pricemodifier",  CategoryPriceModifierTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@modifierlength", CategoryModifierLenghtTextBox.Text);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

            ClearCategoryEventsListBox();
            PopulateCategoryEvents(current_category_viewing);

        }

        void ClearCategoryEventsListBox()
        {
            CategoryEventListBox.Items.Clear();
        }

        void ClearCategoryEventModifiers()
        {
            CategoryEditTextBox.Text = "";
            CategoryPriceModifierTextBox.Text = "";
            CategoryModifierLenghtTextBox.Text = "";
            CateogryEventIdText.Text = "";
            CategoryEventsEditComboBox.SelectedIndex = CategoryIdComboBox.SelectedIndex;
        }

        private void SaveCategoryEvent_Click(object sender, EventArgs e)
        {
            UpdateCategoryEvents(GetFunctions.GetCategoryEventIdFromEvent(CategoryEventListBox.SelectedItem.ToString(), database_object));
        }

        private void AddCategoryEventButton_Click(object sender, EventArgs e)
        {
            if (CreateNewFunctions.DoesThisAlreadyExist("System_CategoryEvents", "Event", "BLANK", database_object))
            {
                MessageBox.Show("There is already a BLANK item", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            CreateNewFunctions.CreateNewCategoryEvent(database_object, current_category_viewing);

            ClearCategoryEventsListBox();
            PopulateCategoryEvents(current_category_viewing);

            //PrimaryKey.UpdatePrimaryKey("System_CategoryEvents", "CategoryEventId", "Event", database_object, CategoryEventListBox);

            PopulateCategoryEventsModifiers(GetFunctions.GetCategoryEventIdFromEvent(CategoryEventListBox.SelectedItem.ToString(), database_object));
        }

        private void SaveCategoryEventButton_Click(object sender, EventArgs e)
        {
            UpdateCategoryEvents(GetFunctions.GetCategoryEventIdFromEvent(CategoryEventListBox.SelectedItem.ToString(), database_object));
        }

        private void CategoryIdComboBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            current_category_viewing = CategoryIdComboBox.SelectedIndex + 1;

            ClearCategoryEventsListBox();
            ClearCategoryEventModifiers();

            PopulateCategoryEvents(current_category_viewing);
        }

        private void DeleteGloabalEventButton_Click(object sender, EventArgs e)
        {
            DeleteFunctions.DeleteRow("System_GlobalEvents", "Event", GlobalEventListBox.SelectedItem.ToString(), database_object);

            GlobalEventListBox.Items.Clear();
            PopulateGlobalEvents();

            PrimaryKey.UpdatePrimaryKey("System_GlobalEvents", "GlobalEventId", "Event", database_object);
        }

        private void DeleteCategoryEventButton_Click(object sender, EventArgs e)
        {
            DeleteFunctions.DeleteRow("System_CategoryEvents", "Event", CategoryEventListBox.SelectedItem.ToString(), database_object);

            CategoryEventListBox.Items.Clear();
            PopulateCategoryEvents(CategoryIdComboBox.SelectedIndex + 1);

            PrimaryKey.UpdatePrimaryKey("System_CategoryEvents", "CategoryEventId", "Event", database_object);
        }
    }
}
