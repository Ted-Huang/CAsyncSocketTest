using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ChangeIndexSample.Msg
{

    public class RequestDefectIndexMsg : BaseMsg
    {
        public RequestDefectIndexMsg()
        {
            cType = (byte)MsgType.MSG_REQUEST_DEFECTINDEX;
            nBodySize = 5;
        }

        public RequestDefectIndexMsg(byte[] buffer)
            : base(buffer)
        {
            if (buffer.Length < Marshal.SizeOf(typeof(RequestDefectIndexMsg)))
                return;

            cIndexType = buffer[13];
            nIndex = BitConverter.ToInt32(buffer, 14);
        }
        public byte cIndexType { get; set; }
        public int nIndex { get; set; }
    }
}
