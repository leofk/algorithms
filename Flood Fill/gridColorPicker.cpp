#include "gridColorPicker.h"

gridColorPicker::gridColorPicker(HSLAPixel gridColor, int gridSpacing)
{
    color = gridColor;
    spacing = gridSpacing;
}

    /**
     * Picks the color for pixel (x, y). If the x or y coordinate is a
     * multiple of the spacing, it will be filled with the fillColor.
     * otherwise, it will be filled with white.
     *
     * @param x The x coordinate to pick a color for.
     * @param y The y coordinat to pick a color for.
     * @return The color chosen for (x, y).
     */
HSLAPixel gridColorPicker::operator()(int x, int y)
{
	if (x % spacing == 0 | y % spacing == 0) {
		return color;
	} else {
		HSLAPixel white;
		return white;
	}
}
