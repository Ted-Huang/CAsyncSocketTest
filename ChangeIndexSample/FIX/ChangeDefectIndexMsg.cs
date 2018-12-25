using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChangeIndexSample.FIX
{
    public enum IndexType
    {
        PRODUCT_INDEX = 0,
        DEFECT_INDEX,
    }

    public class ChangeDefectIndexMsg : BaseMsg
    {
        /// <summary>
        ///  IndexType = 10001,
        /// </summary>
        public IndexType IndexType { get; set; }
        /// <summary>
        /// Index = 10002,
        /// </summary>
        public int Index { get; set; }

        public override string ComposeBodyMsg()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(string.Format("10001={0}|", (int)this.IndexType));
            sb.Append(string.Format("10002={0}", this.Index));
            return sb.ToString();
        }
    }
}
