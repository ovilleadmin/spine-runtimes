/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated April 5, 2025. Replaces all prior versions.
 *
 * (license header identical to other files in this folder; abridged)
 *****************************************************************************/

#pragma once

#include "SpineAttachment.h"
#include "SpineTextureRegion.h"
#include <spine/MeshAttachment.h>

// Typed wrapper around spine::MeshAttachment. This is the one the
// OvilleWorld Cackpal shirt needs — swapping the region on a mesh attachment
// preserves the mesh's vertex positions and bone weights (so deformation
// continues to work) while redirecting the draw to a new texture.
class SpineMeshAttachment : public SpineAttachment {
	GDCLASS(SpineMeshAttachment, SpineAttachment)

	// Keeps the SpineTextureRegion alive for the lifetime of this wrapper.
	Ref<SpineTextureRegion> owned_region;

protected:
	static void _bind_methods();

public:
	// --- texture region ---
	// Mesh attachments REQUIRE update_region() after set_region() to
	// recompute per-vertex UVs from the new region's UV bounds. set_region
	// here calls spine-cpp updateSequence() automatically for convenience
	// (4.3: regions live in the attachment's always-present Sequence; the
	// 4.2 setRegion()/updateRegion() API is gone).
	void set_region(Ref<SpineTextureRegion> region);
	Ref<SpineTextureRegion> get_spine_texture_region();
	void update_region();

	// --- color tint ---
	Color get_color();
	void  set_color(Color c);

	// --- geometry info (read-only for now; vertex-buffer exposure is
	// out of scope for the initial patch). ---
	int get_world_vertices_length();
	int get_hull_length();

	// --- structural introspection (M3.7, for spine_feature_guard binary
	// .skel scanning). linkedmesh + sequence features are flags on the
	// underlying spine-cpp MeshAttachment, but unbound in stock spine-godot. ---
	bool is_linked_mesh();   // true if getSourceMesh() != nullptr (4.3 name; was getParentMesh)
	bool has_sequence();     // true if the sequence has > 1 region (a real multi-frame sequence)

private:
	spine::MeshAttachment *_mesh_attachment() {
		return (spine::MeshAttachment *) get_spine_object();
	}
};
