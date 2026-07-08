#include "multithreading.h"
#include <pthread.h>
#include <stddef.h>

#define MAX_THREADS 16

/**
 * blur_thread - Entry point for a blur thread
 *
 * @arg: Pointer to the blur_portion_t configuration
 *
 * Return: NULL
 */
void *blur_thread(void *arg)
{
	blur_portion((blur_portion_t *)arg);
	pthread_exit(NULL);
}

/**
 * blur_image - Blurs the entirety of an image using a Gaussian Blur
 *
 * @img_blur: Destination image
 * @img:      Source image
 * @kernel:   Convolution kernel to use
 */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel)
{
	pthread_t threads[MAX_THREADS];
	blur_portion_t portions[MAX_THREADS];
	size_t i, num_threads = MAX_THREADS, step;

	if (!img || !img_blur || !kernel || img->h == 0 || img->w == 0)
		return;

	if (img->h < MAX_THREADS)
		num_threads = img->h;

	step = img->h / num_threads;

	for (i = 0; i < num_threads; i++)
	{
		portions[i].img = img;
		portions[i].img_blur = img_blur;
		portions[i].x = 0;
		portions[i].y = i * step;
		portions[i].w = img->w;
		portions[i].h = step;
		if (i == num_threads - 1)
			portions[i].h += img->h % num_threads;
		portions[i].kernel = kernel;

		pthread_create(&threads[i], NULL, blur_thread, &portions[i]);
	}

	for (i = 0; i < num_threads; i++)
		pthread_join(threads[i], NULL);
}
