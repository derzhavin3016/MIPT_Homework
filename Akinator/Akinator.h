#ifndef __AKINATOR_H_
#define __AKINATOR_H_

#define STK_OFF_SEC

#pragma warning (disable: 4267)
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <assert.h>
#include <string>
#include "D:\TX\TXLib.h"
#include "..\List\List.h"
#include "..\Stack\stack.h"
#include "..\Processor\Strings\strings.h" 



#define DEF_NAME "Pig"

#pragma warning (disable: 4996)

namespace ad6
{
  #include <stdexcept>

  char * InputAnswer( const char printfstr[], ... );
  struct Error : public std::runtime_error
  {
    const char* descr;
    int line;
    const char* file;
    const char* func;
    //Error* reason;

    Error( const char* des, int l, const char* filename, const char* function ) : descr(des),
                                                                                  line(l),
                                                                                  file(filename),
                                                                                  func(function),
                                                                                  std::runtime_error(des)
    {
    }

    virtual const std::string what( void ) 
    {
      return descr; 
    }
  };

  const size_t ANSWER_MAX = 1024;
  struct Node
  {
    char *quest;
    Node *parent;
    Node *right;
    Node *left;
    
    // default constructor
    Node( void ) : quest(nullptr),
                   right(nullptr),
                   left(nullptr),
                   parent(nullptr)
    {
    }


    // Constructor by string
    Node( const char str[] ) : quest(new char[ANSWER_MAX]),
                               right(nullptr),
                               left(nullptr),
                               parent(nullptr)
    {
      strcpy(quest, str);
    }

    ~Node( void )
    {
      if (right != nullptr)
      {
        delete right;
        right = nullptr;
      }
      if (left != nullptr)
      {
        delete left;
        left = nullptr;
      }
      if (quest != nullptr)
        delete[] quest;
      parent = nullptr;
    }
  };

  /**
   * \brief Swap two values function by pointers (template).
   * \param [out]   *a  Pointer to first value.
   * \param [out]   *b  Pointer to second value.
   * \return None.
   */
  template <typename Data>
  void Swap( Data* A, Data *B )
  {
    Data tmp = *A;
    *A = *B;
    *B = tmp;
  } /* End of 'Swap' function */


  class Aki
  {
  private:
    Node *root;
    List<char *> Base;

  public:
    // Default constructor
    Aki( void ) : root(new Node(DEF_NAME)),
                  Base()
    {
      Base.Push_tail(root->quest);
    }

    bool SaveTree( const char filename[] ) const;

    bool ReadTree( const char filename[] );

    void Play( void );

    void ProcessLoop( void );

    void Character( const char name[] ) const;

    bool Dump( const char filename[] ) const;

    // Destructor
    ~Aki( void )
    {
      if (root != nullptr)
        delete root;
    }
  private:

    
    void RecDump( FILE *dmp, Node *node ) const;

    bool PrintTree( FILE *f, Node *node ) const;

    bool BuildTree( FILE *f, Node *node );

    Node * Guess( Node *node, int *IsOk );

    bool CreateNodes( Node *prnt );

    Node *Find( const char name[], Node *node ) const;

    bool FillRoute( Stack<int> *answer, Stack<char *> *quest, Node *node ) const;

    bool Compare( const char name1[], const char name2[] ) const;
  };
}


#endif /* __AKINATOR_H_ */
  
