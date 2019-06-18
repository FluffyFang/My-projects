using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace huffmanCsharp
{
    class VectorMaker
    {
        public List<myCount> populateList(string FileAsString)
        {
            List<myCount> charCount = new List<myCount>();

            foreach (var myChar in FileAsString)
            {
                bool found = false;

                for (int i = 0; i < charCount.Count(); i++)
                {
                    if (myChar == charCount[i].character)
                    {
                        myCount tempObject = new myCount();
                        tempObject.character = charCount[i].character;
                        tempObject.occurences = charCount[i].occurences + 1;
                        charCount.RemoveAt(i);
                        charCount.Add(tempObject);
                        found = true;
                    }
                }

                if (!found)
                {
                    myCount temp = new myCount();
                    temp.character = myChar;
                    temp.occurences = 1;
                    charCount.Add(temp);
                }
            }

            return charCount;
        }
    }
}
