using ChangeIndexSample.Msg;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Serialization;

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

            Timer objHBTimer = new Timer();
            objHBTimer.Interval = 3000;
            objHBTimer.Tick += OnHBTimer;
            objHBTimer.Start();

            System.Threading.Tasks.Task.Factory.StartNew(() => Receive());
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

            ChangeDefectIndexMsg objMsg = new ChangeDefectIndexMsg();

            objMsg.cIndexType = (byte)cbIndexType.SelectedIndex;
            objMsg.nIndex = int.Parse(txtIndex.Text);
            List<byte> ls = MsgHelper.GetByte(objMsg);
            socket.Send(ls.ToArray());
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

        private void OnHBTimer(object sender, EventArgs e)
        {
            //send heart beat
            if (socket == null || !socket.Connected)
            {
                return;
            }
            try
            {
                HeartBeatMsg objMsg = new HeartBeatMsg();

                objMsg.cEcho = (byte)ECHO_TYPE.TYPE_QUERY;

                List<byte> ls = MsgHelper.GetByte(objMsg);
                socket.Send(ls.ToArray());
            }
            catch
            {

            }
        }

        private void Receive()
        {
            while(true)
            {
                if (socket != null)
                {
                    //int nBuffer = 0;
                    //byte[] buffer = new byte[1024];
                    //nBuffer = socket.Receive(buffer);

                }
            }
        }
    }
}

