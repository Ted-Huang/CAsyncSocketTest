using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ChangeIndexSample.Msg
{
    
    public class MsgDef
    {
        public static uint AOI_PACKET_START = 0x494f41;	//"AOI"
        public static ushort AOI_PACKET_VER = 0x0100;		//ver. 1.0
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)] // https://en.wikipedia.org/wiki/Data_structure_alignment
    public class BaseMsg
    {
        public BaseMsg()
        {
            dStart = MsgDef.AOI_PACKET_START;
            wVer = MsgDef.AOI_PACKET_VER;
            wReserved = 0;
        }
        public uint dStart { get; set; }             
        public ushort wVer { get; set; }             
        public ushort wReserved { get; set; }        
        public byte cType { get; set; }
        public int nMsgDate { get; set; }
        public int nMsgTime { get; set; }
        public int nSize { get; set; }               
    }
}
