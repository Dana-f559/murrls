#include <raylib.h>
#include <stddef.h>

typedef struct {
    Vector2 *vertices;  // the array of vertex points making up the drop shape
    size_t vcount;      // how many vertices the shape has
    Color color;        // color of the drop
} Drop;

Drop circularDrop(Vector2 center, float radius, size_t vcount, Color color);

// Free memory allocated by a drop
void destroyDrop(Drop *drop);

// Draw the drop using Raylib (probably DrawTriangleFan)
void drawDrop(Drop drop);

// Distort ("marble") the drop using the given formula
void marbleDrop(Drop *drop, Vector2 center);
