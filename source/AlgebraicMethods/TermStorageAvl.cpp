// <plaintext>
#include "TermStorageAvl.h"
#include "ITimeout.h"
#include <algorithm>

// ----------------------------------------------------------------------------- Definitions

   // Use mnemonic constants for valid balance-factor values
enum balance_t { LEFT_HEAVY = -1, BALANCED = 0, RIGHT_HEAVY = 1 };

   // Use mnemonic constants for indicating a change in height
enum height_effect_t { HEIGHT_NOCHANGE = 0, HEIGHT_CHANGE = 1 };

   // Return true if the tree is too heavy on the left side
inline static int
LEFT_IMBALANCE(short bal) {
   return (bal < LEFT_HEAVY);
}

   // Return true if the tree is too heavy on the right side
inline static int
RIGHT_IMBALANCE(short bal) {
   return (bal > RIGHT_HEAVY);
}

// ----------------------------------------------- Constructors and Destructors

AvlNode::AvlNode(Term* item)
   : myData(item), myBal(0)
{
   Reset();

   COSTR("avl node");
}

void AvlNode::Construct(Term* item)
{
	myData = item;
	myBal = 0;
	Reset();

	COSTR("reused avl node");
}

AvlNode::~AvlNode(void) 
{
	if (mySubtree[LEFT])
	{
		mySubtree[LEFT]->Dispose();
	}
	if (mySubtree[RIGHT])
	{
		mySubtree[RIGHT]->Dispose();
	}
	if (myData)
	{
		myData->Dispose();
		myData = NULL;
	}

	DESTR("avl node");
}

void AvlNode::Dispose()
{
	_refCount --;
	if (_refCount == 0)
	{
		delete this;
	}
}


// ------------------------------------------------- Rotating and Re-Balancing


int
AvlNode::RotateOnce(AvlNode * & root, dir_t dir)
{
   dir_t  otherDir = Opposite(dir);
   AvlNode * oldRoot = root;

      // See if otherDir subtree is balanced. If it is, then this
      // rotation will *not* change the overall tree height.
      // Otherwise, this rotation will shorten the tree height.
   int  heightChange = (root->mySubtree[otherDir]->myBal == 0)
                          ? HEIGHT_NOCHANGE
                          : HEIGHT_CHANGE;

      // assign new root
   root = oldRoot->mySubtree[otherDir];

      // new-root exchanges it's "dir" mySubtree for it's parent
   oldRoot->mySubtree[otherDir] = root->mySubtree[dir];
   root->mySubtree[dir] = oldRoot;

      // update balances
   oldRoot->myBal = -((dir == LEFT) ? --(root->myBal) : ++(root->myBal));

   return  heightChange;
}


int
AvlNode::RotateTwice(AvlNode * & root, dir_t dir)
{
   dir_t  otherDir = Opposite(dir);
   AvlNode * oldRoot = root;
   AvlNode * oldOtherDirSubtree = root->mySubtree[otherDir];

      // assign new root
   root = oldRoot->mySubtree[otherDir]->mySubtree[dir];

      // new-root exchanges it's "dir" mySubtree for it's grandparent
   oldRoot->mySubtree[otherDir] = root->mySubtree[dir];
   root->mySubtree[dir] = oldRoot;

      // new-root exchanges it's "other-dir" mySubtree for it's parent
   oldOtherDirSubtree->mySubtree[dir] = root->mySubtree[otherDir];
   root->mySubtree[otherDir] = oldOtherDirSubtree;

      // update balances
   root->mySubtree[LEFT]->myBal  = -std::max(int{root->myBal}, 0);
   root->mySubtree[RIGHT]->myBal = -std::min(int{root->myBal}, 0);
   root->myBal = 0;

      // A double rotation always shortens the overall height of the tree
   return  HEIGHT_CHANGE;
}


int
AvlNode::ReBalance(AvlNode * & root) {
   int  heightChange = HEIGHT_NOCHANGE;

   if (LEFT_IMBALANCE(root->myBal)) {
         // Need a right rotation
      if (root->mySubtree[LEFT]->myBal  ==  RIGHT_HEAVY) {
            // RL rotation needed
         heightChange = RotateTwice(root, RIGHT);
      } else {
            // RR rotation needed
         heightChange = RotateOnce(root, RIGHT);
      }
   } else if (RIGHT_IMBALANCE(root->myBal)) {
         // Need a left rotation
      if (root->mySubtree[RIGHT]->myBal  ==  LEFT_HEAVY) {
            // LR rotation needed
         heightChange = RotateTwice(root, LEFT);
      } else {
            // LL rotation needed
         heightChange = RotateOnce(root, LEFT);
      }
   }

   return  heightChange;
}

// ------------------------------------------------------- Comparisons


cmp_t
AvlNode::Compare(TermKeyType key, cmp_t cmp) const
{
   switch (cmp) {
      case EQ_CMP :  // Standard comparison
         return  (cmp_t)myData->Compare(key);

      case MIN_CMP :  // Find the minimal element in this tree
         return  (mySubtree[LEFT] == NULL) ? EQ_CMP : MIN_CMP;

      case MAX_CMP :  // Find the maximal element in this tree
         return  (mySubtree[RIGHT] == NULL) ? EQ_CMP : MAX_CMP;

	  default:
		  Log::PrintLogF(0, "AvlNode::Compare invalid cmp value\n");
		  throw -1;
   }
}

// ------------------------------------------------------- Search/Insert/Delete


Term*
AvlNode::Search(TermKeyType key, AvlNode * root, cmp_t cmp)
{
   cmp_t result;
   while (root  &&  (result = root->Compare(key, cmp))) {
      root = root->mySubtree[(result < 0) ? LEFT : RIGHT];
   }
   return  (root) ? root->myData : NULL;
}


Term*
AvlNode::Insert(Term*   item,
                         AvlNode    * & root)
{
   int  change;
   return  Insert(item, root, change);
}


Term*
AvlNode::Delete(TermKeyType key, AvlNode * & root, cmp_t cmp)
{
   int  change;
   return  Delete(key, root, change, cmp);
}


Term*
AvlNode::Insert(Term*   item,
				AvlNode* &root,
				int& change)
{
	ITimeout::CheckTimeout();

	// See if the tree is empty
   if (root == NULL)
   {
	   // Insert new node here 
	   root = new AvlNode(item);
	   change =  HEIGHT_CHANGE;
	   return  NULL;
   }

   // Initialize
   Term* found = NULL;
   int  increase = 0;

   // Compare items and determine which direction to search
   cmp_t result = root->Compare(item->Key());
   dir_t dir = (result == MIN_CMP) ? LEFT : RIGHT;

   if (result != EQ_CMP) 
   {
	   // Insert into "dir" subtree 
	   found = Insert(item, root->mySubtree[dir], change);
	   if (found)  return  found;   // already here - dont insert
	   increase = result * change;  // set balance factor increment
   } 
   else
   {   
	   // merge terms
	   root->myData->Merge(item);
	   ITimeout::CheckTimeout(); // in a case of exception, item is disposed later
	   item->Dispose();

	   // key already in tree at this node
	   increase = HEIGHT_NOCHANGE;
	   return root->myData;
   }

   root->myBal += increase;    // update balance factor 

  // -----------------------------------------------------------------------------------
  // re-balance if needed -- height of current tree increases only if its
  // subtree height increases and the current tree needs no rotation.
  // -----------------------------------------------------------------------------------

   change =  (increase && root->myBal)
                  ? (1 - ReBalance(root))
                  : HEIGHT_NOCHANGE;
   return  NULL;
}



Term*
AvlNode::Delete(TermKeyType              key,
                         AvlNode * & root,
                         int                & change,
                         cmp_t                cmp)
{
      // See if the tree is empty
   if (root == NULL) {
         // Key not found
      change = HEIGHT_NOCHANGE;
      return  NULL;
   }

      // Initialize
   Term* found = NULL;
   int  decrease = 0;

      // Compare items and determine which direction to search
   cmp_t  result = root->Compare(key, cmp);
   dir_t  dir = (result == MIN_CMP) ? LEFT : RIGHT;

   if (result != EQ_CMP) {
         // Delete from "dir" subtree 
      found = Delete(key, root->mySubtree[dir], change, cmp);
      if (! found)  return  found;   // not found - can't delete
      decrease = result * change;    // set balance factor decrement
   } else  {   // Found key at this node
      found = root->myData;  // set return value
	  found->AddRef();
      // ----------------------------------------------------------------------------------
      // At this point we know "result" is zero and "root" points to
      // the node that we need to delete.  There are three cases:
      //
      //    1) The node is a leaf.  Remove it and return.
      //
      //    2) The node is a branch (has only 1 child). Make "root"
      //       (the pointer to this node) point to the child.
      //
      //    3) The node has two children. We swap items with the successor
      //       of "root" (the smallest item in its right subtree) and delete
      //       the successor from the right subtree of "root".  The
      //       identifier "decrease" should be reset if the subtree height
      //       decreased due to the deletion of the successor of "root".
      // ----------------------------------------------------------------------------------

      if ((root->mySubtree[LEFT] == NULL) &&
          (root->mySubtree[RIGHT] == NULL)) {
             // We have a leaf -- remove it
			  //delete root;
			  root->Dispose();
			  root = NULL;
			  change = HEIGHT_CHANGE;    // height changed from 1 to 0
			  return  found;
      } else if ((root->mySubtree[LEFT] == NULL) ||
                 (root->mySubtree[RIGHT] == NULL)) {
            // We have one child -- only child becomes new root 
         AvlNode * toDelete = root;
         root = root->mySubtree[(root->mySubtree[RIGHT]) ? RIGHT : LEFT];
         change = HEIGHT_CHANGE;    // We just shortened the subtree
            // Null-out the subtree pointers so we dont recursively delete
         toDelete->mySubtree[LEFT] = toDelete->mySubtree[RIGHT] = NULL;
         //delete  toDelete;
		 toDelete->Dispose();
		 return  found;
      } else {
            // We have two children -- find successor and replace our current
            // data item with that of the successor

		  // not disposing this term is a huge memory leak
		  // strange thing is that program runs twice as fast
		  // when term is not dispose!?
		  root->myData->Dispose();

		  root->myData = Delete(key, root->mySubtree[RIGHT],
			  decrease, MIN_CMP);
      }
   }

   root->myBal -= decrease;       // update balance factor 

   // -------------------------------------------------------------------------------------
   // Rebalance if necessary -- the height of current tree changes if one
   // of two things happens: (1) a rotation was performed which changed
   // the height of the subtree (2) the subtree height decreased and now
   // matches the height of its other subtree (so the current tree now
   // has a zero balance when it previously did not).
   // -------------------------------------------------------------------------------------
   //change = (decrease) ? ((root->myBal) ? balance(root) : HEIGHT_CHANGE)
   //                    : HEIGHT_NOCHANGE ;
   if (decrease) {
      if (root->myBal) {
         change = ReBalance(root);  // rebalance and see if height changed
      } else {
         change = HEIGHT_CHANGE;   // balanced because subtree decreased
      }
   } else {
      change = HEIGHT_NOCHANGE;
   }

   return  found;
}

// ---------------------------------------------------------------------------- Verification


int
AvlNode::Height() const {
   int  leftHeight  = (mySubtree[LEFT])  ? mySubtree[LEFT]->Height()  : 0;
   int  rightHeight = (mySubtree[RIGHT]) ? mySubtree[RIGHT]->Height() : 0;
   return 1 + std::max(leftHeight, rightHeight);
}


int
AvlNode::Check() const {
   int  valid = 1;

      // First verify that subtrees are correct
   if (mySubtree[LEFT])   valid *= mySubtree[LEFT]->Check();
   if (mySubtree[RIGHT])  valid *= mySubtree[RIGHT]->Check();

      // Now get the height of each subtree
   int  leftHeight  = (mySubtree[LEFT])  ? mySubtree[LEFT]->Height()  : 0;
   int  rightHeight = (mySubtree[RIGHT]) ? mySubtree[RIGHT]->Height() : 0;

      // Verify that AVL tree property is satisfied
   int  diffHeight = rightHeight - leftHeight;
   if (LEFT_IMBALANCE(diffHeight) || RIGHT_IMBALANCE(diffHeight)) {
      valid = 0;
   }

      // Verify that balance-factor is correct
   if (diffHeight != myBal) {
      valid = 0;
   }

      // Verify that search-tree property is satisfied
   if ((mySubtree[LEFT])
            &&
       (mySubtree[LEFT]->Compare(Key()) == MIN_CMP)) {
      valid = 0;
   }
   if ((mySubtree[RIGHT])
            &&
       (mySubtree[RIGHT]->Compare(Key()) == MAX_CMP)) {
      valid = 0;
   }

   return  valid;
}

//----------------------------------------------- Routines for dumping the tree


enum TraversalOrder { LTREE, KEY, RTREE };

//
// interface methods
//

void TermStorageAvlTree::Construct()
{
	myRoot = NULL;

	COSTR("reused avl tree");
}

int TermStorageAvlTree::AddTerm(Term* term)
{
	term->AddRef();
	try
	{
		Term* t = Insert(term);

		if (t && t->IsZero())
		{
			Delete(t, EQ_CMP);
			t->Dispose();
		}
	}
	catch (int e)
	{
		term->Dispose();
		return e;
	}

	return 0;
}

void TermStorageAvlTree::RemoveTerm(Term* term)
{
	Delete(term, EQ_CMP);
}

uint TermStorageAvlTree::Count() const
{
	uint cnt = myRoot ? myRoot->Count() : 0;
	return cnt;
}

Term* TermStorageAvlTree::GetTerm(uint index) const
{
	Term* term = myRoot->GetTerm(index);
	if (term == NULL)
	{
		Log::PrintLogF(0, "TermStorageAvlTree::GetTerm returns NULL!\n");
		throw -1;
	}
	return term;
}

// enumeration
void TermStorageAvlTree::EnumReset()
{
	_enumIndex = -1;
}

bool TermStorageAvlTree::EnumMoveNext()
{
	_enumIndex ++;
	return _enumIndex < (int)this->Count();
}

Term* TermStorageAvlTree::EnumGetCurrent() const
{
	return this->GetTerm(_enumIndex);
}

//
// interface support methos
//
uint AvlNode::Count() const
{
	uint c = (this->Subtree(LEFT) == NULL ? 0 : this->Subtree(LEFT)->Count()) +
		(this->Subtree(RIGHT) == NULL ? 0 : this->Subtree(RIGHT)->Count()) + 
		1;

	return c;
}

Term* AvlNode::GetTerm(uint index) const
{
	uint diff = 0;
	if (this->Subtree(LEFT))
	{
		diff = this->Subtree(LEFT)->Count();
		if (diff > index)
		{
			return this->Subtree(LEFT)->GetTerm(index);
		}
		else
		{
			index -= diff;
		}
	}

	if (index == 0)
	{
		return this->Data();
	}

	if (this->Subtree(RIGHT))
	{
		// substuct root node
		return this->Subtree(RIGHT)->GetTerm(index - 1);
	}

	// should not happen!
	return NULL;
}
