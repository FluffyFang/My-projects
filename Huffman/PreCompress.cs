using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace huffmanCsharp
{
    class PreCompress
    {
        public string DoMagic(string myTxtString)
        {
            myTxtString = myTxtString.Replace('0', '¤');
            myTxtString = myTxtString.Replace('1', '«');
            myTxtString = myTxtString.Replace(" the ", "~");
            myTxtString = myTxtString.Replace(" be ", "²");
            myTxtString = myTxtString.Replace(" to ", "¦");
            myTxtString = myTxtString.Replace(" or ", "æ");
            myTxtString = myTxtString.Replace(" of ", "`");
            myTxtString = myTxtString.Replace(" and ", "|");
            myTxtString = myTxtString.Replace('\'', '¬');
            myTxtString = myTxtString.Replace(" that ", "§");
            myTxtString = myTxtString.Replace(" have ", "¶");
            
            return myTxtString;
        }
    }
}
