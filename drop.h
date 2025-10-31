#include <raylib.h>
#include <stddef.h>

typedef struct {
    Vector2* vertices; // the array of vertex points making up the drop shape
    size_t vcount;     // how many vertices the shape has
    Color color;       // color of the drop
} Drop;

/**
 * Create a circular drop with vcount number of vertices around center.
 *
 * @param center The center of the circle.
 * @param radius The radius of the circle.
 * @param vcount The number of vertices.
 * @param color Color of the drop.
 *
 * @return The created Drop struct.
 */
Drop circularDrop(Vector2 center, float radius, size_t vcount, Color color);

/**
 * Free any resources allocated by the drop.
 *
 * @param drop The input drop structure to be destroyed.
 */
void destroyDrop(Drop drop);

/**
 * Draw the drop.
 *
 * @param drop The input drop to be drawen.
 */
void drawDrop(Drop drop);

/**
 * Marble the vertices of the drop given a center and radius. Using this
 * equation: p = c + (p - c) * sqrt(1 + r^2 / (p - c)^2)
 * where p is a vertex point.
 *
 * @param drop The drop to be marbled.
 * @param c The center of the circle from which to be marbled.
 * @param r The radius of the circle from which to be marbled.
 */
void marbleDrop(Drop drop, Vector2 c, float r);
