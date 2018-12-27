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
        Socket m_socket = null;
        const int MAX_RECEIVE_BUFFER_SIZE = 64000;
        byte[] m_buffer = new byte[MAX_RECEIVE_BUFFER_SIZE];
        int m_nReceiveSize = 0;

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

            if (m_socket == null || !m_socket.Connected)
            {
                MessageBox.Show("連線異常");
                return;
            }

            ChangeDefectIndexMsg objMsg = new ChangeDefectIndexMsg();
            objMsg.cIndexType = (byte)cbIndexType.SelectedIndex;
            objMsg.nIndex = int.Parse(txtIndex.Text);
            SendMsg(objMsg);
        }

        private void SendMsg(BaseMsg objMsg)
        {

            if (m_socket == null || !m_socket.Connected)
            {
                Console.WriteLine("連線異常");
                return;
            }

            try
            {
                List<byte> ls = MsgHelper.GetByte(objMsg);
                m_socket.Send(ls.ToArray());
            
            }
            catch(Exception ex)
            {
                Console.WriteLine(string.Format("連線異常"), ex.ToString());
            }
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (m_socket != null)
            {
                try
                {
                    m_socket.Dispose();
                    m_socket = null;
                }
                catch { }
            }
            try
            {
                m_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                m_socket.Connect(txtIP.Text, int.Parse(txtPort.Text));
            }
            catch 
            {
                MessageBox.Show("連線失敗");
            }
            
        }

        private void OnHBTimer(object sender, EventArgs e)
        {
            HeartBeatMsg objMsg = new HeartBeatMsg();
            objMsg.cEcho = (byte)ECHO_TYPE.TYPE_QUERY;

            SendMsg(objMsg);
        }

        private void Receive()
        {
            while(true)
            {
                if (m_socket == null)
                    continue;
                if (!m_socket.Connected)
                    continue;

                int nRecBuffer = 0;

                try
                {
                    nRecBuffer = m_socket.Receive(m_buffer, m_nReceiveSize, m_buffer.Length - m_nReceiveSize, SocketFlags.None);
                    m_nReceiveSize += nRecBuffer;
                }
                catch(Exception ex)
                {
                    Console.WriteLine(string.Format("receive error! {0}", ex.ToString()));
                }

                if (nRecBuffer == 0)
                    continue;
                
                CheckDataBuf();
            }
        }

        private void CheckDataBuf()
        {
            MovePacketToStart();
            int nOffset = 0;
            int nHeaderSize = Marshal.SizeOf(typeof(BaseMsg));
            int nBodySize = BitConverter.ToUInt16(m_buffer, nOffset + 9);
            int nMsgSize = nHeaderSize + nBodySize + 1;
            while (m_nReceiveSize >= nHeaderSize && m_nReceiveSize >= nMsgSize)
            {
                ParseCommand(m_buffer.ToList().GetRange(nOffset, nMsgSize).ToArray());
                //next msg
                m_nReceiveSize -= nMsgSize;
                nOffset += nMsgSize;
                nBodySize = BitConverter.ToUInt16(m_buffer, nOffset + 9);
                nMsgSize = nHeaderSize + nBodySize + 1;
            }
            
        }

        private void MovePacketToStart()
        {
            int nNewSize = m_nReceiveSize;
            for(int x= 0; x< m_nReceiveSize - 4; x++)
            {
                if (MsgDef.AOI_PACKET_START != BitConverter.ToUInt32(m_buffer, x))
                    nNewSize--;
                else
                    break;
            }

            int nOffset = m_nReceiveSize - nNewSize;
            if(nOffset>0)
            {
                for(int x=0; x < m_nReceiveSize; x++) //move buffer
                {
                    if(x - nOffset <0)
                        continue;
                    m_buffer[x-nOffset] = m_buffer[x];
                }

                for (int x = nNewSize; x < nNewSize + nOffset; x++) //reset other to 0
                {
                    m_buffer[x] = 0;
                }
                m_nReceiveSize = nNewSize;
            }
        }

        private void ParseCommand(byte[] buffer)
        {
            if(!PacketCheck(buffer))
                return;

            BaseMsg objMsg = null;
            try
            {
                switch ((MsgType)buffer[8])
                {
                    case MsgType.MSG_HEARTBEAT:
                        objMsg = new HeartBeatMsg(buffer);
                        if (((HeartBeatMsg)objMsg).cEcho == (byte)ECHO_TYPE.TYPE_QUERY)
                        {
                            HeartBeatMsg objHB = new HeartBeatMsg();
                            objHB.cEcho = (byte)ECHO_TYPE.TYPE_ECHO;
                            SendMsg(objHB);
                            return;
                        }
                        break;
                    case MsgType.MSG_CHANGEDEFECT:
                        objMsg = new ChangeDefectIndexMsg(buffer);
                        break;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(string.Format("ParseCommand error! {0}", ex.ToString()));
            }
        }

        private bool PacketCheck(byte[] buffer)
        {
            if (buffer.Length <= Marshal.SizeOf(typeof(BaseMsg)) + 1)
                return false;

            if( MsgDef.AOI_PACKET_START == BitConverter.ToUInt32(buffer, 0))
            {
                return CheckSum(buffer);
            }
            return false;
        }

        private bool CheckSum(byte[] buffer)
        {
            byte sum = 0;
            for (int x = 4; x < buffer.Length - 1; x++)
                sum ^= buffer[x];

            return sum == buffer[buffer.Length - 1];
        }
    }
}

