#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 circle: add a circle to the edge matrix - 
	    takes 3 arguments (cx, cy, r)
	 hermite: add a hermite curve to the edge matrix -
	    takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
	 bezier: add a bezier curve to the edge matrix -
	    takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 xrotate: create an x-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 yrotate: create an y-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 zrotate: create an z-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * pm,
                  screen s) {

  FILE *f;
  char line[256];
  
  clear_screen(s);

  double arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8;
  color c;

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
    
    if (strcmp(line, "quit") == 0){
      exit(0);
    }
    else if (strcmp(line, "line") == 0){
      fgets(line, 255, f);
      line[strlen(line)]='\0';
      sscanf(line, "%lf" "%lf" "%lf" "%lf" "%lf" "%lf", &arg1, &arg2, &arg3, &arg4, &arg5, &arg6);
      add_edge(pm, arg1, arg2, arg3, arg4, arg5, arg6);
    }
    else if (strcmp(line, "circle") == 0){
      fgets(line, 255, f);
      line[strlen(line)]='\0';
      sscanf(line, "%lf" "%lf" "%lf", &arg1, &arg2, &arg3);
      add_circle(pm, arg1, arg2, arg3, 0.01);
    }
    else if (strcmp(line, "hermite") == 0){
      fgets(line, 255, f);
      line[strlen(line)]='\0';
      sscanf(line, "%lf" "%lf" "%lf" "%lf" "%lf" "%lf" "%lf" "%lf", &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8);
      add_curve(pm, arg1, arg2, arg5, arg6, arg3, arg4, arg7, arg8, 0.01, HERMITE_MODE);
    }
    else if (strcmp(line, "bezier") == 0){
      fgets(line, 255, f);
      line[strlen(line)]='\0';
      sscanf(line, "%lf" "%lf" "%lf" "%lf" "%lf" "%lf" "%lf" "%lf", &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8);
      add_curve(pm, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, 0.01, BEZIER_MODE);
    }
    else if (strcmp(line, "ident") == 0){
      ident(transform);
    }
    else if (strcmp(line, "scale") == 0){
      fgets(line, 255, f);
      line[strlen(line)]='\0';
      sscanf(line, "%lf" "%lf" "%lf", &arg1, &arg2, &arg3);
      matrix_mult(make_scale(arg1, arg2, arg3), transform);
    }
    else if (strcmp(line, "translate") == 0){
      fgets(line, 255, f);
      line[strlen(line)]='\0';
      sscanf(line, "%lf" "%lf" "%lf", &arg1, &arg2, &arg3);
      matrix_mult(make_translate(arg1, arg2, arg3), transform);
    }
    else if (strcmp(line, "xrotate") == 0){
      fgets(line, 255, f);
      line[strlen(line)]='\0';
      sscanf(line, "%lf", &arg1);
      matrix_mult(make_rotX(arg1 * M_PI / 180), transform);
    }
    else if (strcmp(line, "yrotate") == 0){
      fgets(line, 255, f);
      line[strlen(line)]='\0';
      sscanf(line, "%lf", &arg1);
      matrix_mult(make_rotY(arg1 * M_PI / 180), transform);
    }
    else if (strcmp(line, "zrotate") == 0){
      fgets(line, 255, f);
      line[strlen(line)]='\0';
      sscanf(line, "%lf", &arg1);
      matrix_mult(make_rotZ(arg1 * M_PI / 180), transform);
    }
    else if (strcmp(line, "apply") == 0){
      matrix_mult(transform, pm);
    }
    else if (strcmp(line, "display") == 0){
      clear_screen(s);
      draw_lines(pm, s, c);
      display(s);
    }
    else if (strcmp(line, "save") == 0){
      clear_screen(s);
      draw_lines(pm, s, c);
      fgets(line, 255, f);
      line[strlen(line)]='\0';
      save_extension(s, line);
    }
    else {
      printf("Parse error\n");
    }
  }
}


