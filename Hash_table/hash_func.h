#ifndef __HASH_FUNC_H_
#define __HASH_FUNC_H_

#include "../Processor/Strings/strings.h"
#include "Hash_table.h"

/**
 * \brief Simply the best hash ever.
 * \param str   pointer to data to calculate hash.
 * \return Calculated hash.
 */
hash_t BestHash( const String *str )
{
  return 1;
} /* End of 'BestHash' function */

/**
 * \brief Fist char hash.
 * \param str   pointer to data to calculate hash.
 * \return Calculated hash.
 */
hash_t FstChrHash( const String *str )
{
  return str->str[0];
} /* End of 'FstChrHash' function */

/**
 * \brief Length hash.
 * \param str   pointer to data to calculate hash.
 * \return Calculated hash.
 */
hash_t LenHash( const String *str )
{
  return str->len;
} /* End of 'FstChrHash' function */

/**
 * \brief Sum bytes hash.
 * \param str   pointer to data to calculate hash.
 * \return Calculated hash.
 */
hash_t SumHash( const String *str )
{
  hash_t hash = 0;
  for (size_t i = 0; i < str->len; i++)
    hash += str->str[i];
  return hash;
} /* End of 'SumHash' function */

template <typename Data>
__inline Data Rol( Data num )
{
  return (num << 1) | ((num & (1 << (sizeof(Data) - 1))) >> (sizeof(Data) - 1));
}

/**
 * \brief Ded hash.
 * \param str   pointer to data to calculate hash.
 * \return Calculated hash.
 */
hash_t DedHash( const String *str )
{
  hash_t hash = 0;
  for (size_t i = 1; i < str->len; i++)
    hash = Rol(hash) ^ str->str[i];

  return hash;
} /* End of 'SumHash' function */

/**
 * \brief FAQ6 hash.
 * \param str   pointer to data to calculate hash.
 * \return Calculated hash.
 */
hash_t FAQ6Hash( const String *str )
{
  hash_t hash = 0;
  
  for (size_t i = 0; i < str->len; i++)
  {
      hash += (unsigned char)(str->str[i]);
      hash += (hash << 10);
      hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  
  return hash;
} /* End of 'FAQ6Hash' function */

/**
 * \brief Ly hash.
 * \param str   pointer to data to calculate hash.
 * \return Calculated hash.
 */
hash_t LyHash( const String *str )
{
  hash_t hash = 0;
  
  for (size_t i = 0; i < str->len; i++)
      hash = (hash * 1664525) + (unsigned char)(str->str[i]) + 1013904223;
  
  return hash;
} /* End of 'LyHash' function */

#endif /* __HASH_FUNC_H_ */
