using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace huffmanCsharp
{
    class HuffmanTree
    {
        public List<node> theActualHuffmanTree = new List<node>();
        public static List<myCount> theCountingTree = new List<myCount>();
        static public string stringBuilder = "";

        public void populateTree(List<myCount> charsList)
        {
            foreach (var element in charsList)
            {
                node tempNodeObject = new node();
                tempNodeObject.theChar = element.character.ToString();
                tempNodeObject.occurences = element.occurences;
                theActualHuffmanTree.Add(tempNodeObject);
            }
            theActualHuffmanTree = theActualHuffmanTree.OrderBy(i => i.occurences).ToList();
            theCountingTree = charsList;
        }


        public node popTheTree()
        {
            var outboundNode = theActualHuffmanTree.ElementAt(0);
            theActualHuffmanTree.RemoveAt(0);
            return outboundNode;
        }


        public void reAddToTree(node leftChildNode, node rightChildNode)
        {
            node joinedNode = new node();
            joinedNode.occurences += leftChildNode.occurences + rightChildNode.occurences;
            joinedNode.theChar = string.Format(@"{0}{1}{2}", joinedNode.theChar, leftChildNode.theChar, rightChildNode.theChar);

            foreach (var item in leftChildNode.theChar)
            {
                int childIndex = theCountingTree.FindIndex(x => x.character == item);
                theCountingTree[childIndex].pathway = "1" + theCountingTree[childIndex].pathway;
            }


            foreach (var item in rightChildNode.theChar)
            {
                int childIndex = theCountingTree.FindIndex(x => x.character == item);
                theCountingTree[childIndex].pathway = "0"+ theCountingTree[childIndex].pathway;
            }

            theActualHuffmanTree.Add(joinedNode);
            theActualHuffmanTree = theActualHuffmanTree.OrderBy(i => i.occurences).ToList();
        }


        public void findpath()
        {
            while (theActualHuffmanTree.Count() > 1)
            {
                var firstNodePopped = popTheTree();
                var secondNodePopped = popTheTree();
                reAddToTree(firstNodePopped, secondNodePopped);
            }
            //shrinkBits();
        }

        public void shrinkBits()
        {
            foreach (var item in theCountingTree)
            {
                item.pathway = Int64.Parse(item.pathway).ToString();
            }
        }
    }
}