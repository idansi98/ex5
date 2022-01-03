#include "writeBMP.h"
#include "readBMP.h"
#include <stdlib.h>
#include <stdbool.h>
extern Image* image;
extern unsigned long n, m;


typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
    // int num;
} pixel_sum;


/* Compute min and max of two integers, respectively */
int min(int a, int b) { return (a < b ? a : b); }
int max(int a, int b) { return (a > b ? a : b); }

int calcIndex(int i, int j, int n) {
	return ((i)*(n)+(j));
}

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
void initialize_pixel_sum(pixel_sum *sum) {
	sum->red = sum->green = sum->blue = 0;
	// sum->num = 0;
	return;
}

/*
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale) {

	// divide by kernel's weight
	sum.red = sum.red / kernelScale;
	sum.green = sum.green / kernelScale;
	sum.blue = sum.blue / kernelScale;
    int first = (sum.red > 0 ? sum.red : 0);
    int second = (sum.green > 0 ? sum.green : 0);
    int third = (sum.blue > 0 ? sum.blue : 0);
    first = (first < 255 ? first : 255);
    second = (second < 255 ? second : 255);
    third = (third < 255 ? third : 255);

	// truncate each pixel's color values to match the range [0,255]
	current_pixel->red = (unsigned char) first;
	current_pixel->green = (unsigned char) second;
	current_pixel->blue = (unsigned char) third;
	return;
}

/*
* sum_pixels_by_weight - Sums pixel values, scaled by given weight
*/
static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight) {
	sum->red += ((int) p.red) * weight;
	sum->green += ((int) p.green) * weight;
	sum->blue += ((int) p.blue) * weight;
	// sum->num++;
	return;
}

/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel applyKernel(int dim, int i, int j, pixel *src, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	int ii, jj, kRow, kCol;
	int currRow, currCol;
	pixel_sum sum;
	pixel current_pixel;
	int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
	int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
	int min_row, min_col, max_row, max_col;
	pixel loop_pixel;

	initialize_pixel_sum(&sum);

	for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
        // compute row index in kernel
        if (ii < i) {
            kRow = 0;
        } else if (ii > i) {
            kRow = 2;
        } else {
            kRow = 1;
        }
		for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {
			// compute column index in kernel
			if (jj < j) {
				kCol = 0;
			} else if (jj > j) {
				kCol = 2;
			} else {
				kCol = 1;
			}

			// apply kernel on pixel at [ii,jj]
			sum_pixels_by_weight(&sum, src[ii*dim+jj], kernel[kRow][kCol]);
		}
	}

	if (filter) {
		// find min and max coordinates
		for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
			for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {
				// check if smaller than min or higher than max and update
				loop_pixel = src[ii*dim+jj];
				if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
					min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
					min_row = ii;
					min_col = jj;
				}
				if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
					max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
					max_row = ii;
					max_col = jj;
				}
			}
		}
		// filter out min and max
		sum_pixels_by_weight(&sum, src[min_row*dim+min_col], -1);
		sum_pixels_by_weight(&sum, src[max_row*dim+max_col], -1);
	}

	// assign kernel's result to pixel at [i,j]
	assign_sum_to_pixel(&current_pixel, sum, kernelScale);
	return current_pixel;
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(int dim, pixel *src, pixel *dst, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	int i, j;
	for (i = kernelSize / 2 ; i < dim - kernelSize / 2; i++) {
		for (j =  kernelSize / 2 ; j < dim - kernelSize / 2 ; j++) {
			dst[i*dim+j] = applyKernel(dim, i, j, src, kernelSize, kernel, kernelScale, filter);
		}
	}
}

void charsToPixels(Image *charsImg, pixel* pixels) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			pixels[row*n + col].red = image->data[3*row*n + 3*col];
			pixels[row*n + col].green = image->data[3*row*n + 3*col + 1];
			pixels[row*n + col].blue = image->data[3*row*n + 3*col + 2];
		}
	}
}

void pixelsToChars(pixel* pixels, Image *charsImg) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			image->data[3*row*n + 3*col] = pixels[row*n + col].red;
			image->data[3*row*n + 3*col + 1] = pixels[row*n + col].green;
			image->data[3*row*n + 3*col + 2] = pixels[row*n + col].blue;
		}
	}
}

void copyPixels(pixel* src, pixel* dst) {
    unsigned long* usrc = (unsigned long*) src;
    unsigned long* udst = (unsigned long*) dst;
	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col+= 10) {
            int index = row*n + col;
			dst[index].red = src[index].red, dst[index].green = src[index].green, dst[index].blue = src[index].blue,
            dst[index+1].red = src[index+1].red, dst[index+1].green = src[index+1].green, dst[index+1].blue = src[index+1].blue,
            dst[index+2].red = src[index+2].red, dst[index+2].green = src[index+2].green, dst[index+2].blue = src[index+2].blue,
            dst[index+3].red = src[index+3].red, dst[index+3].green = src[index+3].green, dst[index+3].blue = src[index+3].blue,
            dst[index+4].red = src[index+4].red, dst[index+4].green = src[index+4].green, dst[index+4].blue = src[index+4].blue,
            dst[index+5].red = src[index+5].red, dst[index+5].green = src[index+5].green, dst[index+5].blue = src[index+5].blue,
            dst[index+6].red = src[index+6].red, dst[index+6].green = src[index+6].green, dst[index+6].blue = src[index+6].blue,
            dst[index+7].red = src[index+7].red, dst[index+7].green = src[index+7].green, dst[index+7].blue = src[index+7].blue,
            dst[index+8].red = src[index+8].red, dst[index+8].green = src[index+8].green, dst[index+8].blue = src[index+8].blue,
            dst[index+9].red = src[index+9].red, dst[index+9].green = src[index+9].green, dst[index+9].blue = src[index+9].blue;
		}
        for (; col < n ; ++col) {
            int index = row*n + col;
            dst[index].red = src[index].red, dst[index].green = src[index].green, dst[index].blue = src[index].blue;
        }
	}
}

void doConvolution(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	pixel* pixelsImg = malloc(m*n*sizeof(pixel));
	pixel* backupOrg = malloc(m*n*sizeof(pixel));

	charsToPixels(image, pixelsImg);
	copyPixels(pixelsImg, backupOrg);

	smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);

	pixelsToChars(pixelsImg, image);

	free(pixelsImg);
	free(backupOrg);
}

void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

	/*
	* [1, 1, 1]
	* [1, 1, 1]
	* [1, 1, 1]
	*/
	int blurKernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	int sharpKernel[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

	if (flag == '1') {	
		// blur image
		doConvolution(image, 3, blurKernel, 9, false);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		doConvolution(image, 3, sharpKernel, 1, false);
		
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		doConvolution(image, 3, blurKernel, 7, true);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		doConvolution(image, 3, sharpKernel, 1, false);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}

