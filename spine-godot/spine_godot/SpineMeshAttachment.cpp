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
	// spine-cpp 4.3: attachments no longer hold a region directly; the region
	// lives in an always-present Sequence (1 entry for plain attachments).
	// The render loop resolves sequence.getRegion(resolveIndex(pose)) and reads
	// UVs from the sequence's cache, so swapping = replace the sequence's
	// region slot + updateSequence().
	auto &sequence = _mesh_attachment()->getSequence();
	auto &regions = sequence.getRegions();
	if (regions.size() > 1) {
		ERR_PRINT("SpineMeshAttachment::set_region: attachment has a multi-frame sequence; replacing its texture would collapse the sequence animation. Not supported.");
		return;
	}
	owned_region = region;
	if (regions.size() == 0)
		regions.add(region->get_spine_object());
	else
		regions[0] = region->get_spine_object();
	// updateSequence() is essential for mesh attachments — the per-vertex UVs
	// are cached (per sequence entry in 4.3) and must be recomputed from the
	// new region's UV bounds. Skip this and the mesh renders with the old
	// region's UVs, which is the most common pitfall of this API.
	_mesh_attachment()->updateSequence();
}

Ref<SpineTextureRegion> SpineMeshAttachment::get_spine_texture_region() {
	return owned_region;
}

void SpineMeshAttachment::update_region() {
	SPINE_CHECK(_mesh_attachment(), )
	_mesh_attachment()->updateSequence();
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
	// spine-cpp 4.3 renamed getParentMesh() -> getSourceMesh().
	return _mesh_attachment()->getSourceMesh() != nullptr;
}

bool SpineMeshAttachment::has_sequence() {
	SPINE_CHECK(_mesh_attachment(), false)
	// spine-cpp 4.3: getSequence() returns a Sequence& (never null — readSequence
	// always creates at least a default 1-region sequence). A "real" animated
	// sequence has more than one region.
	return _mesh_attachment()->getSequence().getRegions().size() > 1;
}
