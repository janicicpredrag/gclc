#pragma once

//#define CRT_DBG

#ifdef CRT_DBG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <stdio.h>
#include "stdlib.h"
#include "math.h"

#include <vector>


/*************************************************************
*
* stdinc.h
*
* Include header for all files in project.
*
**************************************************************/

typedef unsigned int uint;
typedef double REAL;

#define E_TIMEOUT      10023
#define E_TIMEOUTERROR 10024
#define E_COMPLEX      10025

#define MAX2(a,b) ((a)>(b)?(a):(b))
#define MIN2(a,b) ((a)<(b)?(a):(b))

#define DESER_DBG 0

//
// which TermStorage class to use
// Vector
// AVL Tree
// ...
///
#define TERM_STORAGE_CLASS_VECTOR 0
#define TERM_STORAGE_CLASS_AVL_TREE 1

// use object banks instead of normal constructor
//#define USE_OBJECT_BANKS
#ifdef USE_OBJECT_BANKS
#define POWER_BANK
#define XTERMS_BANK
#define UTERMS_BANK
#define UPOLYS_BANK
#define XPOLYS_BANK
#define UPOLYF_BANK
#define AVLNODE_BANK
#define AVLTREE_BANK
#endif
