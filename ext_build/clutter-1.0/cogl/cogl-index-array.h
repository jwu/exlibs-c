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

#ifndef __COGL_INDEX_ARRAY_H__
#define __COGL_INDEX_ARRAY_H__

G_BEGIN_DECLS

/**
 * SECTION:cogl-vertex-indices
 * @short_description: Functions for creating and manipulating vertex
 * indices.
 *
 * FIXME
 */

typedef struct _CoglIndexArray	      CoglIndexArray;

/**
 * cogl_index_array_new:
 * @bytes: The number of bytes to allocate for vertex attribute data.
 *
 * Declares a new #CoglIndexArray of @size bytes to contain vertex
 * indices. Once declared, data can be set using
 * cogl_buffer_set_data() or by mapping it into the application's
 * address space using cogl_buffer_map().
 *
 * Since: 1.4
 * Stability: Unstable
 */
CoglIndexArray *
cogl_index_array_new (gsize bytes);

/**
 * cogl_is_indices_array:
 * @object: A #CoglObject
 *
 * Gets whether the given object references a #CoglIndexArray.
 *
 * Returns: %TRUE if the handle references a #CoglIndexArray,
 *   %FALSE otherwise
 *
 * Since: 1.4
 * Stability: Unstable
 */
gboolean
cogl_is_indices_array (void *object);

G_END_DECLS

#endif /* __COGL_INDEX_ARRAY_H__ */

