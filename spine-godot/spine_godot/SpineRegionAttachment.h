/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated April 5, 2025. Replaces all prior versions.
 *
 * (license header identical to other files in this folder; abridged)
 *****************************************************************************/

#pragma once

#include "SpineAttachment.h"
#include "SpineTextureRegion.h"
#include <spine/RegionAttachment.h>

// Typed wrapper around spine::RegionAttachment. Exposed so GDScript can swap
// the texture region via set_region(), enabling Unity-equivalent
// GetRemappedClone-style runtime clothing changes. See PLAN.md.
class SpineRegionAttachment : public SpineAttachment {
	GDCLASS(SpineRegionAttachment, SpineAttachment)

	// Keeps the SpineTextureRegion alive for the lifetime of this wrapper,
	// so the underlying spine::AtlasRegion stays valid while spine-cpp uses it.
	Ref<SpineTextureRegion> owned_region;

protected:
	static void _bind_methods();

public:
	// --- texture region ---
	void set_region(Ref<SpineTextureRegion> region);
	Ref<SpineTextureRegion> get_spine_texture_region();
	void update_region();

	// --- placement / size ---
	float get_x();        void set_x(float x);
	float get_y();        void set_y(float y);
	float get_width();    void set_width(float w);
	float get_height();   void set_height(float h);
	float get_rotation(); void set_rotation(float r);
	float get_scale_x();  void set_scale_x(float s);
	float get_scale_y();  void set_scale_y(float s);

	// --- color tint (slot.Data R/G/B/A) ---
	Color get_color();
	void  set_color(Color c);

	// --- structural introspection (M3.7, for spine_feature_guard binary
	// .skel scanning). sequence is a flag on the underlying spine-cpp
	// RegionAttachment, unbound in stock spine-godot. ---
	bool has_sequence();     // true if the sequence has > 1 region (a real multi-frame sequence)

private:
	spine::RegionAttachment *_region_attachment() {
		return (spine::RegionAttachment *) get_spine_object();
	}
};
