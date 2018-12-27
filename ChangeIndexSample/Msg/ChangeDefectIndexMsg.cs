using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChangeIndexSample.Msg
{

    public class ChangeDefectIndexMsg : BaseMsg
    {
        public ChangeDefectIndexMsg()
        {
            cType = (byte)MsgType.MSG_CHANGEDEFECT;
            nBodySize = 5;
        }

        public ChangeDefectIndexMsg(byte[] buffer): base(buffer)
        {
            if (buffer.Length < 18)
                return;

            cType = buffer[13];
            nIndex = BitConverter.ToInt32(buffer, 14);
        }
        public byte cIndexType { get; set; }
        public int nIndex { get; set; }
    }
}
