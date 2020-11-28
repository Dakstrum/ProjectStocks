using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp2
{
    public partial class MainMenu : Form
    {
        public MainMenu()
        {
            InitializeComponent();
        }

        private void CompanyEditorButton_Click(object sender, EventArgs e)
        {
            CompanyEditor CE_form = new CompanyEditor();
            CE_form.Show();

        }

        private void EventEditorButton_Click(object sender, EventArgs e)
        {

            EventEditor CE_form = new EventEditor();
            CE_form.Show();

        }

        private void CategoryEditorButton_Click(object sender, EventArgs e)
        {
            CategoryEditor CE_form = new CategoryEditor();
            CE_form.Show();
        }


        private void AINameEditorButton_Click(object sender, EventArgs e)
        {
            AINameEditor CE_form = new AINameEditor();
            CE_form.Show();
        }

        private void CardEditorButton_Click(object sender, EventArgs e)
        {
            CardEditor CE_form = new CardEditor();
            CE_form.Show();
        }

        private void ExportButton_Click(object sender, EventArgs e)
        {

            string fileName = "blinky.db";
            string sourcePath = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
            string targetPath = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);

            string sourceFile = Path.Combine(sourcePath, fileName);
            string destFile = Path.Combine(targetPath, fileName);

            File.Copy(sourceFile, destFile, true);

        }


    }
}
