#include "Lng.h"

/**
 * \brief Get user's answer from stdout function.
 * \brief This function put scanned string to static buffer, allocate memory for the answer and return pointer.
 * \param printfstr   String for printf.
 * \param  ...        Printf parameters.
 * \return  pointer to the answer.
 * \warning Using static buffer.
 */
char * ad6::InputAnswer( const char printfstr[], ... )
{
  static char buf[ad6::ANSWER_MAX] = {};
  va_list args;

  if (printfstr == nullptr)
    return buf;

  va_start(args, printfstr);
  vprintf(printfstr, args);
  va_end(args);

  scanf("\n%1024[^\n]", buf);

  return buf;
} /* End of 'InputAnswer' function */

void ad6::frontend::_pre_par( void )
{
  toks = new token[buf_size + 1];
  unsigned str_counter = 0, pos_counter = 0;
  int pos = 0;

  for (unsigned i = 0; i < buf_size; i++)
  {
    if (isspace(buf[i]))
    {
      if (buf[i] == '\n')
      {
        pos_counter = 0;
        str_counter++;
      }
      continue;
    }

    if (isdigit(buf[i]))
    {
      toks[toks_size++] = token(_getNum(&i), str_counter + 1, pos_counter + 1);
      continue;
    }

    if (isalpha(buf[i]))
    {
      unsigned end_word = _getWord(&i);

      toks[toks_size++] = token(TOK_STR, 0, str_counter + 1, pos_counter + 1, buf + i, end_word);
      i += end_word - 1;
      continue;
    }
    toks[toks_size++] = token(TOK_SMB, 0, str_counter + 1, pos_counter + 1, buf + i, 1);
  }
  toks[toks_size++] = token(TOK_NUL, 0, str_counter + 1, buf_size);
}

/*
*/
unsigned ad6::frontend::_getWord( unsigned *pos )
{
  unsigned new_pos = 0;
  
  do
  {
    new_pos++;
  } while (isalpha((unsigned char)buf[*pos + new_pos]));

  return new_pos;
} /* End of '_getWord' function */

/* 
*/
double ad6::frontend::_getNum( unsigned *pos )
{
  double num = 0;
  unsigned add_pos = 0;
  
  if (sscanf(buf + *pos, "%lg%n", &num, &add_pos) != 1)
    return NAN;

  *pos = *pos + add_pos - 1;
  return num;
} /* End of '_getNum' function */

/**
 * \brief Translate to AST function.
 * \param [in] file_in        name of a file to read code from.
 * \param [in] file_out       name of a file to save tree.
 */
void ad6::frontend::translate( const char file_in[], const char file_out[] )
{
  _par_tree(file_in, file_out);
} /* End of 'translate' function */ 

/**
 * \brief Parse code to AST function.
 * \param [in] file_in        name of a file to read code from.
 * \param [in] file_out       name of a file to save tree.
 */
void ad6::frontend::_par_tree( const char file_in[], const char file_out[] )
{
  _read_par_tree(file_in);
  _save_tree(file_out);
} /* End of 'par_tree' function */

/**
 * \brief Read tree from file function.
 * \param [in] pointer to file structure
 * \param [in]  filename  Name of a file to read from.
 * \return true if all is OK.
 * \return false otherwise.
 */
bool ad6::frontend::_read_par_tree( const char filename[] )
{
  TREE_ASSERT(filename != nullptr, "Incorrect file name");

  buf = FillBuf(filename, &buf_size);

  char IsOk = 0;
  unsigned pos = 0;
  TREE_ASSERT(buf != nullptr, "Error with opening file")

  if (root != nullptr)
    delete root;

  _pre_par();

  root = par.getG(toks);

  return true; 
} /* End of '_read_par_tree' function */


/**
 * \brief Find operator number function.
 * \brief string ppointer to string.
 * \return number of the operator if it was find.
 * \return -1 otherwise.
*/
int ad6::frontend::find_op( char sym )
{

    #define DEF_OP(num, name, calc, diff)                 \
     if (sym == num)                              \
       return num;

  #include "..\cmd.h"

  #undef DEF_OP
  return -1;
} /* End of 'find_op' function */

/**
 * \brief Find variable in tree function.
 * \param [in] start Pointer to tree's node.
 * \param [in] var_num Number of variable in variables array.
*/
bool ad6::frontend::_find_var_tree( node *start, unsigned var_num ) const
{
  if (start->type == TYPE_VAR && start->num == var_num)
    return false;
  
  #define CHECK_NODE(nd_check)                                      \
  bool is_var_##nd_check = true;                                    \
  if (start->##nd_check != nullptr)                                 \
    is_var_##nd_check = _find_var_tree(start->##nd_check, var_num);

  CHECK_NODE(left);

  CHECK_NODE(right);

#undef CHECK_NODE

  return is_var_left && is_var_right;
} /* End of '_find_var_tree' function */


#undef TEX_DMP

#undef TEX_END  
