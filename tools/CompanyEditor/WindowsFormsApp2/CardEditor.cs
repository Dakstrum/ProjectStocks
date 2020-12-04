using System;
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
    public partial class CardEditor : Form
    {
        Database database_object = new Database();

        public CardEditor()
        {
            InitializeComponent();
        }

        private void CardEditor_Load(object sender, EventArgs e)
        {
            PopulateListBox.PopulateCardListBox(CardListBox, database_object);
        }

        private void CardListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            {

                try
                {
                    CardIdText.Text = GetFunctions.GetCardIdFromCardName(CardListBox.SelectedItem.ToString(), database_object).ToString();
                    PopulateCardModifiers(GetFunctions.GetCardIdFromCardName(CardListBox.SelectedItem.ToString(), database_object));
                }
                catch { }

            }
        }

        void PopulateCardModifiers(int card_id)
        {

            database_object.OpenConnection();

            string Tool_System_CompanyEvents_query = "SELECT * FROM System_Cards";
            SQLiteCommand EventSelectCommand = new SQLiteCommand(Tool_System_CompanyEvents_query, database_object.myConnection);
            SQLiteDataReader EventSelectresult = EventSelectCommand.ExecuteReader();
            if (EventSelectresult.HasRows)
            {
                while (EventSelectresult.Read())
                {
                    if (EventSelectresult["CardId"].ToString() == card_id.ToString())
                    {
                        CardEditTextBox.Text           = EventSelectresult["CardName"].ToString();
                        CardPriceModifierTextBox.Text  = EventSelectresult["PriceModifier"].ToString();
                        CardModifierLengthTextBox.Text = EventSelectresult["ModifierLength"].ToString();
                        CardIdText.Text                = EventSelectresult["CardId"].ToString();
                        CardDescriptionTextBox.Text    = EventSelectresult["CardDesc"].ToString();
                        CardPathTextBox.Text           = EventSelectresult["CardPath"].ToString();
                    }
                }
            }
            database_object.CloseConnection();

        }

        void UpdateCard(int card_id)
        {                

            database_object.OpenConnection();
            string query = "Update System_Cards Set CardName = @cardname, CardDesc = @carddesc, CardPath = @cardpath, PriceModifier = @pricemodifier, ModifierLength = @modifierlength Where CardId = " + card_id;
            SQLiteCommand myInsertCommand = new SQLiteCommand(query, database_object.myConnection);

            myInsertCommand.Parameters.AddWithValue("@cardname", CardEditTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@carddesc", CardDescriptionTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@cardpath", CardPathTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@pricemodifier", CardPriceModifierTextBox.Text);
            myInsertCommand.Parameters.AddWithValue("@modifierlength", CardModifierLengthTextBox.Text);

            var result = myInsertCommand.ExecuteNonQuery();
            database_object.CloseConnection();

            CardListBox.Items.Clear();
            PopulateListBox.PopulateCardListBox(CardListBox, database_object);

        }

        private void SaveCardButton_Click(object sender, EventArgs e)
        {
            UpdateCard(int.Parse(CardIdText.Text.ToString()));
        }

        private void AddCardButton_Click(object sender, EventArgs e)
        {

            if (CreateNewFunctions.DoesThisAlreadyExist("System_Cards", "CardName", "BLANK", database_object))
            {
                MessageBox.Show("There is already a BLANK item", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

               
            CreateNewFunctions.CreateNewCard(database_object);

            CardListBox.Items.Clear();
            PopulateListBox.PopulateCardListBox(CardListBox, database_object);

            PrimaryKey.UpdatePrimaryKey("System_Cards", "CardId", "CardName", database_object);

            CardIdText.Text = GetFunctions.GetCardIdFromCardName(CardListBox.SelectedItem.ToString(), database_object).ToString();
            PopulateCardModifiers(GetFunctions.GetCardIdFromCardName(CardListBox.SelectedItem.ToString(), database_object));
        }

        private void DeleteCardButton_Click(object sender, EventArgs e)
        {

            DeleteFunctions.DeleteRow("System_Cards", "CardName", CardListBox.SelectedItem.ToString(), database_object);

            CardListBox.Items.Clear();
            PopulateListBox.PopulateCardListBox(CardListBox, database_object);

            PrimaryKey.UpdatePrimaryKey("System_Cards", "CardId", "CardName", database_object);

            CardListBox.Items.Clear();
            PopulateListBox.PopulateCardListBox(CardListBox, database_object);

        }
    }
}
