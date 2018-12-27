using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChangeIndexSample.Msg
{
    public class MsgHelper
    {
        public static List<byte> GetByte(BaseMsg objMsg)
        {
            List<byte> ls = new List<byte>();

            objMsg.GetType().GetProperties().OrderBy(x => x.DeclaringType.FullName == typeof(BaseMsg).FullName ? 0 : 1 /*base資料在前*/).ToList().ForEach(x =>
            {
                if (x.PropertyType.FullName == typeof(uint).FullName)
                    ls.AddRange(BitConverter.GetBytes((uint)x.GetValue(objMsg)));
                else if (x.PropertyType.FullName == typeof(ushort).FullName)
                    ls.AddRange(BitConverter.GetBytes((ushort)x.GetValue(objMsg)));
                else if (x.PropertyType.FullName == typeof(int).FullName)
                    ls.AddRange(BitConverter.GetBytes((int)x.GetValue(objMsg)));
                else if (x.PropertyType.FullName == typeof(char).FullName)
                    ls.Add(Convert.ToByte((char)x.GetValue(objMsg)));
                else if (x.PropertyType.FullName == typeof(byte).FullName)
                    ls.Add((byte)x.GetValue(objMsg));
            });

            byte bCheckSum = 0;
            ls.GetRange(sizeof(uint), ls.Count - sizeof(uint) /*calculate checksum without dStart */).ForEach(x => bCheckSum ^= x);

            ls.Add(bCheckSum);

            return ls;
        }

    }
}
