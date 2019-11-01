#ifndef __LIST_H_
#define __LIST_H_

#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "..//Stack/input.h"
#include "List_def.h"
//#include "D:\\TX\TXLib.h"

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

/**
 * \brief Clamping value function (template).
 * \param [in] value  Value to clamp.
 * \param [in]  min   Minimal value.
 * \param [in]  max   Maximum value.
 * \return Clamping value.
 */
template <typename Data>
Data Clamp( Data value, Data min, Data max )
{
  return value > max ? max : value < min ? min : value;
} /* End of 'Clamp' function */


// Original namespace
namespace ad6
{
  // Template list class
  template <typename Data>
  class List
  {
  private:
    list_elem<Data> *elems;
    size_t free_plc, head, tail;
    size_t size, maxsize;
    int error;

  public:

    /**
     * \brief Default list constructor function (template).
     * \param None.
     * \return None.
     */
    List( void ) : size(0),
                   free_plc(1),
                   maxsize(LIST_START_SIZE),
                   error(0),
                   elems(nullptr),
                   head(0),
                   tail(0)

    {
      DataCalloc();
    } /* End of 'List' function */

    // Get actual number of head
    int GetHead( void )
    {
      return head;
    }

    // Get actual number of tail
    int GetTail( void )
    {
      return tail;
    }

    /**
     * \brief get actual number of the next element for current one.
     * \param [in] num   actual number of element.
     * \return actual num of next element
     * \return -1 if num incorrect
     */
    int GetNext( size_t num )
    {
      LIST_IF_COR_NUM;
      LIST_IF_FREE;
      return elems[num].next;
    } /* End of 'GetNext' function */

    /**
     * \brief get actual number of the previous element for current one.
     * \param [in] num   actual number of element.
     * \return actual num of previous element
     * \return -1 if num incorrect
     */
    int GetPrev( size_t num )
    {
      LIST_IF_COR_NUM;
      LIST_IF_FREE;
      return elems[num].prev;
    } /* End of 'GetNext' function */

    /**
     * \brief Push value to the end of list function (template).
     * \param [in]  value   Value to push.
     * \return actual number of inserted element if all is ok.
     * \return -1 otherwise.
     */
    int Push_tail( Data value )
    {
      LST_ASSERT();
      LIST_FST_PUSH;
      return Insert_af(value, tail);
    } /* End of 'Push_tail' function */

    /**
     * \brief Push value to the begin of list function (template).
     * \param [in]  value   Value to push.
     * \return actual number of inserted element if all is ok.
     * \return -1 otherwise.
     */
    int Push_head( Data value )
    {
      LST_ASSERT();
      LIST_FST_PUSH;
      return Insert_bef(value, head);
    } /* End of 'Push_head' function */

    /**
     * \brief Insert value to the list before element function (template).
     * \param [in]  value   Value to push.
     * \param [in]  num     actual num of the element to push value before
     * \return actual number of inserted element if all is ok.
     * \return -1 otherwise.
     */
    int Insert_bef( Data value, size_t num )
    {
      LST_ASSERT();
      LIST_IF_COR_NUM;
      LIST_IF_FREE;
      LIST_IF_LAST_FREE;
      
      LIST_INSERT(next, prev, head);

      LST_ASSERT();
      return new_num;
    } /* End of 'Insert_bef' function */

    /**
     * \brief Insert value to the list after element function (template).
     * \param [in]  value   Value to push.
     * \param [in]  num     actual num of the element to push value after.
     * \return actual number of inserted element if all is ok.
     * \return -1 otherwise.
     */
    int Insert_af( Data value, size_t num )
    {
      LST_ASSERT();

      LIST_IF_COR_NUM;
      LIST_IF_FREE;
      LIST_IF_LAST_FREE;

      LIST_INSERT(prev, next, tail);

      LST_ASSERT();
      return new_num;
    } /* End of 'Insert_af' function */

    /**
     * \brief Delete element by actual number function (template).
     * \param [out]  pointer to value (if this value needed) nullptr default.
     * \return true if all is OK.
     * \return false otherwise.
     */
    bool Delete( size_t num, Data *value = nullptr )
    {
      LST_ASSERT();

      LIST_IF_COR_NUM;
      LIST_IF_FREE;
      
      if (size <= 0)
      {
        error = LST_UNDERFLOW;
        LST_ASSERT();
        return false;
      }

      if (value != nullptr)
        *value = elems[num].data;

      if (size != 1)
      {
        if (num == head)
        {
          head = elems[num].next;
          elems[elems[num].next].prev = 0;
        }
        else if (num == tail)
        {
          tail = elems[num].prev;
          elems[elems[num].prev].next = 0;
        }
        else
        {
          elems[elems[num].prev].next = elems[num].next;
          elems[elems[num].next].prev = elems[num].prev;
        }
      }
      else
        head = tail = 0;

      elems[num].next = free_plc;
      elems[num].prev = -1;
      free_plc = num;

      size--;

      LST_ASSERT();
      return true;
    } /* End of 'Pop' function */

    /**
     * \brief Clear list function
     * \brief This function fetch list to the initial state.
     * \warning After this function ALL data will be lost!
     */
    void Kill( void )
    {
      LST_ASSERT();
      
      size = 0;
      head = tail = 0;
      free_plc = 1;

      ChangeSize(LIST_START_SIZE);
      FillFree();

      LST_ASSERT();
    } /* End of 'Kill' function */

    /**
     * \brief List destructor function (template).
     * \param None.
     * \return None.
     */
    ~List( void )
    {
      if (elems != nullptr)
        free(elems);
    } /* End of '~List' function */

    /**
     * \brief List dump function (template).
     * \param [in]  reason Dump reason.
     * \param [in]  filename Name of a file where Dump function called.
     * \param [in]  line Line where Dump function called.
     * \param [in]  funcname Name of a function where Dump function called.
     * \return None.
     */
    void Dump( const char reason[], const char filename[],
      int line, const char funcname[] ) const
    {
      printf("Dump ('%s') from %s (%d) in %s\n", reason, filename, line, funcname);

      printf("List [0x%p] (%s)\n", this, error ? "ERROR!!!" : "ok");

      printf("{\n");
      printf("  free_plc = %d\n", free_plc);
      printf("  head = %d\n  tail = %d\n  size = %d\n  maxsize = %d\n",
                head,        tail,        size,        maxsize);
      printf("  error = %d (%s)\n", error, reason);

      printf("  Number: ");
        for (size_t i = 0; i < maxsize; i++)
          printf(" %05d |", i);
      printf("\n");
      printf("  Data: ");
        for (size_t i = 0; i < maxsize; i++)
          printf(" %05d |", elems[i].data);
      printf("\n");

      printf("  Next: ");
        for (size_t i = 0; i < maxsize; i++)
          printf(" %05d |", elems[i].next);
      printf("\n");

      printf("  Prev: ");
        for (size_t i = 0; i < maxsize; i++)
          printf(" %05d |", elems[i].prev);
      printf("\n");
      printf("}\n");
    } /* End of 'Dump' function */

  private:
    /**
     * \brief Allocate memory for list function(template)
     * \param None
     * \return true if all is OK
     * \return false otherwise.
     */
    bool DataCalloc( void )
    {
      elems = (list_elem<Data> *)calloc(maxsize, sizeof(list_elem<Data>));
      if (elems == nullptr)
      {
        return false;
      }
      // Fill first value of data as poison
      elems[0].data = LIST_POISON_VALUE<Data>;

      // Fill array as free
      FillFree();

      #undef MEM_ALL
      return true;
    } /* End of 'DataCalloc' function */

    /**
     * \brief Fill elems array as a fully free.
     * \brief Next with increasing values, prev with FREE_PREV.
     * \param [in] index     Start index. 1 default
     * \param [in] IsCheck   Boolean variable, 
     *  says if function need to check if the element free (false default).
     */
    void FillFree( size_t index = 1, bool IsCheck = true )
    {
      assert(index > 0 && index < maxsize);

      LST_ASSERT();

      size_t last = 0;
      for (size_t i = index; i < maxsize; i++)  
        if (IsCheck || elems[i].prev == FREE_PREV)
        {
          elems[i].data = 0;
          elems[i].next = i + 1;
          elems[i].prev = FREE_PREV;
          last = i;
        }

        elems[last].next = LAST_FREE;

        LST_ASSERT();
    } /* End of 'FillFree' function */

    /**
     * \brief Find element in list by logical number.
     * \param [in] num logical number
     * \return actual element number.
     * \return -1 if element was not find.
     */
    size_t Find( size_t num )
    {
      LST_ASSERT();
      if (num >= size)
        return -1;
      size_t act = 0;
      if (size - num <= num)
        act = GoTail(num);
      else
        act = GoHead(num);
      
      LST_ASSERT();
      return act;
    } /* End of 'Find' function */

    /**
     * \brief Find element from tail by logical number.
     * \param [in] num logical number
     * \return actual element number.
     * \return -1 if element was not find.
     */
    size_t GoTail( size_t num )
    {
      if (num >= size)
        return -1;

      num = size - 1 - num;
      size_t count = 0, act = tail;

      while (count++ < num)
        act = elems[act].prev;
      
      return act;
    } /* End of 'GoTail' function */

    /**
     * \brief Find element from head by logical number.
     * \param [in] num logical number
     * \return actual element number.
     * \return -1 if element was not find.
     */
    size_t GoHead( size_t num )
    {
      if (num >= size)
        return -1;
      size_t count = 0, act = head;

      while (count++ < num)
        act = elems[act].next;
      
      return act;
    } /* End of 'GoHead' function */

    /**
     * \brief List verificator function (template).
     * \return true if all is ok.
     * \return false otherwise.
     */
    bool ListOk( void )
    {
      #define LIST_IF_BETWEEN(num)  ((num) > maxsize - 1 || (num) < 0)

      LIST_COND_CHECK(elems == nullptr,                         LST_DATA_ERROR);
      LIST_COND_CHECK(size < 0,                                 LST_UNDERFLOW);
      LIST_COND_CHECK(size > maxsize,                           LST_OVERFLOW);
      LIST_COND_CHECK(elems[0].data != LIST_POISON_VALUE<Data>, LST_POI_ERROR);
      LIST_COND_CHECK(LIST_IF_BETWEEN(free_plc),                LST_NO_FREE_ERROR);
      LIST_COND_CHECK(LIST_IF_BETWEEN(head),                    LST_HEAD_ERROR);
      LIST_COND_CHECK(LIST_IF_BETWEEN(tail),                    LST_TAIL_ERROR);

      #undef LIST_IF_BETWEEN
      return true;
    } /* End of 'ListOk' function */

    /**
     * Processing errors in list fucntion (template).
     * \param [in]  error_code  Error code value.
     * \return None.
     */
    void List_Process_Error(const char filename[], int line, const char funcname[]) const
    {
#define LIST_DUMP_CASE(ERR_CODE, STR)  case ERR_CODE:                                      \
                                               Dump(STR, filename, line, funcname);   \
                                               break;
      switch (error)
      {
        LIST_DUMP_CASE(LST_DATA_ERROR,      "Elems was nullptr");
        LIST_DUMP_CASE(LST_HEAD_ERROR,      "Incorrect head value");
        LIST_DUMP_CASE(LST_MEM_ERROR,       "No free memory for list");
        LIST_DUMP_CASE(LST_NEW_SIZE_ERROR,  "New size is too small");
        LIST_DUMP_CASE(LST_NO_FREE_ERROR,   "No free place in list");
        LIST_DUMP_CASE(LST_OVERFLOW,        "List overflow");
        LIST_DUMP_CASE(LST_POI_ERROR,       "Poison values have broken");
        LIST_DUMP_CASE(LST_SIZE_ERROR,      "Incorrect size value");
        LIST_DUMP_CASE(LST_TAIL_ERROR,      "Incorrect tail value");
        LIST_DUMP_CASE(LST_UNDERFLOW,       "List underflow");

      default:
        printf("Unrecognized error code = %d\n", error);
        break;
      }

#undef LIST_DUMP_CASE
    } /* End of 'List_Process_Error' function */

    /**
     * \brief List resize function (template).
     * \param [in]  new_size New size value.
     * \return true if all is ok.
     * \return false otherwise.
     */
    bool Resize( size_t new_size )
    {
      size_t prev_size = ChangeSize(new_size);

      bool IsIncr = new_size > maxsize;

      if (IsIncr)
      {
        elems[free_plc].next = prev_size;
        FillFree(prev_size);
      }
      else
        FillFree(1, true);

      LST_ASSERT();
      return true;
    } /* End of 'Resize' function */

    /**
     * \brief Change list size function (only changes size)
     * \param [in]  New size
     * \return Previous size
     */
    size_t ChangeSize( size_t new_size )
    {
      assert(new_size >= 0);
      LST_ASSERT();

      if (new_size == maxsize)
        return true;

      LIST_COND_CHECK(new_size < LIST_START_SIZE, LST_NEW_SIZE_ERROR);

      list_elem<Data> *new_mem = (list_elem<Data> *)realloc(elems, new_size * sizeof(list_elem<Data>));

      LIST_COND_CHECK(new_mem == nullptr, LST_MEM_ERROR);

      elems = new_mem;
      size_t prev_size = maxsize;
      maxsize = new_size;

      return prev_size;
    } /* End of 'ChangeSize' function */

    /**
     * \brief List assertion function (template).
     * \return true if all id OK.
     * \return false otherwise.
     */
    bool Assert( const char filename[], int line, const char funcname[] )
    {
      if (error != 0)
      {
        List_Process_Error(filename, line, funcname);
        return false;
      }
      if (!ListOk())
      {
        List_Process_Error(filename, line, funcname);
        return false;
      }
      return true;
    } /* End of 'Assert' function */
  };
}


/**
 * \brief Work with list function (template).
 * \param [in, out] this_ Pointer to list structure.
 * \return None.
 */
template <typename Data>
void ListProcLoop( ad6::List<Data> *this_ )
{
  int prmt = 0;
  const char *types[4] = {"%d", "%f", "%lg", "%c"};
  int chosen_type = 0;
  int _where = 0;
  Data ins = 0;
  char promt = 0;
  while(1)
  {
    int OK = InputNumbers(LST_START_PROMT, "*** Input number to start:\n",
      "%d", &prmt);
    assert(OK);
    switch (prmt)
    {
    case 0:
      return;
    case 1:
      OK = InputNumbers("PUSH_HEAD\n", "Input value to push:\n",
        types[chosen_type], &ins);
      assert(OK);
      if (this_->Push_head(ins) == -1)
        return;
      else
        printf("Push succeed\n");
      break;
    case 2:
      OK = InputNumbers("PUSH_TAIL\n", "Input value to push:\n",
        types[chosen_type], &ins);
      assert(OK);
      if (this_->Push_tail(ins) == -1)
        return;
      else
        printf("Push succeed\n");
      break;
    case 3:
      OK = InputNumbers("INSERT_BEFORE\n", "Input value to insert:\n",
        types[chosen_type], &ins);
      assert(OK);
      OK = InputNumbers("", "Input number where to insert:\n",
        "%d", &_where);
      assert(OK);
      if (this_->Insert_bef(ins, _where) == -1)
        return;
      else
        printf("Insert succeed\n");
      break;
    case 4:
      OK = InputNumbers("INSERT_AFTER\n", "Input value to insert:\n",
        types[chosen_type], &ins);
      assert(OK);
      OK = InputNumbers("", "Input number where to insert:\n",
        "%d", &_where);
      assert(OK);
      if (this_->Insert_af(ins, _where) == -1)
        return;
      else
        printf("Insert succeed\n");
      break;
    case 5:
      OK = InputNumbers("DELETE\n", "Input number to delete:\n",
        "%d", &_where);
      assert(OK);
      if (!this_->Delete(_where))
        return;
      else
        printf("Delete succeed\n");
      break;
    case 6:
      this_->Dump("Just info", LIST_LOCATION);
      break;
    case 7:
      printf("Wow, man, not so fast, calm down.\n"
             "Are you sure?\n"
             "I will kill all your data.\n");
      promt = getchar();
      if (promt == 'Y' || promt == 'y')
         this_->Kill();
      printf("OK, this is your choice....\n")
      break;
    default:
      printf("Unrecognized switch\n");
      break;
    }
  }
}
#undef LIST_FST_PUSH

#endif /* __LIST_H_ */

