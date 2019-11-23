/* FILE NAME: stock.h
 * PROGRAMMER: AD6
 * LAST UPDATE: 14.07.2018
 * PURPOSE: Stock handle;
 */

#ifndef __STOCK_H_
#define __STOCK_H_

#include <cassert>

 namespace ad6
 {
   template <typename data>
   int stk_def_cmp( data a, data b )
   {
     return !(a == b);
   }

   template <typename data>
   class stock
   {
   private:
     size_t real_size, max_size;
     data *array;

   public:

     // Default constructor
     stock( void ) : array(nullptr),

     stock( size_t size ) : array(new data[size]),
                            real_size(0),
                            max_size(size)
     {
     
     }

     // Distructor
     ~stock( void )


     // Copy constructor by constant link
     stock( const stock<data> &CopyData ) : array(new data[CopyData.max_size]),


     // Assignment operator
     stock<data> & operator=( const stock<data> & CopyData )


     /**
      * \brief Add new element to stock function.
      * \param NewData  link to new element to add.
      * \return link to stock class.
      */
     stock<data> & add( const data &NewData )
     {
       if (real_size >= max_size)
       {
         if (max_size == 0)
           max_size = 1;
         else
           max_size *= 2;
        resize(max_size);
       }

       array[real_size++] = NewData;
       return *this;
     }

     void resize( size_t size )
     {
        data *new_bulk(new data[size]);

        if (array != nullptr)
        {
          for (size_t i = 0; i < real_size; i++)

          delete[] array;
        }
        array = new_bulk;
     }

     size_t size( void )
     {
       return real_size;
     }


     void clear( void )
     {
       real_size = 0;
     }


     stock<data> & operator<<( const data &NewData )
     {
       return add(NewData);
     }



     data & operator[]( const int Index )
     {
       assert(Index >= 0 && Index <= (int)real_size);
       return array[Index];
     }


     data operator[]( const int Index ) const
     {
       assert(Index >= 0 && Index <= real_size);
       return array[Index];
     }

      /**
       * \brief Find value in stock function
       * \param Data link to data to find.
       * \return number of element in array if it was find
       * \return -1 otherwise
       */
      int find( const data &Data, int (*comp)(data a, data b) = nullptr )
      {
        if (comp == nullptr)
          comp = stk_def_cmp;

        for (size_t i = 0; i < real_size; i++)
          if (comp(array[i], Data) == 0)
            return i;

        return -1;
      } /* End of 'find' function */
   };
}
#endif /* __STOCK_H_ */


/* End of 'stock.h' file */