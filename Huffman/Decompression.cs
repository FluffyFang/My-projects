using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace huffmanCsharp
{
    class Decompression
    {

        public String everything(string filename)
        {
            int startOfText = 0;
            byte[] bytes = System.IO.File.ReadAllBytes(filename);
            for (int i = 0; i < bytes.Length; i++)
            {
                if (bytes[i] == 161 && bytes[i - 1] == 128 && bytes[i - 2] == 226 &&
                    bytes[i - 3] == 161 && bytes[i - 4] == 128 && bytes[i - 5] == 226)
                {
                    startOfText = i + 1;
                }
            }

            string decomBits = "";
            BitArray bits = new BitArray(bytes.Skip(startOfText).ToArray());
            foreach (bool item in bits)
            {
                decomBits += item ? 1 : 0;
            }
            System.IO.File.WriteAllText((Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\" + "bitreoutput.txt"), decomBits);
            return decomBits;
        }

        public Dictionary<string, char> createDecompressMap(string myFileMap)
        {
            char mapChar;
            var decompressMap = new Dictionary<string, char> { };
            string[] stringDict = myFileMap.Split('‡');
            for (int i = 0; i < stringDict.Length; i++)
            {
                mapChar = stringDict[i][0];
                int temp = Int32.Parse(stringDict[i].Substring(1));
                string mapKey = Convert.ToString(temp, 2);
                decompressMap[mapKey] = mapChar;
            }
            foreach (var pair in decompressMap)
            {
                Console.Write(pair.Value + " : " + pair.Key + ":\n");
            }
            return decompressMap;
        }

        public string englishInterpretation(Dictionary<String, char> mapToUse, string bitString)
        {
            string BUFFER = "";
            string outputFile = "";

            foreach (char character in bitString)
            {
                BUFFER += character;
                if (mapToUse.ContainsKey(BUFFER))
                {
                    outputFile += mapToUse[BUFFER];
                    BUFFER = "";
                }

                //var myTemp = mapToUse.FirstOrDefault(x => x.ToString() == BUFFER).Key;
                //if (myTemp != null)
                //{
                //    outputFile += myTemp.Value;
                //    BUFFER = "";
                //}
            }
            return outputFile;
        }

    }
}
