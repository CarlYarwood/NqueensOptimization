/**
 * nqueens
 * @author original author Jon Beck
 * @author modified by Carl Yarwood
 * @version 11/29/2017
 */
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <climits>
#include "matrix.h"
using namespace std;
/**
 *determines if there is a duplicate value in an array
 *@param the array
 *@param the size ofthe array
 *@param the lower range of possible values
 *@param the upper range of possible values
 *@ return wheater there was a duplecate
 */
bool has_dupe(int array[],uint size,int range_upper,int range_lower){
  int count = 0;
  int shift = range_lower *-1;
  int taken[range_upper - range_lower +1];
  for(int i = range_lower; i<range_upper;i++){
    taken[count]=0;
    count++;
  }
  count = 0;
  for(uint i = 0; i< size ; i++){
    if(taken[array[i]+shift] == 0){
      taken[array[i]+shift]++;
    }
    else{
      return true;
    }
  }
  return false;
}

/**
 * This function checks to see if the current placement of queens on
 * the board is ok.  It is ok if no queen attacks another queen.  A
 * return of true means no conflict.  A return of false means at least
 * two queens conflict. I improved this fucntion by first of all getting
 *rid of the row and collumn checks because now with the updated backtrack
 *method duplicate rows and cols are weeded out ahead of time. I then got rid
 *of most of the looping and instead created two correlated arrays which held
 *respective row and collumn values, I then figured out positive diagonals all
 *have the same row - col value, and negitive diagonals have the same row + col
 *value, so I created two arrays representing each diagonal for all the queens
 *and looked for duplicates, if a duplicate was found false would be returned,
 *otherwise true is returned(I have physical notes to show my work in
 *in determining what I found out about the matrix)
 * @param board the n x n chessboard
 * @return the absence of a conflict with this board arrangement
 */
bool ok( const Matrix< bool > & board,int rows[], int cols[], int size )
{
  int diagonal1 [size];
  int diagonal2 [size];

  // let row and col range over the entire board
  //to determin positions of queens in correlated arrays
  //go's over the amount of queens and assigns a specific diagonal
  //value for both directions that will only re-accure within the
  //array if there is another queen on the same diagonal
  for( int i = 0; i < size; i++ )
  {
    diagonal1[i] = rows[i] - cols[i];
    diagonal2[i] = rows[i] + cols[i];
  }

  if(has_dupe(diagonal1,size,board.numrows()*2, board.numrows()*-1)
     ||has_dupe(diagonal2,size,board.numrows()*2, board.numrows()*-1))
  {
    return false;
  }
  return true;
  
  
}

/**
 * a simple procedure to output an ASCII art horizontal line with plus
 * signs where columns will intersect
 */
void hr( uint cols )
{
  cout << "    +";
  for( uint col = 0; col < cols; col++ )
    cout << "---+";
  cout << endl;
}

/**
 * this function dumps the board to std output
 * @param board the board whose arrangement to dump
 */
void printBoard(uint count, const Matrix< bool> & board )
{
  hr( board.numrows());
  for( uint row = 0; row < board.numrows(); row++)
  {
    cout << ' ' << setw(2) << board.numrows() - row << " |";
    for( uint col = 0; col < board.numrows(); col++ )
    {
      if( board.at(row, col) )
        cout << " X |";
      else
        cout << "   |";
    }
    cout << endl;
    hr( board.numrows());
  }

  cout << "     ";
  for( uint col = 0; col < board.numrows(); col++ )
    cout << ' ' << (char)( 'a' + col ) << "  ";
  cout << endl;
  cout<<count<<endl;
}
/**
 *determined wheter or not an arrya of integers containes
 *a certain value
 *@param value to be checked
 *@param array
 *@param size of array
 *@return if the value is not already in array
 */
bool not_contain(uint value, uint taken[], uint size)
{
  for(uint i = 0; i< size; i++ ){
    if(value == taken[i]){
      return false;
    }
  }
  return true;
}

/**
 * This is the recursive backtracking function. When called, k queens
 * have already been placed on the board in columns 0 .. k-1.  We're
 * trying to place the next queen in column k.
 * @param k the column in which to place the current queen
 * @param board the board on which to place the queen
 */
void r_backtrack(uint  taken[], uint & count, uint k, Matrix< bool > & board, int rows[], int cols[] )
{
  // are we done?
  if( k == board.numrows() )
  {
    // if so, report and exit
    printBoard(count, board );
    exit( 0 );
  }
  cols[k] = k;

  // try each row in turn, for this column
  for( uint row = 0; row < board.numrows(); row++ )
  {
    if(not_contain(row, taken, board.numrows())){
    count++;
    // put a queen here
    board.at(row, k) = true;
    taken[row] = row;
    rows[k]=row;

    // did that cause a conflict?
    if( ok( board,rows, cols,k+1 ))
    {
      // keep going
      r_backtrack(taken, count, k + 1, board, rows, cols );
    }
    // if that didn't work, un-try the current attempt
    board.at(row, k) = false;
    taken[row] = UINT_MAX;
    rows[k] = INT_MAX;
    }
  }
}


/**
 * set up the board and start the backtracking algorithm at the
 * top of the search tree
 */
int main( int argc, char* argv[] )
{
  uint count = 0;
  if( argc != 2 )
  {
    cout << "Usage: " << argv[0] << " n" << endl;
    cout << "       where n is the number of queens to place" << endl;
    cout << "       on an n x n chessboard, with 0 < n < 26" << endl;
    exit( 2 );
  }

  uint n = stoul( argv[1] );
  assert (n > 0 && n < 26);

  // initialize a board with no queens
  uint taken[n];
  int rows[n];
  int cols[n];
  Matrix< bool > board( n, n );
  for( uint row = 0; row < n; row++){
    for( uint col = 0; col < n; col++ ){
      board.at(row, col) = false;
    }
    taken[row] = UINT_MAX;
    rows[row] = INT_MAX;
    cols[row] = INT_MAX;
  }

  // start with column 0
  r_backtrack(taken, count, 0, board, rows , cols );
  cout << "No solution" << endl;
  cout<<count<<endl;
  exit( 1 );
}
