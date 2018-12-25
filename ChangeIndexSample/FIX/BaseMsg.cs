using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChangeIndexSample.FIX
{
    public abstract class BaseMsg
    {
        public static Encoding BaseMsgEncoding = Encoding.Unicode;
        public BaseMsg()
        {
            BeginString = "8=AOI|";
        }

        /// <summary>
        /// BeginString = 8,
        /// </summary>
        public string BeginString { get; set; }
        /// <summary>
        ///  BodyLength = 9,
        /// </summary>
        public int BodyLength { get; set; }
        /// <summary>
        /// MsgType = 35,
        /// </summary>
        public string MsgType { get; set; }
        /// <summary>
        /// MsgTime = 52,
        /// </summary>
        public string MsgTime { get; set; }
        /// <summary>
        /// CheckSum = 10,
        /// </summary>
        public int CheckSum { get; set; }

        public string GetMsg()
        {
            int nCheckSum = 0;
            this.MsgTime = DateTime.Now.ToString("yyyyMMdd-HH:mm:ss.fff");
            StringBuilder sb = new StringBuilder();
            sb.Append(string.Format("35={0}|", this.MsgType));
            sb.Append(string.Format("52={0}|", this.MsgTime));
            sb.Append(string.Format("{0}", ComposeBodyMsg().TrimEnd('|')));

            BodyLength = (BaseMsgEncoding.GetBytes(sb.ToString()).Length + 1);

            string strMsg = string.Format("{0}9={1}|{2}", this.BeginString, BodyLength.ToString().PadLeft(5, '0'), sb.ToString());
            byte[] MsgByte = BaseMsgEncoding.GetBytes(strMsg);
            for (int x = 0; x < MsgByte.Length; x++)
            {
                nCheckSum += MsgByte[x];
            }

            return strMsg + string.Format("|10={0}", nCheckSum % 256);
        }

        public static string GetHeartBeatMsg()
        {
            return "8=AOI|9=00005|35=0";
        }

        abstract public string ComposeBodyMsg();

    }
}
