/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated April 5, 2025. Replaces all prior versions.
 *
 * (license header identical to other files in this folder; abridged)
 *****************************************************************************/

#include "SpineMeshAttachment.h"

void SpineMeshAttachment::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_region", "region"), &SpineMeshAttachment::set_region);
	ClassDB::bind_method(D_METHOD("get_spine_texture_region"), &SpineMeshAttachment::get_spine_texture_region);
	ClassDB::bind_method(D_METHOD("update_region"), &SpineMeshAttachment::update_region);

	ClassDB::bind_method(D_METHOD("get_color"), &SpineMeshAttachment::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "c"), &SpineMeshAttachment::set_color);

	ClassDB::bind_method(D_METHOD("get_world_vertices_length"), &SpineMeshAttachment::get_world_vertices_length);
	ClassDB::bind_method(D_METHOD("get_hull_length"), &SpineMeshAttachment::get_hull_length);

	ClassDB::bind_method(D_METHOD("is_linked_mesh"), &SpineMeshAttachment::is_linked_mesh);
	ClassDB::bind_method(D_METHOD("has_sequence"), &SpineMeshAttachment::has_sequence);
}

void SpineMeshAttachment::set_region(Ref<SpineTextureRegion> region) {
	SPINE_CHECK(_mesh_attachment(), )
	if (!region.is_valid() || !region->get_spine_object()) {
		ERR_PRINT("SpineMeshAttachment::set_region: region is invalid");
		return;
	}
	owned_region = region;
	_mesh_attachment()->setRegion(region->get_spine_object());
	// updateRegion() is essential for mesh attachments — the per-vertex UVs
	// are cached and must be recomputed from the new region's UV bounds.
	// Skip this and the mesh will render with the old region's UVs, which
	// is the most common pitfall of this API.
	_mesh_attachment()->updateRegion();
}

Ref<SpineTextureRegion> SpineMeshAttachment::get_spine_texture_region() {
	return owned_region;
}

void SpineMeshAttachment::update_region() {
	SPINE_CHECK(_mesh_attachment(), )
	_mesh_attachment()->updateRegion();
}

Color SpineMeshAttachment::get_color() {
	SPINE_CHECK(_mesh_attachment(), Color())
	auto &c = _mesh_attachment()->getColor();
	return Color(c.r, c.g, c.b, c.a);
}

void SpineMeshAttachment::set_color(Color c) {
	SPINE_CHECK(_mesh_attachment(), )
	_mesh_attachment()->getColor().set(c.r, c.g, c.b, c.a);
}

int SpineMeshAttachment::get_world_vertices_length() {
	SPINE_CHECK(_mesh_attachment(), 0)
	return _mesh_attachment()->getWorldVerticesLength();
}

int SpineMeshAttachment::get_hull_length() {
	SPINE_CHECK(_mesh_attachment(), 0)
	return _mesh_attachment()->getHullLength();
}

bool SpineMeshAttachment::is_linked_mesh() {
	SPINE_CHECK(_mesh_attachment(), false)
	return _mesh_attachment()->getParentMesh() != nullptr;
}

bool SpineMeshAttachment::has_sequence() {
	SPINE_CHECK(_mesh_attachment(), false)
	// spine-cpp 4.3: getSequence() returns a Sequence& (never null — readSequence
	// always creates at least a default 1-region sequence). A "real" animated
	// sequence has more than one region.
	return _mesh_attachment()->getSequence().getRegions().size() > 1;
}
