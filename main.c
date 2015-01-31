#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

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

void voronoi(
	unsigned char *data,
	int width,
	int height,
	POINT *points,
	int n_points,
	int *mapping);

POINT* generate_points(
	int n, 
	int x_upper, 
	int y_upper);

unsigned char* open_bmp(
	const char *filename,
	int *width,
	int *height);

#define N_POINTS 30

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
  points = generate_points(N_POINTS, width, height);

  int *point_mapping = (int*) malloc(width * height);

  voronoi(
   data, width, height, points, N_POINTS, point_mapping);

  return 0;
}

void voronoi(
	unsigned char *data,
	int width,
	int height,
	POINT *points,
	int n_points,
	int *mapping) {

  int x, y;
  for(y=0; y<width; y++) {
    for(x=0; x<height; x++) {
      int i;
      int closest;
      float closest_dist = width + height; // over max dist
      for(i=0; i<n_points; i++) {
        
        float  dx = x - points[i].x;
        float  dy = y - points[i].y;
        
        float dist = sqrtf(powf(dx, 2) + powf(dy, 2));

        if(dist < closest_dist) {
          closest = i;
        }
      }
      mapping[x * y] = closest;
    }
  }

  int i;
  for(i=0; i<n_points; i++) {
    float sum = 0;
    float mean;
    float count = 0;

    //find mean of pixels for point i
    int j;
    for(j=0; j<width*height; j++) {
      if(mapping[j] == i) {
        sum += data[j];
        count ++;
      }
    }

    mean = sum / count;

    for(j=0; j<width*height; j++) {
      if(mapping[j] == i) {
        data[j] = (unsigned char) ceilf(mean);
      }
    }
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
