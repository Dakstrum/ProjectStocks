using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
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
    }
}
