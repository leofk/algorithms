#include "block.h"
#include <cmath>
#include <iostream>

/* Returns the width, in pixels
 * of the current block
 */
int Block::width() const
{
    return data[0].size();
}

/* Returns the width, in pixels
 * of the current block
 */
int Block::height() const
{
    return data.size();
}

/* Given an image whose size is large enough, place
 * the current block in the image so that its upper left corner
 * is at position column, row. Existing pixels in the image
 * will be replaced by those of the block.
 */
void Block::render(PNG &im, int column, int row) const
{
    int xImg;
    int yImg = row;
    HSLAPixel * pixelToChange;

    for (unsigned long yData = 0; yData < data.size(); yData++) {
        xImg = column;
            for (unsigned long xData = 0; xData < data[0].size(); xData++) {
                pixelToChange = im.getPixel(xImg, yImg);
                *pixelToChange = data[yData][xData];
                pixelToChange = NULL;
                xImg++;
            }
        yImg++;
    }
}

/* create a block of pixels whose color values are the same as the
 * rectangle in the image described by the upper left corner (column, row)
 * whose size is width x height.
 */
void Block::build(PNG &im, int column, int row, int width, int height)
{
    int xImg;
    int yImg = row;

    data.resize(height);
    for (unsigned long i = 0; i < data.size(); i++) {
        data[i].resize(width);
    }

    for (int yData = 0; yData < height; yData++) {
        vector<HSLAPixel> yArr;
        xImg = column;
            for (int xData = 0; xData < width; xData++) {
                HSLAPixel * pixel = im.getPixel(xImg, yImg);
                yArr.push_back(*pixel);
                pixel = NULL;
                xImg++;
            }
        data[yData] = yArr;
        yImg++;
    }
}

/* Flip the current block across its horizontal midline.
 * This function changes the existing block.
 */
void Block::flipVert()
{   
    int oppY;
    
    for (unsigned long x = 0; x < data[0].size(); x++) {
        oppY = data.size() - 1;
        for (unsigned long y = 0; y < data.size()/2; y++) {
            HSLAPixel temp = data[oppY][x];
            data[oppY][x] = data[y][x];
            data[y][x] = temp;
            oppY--;
        }
    }
}

/* Flip the current block across its vertical midline.
 * This function changes the existing block.
 */
void Block::flipHoriz()
{
    int oppX;

    for (unsigned long y = 0; y < data.size(); y++) {
        oppX = data[0].size() - 1;
        for (unsigned long x = 0; x < data[0].size()/2; x++) {
            HSLAPixel temp = data[y][oppX];
            data[y][oppX] = data[y][x];
            data[y][x] = temp;
            oppX--;
        }
    }
}

/* Rotate the current block 90 degrees counterclockwise.
 * This function changes the existing block.
 */
void Block::rotateRight()
{
    for (unsigned long y = 0; y < data.size(); y++) {
        for (unsigned long x = 0; x < data[0].size(); x++) {
            HSLAPixel temp = data[y][x];
            data[y][x] = data[x][y];
            data[x][y] = temp;
            
        }
    }

    flipHoriz();

}
