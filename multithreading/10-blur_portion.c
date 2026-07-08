#include "multithreading.h"

/**
 * get_pixel - retrieves a pixel from an image, clamping out-of-bounds
 * coordinates to the nearest edge pixel
 *
 * @img: image to read the pixel from
 * @x:   x coordinate of the pixel, may be negative or out of bounds
 * @y:   y coordinate of the pixel, may be negative or out of bounds
 *
 * Return: the pixel located at the (clamped) coordinates
 */
static pixel_t get_pixel(img_t const *img, long int x, long int y)
{
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if ((size_t)x >= img->w)
		x = (long int)img->w - 1;
	if ((size_t)y >= img->h)
		y = (long int)img->h - 1;

	return (img->pixels[(y * (long int)img->w) + x]);
}

/**
 * blur_pixel - computes the blurred value of a single pixel by
 * applying a convolution kernel centered on that pixel
 *
 * @img:    source image to read pixels from
 * @kernel: convolution kernel to apply
 * @x:      x coordinate of the pixel to blur
 * @y:      y coordinate of the pixel to blur
 *
 * Return: the resulting blurred pixel
 */
static pixel_t blur_pixel(img_t const *img, kernel_t const *kernel,
	size_t x, size_t y)
{
	float sum_r = 0, sum_g = 0, sum_b = 0, sum_k = 0, weight;
	long int half = (long int)kernel->size / 2, i, j;
	pixel_t p, result;

	for (i = 0; i < (long int)kernel->size; i++)
	{
		for (j = 0; j < (long int)kernel->size; j++)
		{
			weight = kernel->matrix[i][j];
			p = get_pixel(img, (long int)x + j - half,
				(long int)y + i - half);
			sum_r += p.r * weight;
			sum_g += p.g * weight;
			sum_b += p.b * weight;
			sum_k += weight;
		}
	}
	if (sum_k == 0)
		sum_k = 1;
	result.r = (uint8_t)(sum_r / sum_k);
	result.g = (uint8_t)(sum_g / sum_k);
	result.b = (uint8_t)(sum_b / sum_k);
	return (result);
}

/**
 * blur_portion - blurs a rectangular portion of an image using a
 * Gaussian blur convolution kernel
 *
 * @portion: pointer to the blur_portion_t structure describing the
 * source image, destination image, the region to blur and the
 * convolution kernel to use
 */
void blur_portion(blur_portion_t const *portion)
{
	size_t x, y, x_end = portion->x + portion->w, y_end = portion->y + portion->h;

	for (y = portion->y; y < y_end && y < portion->img->h; y++)
	{
		for (x = portion->x; x < x_end && x < portion->img->w; x++)
		{
			portion->img_blur->pixels[(y * portion->img->w) + x] =
				blur_pixel(portion->img, portion->kernel, x, y);
		}
	}
}
