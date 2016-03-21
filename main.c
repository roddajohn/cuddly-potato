#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main( int argc, char **argv ) {

  screen s;
  struct matrix *edges;
  struct matrix *transform;

  color c;
  c.red = 0;
  c.blue = MAX_COLOR;
  c.green = 0;

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);

  int i;

  clear_screen(s);

  add_curve(edges, 0, 0, 600, 600, 0, 600, 600, 0, 0.0001, BEZIER_MODE);
  transform = make_rotZ(.5);

  draw_lines(edges, s, c);
  save_extension(s, "test.png");

  for (i = 0; i < 360; i++) {
    draw_lines(edges, s, c);

    matrix_mult(transform, edges);
  }


  save_extension(s, "something.png");

  /*  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );
  */
  
  free_matrix( transform );
  free_matrix( edges );
}  
