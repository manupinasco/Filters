#include "helpers.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE new_pixel;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            new_pixel = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = new_pixel;
            image[i][j].rgbtGreen = new_pixel;
            image[i][j].rgbtRed = new_pixel;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int new_red;
    int new_green;
    int new_blue;
    RGBTRIPLE buffer[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            new_blue = round(.131 * (int) image[i][j].rgbtBlue + .534 * (int) image[i][j].rgbtGreen + .272 * (int) image[i][j].rgbtRed);
            if (new_blue > 255)
            {
                new_blue = 255;
            }
            if (new_blue < 0)
            {
                new_blue = 0;
            }
            new_green = round(.168 * (int) image[i][j].rgbtBlue + .686 * (int) image[i][j].rgbtGreen + .349 * (int) image[i][j].rgbtRed);
            if (new_green > 255)
            {
                new_green = 255;
            }
            if (new_green < 0)
            {
                new_green = 0;
            }
            new_red = round(.189 * (int) image[i][j].rgbtBlue + .769 * (int) image[i][j].rgbtGreen + .393 * (int) image[i][j].rgbtRed);
            if (new_red > 255)
            {
                new_green = 255;
            }
            if (new_red < 0)
            {
                new_red = 0;
            }
            buffer[i][j].rgbtBlue = new_blue;
            buffer[i][j].rgbtGreen = new_green;
            buffer[i][j].rgbtRed = new_red;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = buffer[i][j];
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            buffer[i][width - j - 1] = image[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = buffer[i][j];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE new_red;
    BYTE new_green;
    BYTE new_blue;
    RGBTRIPLE buffer[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 && j != 0 && j != width - 1)
            {
                new_blue = round((image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j].rgbtBlue) / 6.0);
                new_green = round((image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j].rgbtGreen) / 6.0);
                new_red = round((image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i][j].rgbtRed) / 6.0);
                buffer[i][j].rgbtBlue = new_blue;
                buffer[i][j].rgbtGreen = new_green;
                buffer[i][j].rgbtRed = new_red;
            }
            else if (i == 0 && j == 0)
            {
                new_blue = round((image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j].rgbtBlue) / 4.0);
                new_green = round((image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j].rgbtGreen) / 4.0);
                new_red = round((image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i][j].rgbtRed) / 4.0);
                buffer[i][j].rgbtBlue = new_blue;
                buffer[i][j].rgbtGreen = new_green;
                buffer[i][j].rgbtRed = new_red;
            }
            else if (i == 0 && j == width - 1)
            {
                new_blue = round((image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j].rgbtBlue) / 4.0);
                new_green = round((image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j].rgbtGreen) / 4.0);
                new_red = round((image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i][j].rgbtRed) / 4.0);
                buffer[i][j].rgbtBlue = new_blue;
                buffer[i][j].rgbtGreen = new_green;
                buffer[i][j].rgbtRed = new_red;
            }
            else if (i != 0 && j == width - 1 && i != height - 1) {
                new_blue = round((image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j].rgbtBlue) / 6.0);
                new_green = round((image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j].rgbtGreen) / 6.0);
                new_red = round((image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i + 1][j].rgbtRed + image[i][j].rgbtRed) / 6.0);
                buffer[i][j].rgbtBlue = new_blue;
                buffer[i][j].rgbtGreen = new_green;
                buffer[i][j].rgbtRed = new_red;
            }
            else if (i != 0 && j == 0 && i != height - 1) {
                new_blue = round((image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j].rgbtBlue) / 6.0);
                new_green = round((image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j].rgbtGreen) / 6.0);
                new_red = round((image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i - 1][j].rgbtRed + image[i + 1][j].rgbtRed + image[i][j].rgbtRed) / 6.0);
                buffer[i][j].rgbtBlue = new_blue;
                buffer[i][j].rgbtGreen = new_green;
                buffer[i][j].rgbtRed = new_red;
            }
            else if (i == height - 1 && j != 0 && j != width - 1) {
                new_blue = round((image[i][j + 1].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue) / 6.0);
                new_green = round((image[i][j + 1].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen) / 6.0);
                new_red = round((image[i][j + 1].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i][j].rgbtRed) / 6.0);
                buffer[i][j].rgbtBlue = new_blue;
                buffer[i][j].rgbtGreen = new_green;
                buffer[i][j].rgbtRed = new_red;
            }
            else if (i == height - 1 && j == 0) {
                new_blue = round((image[i][j + 1].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue) / 4.0);
                new_green = round((image[i][j + 1].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen) / 4.0);
                new_red = round((image[i][j + 1].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i - 1][j].rgbtRed + image[i][j].rgbtRed) / 4.0);
                buffer[i][j].rgbtBlue = new_blue;
                buffer[i][j].rgbtGreen = new_green;
                buffer[i][j].rgbtRed = new_red;
            }
            else if (i == height - 1 && j == width - 1) {
                new_blue = round((image[i][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue) / 4.0);
                new_green = round((image[i][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen) / 4.0);
                new_red = round((image[i][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i][j].rgbtRed) / 4.0);
                buffer[i][j].rgbtBlue = new_blue;
                buffer[i][j].rgbtGreen = new_green;
                buffer[i][j].rgbtRed = new_red;
            }
            else {
                new_blue = round((image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j].rgbtBlue) / 9.0);
                new_green = round((image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j].rgbtGreen) / 9.0);
                new_red = round((image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i + 1][j].rgbtRed + image[i][j].rgbtRed) / 9.0);
                buffer[i][j].rgbtBlue = new_blue;
                buffer[i][j].rgbtGreen = new_green;
                buffer[i][j].rgbtRed = new_red;
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = buffer[i][j];
        }
    }
}
