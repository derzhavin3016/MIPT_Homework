#ifndef __stack_h_

#include <stdio.h>
#include <iostream>
#include <assert.h>

#define stack_location __FILE__, __LINE__, __FUNCSIG__
#define var_name(var) #var

const size_t stack_start_size = 5;
const size_t stack_delta = 2;

typedef unsigned long long canary_t;

const canary_t owl1_control = 0xDEAD;
const canary_t owl2_control = 0xBEDA;

template <typename Data>
const Data stack_poison_value = -6699;

template <typename Data>
struct Stack
{
  canary_t owl1;
  Data *data;
  int size, maxsize;
  int error;
  unsigned hash;
  canary_t owl2;
};

enum
{
  STK_UNDERFLOW =   -1,
  STK_OVERFLOW  =    1,
  STK_OWL1_ERROR,   //2
  STK_OWL2_ERROR,   //3
  STK_HASH_ERROR,   //4
  STK_SIZE_ERROR,   //5
  STK_DATA_ERROR,   //6
  STK_MEM_ERROR,    //7
  STK_NEW_SIZE_ERROR //8
};

/**
 * \brief Stack initialization function (template).
 * \param this_  Pointer to stack structure.
 * \return true if all is ok.
 * \return false otherwise.
 */
template <typename Data>
bool StackInit( Stack<Data> *this_ )
{
  assert(this_ != nullptr);


  this_->owl1 = owl1_control;
  this_->size = 0;
  this_->maxsize = stack_start_size;
  if ((this_->data = (Data *)calloc(this_->maxsize, sizeof(this_->data[0])))
    == nullptr)
  {
    this_->error = STK_DATA_ERROR;
    return false;
  }
  StackFillPoi(this_, 0);
  this_->error = 0;
  this_->hash = 0;
  this_->owl2 = owl2_control;
  this_->hash = StackHashCalc(this_);

  assert(StackAssert(this_));
  return true;
} /* End of 'StackInit' function */

/**
 * \brief Push value to stack function (template).
 * \param this_  Pointer to stack structure.
 * \param value Value to push.
 * \return true if all is ok.
 * \return false otherwise.
 */
template <typename Data>
bool StackPush( Stack<Data> *this_, Data value )
{
  assert(StackAssert(this_));

  if (this_->size >= this_->maxsize)
    if (!StackResize(this_, this_->maxsize * 2))
      return false;

  this_->data[this_->size++] = value;
  this_->hash = StackHashCalc(this_);

  assert(StackAssert(this_));
  return true;
} /* End of 'StackPush' function */

/**
 * \brief Pop value from stack function (template).
 * \param this_  Pointer to stack structure.
 * \param err    Pointer to return a value.
 * \return true if all is OK.
 * \return false otherwise.
 */
template <typename Data>
bool StackPop( Stack<Data> *this_, Data *value )
{
  assert(StackAssert(this_));

  if (this_->size == 0)
  {
    this_->error = STK_UNDERFLOW;
    return false;
  }
  if (this_->maxsize > stack_start_size &&
    this_->size == this_->maxsize - stack_delta)
    if (!StackResize(this_, this_->maxsize / 2))
      return false;

  *value = this_->data[--this_->size];
  StackFillPoi(this_, this_->size);
  this_->hash = StackHashCalc(this_);
  assert(StackAssert(this_));
  return true;
} /* End of 'StackPop' function */

/**
 * \brief Stack destructor function (template).
 * \param this_  Pointer to stack structure.
 * \return None.
 */
template <typename Data>
void StackClose( Stack<Data> *this_ )
{
  assert(StackAssert(this_));

  free(this_->data);
  this_->size = 0;
} /* End of 'StackClose' function */

/**
 * \brief Stack verificator function (template).
 * \param this_  Pointer to stack structure.
 * \return true if all is ok.
 * \return false otherwise.
 */
template <typename Data>
bool StackOk( Stack<Data> *this_ )
{
  if (this_ == nullptr)
    return false;

  if (this_->data == nullptr)
  {
    this_->error = STK_DATA_ERROR;
    return false;
  }

  if (this_->size < 0)
  {
    this_->error = STK_UNDERFLOW;
    return false;
  }

  if (this_->size > this_->maxsize)
  {
    this_->error = STK_OVERFLOW;
    return false;
  }

  if (this_->owl1 != owl1_control)
  {
    this_->error = STK_OWL1_ERROR;
    return false;
  }

  if (this_->owl2 != owl2_control)
  {
    this_->error = STK_OWL2_ERROR;
    return false;
  }

  unsigned prev_hash = this_->hash;
  if (StackHashCalc(this_) != prev_hash)
  {
    this_->error = STK_HASH_ERROR;
    return false;
  }
  return true;
} /* End of 'StackOk' function */

/**
 * \brief Fill stack by poison values function (template).
 * \param this_  Pointer to stack structure.
 * \param num    Start number to fill.
 * \return None.
 */
template <typename Data>
void StackFillPoi( Stack<Data> *this_, size_t num )
{
  assert(this_ != nullptr);

  for (size_t i = num; i < (size_t)this_->maxsize; i++)
    this_->data[i] = stack_poison_value<Data>;
} /* End of 'StackFillPoi' function */

/**
 * Processing errors in stack fucntion (template).
 * \param [in]  error_code  Error code value.
 * \return None.
 */
template <typename Data>
void Stack_Process_Error( Stack<Data> *this_ )
{
  if (this_ == nullptr)
  {
    StackDump(this_, "", stack_location);
    return;
  }
  switch (this_->error)
  {
  case STK_DATA_ERROR:
    StackDump(this_, "Data was nullptr", stack_location);
    break;
  case STK_HASH_ERROR:
    StackDump(this_, "Different hashes", stack_location);
    break;
  case STK_OVERFLOW:
    StackDump(this_, "Stack overflow", stack_location);
    break;
  case STK_OWL1_ERROR:
    StackDump(this_, "First owl have broken", stack_location);
    break;
  case STK_OWL2_ERROR:
    StackDump(this_, "Second owl have broken", stack_location);
    break;
  case STK_SIZE_ERROR:
    StackDump(this_, "Incorrect size value", stack_location);
    break;
  case STK_UNDERFLOW:
    StackDump(this_, "Stack underflow", stack_location);
    break;
  case STK_NEW_SIZE_ERROR:
    StackDump(this_, "New size too small", stack_location);
    break;
  case STK_MEM_ERROR:
    StackDump(this_, "No free memory for stack", stack_location);
    break;
  default:
    printf("Unrecognized error code = %d\n", this_->error);
    break;
  }
} /* End of 'Stack_Process_Error' function */

/**
 * \brief Stack dump function (template).
 * \param this_  Pointer to stack structure.
 * \param reason Dump reason.
 * \param filename Name of a file where Dump function called.
 * \param line Line where Dump function called.
 * \param funcname Name of a function where Dump function called.
 * \return None.
 */
template <typename Data>
void StackDump( Stack<Data> *this_, const char reason[], const char filename[],
  int line, const char funcname[] )
{
  if (this_ == nullptr)
  {
    printf("Dump (%s) from %s (%d) %s\n", "Pointer to stack was nullptr",
      filename, line, funcname);
    printf("Stack \"%s\" [0x%p] (%s)\n{\n}", var_name(this_),this_, "ERROR!!!");
    return;
  }
  printf("Dump (%s) from %s (%d) %s\n", reason, filename, line, funcname);
  printf("Stack \"%s\"[0x%p] (%s)\n", var_name(*this_), this_, StackOk(this_) ? "ok" : "ERROR!!!");

  printf("{\n");
  printf("  owl1 = %llX\n", this_->owl1);
  printf("  size = %d\n", this_->size);
  printf("  maxsize = %d\n", this_->maxsize);
  printf("  data[%d]%s\n", this_->maxsize, this_->data == nullptr ? 
    " - NULLPTR!!!" : "");
  printf("  {\n");
  if (this_->data != nullptr)
    for (int i = 0; i < this_->maxsize; i++)
    {
      printf("  %c [%d] = ", i < this_->size ? '*' : ' ', i);
      std::cout << this_->data[i];
      if (this_->data[i] == stack_poison_value<Data>)
        printf(" (POISON ?)");
      printf("\n");
    }
  printf("  }\n");
  printf("  error = %d (%s)\n", this_->error, this_->error ? reason : "ok");
  printf("  owl2 = %llX\n", this_->owl2);
  printf("}\n");
} /* End of 'StackDump' function */

/**
 * \brief Hash calculation function (template).
 * \param this_  Pointer to value to calculate hash.
 * \return Calculated hash.
 */
template <typename Data>
unsigned HashCalc( const Data *this_ )
{
  assert(this_ != nullptr);
  const unsigned char *bytes = (unsigned char *)this_;
  size_t bytes_len = sizeof(*this_);
  unsigned hash = 0;

  for (size_t i = 0; i < bytes_len; i++)
    hash += i * (bytes[i] << (i % 4));

  return hash;
} /* End of 'HashCalc' function */

/**
 * \brief Stack hash calculation function (template).
 * \param this_  Pointer to stack.
 * \return Calculated hash.
 * \warning This function also write new hash value to stack structure.
 */
template <typename Data>
unsigned StackHashCalc( Stack<Data> *this_ )
{
  assert(this_ != nullptr);
  
  this_->hash = 0;
  return this_->hash = HashCalc(this_);
} /* End of 'StackHashCalc' function */

/**
 * \brief Stack resize function (template).
 * \param this_  Pointer to stack.
 * \param new_size New size value.
 * \return true if all is ok.
 * \return false otherwise.
 */
template <typename Data>
bool StackResize( Stack<Data> *this_, size_t new_size )
{
  assert(StackAssert(this_));
  if (new_size < stack_start_size)
  {
    this_->error = STK_NEW_SIZE_ERROR;
    return false;
  }

  Data *new_mem = (Data *)realloc(this_->data, new_size);
  if (new_mem == nullptr)
  {
    this_->error = STK_MEM_ERROR;
    return false;
  }

  this_->data = new_mem;
  this_->maxsize = new_size;
  return true;
} /* End of 'StackResize' function */

/**
 * Input one value template fucntion.
 * \param [in]  promt     Promt string to print.
 * \param [in]  scanfstr  String for scanf fucntion (with expected value).
 * \param [out] value     Pointer to value to input.
 * \return 1 if all is OK, 0 otherwise.
 */
template <typename T>
int Input( const char promt[], const char scanfstr[], T *value )
{
  if (promt == NULL || scanfstr == NULL || value == NULL)
    return 0;

  printf(promt);
  return scanf(scanfstr, value);
} /* End of 'Input' function */

/**
 * \brief Stack assertion function (template).
 * \param this_ Pointer to stack structure.
 * \return true if all id OK.
 * \return false otherwise.
 */
template <typename Data>
bool StackAssert( Stack<Data> *this_ )
{
  if (!StackOk(this_))
  {
    Stack_Process_Error(this_);
    return false;
  }
  return true;
} /* End of 'StackAssert' function */
#endif /* __stack_h_ */


