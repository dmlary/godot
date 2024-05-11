/**************************************************************************/
/*  test_grid_map.h                                                       */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef TEST_GRID_MAP_H
#define TEST_GRID_MAP_H

#include "../grid_map.h"
#include "core/math/math_defs.h"
#include "core/math/vector3i.h"
#include "tests/core/math/test_vector4.h"
#include "tests/test_macros.h"

namespace TestGridMap {

TEST_CASE("[SceneTree][GridMap] hex cell set_cell_size()") {
	GridMap map;
	map.set_cell_shape(GridMap::CELL_SHAPE_HEXAGON);
	map.set_cell_size(Vector3(1.0, 3.0, 200.0));
	CHECK(map.get_cell_size().x == 1.0);
	CHECK(map.get_cell_size().y == 3.0);
	CHECK_MESSAGE(map.get_cell_size().z == 1.0,
			"set_cell_size() hex cell, z value should be overwritten with x");
}

#define CHECK_HEX_BOUNDARIES(p, c)                                                     \
	{                                                                                  \
		Vector3i index = map.local_to_map((p));                                        \
		CHECK_MESSAGE(index == (c), #p " center not in expected cell");                \
		index = map.local_to_map((p) + Vector3(0, 0, 0.499));                          \
		CHECK_MESSAGE(index == (c), #p " east edge not in expected cell");             \
		index = map.local_to_map((p) + Vector3(Math_SQRT3 / 2.0 - 0.001, 0, 0.499));   \
		CHECK_MESSAGE(index == (c), #p " northeast vertex not in expected cell");      \
		index = map.local_to_map((p) + Vector3(0, 0, 0.999));                          \
		CHECK_MESSAGE(index == (c), #p " north vertex not in expected cell");          \
		index = map.local_to_map((p) + Vector3(-Math_SQRT3 / 2.0 + 0.001, 0, 0.499));  \
		CHECK_MESSAGE(index == (c), #p " northwest vertex not in expected cell");      \
		index = map.local_to_map((p) + Vector3(0, 0, -0.499));                         \
		CHECK_MESSAGE(index == (c), #p " west edge not in expected cell");             \
		index = map.local_to_map((p) + Vector3(-Math_SQRT3 / 2.0 + 0.001, 0, -0.499)); \
		CHECK_MESSAGE(index == (c), #p " southwest vertex not in expected cell");      \
		index = map.local_to_map((p) + Vector3(0, 0, -0.99));                          \
		CHECK_MESSAGE(index == (c), #p " south vertex not in expected cell");          \
		index = map.local_to_map((p) + Vector3(Math_SQRT3 / 2.0 - 0.001, 0, -0.49));   \
		CHECK_MESSAGE(index == (c), #p " southeast vertex not in expected cell");      \
	}

TEST_CASE("[SceneTree][GridMap] local_to_map() for hex cells") {
	GridMap map;
	map.set_cell_shape(GridMap::CELL_SHAPE_HEXAGON);
	map.set_cell_size(Vector3(1.0, 1.0, 1.0));

	// verify origin
	CHECK_HEX_BOUNDARIES(Vector3(0, 0, 0), Vector3i(0, 0, 0));
	// north two rows
	CHECK_HEX_BOUNDARIES(Vector3(0, 0, 3), Vector3i(-1, 0, 2));
	// south two rows
	CHECK_HEX_BOUNDARIES(Vector3(0, 0, -3), Vector3i(1, 0, -2));
	// east one cell
	CHECK_HEX_BOUNDARIES(Vector3(Math_SQRT3, 0, 0), Vector3i(1, 0, 0));
	// west one cell
	CHECK_HEX_BOUNDARIES(Vector3(-Math_SQRT3, 0, 0), Vector3i(-1, 0, 0));
	// northeast one cell
	CHECK_HEX_BOUNDARIES(Vector3(Math_SQRT3 / 2, 0, 1.5), Vector3i(0, 0, 1));
	// northwest one cell
	CHECK_HEX_BOUNDARIES(Vector3(-Math_SQRT3 / 2, 0, 1.5), Vector3i(-1, 0, 1));
	// southwest one cell
	CHECK_HEX_BOUNDARIES(Vector3(-Math_SQRT3 / 2, 0, -1.5), Vector3i(0, 0, -1));
}

TEST_CASE("[SceneTree][GridMap] map_to_local() for hex cells") {
	GridMap map;
	map.set_cell_shape(GridMap::CELL_SHAPE_HEXAGON);
	map.set_cell_size(Vector3(1.0, 1.0, 1.0));

	CHECK(map.map_to_local(Vector3i(0, 0, 0)) == Vector3(0, 0, 0));
	CHECK(map.map_to_local(Vector3i(-1, 0, 2)) == Vector3(0, 0, 3));
	CHECK(map.map_to_local(Vector3i(1, 0, -2)) == Vector3(0, 0, -3));
	CHECK(map.map_to_local(Vector3i(1, 0, 0)) == Vector3(Math_SQRT3, 0, 0));
	CHECK(map.map_to_local(Vector3i(-1, 0, 0)) == Vector3(-Math_SQRT3, 0, 0));
	CHECK(map.map_to_local(Vector3i(0, 0, 1)) == Vector3(Math_SQRT3 / 2, 0, 1.5));
	CHECK(map.map_to_local(Vector3i(-1, 0, 1)) == Vector3(-Math_SQRT3 / 2, 0, 1.5));
	CHECK(map.map_to_local(Vector3i(-1, 0, 1)) == Vector3(-Math_SQRT3 / 2, 0, 1.5));
	CHECK(map.map_to_local(Vector3i(0, 0, -1)) == Vector3(-Math_SQRT3 / 2, 0, -1.5));
}

} // namespace TestGridMap

#endif // TEST_GRID_MAP_H
