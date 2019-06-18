using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace huffmanCsharp
{
    public class myCount
    {
        public char character;
        public int occurences;
        public string pathway;
    };

    public class node
    {
        public string theChar;
        public int occurences;
        public string pathway;
    };

    class Program
    {

        static void Main(string[] args)
        {
            PreCompress preCompression = new PreCompress();
            Compression myCompressor = new Compression();

            string path = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
            //string fileToCompress = "USConstitution.txt";
            //string fileName = "quotes.txt";
            string fileToCompress = args[0];
            string fileToDecompress = "doIwork.yus";


            ///reading in the textfile, converting it to a string, performes deep-tissue precompression data massage
            string myTxtString = "";
            using (System.IO.StreamReader myFile = new System.IO.StreamReader(path + @"\" + fileToCompress)) ///System.IO.StreamReader myFile = new System.IO.StreamReader(path + @"\" + fileName);
            {
                myTxtString = myFile.ReadToEnd();
            }
            myTxtString = preCompression.DoMagic(myTxtString);

            var teststring = myTxtString.Substring(0, 9);
            if (!teststring.Contains("92837465")) //notcompressed
            {

                ///Create and populate the list(vector)
                VectorMaker myVectorMaker = new VectorMaker();
                List<myCount> charsList = myVectorMaker.populateList(myTxtString);


                ///Create, populate, and sort the Huffman Tree
                HuffmanTree theHuffTree = new HuffmanTree();
                theHuffTree.populateTree(charsList);
                theHuffTree.findpath();


                ///Transform the keys of the map into bits
                Dictionary<String, char> myMap = myCompressor.createMap();
                string buffer = "192837465\n" + myMap.Count.ToString();
                string temprr = "";
                foreach (var item in myMap)
                {
                    temprr += item.Value + " " + item.Key + "\n";
                }
                //System.IO.File.WriteAllText((path + "\\" + "Dictionary.txt"), temprr);


                ///FileStream>>BitString>>BitArray>>ByteArray
                var myBitString = myCompressor.CompressDoc(myTxtString);
                var myBitArray = myCompressor.Encode(myBitString);
                var myByteArray = myCompressor.makeByteArray(myBitArray);

                ///Writes Magic#, Map, Bytes to binary file
                myCompressor.writeMagicNumberAndDictionary(myMap);
                myCompressor.consolodateInfo((path + @"\" + fileToDecompress), myByteArray);
            }
            else //compressed
            {
                ///Parses map and magic number, Converts byte file to bit string, 
                Decompression myDecompressor = new Decompression();
                string textAsBits = myDecompressor.everything(path + @"\" + fileToDecompress);
                string mapstring = "";
                var myMap = myDecompressor.createDecompressMap(mapstring);


                ///Replaces bit values with characters
                string outputFile = myDecompressor.englishInterpretation(myMap, textAsBits);
                postDecompression SauceFree = new postDecompression();
                outputFile = SauceFree.lessMagic(outputFile);


                ///Writes Character string to output file
                System.IO.File.WriteAllText((path + "\\" + "output.txt"), outputFile);
            }

            ///ends program
            Console.WriteLine("Press Enter to Continue...");
            Console.ReadLine();
        }
    }
}



