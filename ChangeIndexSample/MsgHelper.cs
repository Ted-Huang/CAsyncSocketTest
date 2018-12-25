using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChangeIndexSample
{

    enum FIXMsg
    {
        BeginString = 8,
        BodyLength = 9,
        CheckSum = 10,
        MsgType = 35,
        MsgTime = 52,
        IndexType = 10001,
        Index = 10002,
    }

    class MsgHelper
    {
        public static void Send()
        {

        }
    }
}
