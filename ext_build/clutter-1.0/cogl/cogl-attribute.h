/*
 * Cogl
 *
 * An object oriented GL/GLES Abstraction/Utility Layer
 *
 * Copyright (C) 2010 Intel Corporation.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
 * Authors:
 *   Robert Bragg <robert@linux.intel.com>
 */

#if !defined(__COGL_H_INSIDE__) && !defined(CLUTTER_COMPILATION)
#error "Only <cogl/cogl.h> can be included directly."
#endif

#ifndef __COGL_ATTRIBUTE_H__
#define __COGL_ATTRIBUTE_H__

#include <cogl/cogl-vertex-array.h>
#include <cogl/cogl-indices.h>

G_BEGIN_DECLS

/**
 * SECTION:cogl-attribute
 * @short_description: Functions for declaring and drawing vertex
 *    attributes
 *
 * FIXME
 */

typedef struct _CoglAttribute CoglAttribute;

/**
 * cogl_attribute_new:
 * @array: The #CoglVertexArray containing the actual attribute data
 * @name: The name of the attribute (used to reference it from GLSL)
 * @stride: The number of bytes to jump to get to the next attribute
 *          value for the next vertex. (Usually
 *          <pre>sizeof (MyVertex)</pre>)
 * @offset: The byte offset from the start of @array for the first
 *          attribute value. (Usually
 *          <pre>offsetof (MyVertex, component0)</pre>
 * @components: The number of components (e.g. 4 for an rgba color or
 *              3 for and (x,y,z) position)
 * @type: FIXME
 *
 * Describes the layout for a list of vertex attribute values (For
 * example, a list of texture coordinates or colors).
 *
 * The @name is used to access the attribute inside a GLSL vertex
 * shader and there are some special names you should use if they are
 * applicable:
 *  <itemizedlist>
 *    <listitem>"cogl_position_in" (used for vertex positions)</listitem>
 *    <listitem>"cogl_color_in" (used for vertex colors)</listitem>
 *    <listitem>"cogl_tex_coord0_in", "cogl_tex_coord1", ...
 * (used for vertex texture coordinates)</listitem>
 *    <listitem>"cogl_normal_in" (used for vertex normals)</listitem>
 *  </itemizedlist>
 *
 * The attribute values corresponding to different vertices can either
 * be tightly packed or interleaved with other attribute values. For
 * example it's common to define a structure for a single vertex like:
 * |[
 * typedef struct
 * {
 *   float x, y, z; /<!-- -->* position attribute *<!-- -->/
 *   float s, t; /<!-- -->* texture coordinate attribute *<!-- -->/
 * } MyVertex;
 * ]|
 *
 * And then create an array of vertex data something like:
 * |[
 * MyVertex vertices[100] = { .... }
 * ]|
 *
 * In this case, to describe either the position or texture coordinate
 * attribute you have to move <pre>sizeof (MyVertex)</pre> bytes to
 * move from one vertex to the next.  This is called the attribute
 * @stride. If you weren't interleving attributes and you instead had
 * a packed array of float x, y pairs then the attribute stride would
 * be <pre>(2 * sizeof (float))</pre>. So the @stride is the number of
 * bytes to move to find the attribute value of the next vertex.
 *
 * Normally a list of attributes starts at the beginning of an array.
 * So for the <pre>MyVertex</pre> example above the @offset is the
 * offset inside the <pre>MyVertex</pre> structure to the first
 * component of the attribute. For the texture coordinate attribute
 * the offset would be <pre>offsetof (MyVertex, s)</pre> or instead of
 * using the offsetof macro you could use <pre>sizeof (float) * 3</pre>.
 * If you've divided your @array into blocks of non-interleved
 * attributes then you will need to calculate the @offset as the
 * number of bytes in blocks preceding the attribute you're
 * describing.
 *
 * An attribute often has more than one component. For example a color
 * is often comprised of 4 red, green, blue and alpha @components, and a
 * position may be comprised of 2 x and y @components. You should aim
 * to keep the number of components to a minimum as more components
 * means more data needs to be mapped into the GPU which can be a
 * bottlneck when dealing with a large number of vertices.
 *
 * Finally you need to specify the component data type. Here you
 * should aim to use the smallest type that meets your precision
 * requirements. Again the larger the type then more data needs to be
 * mapped into the GPU which can be a bottlneck when dealing with
 * a large number of vertices.
 *
 * Returns: A newly allocated #CoglAttribute describing the
 *          layout for a list of attribute values stored in @array.
 *
 * Since: 1.4
 * Stability: Unstable
 */
/* XXX: look for a precedent to see if the stride/offset args should
 * have a different order. */
CoglAttribute *
cogl_attribute_new (CoglVertexArray *array,
                    const char *name,
                    gsize stride,
                    gsize offset,
                    int components,
                    CoglAttributeType type);

/**
 * cogl_is_attribute:
 * @object: A #CoglObject
 *
 * Gets whether the given object references a #CoglAttribute.
 *
 * Return value: %TRUE if the handle references a #CoglAttribute,
 *   %FALSE otherwise
 */
gboolean
cogl_is_attribute (void *object);

void
cogl_draw_attributes (CoglVerticesMode mode,
                      int first_vertex,
                      int n_vertices,
                      ...) G_GNUC_NULL_TERMINATED;

void
cogl_draw_attributes_array (CoglVerticesMode mode,
                            int first_vertex,
                            int n_vertices,
                            CoglAttribute **attributes);

void
cogl_draw_indexed_attributes (CoglVerticesMode mode,
                              int first_vertex,
                              int n_vertices,
                              CoglIndices *indices,
                              ...) G_GNUC_NULL_TERMINATED;

void
cogl_draw_indexed_attributes_array (CoglVerticesMode mode,
                                    int first_vertex,
                                    int n_vertices,
                                    CoglIndices *indices,
                                    CoglAttribute **attributes);

G_END_DECLS

#endif /* __COGL_ATTRIBUTE_H__ */

