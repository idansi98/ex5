// 206821258 Idan Simai
#define KERNEL_SIZE 3
#pragma GCC optimize("Ofast")
#include <stdbool.h>
#include "readBMP.h"
#include "writeBMP.h"
#include <stdlib.h>

/*
* [1, 1, 1]
* [1, 1, 1]
* [1, 1, 1]
*/
int blurKernel[KERNEL_SIZE][KERNEL_SIZE] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

/*
* [-1, -1, -1]
* [-1, 9, -1]
* [-1, -1, -1]
*/
int sharpKernel[KERNEL_SIZE][KERNEL_SIZE] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};
Image *image;
unsigned long n, m;

typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
} pixel_sum;

void pixelsCopy(pixel* src, pixel* dst);

/*
 *  Applies kernel for pixel at (i,j)
 */

static pixel applyKernel(int dim, int x, int y, pixel *src, int kernel[3][3], int kernelScale, bool filter) {
      if (kernel == blurKernel) {
          pixel_sum sum;
          pixel current_pixel;
          int min_row, min_col, max_row, max_col;
          //Initialize sum.
          sum.red = 0, sum.green = 0, sum.blue = 0;
          int sI = x - 1;
          int sJ = y - 1;
          int start = sI * dim + sJ;
          current_pixel = src[start];
          sum.red += (int) current_pixel.red, sum.green += (int) current_pixel.green, sum.blue += (int) current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red += (int) current_pixel.red, sum.green += (int) current_pixel.green, sum.blue += (int) current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red += (int) current_pixel.red, sum.green += (int) current_pixel.green, sum.blue += (int) current_pixel.blue;
          start+= (dim - 2);
          current_pixel = src[start];
          sum.red += (int) current_pixel.red, sum.green += (int) current_pixel.green, sum.blue += (int) current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red += (int) current_pixel.red, sum.green += (int) current_pixel.green, sum.blue += (int) current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red += (int) current_pixel.red, sum.green += (int) current_pixel.green, sum.blue += (int) current_pixel.blue;
          start+= (dim - 2);
          current_pixel = src[start];
          sum.red += (int) current_pixel.red, sum.green += (int) current_pixel.green, sum.blue += (int) current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red += (int) current_pixel.red, sum.green += (int) current_pixel.green, sum.blue += (int) current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red += (int) current_pixel.red, sum.green += (int) current_pixel.green, sum.blue += (int) current_pixel.blue;

          if (filter) {
              int i = sI;
              int j = sJ;
              pixel loop_pixel;
              int min_intensity;
              int max_intensity;
              int looped_pixel_intensity;
              pixel* pixel_Pointer = &src[sI * dim + sJ];

              //We start the loop here.
              //1.
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Assigning.
              min_row = i;
              min_col = j;
              max_row = i;
              max_col = j;
              min_intensity = looped_pixel_intensity;
              max_intensity = looped_pixel_intensity;
              j++;
              //2.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              j++;
              //3.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              i++;
              j = sJ;
              //4.
              pixel_Pointer += (dim - 2);
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              j++;
              //5.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              j++;
              //6.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              i++;
              j = sJ;
              //7.
              pixel_Pointer += (dim - 2);
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              j++;
              //8.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity  >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              j++;
              //9.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity  >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              current_pixel = src[min_row * dim + min_col];
              //Decrease the sum colors with the current pixel's results.
              sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;
              current_pixel = src[max_row*dim + max_col];
              sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;
          }
          //Set the colors to their right value.
          if (sum.red >= 256 * kernelScale) {
              sum.red = 255;
          }
          else if (sum.red < 0) {
              sum.red = 0;
          }
          else {
              sum.red /= kernelScale;
          }
          if (sum.green >= 256 * kernelScale) {
              sum.green = 255;
          }
          else if (sum.green < 0) {
              sum.green = 0;
          }
          else {
              sum.green /= kernelScale;
          }
          if (sum.blue >= 256 * kernelScale) {
              sum.blue = 255;
          }
          else if (sum.blue < 0) {
              sum.blue = 0;
          }
          else {
              sum.blue /= kernelScale;
          }
          //Truncate each pixel's color values to match the range [0,255].
          //Assign kernel's result to pixel at [i,j].
          current_pixel.red = (unsigned char) sum.red;
          current_pixel.green = (unsigned char) sum.green;
          current_pixel.blue = (unsigned char) sum.blue;
          return current_pixel;
      }
      else {
          pixel_sum sum;
          pixel current_pixel;
          int min_row, min_col, max_row, max_col;
          //Initialize sum.
          sum.red = 0, sum.green = 0, sum.blue = 0;
          int sI = x - 1;
          int sJ = y - 1;
          int start = sI * dim + sJ;
          current_pixel = src[start];
          sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;
          start+= (dim - 2);
          current_pixel = src[start];
          sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red += (int) 9 * current_pixel.red, sum.green += (int) 9 * current_pixel.green, sum.blue += (int) 9 * current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;
          start+= (dim - 2);
          current_pixel = src[start];
          sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;
          start++;
          current_pixel = src[start];
          sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;


          if (filter) {
              int i = sI;
              int j = sJ;
              pixel loop_pixel;
              pixel* pixel_Pointer = &src[sI * dim + sJ];
              int min_intensity;
              int max_intensity;
              int looped_pixel_intensity;

              //We start the loop here.
              //1.
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Assigning.
              min_row = i;
              min_col = j;
              max_row = i;
              max_col = j;
              min_intensity = looped_pixel_intensity;
              max_intensity = looped_pixel_intensity;
              j++;
              //2.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity  >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              j++;
              //3.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity  >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              i++;
              j = sJ;
              //4.
              pixel_Pointer += (dim - 2);
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity  >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              j++;
              //5.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity  >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              j++;
              //6.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity  >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              i++;
              j = sJ;
              //7.
              pixel_Pointer += (dim - 2);
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity  >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              j++;
              //8.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity  >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              j++;
              //9.
              pixel_Pointer++;
              loop_pixel = *pixel_Pointer;
              looped_pixel_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
              //Set the intensity.
              if (min_intensity  >= looped_pixel_intensity) {
                  min_intensity = looped_pixel_intensity;
                  min_row = i;
                  min_col = j;
              } else  if (max_intensity < looped_pixel_intensity) {
                  max_intensity = looped_pixel_intensity;
                  max_row = i;
                  max_col = j;
              }
              current_pixel = src[min_row * dim + min_col];
              //Decrease the sum colors with the current pixel's results.
              sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;
              current_pixel = src[max_row*dim + max_col];
              sum.red -= (int) current_pixel.red, sum.green -= (int) current_pixel.green, sum.blue -= (int) current_pixel.blue;
          }
          //Set the colors to their right value.
          if (sum.red >= 256 * kernelScale) {
              sum.red = 255;
          }
          else if (sum.red < 0) {
              sum.red = 0;
          }
          else {
              sum.red /= kernelScale;
          }
          if (sum.green >= 256 * kernelScale) {
              sum.green = 255;
          }
          else if (sum.green < 0) {
              sum.green = 0;
          }
          else {
              sum.green /= kernelScale;
          }
          if (sum.blue >= 256 * kernelScale) {
              sum.blue = 255;
          }
          else if (sum.blue < 0) {
              sum.blue = 0;
          }
          else {
              sum.blue /= kernelScale;
          }
          //Truncate each pixel's color values to match the range [0,255].
          //Assign kernel's result to pixel at [i,j].
          current_pixel.red = (unsigned char) sum.red;
          current_pixel.green = (unsigned char) sum.green;
          current_pixel.blue = (unsigned char) sum.blue;
          return current_pixel;
      }
    }

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2.
*/
void smooth(int dim, pixel *src, pixel *dst, int kernel[KERNEL_SIZE][KERNEL_SIZE], int kernelScale, bool filter) {
	int i, j;
    int maxRange = dim - 1;
    int viceRange = maxRange - 27;
	for (i = 1 ; i < maxRange; i++) {
        for (j =  1 ; j < viceRange ; j += 25) {
          int num = (i * dim + j);
          dst[num] = applyKernel(dim, i, j, src, kernel, kernelScale, filter);
          dst[num + 1] = applyKernel(dim, i, j+1, src, kernel, kernelScale, filter);
          dst[num + 2] = applyKernel(dim, i, j+2, src, kernel, kernelScale, filter);
          dst[num + 3] = applyKernel(dim, i, j+3, src, kernel, kernelScale, filter);
          dst[num + 4] = applyKernel(dim, i, j+4, src, kernel, kernelScale, filter);
          dst[num + 5] = applyKernel(dim, i, j+5, src, kernel, kernelScale, filter);
          dst[num + 6] = applyKernel(dim, i, j+6, src, kernel, kernelScale, filter);
          dst[num + 7] = applyKernel(dim, i, j+7, src, kernel, kernelScale, filter);
          dst[num + 8] = applyKernel(dim, i, j+8, src, kernel, kernelScale, filter);
          dst[num + 9] = applyKernel(dim, i, j+9, src, kernel, kernelScale, filter);
          dst[num + 10] = applyKernel(dim, i, j+10, src, kernel, kernelScale, filter);
          dst[num + 11] = applyKernel(dim, i, j+11, src, kernel, kernelScale, filter);
          dst[num + 12] = applyKernel(dim, i, j+12, src, kernel, kernelScale, filter);
          dst[num + 13] = applyKernel(dim, i, j+13, src, kernel, kernelScale, filter);
          dst[num + 14] = applyKernel(dim, i, j+14, src, kernel, kernelScale, filter);
          dst[num + 15] = applyKernel(dim, i, j+15, src, kernel, kernelScale, filter);
          dst[num + 16] = applyKernel(dim, i, j+16, src, kernel, kernelScale, filter);
          dst[num + 17] = applyKernel(dim, i, j+17, src, kernel, kernelScale, filter);
          dst[num + 18] = applyKernel(dim, i, j+18, src, kernel, kernelScale, filter);
          dst[num + 19] = applyKernel(dim, i, j+19, src, kernel, kernelScale, filter);
          dst[num + 20] = applyKernel(dim, i, j+20, src, kernel, kernelScale, filter);
          dst[num + 21] = applyKernel(dim, i, j+21, src, kernel, kernelScale, filter);
          dst[num + 22] = applyKernel(dim, i, j+22, src, kernel, kernelScale, filter);
          dst[num + 23] = applyKernel(dim, i, j+23, src, kernel, kernelScale, filter);
          dst[num + 24] = applyKernel(dim, i, j+24, src, kernel, kernelScale, filter);
        }
		for (; j < maxRange ; j++) {
			dst[i * dim + j] = applyKernel(dim, i, j, src, kernel, kernelScale, filter);
		}
	}
}

void pixelsCopy(pixel* src, pixel* dst) {
    unsigned long* long_end = (unsigned long *) (src + n * m);
    char* short_end = (char *) (src + n * m);
    unsigned long *source = (unsigned long*) src;
    unsigned long *destination = (unsigned long*) dst;
    //Loop to switch between 2 memory locations.
    while (source + 15 < long_end) {
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
        *destination = *source, source++, destination++;
    }
    char *short_src = (char*) source;
    char *short_dst = (char*) destination;
    while (short_src < short_end) {
        *short_dst = *short_src;
        short_src++;
        short_dst++;
    }
}

void doConvolution(Image *image, int kernel[KERNEL_SIZE][KERNEL_SIZE], int kernelScale, bool filter) {
    unsigned long size = (m * n * sizeof(pixel));
	pixel* pixelsImg = malloc(size);
	pixel* backupOrg = malloc(size);
    pixelsCopy((pixel *) &image->data[0], pixelsImg);
	pixelsCopy(pixelsImg, backupOrg);
	smooth(m, backupOrg, pixelsImg, kernel, kernelScale, filter);
    pixelsCopy(pixelsImg, (void *) &image->data[0]);
	free(pixelsImg);
	free(backupOrg);
}


void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {
	if (flag == '1') {
		// blur image
		doConvolution(image, blurKernel, 9, false);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);

		// sharpen the resulting image
		doConvolution(image, sharpKernel, 1, false);

		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);
	} else {
		// apply extermum filtered kernel to blur image
		doConvolution(image, blurKernel, 7, true);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		doConvolution(image, sharpKernel, 1, false);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);
	}
}