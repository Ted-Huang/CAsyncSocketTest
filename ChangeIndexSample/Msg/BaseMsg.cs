using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ChangeIndexSample.Msg
{
    
    public class MsgDef
    {
        public static uint AOI_PACKET_START = 0x494f41;	    //"AOI"
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

        public BaseMsg(byte[] buffer)
        {
            if (buffer.Length <= Marshal.SizeOf(typeof(BaseMsg)))
                return;

            dStart = BitConverter.ToUInt32(buffer, 0);
            wVer = BitConverter.ToUInt16(buffer, 4);
            wReserved = BitConverter.ToUInt16(buffer, 6);
            cType = buffer[8];
            nBodySize = BitConverter.ToUInt16(buffer, 9);
        }

        public uint dStart { get; set; }             
        public ushort wVer { get; set; }             
        public ushort wReserved { get; set; }        
        public byte cType { get; set; }
        public int nBodySize { get; set; }               
    }
}
