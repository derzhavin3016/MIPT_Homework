#include "Hash_table.h"

#define TBL_LOCATION __FILE__, __LINE__, __FUNCSIG__

#define TBL_ASSERT  assert(TableOk(TBL_LOCATION))
/**
 * \brief Clear hash table function
 */
void ad6::hash_table::Clear( void )
{
  for (int i = 0; i < TABLE_SIZE; i++)
    table[i].Kill();
} /* End of 'Clear' function */

/**
 * \brief LoadTxt from file function
 * \param [in] filename  Name of a file to read from.
 * \return true if all is OK
 * \return false otherwise
 */
bool ad6::hash_table::LoadTxt( const char filename[] )
{
  assert(filename != nullptr);
  TBL_ASSERT;

  buf = FillBuf(filename, &buf_size);
  if (buf == nullptr)
    return false;

  strs = CreateStringsPtrs(buf, buf_size, &strs_size, '\n');
  if (strs == nullptr)
  {
    printf("Error in creating strings.\n");
    return false;
  }
  TBL_ASSERT;
  return true;
} /* End of 'LoadTxt' function */


/**
 * \brief Insert to hash table function
 * \param [in] str    pointer to string to insert.
 * \param [in] IsDic  boolean variable shows is searching needed (true default).
 */
bool ad6::hash_table::Insert( const String *str, bool IsDic /*= true*/ )
{
  TBL_ASSERT;
  hash_t h = Hash_func(str);

  try
  {
    if (IsDic || Find(str, h) == 0)
      table[h % TABLE_SIZE].Push_tail(str);
  }
  catch (...)
  {
    printf("Error in list\n");
    return false;
  }
  TBL_ASSERT;
  return true;
} /* End of 'Insert' function */


/**
 * \brief Find  hash table function
 * \param [in] Hash pointer to hash function.
 * \param [in] IsDic  boolean variable shows is searching needed (true default).
 * \param [in] hsh     Hash of a string(if it has already calculated) (0 default).
 * \return hash of a string if string was find
 * \return 0 if string wasn't find.
 */
hash_t ad6::hash_table::Find( const String *str, hash_t hsh = 0 )
{
  TBL_ASSERT;
  if (hsh)
    hsh = Hash_func(str);

  if (table[hsh % TABLE_SIZE].FindValue(str) == -1)
    return 0;

  TBL_ASSERT;
  return hsh;
} /* End of 'Find' function */

/**
 * \brief Hash all words from buf function.
 * \param [in] Hash pointer to hash function.
 * \param [in] IsDic  boolean variable shows is searching needed (true default).
 */
void ad6::hash_table::Hashing( hash_t (*Hash)( const String *str ), bool IsDic /* = true*/ )
{
  TBL_ASSERT;
  Hash_func = Hash;
  for (size_t cnt = 0; cnt < strs_size; cnt++)
    Insert(strs + cnt, IsDic);
  TBL_ASSERT;
} /* End of 'Hashing' function */


/**
 * \brief Put data to CSV file function
 * \param [in] file_out  Name of a file to read from.
 * \param [in] sep_sym   Separation symbol
 * \param [in] name      Name of hash function
 * \return true if all is OK
 * \return false otherwise
 */
bool ad6::hash_table::PutToCSV( const char file_out[], char sep_sym, const char name[] )
{
  TBL_ASSERT;
  FILE *F = fopen(file_out, "a");
  if (F == nullptr)
  {
    printf("Error with file opening");
    return false;
  }

  if (!IsHeaderPrint)
    PrintCSVHeader(F, sep_sym);

  fprintf(F, "%s", name);
  for (size_t i = 0; i < TABLE_SIZE; i++)
  {
    fprintf(F, "%4d%c", table[i].GetSize(), sep_sym);
  }
  fprintf(F, "\n");

  fclose(F);
  TBL_ASSERT;
  return true;
} /* End of 'PutToCSV' function */


/**
 * \brief Hash table verificator function
 * \param [in]  filename Name of a file where Dump function called.
 * \param [in]  line Line where Dump function called.
 * \param [in]  funcname Name of a function where Dump function called.
 */
bool ad6::hash_table::TableOk( const char filename[], int line, const char funcname[] )
{
  if (Hash_func == nullptr)
  {
    printf("Hashing function was nullptr\n");
    return false;
  }
  for (size_t i = 0; i < TABLE_SIZE; i++)
    if (!table[i].Assert(filename, line, funcname))
      return false;
  return true;
} /* End of 'TableOk' function */

/**
 * \brief Print Header to .csv file function
 * \param [in] f  Pointer to opened file to read from.
 * \param [in] sep_sym   Separation symbol
 */
void ad6::hash_table::PrintCSVHeader( FILE* f,  char sep_sym )
{
  assert(f != nullptr);
  TBL_ASSERT;

  fprintf(f, "List; ");
  for (size_t i = 0; i < TABLE_SIZE; i++)
  {
    fprintf(f, "%4d%c", i, sep_sym);
  }
  fprintf(f, "\n");

  IsHeaderPrint = true;
  TBL_ASSERT;
} /* End of 'PrintCSVHeader' function */

#undef TBL_ASSERT

#undef TBL_LOCATION