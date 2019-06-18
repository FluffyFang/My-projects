using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace huffmanCsharp
{
    class Compression : HuffmanTree
    {
        string compressedString, myPath, firstFileName, magicNumber;

        public Compression()
        {
            compressedString = "";
            myPath = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + @"/";
            firstFileName = "doIwork.yus";
            magicNumber = "192837465";

        }

        public string CompressDoc(string txtFileAsString)
        {
            compressedString = txtFileAsString;
            theCountingTree = theCountingTree.OrderByDescending(x => x.occurences).ToList();
            foreach (var item in theCountingTree)
            {
                int index = theCountingTree.FindIndex(x => x.character == item.character);

                string theChar = theCountingTree[index].character.ToString();
                string bitReplacement = theCountingTree[index].pathway.ToString();

                compressedString = compressedString.Replace(theChar, bitReplacement);
            }
            return compressedString;
        }

        public BitArray Encode(string source)
        {
            List<bool> encodedSource = new List<bool>();

            foreach (var item in source)
            {
                encodedSource.Add((item == '1') ? true : false);
            }

            return new BitArray(encodedSource.ToArray());
        }

        public Dictionary<string, char> createMap()
        {
            var myMap = new Dictionary<string, char> { };

            foreach (var item in theCountingTree)
            {
                myMap[item.pathway] = item.character;
            }
            return myMap;
        }

        public byte[] makeByteArray(BitArray someBits)
        {
            BitArray bit_array = someBits;
            byte[] bytes = new byte[bit_array.Length / 8 + (bit_array.Length % 8 == 0 ? 0 : 1)];
            bit_array.CopyTo(bytes, 0);
            return bytes;
        }

        public void writeMagicNumberAndDictionary(Dictionary<string, char> myMap)
        {
            var inputString = magicNumber;
            inputString += dictionaryToString(myMap);
            System.IO.File.WriteAllText((myPath + firstFileName), inputString);
        }

        internal void consolodateInfo(string path, byte[] bytes)
        {
            //appends following into the original written file
            using (var stream = new FileStream(path, FileMode.Append))
            {
                //writes a block of bytes to the filestream
                foreach (var item in bytes)
                {
                    stream.WriteByte(item);
                }
            }
        }

        public string dictionaryToString(Dictionary<string, char> myMap)
        {
            int tempBuffer;
            string buffer = "";
            foreach (var pair in myMap)
            {
                tempBuffer = 0;
                for (int i = 0; i < pair.Key.Length; i++)
                {
                    if (pair.Key[pair.Key.Length - i - 1] == '0') continue;
                    tempBuffer += (int)Math.Pow(2, i);
                }
                buffer += pair.Value + tempBuffer.ToString() + '‡';
            }
            buffer += "‡";
            return buffer;
        }
    }
}
