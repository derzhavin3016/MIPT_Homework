#ifndef __PARSER_H_
#define __PARSER_H_

#include "var_fnc.h"
#include "..\Tokens\Tok.h"
#include "..\lang.h"


namespace ad6
{   

  class parser
  {
  private: 
      const token *ptr;
      stock<var> variables;
      stock<fnc> functions;

      int act_fnc;
  public:

    // default constructor
    parser( void ) : variables(),
                     functions(),
                     ptr(nullptr),
                     act_fnc(GLOBAL_VAR)
    {
    }

    // destructor
    ~parser( void )
    {
    }

    unsigned var_size( void )
    {
      return variables.size();
    }

    int get_args_num( unsigned num ) const
    {
      return functions[num].get_args();
    }

    node * getG( const token *tok );

    int find_var( const  char str[] );

    string & get_var( unsigned num ) const;

  private:
    node * _getE( void );
    
    node * _getArgsE( void );

    node * _getComp( void );

    node * _getT( void );
    
    node * _getP( void );
    
    node * _getN( void );
    
    node * _getId( void );

    node * _getPow( void );

    node * _getAct( void );

    node * _getFunc( void );

    node * _getOp( void );

    node * _getAss( void );

    node * _getIf( void );

    node * _getWhile( void );

    node * _getRet( void );

    node * _getDec( void );

    node * _getIns( void );

    node * _getArgs( void );

    unsigned _var_add( const string &st );

    bool _check_ptr_type( tok_type check );

    node * _getPut( void );

    node * _getDeriv( void );

    node * _getGet( void );
  };

  // Process error structure
  struct syntax_error
  {
    const char* descr;
    int line, pos;
    const char* file;
    const char* func;
    //Error* reason;

    syntax_error( const char* des, int l, int pos ) : descr(des),
                                                      line(l),
                                                      pos(pos)
    {
    }
  };
}

#endif /* __PARSER_H_ */

