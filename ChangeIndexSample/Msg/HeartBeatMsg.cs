using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChangeIndexSample.Msg
{
    enum ECHO_TYPE
    {
        TYPE_QUERY = 0,
        TYPE_ECHO = 1
    };
    public class HeartBeatMsg : BaseMsg
    {
        public HeartBeatMsg()
        {
            cType = (byte)MsgType.MSG_HEARTBEAT;
            nSize = 1;
        }
        public byte cEcho { get; set; }
    }
}
