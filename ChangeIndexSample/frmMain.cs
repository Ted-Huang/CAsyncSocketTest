using ChangeIndexSample.FIX;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ChangeIndexSample
{
    public partial class frmMain : Form
    {        
        /// <summary>
        /// Socket 通訊端介面
        /// </summary>
        private Socket socket = null;
        public frmMain()
        {
            InitializeComponent();
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            cbIndexType.Items.Add("Product Index");
            cbIndexType.Items.Add("Defect Index");
            cbIndexType.SelectedIndex = 0;
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            int nCheck;
            if (!int.TryParse(txtIndex.Text, out nCheck))
            {
                MessageBox.Show("index 非數字!");
                return;
            }

            if (socket == null || !socket.Connected)
            {
                MessageBox.Show("連線異常");
                return;
            }

            ChangeDefectIndexMsg obj = new ChangeDefectIndexMsg();
            obj.MsgType = MsgType.CHANGE_INDEX;
            obj.IndexType = (IndexType)cbIndexType.SelectedIndex;
            obj.Index = int.Parse(txtIndex.Text);

            string str = obj.GetMsg();
            Console.WriteLine(str);
            socket.Send(Encoding.Unicode.GetBytes(str));
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (socket != null)
            {
                try
                {
                    socket.Dispose();
                    socket = null;
                }
                catch { }
            }
            try
            {
                socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                socket.Connect(txtIP.Text, int.Parse(txtPort.Text));
            }
            catch 
            {
                MessageBox.Show("連線失敗");
            }
            
        }
    }
}

