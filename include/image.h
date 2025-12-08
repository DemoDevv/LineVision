/**
 * @file image.h
 * @brief Image Processing Library for Barcode Recognition
 *
 * This library provides essential image processing functions for loading,
 * manipulating, and preparing images for barcode detection and decoding.
 * It includes binarization using Otsu's method, which is particularly
 * effective for barcode image preprocessing.
 *
 * The library uses stb_image for image I/O operations, supporting common
 * formats like PNG, JPEG, BMP, and more.
 *
 * @author [Your name]
 * @date [Date]
 * @version 1.0
 */
#pragma once

#include <stdint.h>

/**
 * @struct Image
 * @brief Represents a raster image with pixel data
 *
 * This structure stores image dimensions, channel information, and raw
 * pixel data. The data is stored in row-major order with interleaved channels.
 *
 * Memory layout: [R G B] [R G B] [R G B] ... for RGB images
 *                [Y] [Y] [Y] ... for grayscale images
 */
typedef struct {
    /** @brief Image width in pixels */
    int width;
    /** @brief Image height in pixels */
    int height;
    /** @brief Number of color channels (1=grayscale, 3=RGB, 4=RGBA) */
    int channels;
    /** @brief Raw pixel data (dynamically allocated) */
    unsigned char* data;
} Image;

/**
 * @brief Loads an image from a file
 *
 * Opens and decodes an image file using stb_image. Supports common formats
 * including PNG, JPEG, BMP, TGA, and others. The image data is allocated
 * dynamically and must be freed with close_image().
 *
 * @param filename Path to the image file
 * @param desired_channels Number of channels to force:
 *                         - 0: Keep original channels
 *                         - 1: Convert to grayscale
 *                         - 3: Convert to RGB
 *                         - 4: Convert to RGBA
 *
 * @return Pointer to a dynamically allocated Image structure,
 *         or NULL if the file cannot be loaded or memory allocation fails
 *
 * @note The returned Image must be freed with close_image()
 * @note If desired_channels is 0, the original channel count is preserved
 * @note For barcode processing, desired_channels=1 (grayscale) is recommended
 *
 * @example
 * Image* img = open_image("barcode.png", 1); // Load as grayscale
 * if (img) {
 *     // Process image...
 *     close_image(img);
 * }
 */
Image* open_image(const char* filename, int desired_channels);

/**
 * @brief Frees all memory associated with an image
 *
 * Releases both the pixel data and the Image structure itself.
 *
 * @param image Pointer to the image to free
 *
 * @note This function is safe to call with a NULL pointer
 * @note After calling this function, the image pointer becomes invalid
 */
void close_image(Image* image);

/**
 * @brief Prints all pixel values of an image to standard output
 *
 * Outputs the raw pixel data in a human-readable format. Each row is printed
 * on a separate line, with pixel values separated by spaces. For multi-channel
 * images, all channel values for each pixel are printed consecutively.
 *
 * @param image Pointer to the image to print
 *
 * @note Safe to call with NULL pointer (does nothing)
 * @warning This can produce very large output for high-resolution images.
 *          Use primarily for debugging small images or regions of interest.
 */
void print_image(Image* image);

/**
 * @brief Prints basic information about an image
 *
 * Displays the image dimensions and number of channels to standard output.
 * Useful for debugging and verification.
 *
 * @param image Pointer to the image to inspect
 *
 * @note Safe to call with NULL pointer (does nothing)
 *
 * @example
 * Output format:
 * Image Info:
 * Width: 640
 * Height: 480
 * Channels: 1
 */
void print_image_info(Image* image);

/**
 * @brief Calculates the optimal binarization threshold using Otsu's method
 *
 * Otsu's method automatically determines the best threshold for converting
 * a grayscale image to binary by maximizing the inter-class variance between
 * foreground and background pixels. This is particularly effective for
 * barcode images where there is a clear separation between bars and spaces.
 *
 * Algorithm overview:
 * 1. Compute the histogram of grayscale values
 * 2. For each possible threshold (0-255):
 *    - Calculate the inter-class variance
 *    - Track the threshold with maximum variance
 * 3. Return the optimal threshold
 *
 * @param gray Pointer to grayscale pixel data (values 0-255)
 * @param length Total number of pixels in the data
 *
 * @return Optimal threshold value (0-255) that maximizes inter-class variance
 *
 * @note The input must be grayscale (single channel) data
 * @note For color images, convert to grayscale first before calling this function with rgb_to_grayscale(image);
 * @note Time complexity: O(length + 256) ≈ O(length)
 *
 * @example
 * int threshold = otsu_threshold(image->data, image->width * image->height);
 * printf("Optimal threshold: %d\n", threshold);
 */
int otsu_threshold(const uint8_t* gray, int length);

/**
 * @brief Converts an image to binary using a threshold
 *
 * Applies threshold-based binarization to all pixels in the image.
 * Pixels with values greater than the threshold are set to 255 (white),
 * while pixels with values less than or equal to the threshold are set
 * to 0 (black).
 *
 * This operation modifies the image in-place.
 *
 * @param image Pointer to the image to binarize (modified in-place)
 * @param threshold Threshold value (0-255) for binarization
 *
 * @note Safe to call with NULL pointer (does nothing)
 * @note The image is modified directly; no new image is created
 * @note Works with grayscale images (single channel)
 * @note For optimal results, use threshold from otsu_threshold()
 *
 * @example
 * // Automatic binarization using Otsu's method
 * int threshold = otsu_threshold(image->data, image->width * image->height);
 * binarization(image, threshold);
 *
 * // Manual threshold
 * binarization(image, 128); // Simple mid-point threshold
 */
void binarization(Image* image, int threshold);

// Convert RGB image to grayscale
void rgb_to_grayscale(Image* image);

/**
 * @brief Saves an image to a PNG file
 *
 * Writes the image data to disk in PNG format using stb_image_write.
 * Useful for saving processed images (e.g., after binarization).
 *
 * @param image Pointer to the image to save
 * @param filename Output file path (should end with .png)
 *
 * @note Safe to call with NULL image pointer (does nothing)
 * @note The function does not return an error code; check file system
 *       permissions and disk space manually if needed
 *
 * @example
 * save_image_png(processed_img, "output_binarized.png");
 */
void save_image_png(Image* image, const char* filename);
