#ifndef _kway_Tree_h_
#define _kway_Tree_h_

#include <memory>

using std::tr1::shared_ptr;

// simple linked list using smartptrs to nodes
class smartLLNode
{
  public:
  long   data;

  smartLLNode(long pData) : data(pData)
      {  }
  
  private:
  friend class smartLL;
  shared_ptr<smartLLNode> next;
};

class smartLL
{
  public:
  void insert(long pData)
    {
      shared_ptr<smartLLNode> elem(new smartLLNode(pData));
      elem->next = ancList;
      ancList = elem;
    }
 shared_ptr<smartLLNode> ancList;
};

// forw decl so we can typedef a pointer
class kwayNode;
typedef kwayNode*  kwayNodePtr;


// conceptually
// key | minchild | maxchild | count | ancestorList |... array of ptrs to children ... |
class kwayNode 
{
   public:
         long  key;
         long  childMin;
         long  childMax;
         long  childCount;
         smartLL      ancestorList;
         kwayNodePtr  *arrChildren;

         kwayNode(long pKey, long pChildMin, long pChildMax): key(pKey), childMin(pChildMin),
                     childMax(pChildMax)
             {
                childCount = childMax - childMin + 1;
                arrChildren = new kwayNodePtr[childCount];
             }
};

// some conceptual notes about the kwayNode & ancestorList
//  (0) the denominations = regions; so the ancestors of a node are the region vals
//  (1) the 6 in tree path 1-3-6 is different from the 6 in 1-4-6 
//       (their children will typically be different)
//  (2) wanted an eye on the future and to be able to distribute different subtrees
//       of the tree to different processors THEREFORE:
//       (a) don't want to be forced to store entire tree in memory as it will
//             get large as the problem parameters(D & R) increase
//       (b)  want to be able to store & load works in progress - so a particular
//            level of (sub)tree can be a starting point
//       (c)  we know how many ancestors there should be 
//             level[1] node ---> 1 ancestor // level[2] node ---> 2 ancestors
//             level[3] node ---> 3 ancestors  etc.
//
//       (d) chosen idea for tracking ancestors is that of "prefixes" so that
//            related siblings can use the same "prefix" ancestor list and not take
//            up too much memory - NOR take up the time to copy the ancestor list 
//              umpteen times
//       (e)  linked list structure was chosen because we can facilitate (2d) noted
//              above AND it doesn't take up much memory [will only have at most
//               R (# of denominations) levels to the tree so < R ancestors]
//             AND we can  accomplish distributed computation of subtrees,
//               without much difficulty, if we have to
//  (3) we will want to clean up memory of some ancestors when we have finished
//        with all its children/subtrees - SMART POINTER can facilitate this
//       [in particular  shared_ptr<>]
#endif