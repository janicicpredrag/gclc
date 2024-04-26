#pragma once

#include "TermStorage.h"

// Indices into a subtree array
//     NOTE: I would place this inside the AvlNode class but 
//           when I do, g++ complains when I use dir_t. Even
//           when I prefix it with AvlNode:: or AvlNode::
//           (If you can get this working please let me know)
//
enum  dir_t { LEFT = 0, RIGHT = 1 };

// cmp_t is an enumeration type indicating the result of a
// comparison.
enum  cmp_t {
	MIN_CMP = -1,   // less than
	EQ_CMP  = 0,    // equal to
	MAX_CMP = 1     // greater than
};

// AvlNode -- Class to implement an AVL Tree
//
class AvlNode  : public Object
{
public:
	// Max number of subtrees per node
   enum  { MAX_SUBTREES = 2 };

   // Return the opposite direction of the given index
   static  dir_t
   Opposite(dir_t dir) { 
     return dir_t(1 - int(dir));
   }

   // ----- Constructors and destructors: 

   AvlNode(Term* item=NULL);
   void Construct(Term* item=NULL);
   ~AvlNode(void);
   void Dispose();

   // ----- Query attributes:

   // Get this node's data
   Term*
   Data() const { return  myData; }

   // Get this node's key field
   TermKeyType
   Key() const { return  myData->Key(); }

   // Query the balance factor, it will be a value between -1 .. 1
   // where:
   //     -1 => left subtree is taller than right subtree
   //      0 => left and right subtree are equal in height
   //      1 => right subtree is taller than left subtree
   short
   Bal(void) const { return  myBal; }

   // Get the item at the top of the left/right subtree of this
   // item (the result may be NULL if there is no such item).
   //
   AvlNode *
   Subtree(dir_t dir) const { return  mySubtree[dir]; }

   // ----- Search/Insert/Delete
   //
   //   NOTE: These are all static functions instead of member functions
   //         because most of them need to modify the given tree root
   //         pointer. If these were instance member functions than
   //         that would correspond to having to modify the 'this'
   //         pointer, which is not allowed in C++. Most of the 
   //         functions that are static and which take an AVL tree
   //         pointer as a parameter are static for this reason.
   
   // Look for the given key, return NULL if not found,
   // otherwise return the item's address.
   static Term*
   Search(TermKeyType key, AvlNode * root, cmp_t cmp=EQ_CMP);

   // Insert the given key, return NULL if it was inserted,
   // otherwise return the existing item with the same key.
   static Term*
   Insert(Term* item, AvlNode * & root);

   // Delete the given key from the tree. Return the corresponding
   // node, or return NULL if it was not found.
   static Term*
   Delete(TermKeyType key, AvlNode * & root, cmp_t cmp=EQ_CMP);

   // Verification 

   // Return the height of this tree
   int
   Height() const;

   // Verify this tree is a valid AVL tree, return TRUE if it is,
   // return FALSE otherwise
   int
   Check() const;

   // interface support methos
   uint Count() const;
   Term* GetTerm(uint index) const;

private:

   // ----- Private data

   Term* myData;  // Data field
   AvlNode    * mySubtree[MAX_SUBTREES];   // Pointers to subtrees
   short      myBal;   // Balance factor

   // Reset all subtrees to null and clear the balance factor
   void
   Reset(void) {
      myBal = 0 ;
      mySubtree[LEFT] = mySubtree[RIGHT] = NULL ;
   }

   // ----- Routines that do the *real* insertion/deletion

   // Insert the given key into the given tree. Return the node if
   // it already exists. Otherwise return NULL to indicate that
   // the key was successfully inserted.  Upon return, the "change"
   // parameter will be '1' if the tree height changed as a result
   // of the insertion (otherwise "change" will be 0).
   static Term*
   Insert(Term* item,
             AvlNode * & root,
             int & change);

   // Delete the given key from the given tree. Return NULL if the
   // key is not found in the tree. Otherwise return a pointer to the
   // node that was removed from the tree.  Upon return, the "change"
   // parameter will be '1' if the tree height changed as a result
   // of the deletion (otherwise "change" will be 0).
   static Term*
   Delete(TermKeyType key,
             AvlNode * & root,
             int & change,
             cmp_t cmp=EQ_CMP);

   // Routines for rebalancing and rotating subtrees

   // Perform an XX rotation for the given direction 'X'.
   // Return 1 if the tree height changes due to rotation,
   // otherwise return 0.
   static int
   RotateOnce(AvlNode * & root, dir_t dir);

   // Perform an XY rotation for the given direction 'X'
   // Return 1 if the tree height changes due to rotation,
   // otherwise return 0.
   static int
   RotateTwice(AvlNode * & root, dir_t dir);

   // Rebalance a (sub)tree if it has become imbalanced
   static int
   ReBalance(AvlNode * & root);

   // Perform a comparison of the given key against the given
   // item using the given criteria (min, max, or equivalence
   // comparison). Returns:
   //   EQ_CMP if the keys are equivalent
   //   MIN_CMP if this key is less than the item's key
   //   MAX_CMP if this key is greater than item's key
   cmp_t
   Compare(TermKeyType key, cmp_t cmp=EQ_CMP) const;

private:
	// Disallow copying and assignment
   AvlNode(const AvlNode &);
   AvlNode & operator=(const AvlNode &);

};


// Class TermStorageAvlTree is a simple container object to "house" an AvlNode
// that represents the root-node of and TermStorageAvlTree. Most of the member
// functions simply delegate to the root AvlNode.

class TermStorageAvlTree : public TermStorage
{
private:
	// Disallow copying and assingment
   TermStorageAvlTree(const TermStorageAvlTree &);
   TermStorageAvlTree & operator=(const TermStorageAvlTree &);

   // Member data
   AvlNode * myRoot;   // The root of the tree

   // enumeration
   int _enumIndex;

public:
	// Constructor and destructor
   TermStorageAvlTree() : myRoot(NULL) 
   {
	   COSTR("avl tree");
   };
   ~TermStorageAvlTree() 
   { 
	   if (myRoot)
	   {
		   myRoot->Dispose();
	   }
	   DESTR("avl tree");
   }
   void Construct();

   // See if the tree is empty
   int
   IsEmpty() const {
      return  (myRoot == NULL);
   }

   // Search, Insert, Delete, and Check
   Term*
   Search(TermKeyType key, cmp_t cmp=EQ_CMP) {
      return  AvlNode::Search(key, myRoot, cmp);
   }

   Term*
   Insert(Term* item) {
      return  AvlNode::Insert(item, myRoot);
   }

   Term*
   Delete(TermKeyType key, cmp_t cmp=EQ_CMP) {
      return  AvlNode::Delete(key, myRoot, cmp);
   }

   int
   Check() const {
      return  (myRoot) ? myRoot->Check() : 1;
   }

   // interface methods
   int AddTerm(Term* term);
   void RemoveTerm(Term* term);
   uint Count() const;

   Term* GetTerm(uint index) const;

   // enumeration
   void EnumReset();
   bool EnumMoveNext();
   Term* EnumGetCurrent() const;
};
