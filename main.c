#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#pragma pack(push, 1)

typedef struct {
 unsigned short int type;
 unsigned int size;
 unsigned short int r1;
 unsigned short int r2;
 unsigned int offset;
} BMP_HEADER;

typedef struct {
 unsigned int size; //Header size in bytes
 int width;
 int height;
 unsigned short int planes;
 unsigned short int bits; //bits per pixel
 unsigned int compression; //compression type
 unsigned int imagesize;  //image size in bytes
 int xres;
 int yres;
 unsigned int ncolors;
 unsigned int imp_colors;
} BMP_INFO;
#pragma pack(pop)

typedef struct {
  int x, y;
}POINT;

unsigned char* voronoi(
	unsigned char *data,
	int width,
	int height,
	POINT *points
	int n_points);

POINT* generate_points(
	int n, 
	int x_upper, 
	int y_upper);

unsigned char* open_bmp(
	const char *filename,
	int *width,
	int *height);


int main(int argc, char ** argv) {
  
  int width;
  int height;
  
  unsigned char *data;
  data = open_bmp("lena.bmp", &width, &height);

  unsigned char *blue = (unsigned char*)malloc(
		width * 
		height);
  unsigned char *green = (unsigned char*)malloc(
		width * 
		height);
  unsigned char *red = (unsigned char*)malloc(
		width * 
		height);

  int i;
  for(i=0; i<width * height * 3; i+=3) {
    blue[i/3] = data[i];
    green[i/3] = data[i+1];
    red[i/3] = data[i+2];
  }

  POINT* points;
  points = generate_points(30, width, height);

  return 0;
}

unsigned char* voronoi(
	unsigned char *data,
	int width,
	int height,
	POINT *points,
	int n_points) {

  int i;
  for(int i=0; i<n_points; i++) {
    /*TODO Joe: for each point, find pixels closest, then avaerage
     * those pixels then return the results
     */
  }
}

POINT* generate_points(
	int n, 
	int x_upper, 
	int y_upper) {

  srand((int)time(NULL));
  POINT *points = malloc(sizeof(POINT) * n);
  
  int i;
  for(i=0; i<n; i++) {
    POINT p;
    p.x = rand() % x_upper;
    p.y = rand() % y_upper;
    points[i] = p;
  }

  return points;
}


unsigned char* open_bmp(
	const char *filename,
	int *width,
	int *height) {

  BMP_HEADER header;
  BMP_INFO info;

  FILE *f = fopen(filename ,"rb");

  fread(&header, sizeof(BMP_HEADER), 1, f);
   
  fread(&info,sizeof(BMP_INFO), 1, f);

  *width = info.width;
  *height = info.height;

  int data_size = info.width * info.height;

  unsigned char *data = malloc(
		data_size * sizeof(unsigned char) * 3);

  fread(data, 
	data_size * sizeof(unsigned char) * 3, 
	1, 
	f);

  fclose(f);

  return data;
}
