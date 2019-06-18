using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace huffmanCsharp
{
    class postDecompression
    {
        public string lessMagic(string myTxtString)
        {
            myTxtString = myTxtString.Replace("~", " the ");
            myTxtString = myTxtString.Replace('¤', '0');
            myTxtString = myTxtString.Replace('«', '1');
            myTxtString = myTxtString.Replace("²", " be ");
            myTxtString = myTxtString.Replace("¦", " to ");
            myTxtString = myTxtString.Replace("æ"," or ");
            myTxtString = myTxtString.Replace("`", " of ");
            myTxtString = myTxtString.Replace("|", " and ");
            myTxtString = myTxtString.Replace('¬', '\'');
            myTxtString = myTxtString.Replace("§"," that ");
            myTxtString = myTxtString.Replace("¶", " have ");
            return myTxtString;

        }
    }
}
