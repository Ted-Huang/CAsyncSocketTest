using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

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

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
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
            if (buffer.Length < Marshal.SizeOf(typeof(ResponseDefectIndexMsg)))
                return;

            cRtnCode = buffer[13];
        }

        public byte cRtnCode { get; set; }
    }
}
