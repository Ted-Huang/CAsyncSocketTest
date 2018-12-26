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
            nSize = 5;

        }
        public byte cIndexType { get; set; }
        public int nIndex { get; set; }
    }
}
