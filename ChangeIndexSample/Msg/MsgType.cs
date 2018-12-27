using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChangeIndexSample.Msg
{
    public enum MsgType
    {
        MSG_HEARTBEAT = 0,
        MSG_REQUEST_DEFECTINDEX,
        MSG_RESPONSE_DEFECTINDEX,
    }
}
