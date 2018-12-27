using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChangeIndexSample.Msg
{
    public enum RespDefectIndexCode
    {
        /// <summary>
        /// 成功
        /// </summary>
        Success = 0,
        /// <summary>
        /// 專案尚未就緒
        /// </summary>
        ProjectNotReady,
        /// <summary>
        /// index不存在
        /// </summary>
        IndexNotExist,
        /// <summary>
        /// 尚未開始檢測
        /// </summary>
        InspectionNotRunning,
    }
    public class ResponseDefectIndexMsg : BaseMsg
    {       
        public ResponseDefectIndexMsg()
        {
            cType = (byte)MsgType.MSG_REQUEST_DEFECTINDEX;
            nBodySize = 5;
        }

        public ResponseDefectIndexMsg(byte[] buffer)
            : base(buffer)
        {
            if (buffer.Length < 18)
                return;

            cRtnCode = buffer[13];
        }

        public byte cRtnCode { get; set; }
    }
}
